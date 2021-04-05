#include "utils.h"

float rand_float(bool negativeRange)
{
	float x = (float)rand()/(float)RAND_MAX;
	if (negativeRange)
	{
		x = neg_range(x);
	}
	return x;
}

float clamp(float x, float min, float max)
{
	return fmax(fmin(x, max), min);
}

float sign(float x)
{
	if (x < 0)
	{
		return -1.f;
	}
	return 1.f;
}
float neg_range(float x)
{
	return (x*2.f)-1.f;
}