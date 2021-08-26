#ifndef PHYSICS_SPRITE_H
#define PHYSICS_SPRITE_H

#include <vector>

#include "vec2.hpp"
#include "texture.hpp"

class PhysicsSprite
{
    public:
        PhysicsSprite(int x, int y, int wt, int ht);

        void set_run_sprite(SpriteData s);
        void set_idle_sprite(SpriteData s);
        void set_jump_sprite(SpriteData s);
        void set_fall_sprite(SpriteData s);
        void set_static_texture(PNGTexture *t);

        void set_collision_sprites(std::vector<PhysicsSprite> &sprite_vec);

        bool render_as_player_anim(float dt);
        bool render_as_static_obj();

        void update(float dt);
        void floor_collision(int y);
        SDL_Rect *AABB_collide();

        void jump(float impulse_vel);

        void set_pos(fvec2 p);
        void set_vel_x(float v);
        void set_vel_y(float v);
        void set_acc(fvec2 a);

        fvec2 get_pos();
        fvec2 get_vel();
        fvec2 get_acc();
        SDL_Rect *get_quad();

        bool m_bRunning = false;
        bool m_bOnFloor = false;
        bool m_bFalling = false;
        bool m_bJumping = false;
        bool m_bLastAnimEndedFacingRight = true;

    private:
        fvec2 m_fvec2Pos;
        fvec2 m_fvec2Vel;
        fvec2 m_fvec2Acc;
        int m_nWt, m_nHt;

        SpriteData m_sRunData;
        SpriteData m_sIdleData;
        SpriteData m_sJumpData;
        SpriteData m_sFallData;

        PNGTexture *m_pTexStatic;
        SDL_Rect m_rectCurrClip = { 0, 0, 32, 32 };

        std::vector<PhysicsSprite> m_vecColliders;
};



#endif /* PHYSICS_SPRITE_H */