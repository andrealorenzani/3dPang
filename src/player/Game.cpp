#include "Game.h"

namespace pang {

Game::Game(SceneGraph* sg) : eye1(sg->defaultView), eye2(sg->defaultView), scene(sg) {}

void Game::resetViews()
{
    eye1 = scene->defaultView;
    eye2 = scene->defaultView;
}

// The model faces +Z at 0 degrees; the rotation follows the walking direction,
// including diagonals when two arrows are pressed.

void Game::up(bool pressed, PlayerGroup* g)
{
    if (pressed) {
        if (g->movement->vx == 0) {
            g->player->setRunning();
            g->rotation->angle = 180.0f;
        } else {
            g->rotation->angle = g->movement->vx > 0 ? 135.0f : 225.0f;
        }
        g->movement->vz = -(g->movement->startVz);
    } else {
        g->movement->vz = 0;
        if (g->movement->vx == 0)
            g->player->setStand();
        else
            g->rotation->angle = g->movement->vx > 0 ? 90.0f : 270.0f;
    }
}

void Game::down(bool pressed, PlayerGroup* g)
{
    if (pressed) {
        if (g->movement->vx == 0) {
            g->player->setRunning();
            g->rotation->angle = 0.0f;
        } else {
            g->rotation->angle = g->movement->vx > 0 ? 45.0f : 315.0f;
        }
        g->movement->vz = g->movement->startVz;
    } else {
        g->movement->vz = 0;
        if (g->movement->vx == 0)
            g->player->setStand();
        else
            g->rotation->angle = g->movement->vx > 0 ? 90.0f : 270.0f;
    }
}

void Game::left(bool pressed, PlayerGroup* g)
{
    if (pressed) {
        if (g->movement->vz == 0) {
            g->player->setRunning();
            g->rotation->angle = 270.0f;
        } else {
            g->rotation->angle = g->movement->vz > 0 ? 315.0f : 225.0f;
        }
        g->movement->vx = -(g->movement->startVx);
    } else {
        g->movement->vx = 0;
        if (g->movement->vz == 0)
            g->player->setStand();
        else
            g->rotation->angle = g->movement->vz > 0 ? 0.0f : 180.0f;
    }
}

void Game::right(bool pressed, PlayerGroup* g)
{
    if (pressed) {
        if (g->movement->vz == 0) {
            g->player->setRunning();
            g->rotation->angle = 90.0f;
        } else {
            g->rotation->angle = g->movement->vz > 0 ? 45.0f : 135.0f;
        }
        g->movement->vx = g->movement->startVx;
    } else {
        g->movement->vx = 0;
        if (g->movement->vz == 0)
            g->player->setStand();
        else
            g->rotation->angle = g->movement->vz > 0 ? 0.0f : 180.0f;
    }
}

void Game::fire(bool pressed, PlayerGroup* g)
{
    if (!pressed) {
        if (g->movement->vx != 0 || g->movement->vz != 0)
            g->player->setRunning();
        else
            g->player->setStand();
        return;
    }
    // At "easy" and "medium" difficulty players can shoot even when dead
    if (scene->difficulty >= HARD && g->player->dead && g->dying)
        return;
    g->player->setAttack();
    if (g->maxShots - g->shotsInPlay <= 0)
        return;
    auto* shot = new ProjectileGroup(g);
    // Shots get shorter and less detailed with difficulty and detail level
    switch (g->weapon) {
    case HARPOON:
        shot->projectile = new HarpoonShot(2.0f + (0.3f * float(scene->difficulty)), 5 + (5 * scene->detail));
        break;
    case QUAD_HARPOON:
        shot->projectile = new QuadHarpoonShot(2.0f + (0.3f * float(scene->difficulty)), 5 + (5 * scene->detail));
        break;
    case GUN:
    default:
        shot->projectile = new GunShot(2.5f + 0.5f * float(scene->difficulty), 20.0f - 2.0f * float(scene->difficulty));
        break;
    }
    scene->shots.push_front(shot);
    g->shotsInPlay++;
}

void Game::climb(bool pressed, PlayerGroup* g, int direction)
{
    if (!pressed) {
        g->climb->stop();
        return;
    }
    if (!g->nearLadder)
        return;
    if (direction == 1)
        g->climb->up();
    else
        g->climb->down();
}

void Game::onKey(SDL_Keycode key, bool pressed)
{
    PlayerGroup* g1 = scene->player1;
    PlayerGroup* g2 = scene->player2;
    switch (key) {
    // Player 1
    case SDLK_UP: if (g1) up(pressed, g1); break;
    case SDLK_DOWN: if (g1) down(pressed, g1); break;
    case SDLK_LEFT: if (g1) left(pressed, g1); break;
    case SDLK_RIGHT: if (g1) right(pressed, g1); break;
    case SDLK_M: if (g1) fire(pressed, g1); break;
    case SDLK_J: if (g1) climb(pressed, g1, 1); break;
    case SDLK_N: if (g1) climb(pressed, g1, -1); break;
    // Player 2 (in 2005 R/F moved player 1 by mistake)
    case SDLK_W: if (g2) up(pressed, g2); break;
    case SDLK_S: if (g2) down(pressed, g2); break;
    case SDLK_A: if (g2) left(pressed, g2); break;
    case SDLK_D: if (g2) right(pressed, g2); break;
    case SDLK_Q: if (g2) fire(pressed, g2); break;
    case SDLK_R: if (g2) climb(pressed, g2, 1); break;
    case SDLK_F: if (g2) climb(pressed, g2, -1); break;
    // Views: F2 (fixed), F3-F5 for player 1; F6-F8, F9 (fixed) for player 2
    case SDLK_F2: if (g1 && pressed) eye1 = scene->defaultView; break;
    case SDLK_F3: if (g1 && pressed) eye1 = scene->views[0].eye; break;
    case SDLK_F4: if (g1 && pressed) eye1 = scene->views[1].eye; break;
    case SDLK_F5: if (g1 && pressed) eye1 = scene->views[2].eye; break;
    case SDLK_F6: if (g2 && pressed) eye2 = scene->views[3].eye; break;
    case SDLK_F7: if (g2 && pressed) eye2 = scene->views[4].eye; break;
    case SDLK_F8: if (g2 && pressed) eye2 = scene->views[5].eye; break;
    case SDLK_F9: if (g2 && pressed) eye2 = scene->defaultView; break;
    default: break;
    }
}

} // namespace pang
