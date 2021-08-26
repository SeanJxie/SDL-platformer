#include "vec2.hpp"


float mag(fvec2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

fvec2 norm(fvec2 v)
{
    float m = mag(v);
    fvec2 ret;
    ret.x = v.x / m;
    ret.y = v.y / m;
    return ret;
}

fvec2 unit_from_deg(float dir)
{
    fvec2 ret;
    ret.x = cosf(RAD(dir));
    ret.y = sinf(RAD(dir));

    return ret;
}