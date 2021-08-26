#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "vec2.hpp"


class PNGTexture
{
    public:
        PNGTexture();
        ~PNGTexture();

        void set_renderer(SDL_Renderer *hRenderer);
        void free();
        bool load_from_path_str(const char *path, int wt, int ht);
        bool load_from_text(TTF_Font *font, const char *txt, SDL_Color col);
        bool render(int x, int y, SDL_Rect *tex_clip, double rot_angle, SDL_Point *rot_origin, SDL_RendererFlip flip);
    
    private:
        SDL_Texture *mTex;
        SDL_Renderer *m_hTargetRenderer;

        int mWt;
        int mHt;
};

struct SpriteData
{
    PNGTexture *sprite_sheet;

    int curr_tex_idx = 0;
    int nsprites;
    
    float dt_accum = 0.0f;
    float dt_inter;

    vec2 ss_dimensions;
    vec2 s_dimensions;
};


#endif /* TEXTURE_H */