#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define FLAG(n) std::cout << "HERE " << n << std::endl;

int clamp(int n, int min, int max);
float fclamp(float f, float min, float max);
void write_err(const char *msg);

#endif /* UTILS_H */