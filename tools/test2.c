#include <stdio.h>

#include "../srs.h"
#include "../util.h"

srs_time_t srs_time[3];

int main(void)
{
  srs_t *srs      = NULL;   /* srs structure */
  
  char  *srsaddr   = NULL;   /* srs address storage */
  char  *secret    = "asshat";
  char  *addr1     = "jcouzens@uhfco.net";
  char  *addr2     = "james@baystreet.com";
  
  int   i          = 0;
  int   age        = 365000;
  int   hash_len   = 4;
  int   hash_min   = 4;
  
  while (i <= 10000)
  {
    srs = SRS_new(secret, age, hash_len, hash_min);
  
    /* simulate email from HOSTA to HOSTB */
    if ((srsaddr = SRS_forward(srs, addr1, addr2)) == NULL)
    {
      printf("Rewriting of address (%s) failed!  !@$#\n", addr1);
      srs = SRS_del(srs);
      return(FALSE);
    }
  
    xfree(srsaddr);
    srs = SRS_del(srs);
    i++;
  }
  
  printf ("done\n");
  return 0;
}
