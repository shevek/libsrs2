#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system */
#include <unistd.h>     /* execv */
#include <sys/types.h>  /* typedefs */
#include <time.h>       /* time */
#include <sys/time.h>   /* timeval / timezone / gettimeofday */

typedef struct srs_time_s
{
  struct timeval  start;     /* sample start */
  struct timeval  finish;    /* sample finish */
  struct timezone tzp;       /* timezone */
} srs_time_t;

int main(int argc, char **argv)
{
  srs_time_t  srs_time[1];
  u_int32_t   i            = 0;
  int32_t     fw           = 0;

  gettimeofday(&srs_time[0].start, &srs_time[0].tzp);

  if (argc <= 1 || argv[1] == NULL)
  {
    printf("You must supply a number of forwards to generate eg: ./test 1000\n");
    return(0);
  }

  fw = atoi(argv[1]);
  for (i = 0; i <= fw; i++)
  {	
    system("./srs -q 1 -f jcouzens@obscurity.org -t james@pobox.com -s asshat");
  }
  
  gettimeofday(&srs_time[0].finish, &srs_time[0].tzp);

  printf("%i forwards executed in %lu.%u seconds\n", fw,
    (srs_time[0].finish.tv_sec - srs_time[0].start.tv_sec),
    (u_int8_t)(srs_time[0].finish.tv_usec - srs_time[0].start.tv_usec));

  return(1);
}
/* end test.c */
