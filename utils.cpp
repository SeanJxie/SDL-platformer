#include "utils.hpp"

int clamp(int n, int min, int max)
{
    return (n > max) ? max : ((n < min) ? min : n);
}

float fclamp(float f, float min, float max)
{
    return (f > max) ? max : ((f < min) ? min : f);
}


void write_err(const char *msg) {
    std::cout << msg << std::endl << "SDL: " << SDL_GetError() << std::endl << "IMG: " << IMG_GetError() << std::endl << "TTF: " << TTF_GetError() << std::endl;
}