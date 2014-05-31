#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "misc.h"

void randomize (void)
{
#ifdef UNIX
	struct timeval *tv  = (struct timeval  *) malloc(sizeof(struct timeval ));
	struct timezone *tz = (struct timezone *) malloc(sizeof(struct timezone));
	gettimeofday(tv,tz);
	srand(tv->tv_usec);
	free(tv);
	free(tz);
#else
  srand (time(NULL));
#endif
}

int randint (int min, int max)
{
	return (min + (int) ((max+1) * (rand() / (RAND_MAX+1.0))));
}
