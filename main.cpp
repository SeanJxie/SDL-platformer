#include <iostream>
#include <math.h>

#include "texture.hpp"
#include "physics_sprite.hpp"
#include "vec2.hpp"
#include "utils.hpp"

#define RENDER_FLAG SDL_RENDERER_ACCELERATED
#define WINDOW_FLAG SDL_WINDOW_RESIZABLE

/*
 * TODO: Config files for ALL THE HARDCODED LITERALS
 *
 * 
 * 
 */


const char *WINTT = "We Don't Jump Here";
const int   WINWT = 800;
const int   WINHT = 800;

PNGTexture g_texRun;
int g_nRunWt = 200;
int g_nRunHt = 200;

PNGTexture g_texIdle;
int g_nIdleWt = 200;
int g_nIdleHt = 200;

PNGTexture g_texJump;
int g_nJumpWt = 200;
int g_nJumpHt = 200;

PNGTexture g_texFall;
int g_nFallWt = 200;
int g_nFallHt = 200;

PNGTexture g_texFont;
int g_nFontSize = 30;
TTF_Font *g_pFont;
SDL_Color g_colFont = { 0x00, 0x00, 0x00, 0xFF };

PNGTexture g_texFloor;
int g_nFloorWt = WINWT;
int g_nFloorHt = 100;

PNGTexture g_texPlat;
int g_nPlatWt = 400;
int g_nPlatHt = 50;

SDL_Window   *g_hWin = NULL;
SDL_Renderer *g_hRend = NULL;


bool init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        return false;
    }

    if (TTF_Init() == -1) {
        return false;
    }

    g_hWin = SDL_CreateWindow(WINTT, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINWT, WINHT, WINDOW_FLAG);
    if (g_hWin == NULL) {
        return false;
    }

    g_hRend = SDL_CreateRenderer(g_hWin, -1, RENDER_FLAG);
    if (g_hRend == NULL) {
        return false;
    }

    return true;
}

bool load_media()
{
    g_texRun.set_renderer(g_hRend);
    if (!g_texRun.load_from_path_str("assets\\Pink_Monster_Run_6_Dust.png", g_nRunWt, g_nRunHt)) {
        return false;
    }

    g_texIdle.set_renderer(g_hRend);
    if (!g_texIdle.load_from_path_str("assets\\Pink_Monster_Idle_4.png", g_nIdleWt, g_nIdleHt)) {
        return false;
    }

    g_texJump.set_renderer(g_hRend);
    if (!g_texJump.load_from_path_str("assets\\Pink_Monster_Jump_1.png", g_nJumpWt, g_nJumpHt)) {
        return false;
    }

    g_texFall.set_renderer(g_hRend);
    if (!g_texFall.load_from_path_str("assets\\Pink_Monster_Fall_1.png", g_nFallWt, g_nFallHt)) {
        return false;
    }

    g_texFloor.set_renderer(g_hRend);
    if (!g_texFloor.load_from_path_str("assets\\default.png", g_nFloorWt, g_nFloorHt)) {
        return false;
    }

    g_texPlat.set_renderer(g_hRend);
    if (!g_texPlat.load_from_path_str("assets\\default.png", g_nPlatWt, g_nPlatHt)) {
        return false;
    }

    g_pFont = TTF_OpenFont("assets\\Planes_ValMore.ttf", g_nFontSize);
    g_texFont.set_renderer(g_hRend);
    if (!g_texFont.load_from_text(g_pFont, "A Demo of Textures and Sprite Sheets in SDL!", g_colFont)) {
        return false;
    }   

    return true;
}

void handle_input(SDL_Event &event, bool &mouse, bool &u, bool &d, bool &l, bool &r)
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_w: u = true; break;
                case SDLK_s: d = true; break;
                case SDLK_a: l = true; break;
                case SDLK_d: r = true; break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
                case SDLK_w: u = false; break;
                case SDLK_s: d = false; break;
                case SDLK_a: l = false; break;
                case SDLK_d: r = false; break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN: mouse = true; break;
        case SDL_MOUSEBUTTONUP: mouse = false; break;
    }
}

void cleanup()
{
    TTF_CloseFont(g_pFont);
    g_texRun.free();
    g_texIdle.free();
    g_texJump.free();
    g_texFall.free();
    g_texPlat.free();
    g_texFloor.free();
    g_texFont.free();

    SDL_DestroyRenderer(g_hRend);
	SDL_DestroyWindow(g_hWin);

    g_hRend = NULL;
    g_hWin = NULL;

    TTF_Quit();
    IMG_Quit();
	SDL_Quit();
}


int main(int argc, char *argv[])
{
    if (!init()) 
    {
        write_err("[ERROR] could not initialize.");
        return EXIT_FAILURE;
    }

    if (!load_media())
    {
        write_err("[ERROR] could not load media.");
        return EXIT_FAILURE;
    }

    /* Game vars */

    /* Movement */
    float fMoveSpeed = 0.6f;

    /* Physics sprites */
    int nWt = 200;  // Used for geometry size
    int nHt = 200;  // Used for geometry size

    PhysicsSprite player(WINWT / 2 - nWt  / 2, -500, nWt, nHt);
    PhysicsSprite platform(WINWT / 2 - g_nPlatWt / 2, WINHT / 2 - g_nPlatHt / 2, g_nPlatWt, g_nPlatHt);
    
    /* Sprite data */
    SpriteData runSprite;
    runSprite.sprite_sheet = &g_texRun;
    runSprite.ss_dimensions = { 192, 32 };
    runSprite.s_dimensions = { 32, 32 };
    runSprite.dt_inter = 70.0f;
    runSprite.nsprites = 6;

    SpriteData idleSprite;
    idleSprite.sprite_sheet = &g_texIdle;
    idleSprite.ss_dimensions = { 128, 32 };
    idleSprite.s_dimensions = { 32, 32 };
    idleSprite.dt_inter = 140.0f;
    idleSprite.nsprites = 4;

    SpriteData jumpSprite;
    jumpSprite.sprite_sheet = &g_texJump;
    jumpSprite.ss_dimensions = { 32, 32 };
    jumpSprite.s_dimensions = { 32, 32 };
    jumpSprite.dt_inter = 1.0f;
    jumpSprite.nsprites = 1;

    SpriteData fallSprite;
    fallSprite.sprite_sheet = &g_texFall;
    fallSprite.ss_dimensions = { 32, 32 };
    fallSprite.s_dimensions = { 32, 32 };
    fallSprite.dt_inter = 1.0f;
    fallSprite.nsprites = 1;

    player.set_run_sprite(runSprite);
    player.set_idle_sprite(idleSprite);
    player.set_jump_sprite(jumpSprite);
    player.set_fall_sprite(fallSprite);

    platform.set_static_texture(&g_texPlat);

    /* Collisions */
    std::vector<PhysicsSprite> vCollsion = { platform };
    player.set_collision_sprites(vCollsion);

    /* Input */
    bool bMousePressed = false;
    bool bWpressed = false, bSpressed = false, bApressed = false, bDpressed = false;

    /* Main loop */
    bool bGameRunning = true;
    SDL_Event event;

    Uint64 nCurr = SDL_GetPerformanceCounter();
    Uint64 nPrev = 0;
    float fDeltaTime = 0.0f;

    int nFrameRateSampleSize = 1000;
    float fFrameRateSum = 0.0f;
    int nFrameCount = 0;

    while (bGameRunning)
    {
        //SDL_ShowCursor(false);

        /* Process dt on update start */
        nPrev = nCurr;
        nCurr = SDL_GetPerformanceCounter();
        fDeltaTime = (double)((nCurr - nPrev) * 1000 / (double)SDL_GetPerformanceFrequency());

        /* Process frame rate */
        float fCurrFrameRate = 1.0f / (fDeltaTime * 0.001f);
        fFrameRateSum += fclamp(fCurrFrameRate, 0.0f, 20000.0f); // First frame yields very small fDeltaTime

        if (nFrameCount == nFrameRateSampleSize) {
            std::cout << "Frame rate: " << (int)(fFrameRateSum / nFrameRateSampleSize) << " FPS   \r";
            fFrameRateSum = 0.0f;
            nFrameCount = 0;
        }

        nFrameCount++;
    
        /* Poll and handle events */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) {
                bGameRunning = false;
            }
            handle_input(event, bMousePressed, bWpressed, bSpressed, bApressed, bDpressed);
        }
        
        /* x-axis movement */
        if (bDpressed && bApressed) {
            player.set_vel_x(0.0f);
        } else if (bDpressed) {
            player.set_vel_x(fMoveSpeed);
        } else if (bApressed) {
            player.set_vel_x(-fMoveSpeed);
        } else {
            player.set_vel_x(0.0f);
        }
        
        if (bWpressed && player.m_bOnFloor) {
            player.jump(2.0f);
        }
        
        /* Position update */
        player.update(fDeltaTime);

        /* Gravity */
        player.set_acc({ 0.0f, 0.005f });
        
        player.floor_collision(WINHT - g_nFloorHt);
        //FLAG(player.get_vel().y);

        /* Window update */
        SDL_RenderClear(g_hRend);
 
        if (!player.render_as_player_anim(fDeltaTime)) 
        {
            write_err("[ERROR] Could not render player texture.");
            return EXIT_FAILURE;
        }

        if (!platform.render_as_static_obj()) 
        {
            write_err("[ERROR] Could not render platform texture.");
            return EXIT_FAILURE;
        }

        if (!g_texFloor.render(0, WINHT - g_nFloorHt, NULL, 0.0, NULL, SDL_FLIP_NONE))
        {
            write_err("[ERROR] Could not render floor texture.");
            return EXIT_FAILURE;
        }

        SDL_SetRenderDrawColor(g_hRend, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(g_hRend, player.get_quad());

        // if (!g_texFont.render(0, 0, NULL, 0.0, NULL, SDL_FLIP_NONE)) 
        // {
        //     write_err("[ERROR] Could not render text texture.");
        //     return EXIT_FAILURE;
        // }

        SDL_SetRenderDrawColor(g_hRend, 0x90, 0xEE, 0x90, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(g_hRend);
    }

    cleanup();

    return EXIT_SUCCESS;
}