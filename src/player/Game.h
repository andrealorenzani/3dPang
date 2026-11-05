#pragma once
// Keyboard control of the players during a game.

#include "SceneGraph.h"

#include <SDL3/SDL_keycode.h>

namespace pang {

class Game {
public:
    // Camera of each player: the default view or one of the F3-F8 views
    Point3i eye1, eye2;

    explicit Game(SceneGraph* sg);
    // Called when a game starts: back to the default views
    void resetViews();
    void onKey(SDL_Keycode key, bool pressed);

private:
    void up(bool pressed, PlayerGroup* g);
    void down(bool pressed, PlayerGroup* g);
    void left(bool pressed, PlayerGroup* g);
    void right(bool pressed, PlayerGroup* g);
    void fire(bool pressed, PlayerGroup* g);
    void climb(bool pressed, PlayerGroup* g, int direction);

    SceneGraph* scene;
};

} // namespace pang
