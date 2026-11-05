#pragma once
// The player menu, drawn with OpenGL text. It is a small state machine:
// `state` is the (sub)menu shown and `pos` the highlighted entry.

#include "SceneGraph.h"
#include "TextWriter.h"

#include <SDL3/SDL_keycode.h>

namespace pang {

class Menu {
public:
    bool active = true;

    explicit Menu(SceneGraph* sg);
    // Must be called with a current OpenGL context
    void init() { writer.init(); }
    void draw();
    // Handles a key; returns false when the user chose "Quit"
    bool onKey(SDL_Keycode key);
    void setVideoSize(int w, int h) { writer.setVideoSize(w, h); }
    // Called when a game is over: show the hall of fame
    void showHallOfFame();
    // True once after the user started a game
    bool takeGameStarted();

private:
    enum State { MAIN, CHOOSE, DIFFICULTY, DETAIL, PICK_LEVEL, PICK_CAMPAIGN, HALL_OF_FAME, PLAYERS };

    void item(int index, int y, const QString& text, float scale = 2.0f);
    void mainMenu();
    void playersMenu();
    void chooseMenu();
    void difficultyMenu();
    void detailMenu();
    void fileMenu(bool campaigns);
    void hallOfFame();
    void evaluate();
    void clampPos(int last);
    static QString difficultyName(int d);

    TextWriter writer;
    SceneGraph* scene;
    State state = MAIN;
    int pos = 0;
    bool keepRunning = true;
    bool gameStarted = false;
    // Size helpers, recomputed at every draw
    float unit = 1.0f;
    int cx = 320, cy = 240;
    QStringList files;
};

} // namespace pang
