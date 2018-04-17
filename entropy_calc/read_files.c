#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static int myCompare (const void * a, const void * b)
{
    return strcmp (*(const char **) a, *(const char **) b);
}

int main(int argc, char* argv[]) 
{
  DIR *dp;
  struct dirent *ep;

  int count=0;
  int i=0;

  dp = opendir (argv[1]);
  if (dp != NULL)
    {
      while (ep = readdir (dp)){
        char* name = ep->d_name;
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
          count = count + 1;
        //puts (ep->d_name);
      }
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");

  printf("\ncount: %i\n", count);

  const char *files[count];

  dp = opendir (argv[1]);
  if (dp != NULL)
    {
      while (ep = readdir (dp)){
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


  qsort(files, count, sizeof(const char *), myCompare);

  int x;
  for (x=0; x<count; x++){
    printf("%s\n", files[x]);
  }

  return 0;
}
