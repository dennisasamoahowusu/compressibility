#include <stdio.h>
#include "avg_mean.h"
#include "shannon_f.h"
#include "heuristic.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h> 

/*int compare_naturally1(const void* a_ptr, const void* b_ptr) {
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
*/

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

double shannon_entropy(const char* filename){
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

  return shannon_f(input_data, file_size);
}

enum compress_advice heuristic_wrapper(const char* filename){
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

  return heuristic(input_data, file_size);
}

/*
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

*/

/*
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
          //printf("%s\n", name);
          files[i] = name;
          i = i+1;
        }
      }
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");

  int x;
  for (x=0; x<count; x++){
    //int32_t avg_mean = avg_mean_entropy(files[x]);
    printf("%s\n", files[x]);
    //fprintf(outF, "%s | %i\n", files[x], 5);
  }

  //qsort(files, count, sizeof(const char *), compare_naturally1);
}

*/

int main(int argc, char* argv[]) 
{
  const char* source_dir = argv[2];
  const char* output_file = argv[3];
  size_t arglen = strlen(argv[2]);
  const char* type = argv[1];

  FILE *outF;
  outF = fopen(output_file, "a");

  DIR *dp;
  struct dirent *ep;

  dp = opendir (source_dir);
  if (dp != NULL){
      while ((ep = readdir (dp))){
        char* name = ep->d_name;
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0){
          char *fullpath = malloc(arglen + strlen(ep->d_name) + 2);
          if (fullpath == NULL) { 
            printf("Full path error"); 
            /* deal with error and exit */ 
          } else {
            sprintf(fullpath, "%s/%s", source_dir, ep->d_name);
            /* use fullpath */
            //printf("%s\n", fullpath);

            if (strcmp(type,"avg_mean") == 0){
              int32_t avg_mean = avg_mean_entropy(fullpath);
              //printf("%i\n", avg_mean);
              fprintf(outF, "%s %i\n", fullpath, avg_mean);
            } else if (strcmp(type,"shannon") == 0){
              double shannon = shannon_entropy(fullpath);
              fprintf(outF, "%s %f\n", fullpath, shannon);
            } else if (strcmp(type,"heuristic") == 0) {
              enum compress_advice advice = heuristic_wrapper(fullpath);
              char *advice_str;
			switch (advice) {
			case COMPRESS_NONE:
				advice_str = "1";
				printf("Not Compress\n");
				break;;
			case COMPRESS_COST_UNKNOWN:
				advice_str = "0";
				printf("Unknown\n");
				break;;
			case COMPRESS_COST_EASY:
				advice_str = "0.25";
				printf("Compress cost: EASY\n");
				break;;
			case COMPRESS_COST_MEDIUM:
				advice_str = "0.5";
				printf("Compress cost: MEDIUM\n");
				break;;
			case COMPRESS_COST_HARD:
				advice_str = "0.75";
				printf("Compress cost: HARD\n");
				break;;
			}
              fprintf(outF, "%s %s\n", fullpath, advice_str);
            } else {
              printf("\nType argument not provided or wrong type\n");
              return 1;
            }
            free(fullpath);
          }
        }
      }
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");


  fclose(outF);

  return 0;
}

