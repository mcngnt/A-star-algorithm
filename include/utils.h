#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include <cstdio>

#define SCREEN_W 1500
#define SCREEN_H 1000

#define PI 3.14

float rand_float(bool);
float clamp(float, float, float);
float sign(float);
float neg_range(float);

#endif