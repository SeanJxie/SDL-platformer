#include "texture.hpp"
#include <iostream>
#define FLAG(n) std::cout << "HERE " << n << std::endl;

PNGTexture::PNGTexture()
{
    mTex = NULL;
}

PNGTexture::~PNGTexture()
{
    free();
}

void PNGTexture::set_renderer(SDL_Renderer *hRenderer)
{
    m_hTargetRenderer = hRenderer;
}

void PNGTexture::free() 
{
    if (mTex != NULL)
    {
        SDL_DestroyTexture(mTex);
        mTex = NULL;
        mWt = 0;
        mHt = 0;
    }
}

bool PNGTexture::load_from_path_str(const char *path, int wt, int ht)
{
    free();
    mWt = wt;
    mHt = ht;
    
    SDL_Surface *load_surface = IMG_Load(path);
    if (load_surface == NULL) {
        return false;
    }

    mTex = SDL_CreateTextureFromSurface(m_hTargetRenderer, load_surface);
    if (mTex == NULL) {
        return false;
    }

    SDL_FreeSurface(load_surface);   

    return true;
}

bool PNGTexture::load_from_text(TTF_Font *font, const char *txt, SDL_Color col)
{
    free();
    
    SDL_Surface *txt_surface = TTF_RenderText_Solid(font, txt, col);
    if (txt_surface == NULL) {
        return false;
    }
    
    mWt = txt_surface->w;
    mHt = txt_surface->h;
    
    mTex = SDL_CreateTextureFromSurface(m_hTargetRenderer, txt_surface);
    if (mTex == NULL) {
        return false;
    }
    
    SDL_FreeSurface(txt_surface);   
    
    return true;
}

bool PNGTexture::render(int x, int y, SDL_Rect *tex_clip, double rot_angle, SDL_Point *rot_origin, SDL_RendererFlip flip)
{
    SDL_Rect quad = { x, y, mWt, mHt };

    /* Code below enables true sprite size */
    // if (tex_clip != NULL)
    // {
    //     quad.w = tex_clip->w;
    //     quad.h = tex_clip->h;
    // }

    if (SDL_RenderCopyEx(m_hTargetRenderer, mTex, tex_clip, &quad, rot_angle, rot_origin, flip) == -1) {
        return false;
    }

    return true;
}