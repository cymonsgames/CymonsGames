#ifndef __MISC_H_
#define __MISC_H_

// seed the RNG using microseconds from gettimeofday()
void randomize (void); 

// generate a random integer, min <= X <= max
int randint (int min, int max);

#endif /* __MISC_H_ */
