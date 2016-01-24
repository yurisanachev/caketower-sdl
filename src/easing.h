#ifndef _EASING_H_
#define _EASING_H_
#include <math.h>

double elasticOut(double p)
{
	return sin(-13 * M_PI_2 * (p + 1)) * pow(2, -10 * p) + 1;
}

double elasticIn(double p)
{
	return sin(13 * M_PI_2 * p) * pow(2, 10 * (p - 1));
}

double backIn(double p)
{
	return p * p * p - p * sin(p * M_PI);
}

double backOut(double p)
{
	double f = (1 - p);
	return 1 - (f * f * f - f * sin(f * M_PI));
}

double expoOut(double p)
{
	return (p == 1.0) ? p : 1 - pow(2, -10 * p);
}

double expoIn(double p)
{
	return (p == 0.0) ? p : pow(2, 10 * (p - 1));
}

#endif 
