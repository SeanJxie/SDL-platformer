#ifndef VEC2_H
#define VEC2_H

#include <math.h>

#define DEG(R) (float)(R * 180.0 / M_PI)
#define RAD(D) (float)(D * M_PI / 180.0)

struct fvec2
{
    float x;
    float y;
};

struct vec2
{
    int x;
    int y;
};

float mag(fvec2 v);
fvec2 norm(fvec2 v);
fvec2 step(float dir);


#endif /* VEC2_H */