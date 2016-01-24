#ifndef _EASING_H_
#define _EASING_H_
#include <math.h>

double elasticOut(double p)
{
	return sin(-13 * M_PI_2 * (p + 1)) * pow(2, -10 * p) + 1;
}

#endif 
