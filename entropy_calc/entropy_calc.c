#include <stdio.h>
#include "avg_mean.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

int compare_naturally1(const void* a_ptr, const void* b_ptr) {
    const char * a = *(const char**)a_ptr;
    const char * b = *(const char**)b_ptr;

    while (*a == *b) {
        if(*a == '\0')
            return 0;
        a++, b++;
    }

    int diff = strspn(a, "0123456789") - strspn(b, "0123456789");
    if (diff)
        return diff;
    return *a - *b;
}

//static int myCompare (const void * a, const void * b)
//{
//    return strcmp (*(const char **) a, *(const char **) b);
//}

int32_t avg_mean_entropy(const char* filename){
  struct stat file_stat;
  uint64_t file_size;
  uint8_t *input_data;
  int64_t fd;

  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    printf("Can't open file: %s\n", filename);
    return 1;
  }

  /* Get the size of the file. */
  fstat (fd, &file_stat);
  file_size = file_stat.st_size;

  int prot = PROT_READ|PROT_NONE;
  int map_flags =  MAP_SHARED;
  input_data = (uint8_t *) mmap (0, file_size, prot, map_flags, fd, 0);

  return avg_mean(input_data, file_size);
}

int n_files_in_folder(const char* dirpath){
  DIR *dp;
  struct dirent *ep;

  int count=0;

  dp = opendir (dirpath);
  if (dp != NULL){
      while ((ep = readdir (dp))){
        char* name = ep->d_name;
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
          count = count + 1;
      }
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");

  return count;
}

void files_in_folder(const char* dirpath, const char**files, int count){
  DIR *dp;
  struct dirent *ep;
  int i=0;

  dp = opendir (dirpath);
  if (dp != NULL)
    {
      while ((ep = readdir (dp))){
        char* name = ep->d_name;
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0){
          files[i] = name;
          i = i+1;
        }
      }
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");

  qsort(files, count, sizeof(const char *), compare_naturally1);
}

int main(int argc, char* argv[]) 
{
  //int32_t avg_mean = avg_mean_entropy(argv[1]);
  int n_files = n_files_in_folder(argv[1]);
  //printf("%i\n", n_files);
  const char* files[n_files];
  files_in_folder(argv[1], files, n_files);

  int x;
  for (x=0; x<n_files; x++){
    printf("%s\n", files[x]);
  }


/*
  struct stat file_stat;
  uint64_t file_size;
  uint8_t *input_data;
  int64_t fd;

  if (argc != 2) {
    return 1;
  }
  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    printf("Can't open file: %s\n", argv[1]);
    return 1;
  }*/

  /* Get the size of the file. */

/*
  fstat (fd, &file_stat);
  file_size = file_stat.st_size;

  int prot = PROT_READ|PROT_NONE;
  int map_flags =  MAP_SHARED;
  input_data = (uint8_t *) mmap (0, file_size, prot, map_flags, fd, 0);

  avg_mean(input_data, file_size);
*/

  return 0;
}

