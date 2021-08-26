#include <iostream>

#include "physics_sprite.hpp"
#include "utils.hpp"


PhysicsSprite::PhysicsSprite(int x, int y, int wt, int ht)
{
    m_fvec2Pos = { (float)x, (float)y };
    m_nWt = wt;
    m_nHt = ht;
}

void PhysicsSprite::set_run_sprite(SpriteData s)
{
    m_sRunData = s;
}

void PhysicsSprite::set_idle_sprite(SpriteData s)
{
    m_sIdleData = s;
}

void PhysicsSprite::set_jump_sprite(SpriteData s)
{
    m_sJumpData = s;
}

void PhysicsSprite::set_fall_sprite(SpriteData s)
{
    m_sFallData = s;
}

void PhysicsSprite::set_static_texture(PNGTexture *t)
{
    m_pTexStatic = t;
}

void PhysicsSprite::set_collision_sprites(std::vector<PhysicsSprite> &sprite_vec)
{
    m_vecColliders = sprite_vec;
}

bool PhysicsSprite::render_as_player_anim(float dt)
{
    /* Process animation frame */

    if (m_bRunning && !(m_bJumping || m_bFalling)) /* Run */
    {
        if (m_sRunData.dt_accum >= m_sRunData.dt_inter) {
            m_sRunData.curr_tex_idx += 1;
            m_sRunData.dt_accum = 0.0f;
        }

        m_rectCurrClip = { (m_sRunData.curr_tex_idx % m_sRunData.nsprites) * m_sRunData.s_dimensions.x, 0, m_sRunData.s_dimensions.x, m_sRunData.s_dimensions.y};

        m_sRunData.dt_accum += dt;
        
        if (m_fvec2Vel.x > 0.0f) {
            m_bLastAnimEndedFacingRight = true;
        } else if (m_fvec2Vel.x < 0.0f) {
            m_bLastAnimEndedFacingRight = false;
        }

        return m_sRunData.sprite_sheet->render((int)m_fvec2Pos.x, (int)m_fvec2Pos.y, &m_rectCurrClip, 0.0, NULL, m_bLastAnimEndedFacingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
    } 

    else if (m_bJumping) /* Jump */
    {
        if (m_sJumpData.dt_accum >= m_sJumpData.dt_inter) {
            m_sJumpData.curr_tex_idx += 1;
            m_sJumpData.dt_accum = 0.0f;
        }
        m_rectCurrClip = { clamp(m_sJumpData.curr_tex_idx, 0, m_sJumpData.nsprites - 1) * m_sJumpData.s_dimensions.x, 0, m_sJumpData.s_dimensions.x, m_sJumpData.s_dimensions.y};

        m_sJumpData.dt_accum += dt;

        if (m_fvec2Vel.x > 0.0f) {
            m_bLastAnimEndedFacingRight = true;
        } else if (m_fvec2Vel.x < 0.0f) {
            m_bLastAnimEndedFacingRight = false;
        }

        return m_sJumpData.sprite_sheet->render((int)m_fvec2Pos.x, (int)m_fvec2Pos.y, &m_rectCurrClip, 0.0, NULL, m_bLastAnimEndedFacingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
    }

    else if (m_bFalling) /* Fall */
    {
        if (m_sFallData.dt_accum >= m_sFallData.dt_inter) {
            m_sFallData.curr_tex_idx += 1;
            m_sFallData.dt_accum = 0.0f;
        }

        m_rectCurrClip = { clamp(m_sFallData.curr_tex_idx, 0, m_sFallData.nsprites - 1) * m_sFallData.s_dimensions.x, 0, m_sFallData.s_dimensions.x, m_sFallData.s_dimensions.y};

        m_sFallData.dt_accum += dt;

        if (m_fvec2Vel.x > 0.0f) {
            m_bLastAnimEndedFacingRight = true;
        } else if (m_fvec2Vel.x < 0.0f) {
            m_bLastAnimEndedFacingRight = false;
        }

        return m_sFallData.sprite_sheet->render((int)m_fvec2Pos.x, (int)m_fvec2Pos.y, &m_rectCurrClip, 0.0, NULL, m_bLastAnimEndedFacingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

    }

    else  /* Idle */
    { 
        if (m_sIdleData.dt_accum >= m_sIdleData.dt_inter) {
            m_sIdleData.curr_tex_idx += 1;
            m_sIdleData.dt_accum = 0.0f;
        }

        m_rectCurrClip = { (m_sIdleData.curr_tex_idx % m_sIdleData.nsprites) * m_sIdleData.s_dimensions.x, 0, m_sIdleData.s_dimensions.x, m_sIdleData.s_dimensions.y};
    
        m_sIdleData.dt_accum += dt;

        return m_sIdleData.sprite_sheet->render((int)m_fvec2Pos.x, (int)m_fvec2Pos.y, &m_rectCurrClip, 0.0, NULL, m_bLastAnimEndedFacingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
    }
}

bool PhysicsSprite::render_as_static_obj()
{
    return m_pTexStatic->render((int)m_fvec2Pos.x, (int)m_fvec2Pos.y, NULL, 0.0, NULL, SDL_FLIP_NONE);
}

void PhysicsSprite::update(float dt)
{
    /* Update sprite states */
    m_bRunning = fabs(m_fvec2Vel.x) > 0.0f;
    m_bJumping = m_fvec2Vel.y < 0.0f;
    m_bFalling = m_fvec2Vel.y > 0.0f;
    m_bOnFloor = m_fvec2Vel.y == 0.0f;

    /* Update velocity with acceleration */
    m_fvec2Vel.x += m_fvec2Acc.x * dt;
    m_fvec2Vel.y += m_fvec2Acc.y * dt;
    /* Update position with velocity */
    m_fvec2Pos.y += m_fvec2Vel.y * dt;
    m_fvec2Pos.x += m_fvec2Vel.x * dt;

    /* Update position based on velocity and collisions */
    SDL_Rect *collisionRect = AABB_collide();
    if (collisionRect != NULL)
    {
        if (m_fvec2Vel.y > 0.0f) 
        {
            m_fvec2Pos.y = collisionRect->y - m_nHt;
            m_fvec2Vel.y = 0.0f;
        }
        else if (m_fvec2Vel.y < 0.0f)
        {
            m_fvec2Pos.y = collisionRect->y + collisionRect->h;
            m_fvec2Vel.y = 0.0001f;
        }
    }
}

void PhysicsSprite::floor_collision(int y)
{
    if ((int)(m_fvec2Pos.y) + m_nHt >= y)
    {
        m_fvec2Vel.y = 0.0f;
        m_fvec2Pos.y = y - m_nHt;
    }
}

SDL_Rect *PhysicsSprite::AABB_collide()
{
    for (PhysicsSprite &other: m_vecColliders) // Iterate by reference
    {
        SDL_Rect *r1 = this->get_quad(); // For clarification :)
        SDL_Rect *r2 = other.get_quad();
        if (r1->x < r2->x + r2->w && r1->x + r1->w > r2->x && r1->y < r2->y + r2->h && r1->y + r1->h >= r2->y) {
            return r2;
        }
        delete r1;
        delete r2;
    }

    return NULL;
}

void PhysicsSprite::jump(float impulse_vel)
{
    set_vel_y(-impulse_vel);
}

void PhysicsSprite::set_pos(fvec2 p)
{
    m_fvec2Pos = p;
}

void PhysicsSprite::set_vel_x(float v)
{
    m_fvec2Vel.x = v;
}

void PhysicsSprite::set_vel_y(float v)
{
    m_fvec2Vel.y = v;
}

void PhysicsSprite::set_acc(fvec2 a)
{
    m_fvec2Acc = a;
}

fvec2 PhysicsSprite::get_pos()
{
    return m_fvec2Pos;
}

fvec2 PhysicsSprite::get_vel()
{
    return m_fvec2Vel;
}

fvec2 PhysicsSprite::get_acc()
{
    return m_fvec2Acc;
}

SDL_Rect *PhysicsSprite::get_quad()
{
    SDL_Rect *ret = new SDL_Rect;
    ret->x = (int)m_fvec2Pos.x;
    ret->y = (int)m_fvec2Pos.y;
    ret->w = m_nWt;
    ret->h = m_nHt;
    return ret;
}
