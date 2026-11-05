#include "Menu.h"

#include "DataPaths.h"

#include <QDir>
#include <QFileInfo>

namespace pang {

namespace {
const char* const kDifficulties[] = {"Easy", "Medium", "Hard", "Nightmare", "Superhuman"};
const char* const kDetails[] = {"Low", "Medium", "High"};
} // namespace

Menu::Menu(SceneGraph* sg) : scene(sg)
{
    // Preselect the classic campaign, if it is there
    if (QFileInfo::exists(paths::resolve(QStringLiteral("campaigns/classic.campaign"))))
        scene->selectedCampaign = QStringLiteral("classic.campaign");
}

QString Menu::difficultyName(int d)
{
    return (d >= 0 && d < 5) ? QString::fromLatin1(kDifficulties[d]) : QStringLiteral("?");
}

bool Menu::takeGameStarted()
{
    const bool started = gameStarted;
    gameStarted = false;
    return started;
}

void Menu::clampPos(int last)
{
    if (pos > last)
        pos = last;
    if (pos < 0)
        pos = 0;
}

// Draws one entry centred on the screen, highlighted when selected
void Menu::item(int index, int y, const QString& text, float scale)
{
    const bool selected = (index == pos);
    writer.setHCentered(true);
    if (selected)
        writer.prepare(0, 1, 1, 1, cx, cy + int(y * unit), 0, unit * scale, unit * scale);
    else
        writer.prepare(0.5f, 0.5f, 1, 1, cx, cy + int(y * unit), 0, unit * scale, unit * scale);
    writer.write(selected ? QStringLiteral("[ ") + text + QStringLiteral(" ]") : text);
    writer.setHCentered(false);
}

void Menu::draw()
{
    const int w = writer.getVideoWidth();
    const int h = writer.getVideoHeight();
    // Text is sized for 640 pixels and scaled with the window
    unit = float(w) / 640.0f;
    cx = w / 2;
    cy = h / 2;

    // The menu is an empty OpenGL scene with text. Reset what a bomb may have left.
    glViewport(0, 0, w, h);
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    glColorMask(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    writer.prepare(1.0f, 1.0f, 1.0f, 0.7f, 0, h - 5, 0.0f, 0.8f, 0.8f);
    writer.write(QStringLiteral("(C) 2004-2005 Andrea Lorenzani and Eva Tamburini"));
    writer.prepare(1.0f, 1.0f, 1.0f, 0.7f, 0, h - 18, 0.0f, 0.8f, 0.8f);
    writer.write(QStringLiteral("Inspired by the famous game \"Pang\" (C) Mitchell - 1989"));
    writer.prepare(1.0f, 1.0f, 1.0f, 0.7f, 0, h - 31, 0.0f, 0.8f, 0.8f);
    writer.write(QStringLiteral("Project for the \"Costruzione di Interfacce\" course - version 2.0"));

    switch (state) {
    case CHOOSE: chooseMenu(); break;
    case DIFFICULTY: difficultyMenu(); break;
    case DETAIL: detailMenu(); break;
    case PICK_LEVEL: fileMenu(false); break;
    case PICK_CAMPAIGN: fileMenu(true); break;
    case HALL_OF_FAME: hallOfFame(); break;
    case PLAYERS: playersMenu(); break;
    case MAIN:
    default: mainMenu(); break;
    }
}

bool Menu::onKey(SDL_Keycode key)
{
    switch (key) {
    case SDLK_DOWN: pos++; break;
    case SDLK_UP: pos--; break;
    case SDLK_RETURN:
    case SDLK_KP_ENTER: evaluate(); break;
    case SDLK_ESCAPE:
        // One level back
        if (state == CHOOSE || state == DIFFICULTY || state == DETAIL || state == PLAYERS || state == HALL_OF_FAME) {
            state = MAIN;
            pos = 0;
        } else if (state == PICK_LEVEL || state == PICK_CAMPAIGN) {
            state = CHOOSE;
            pos = 0;
        }
        break;
    default: break;
    }
    return keepRunning;
}

void Menu::mainMenu()
{
    clampPos(5);
    item(0, 60, QStringLiteral("New Game"));
    item(1, 20, QStringLiteral("Choose Level or Campaign"));
    item(2, -20, QStringLiteral("Difficulty: ") + difficultyName(scene->difficulty));
    item(3, -60, QStringLiteral("Detail: ") + QString::fromLatin1(kDetails[qBound(0, scene->detail, 2)]));
    item(4, -100, QStringLiteral("Hall of Fame"));
    item(5, -140, QStringLiteral("Quit"));
}

void Menu::playersMenu()
{
    clampPos(2);
    item(0, 40, QStringLiteral("One Player"));
    item(1, 0, QStringLiteral("Two Players"));
    item(2, -40, QStringLiteral("Back to the main menu"));
}

void Menu::chooseMenu()
{
    clampPos(2);
    const QString none = QStringLiteral("(none)");
    item(0, 60, QStringLiteral("Load a level"));
    item(1, 0, QStringLiteral("Load a campaign"));
    item(2, -80, QStringLiteral("Back to the main menu"));
    writer.setHCentered(true);
    writer.prepare(1, 1, 1, 0.5f, cx, cy + int(35 * unit), 0, unit * 1.25f, unit * 1.25f);
    writer.write(scene->selectedLevel.isEmpty() ? none : scene->selectedLevel);
    writer.prepare(1, 1, 1, 0.5f, cx, cy - int(25 * unit), 0, unit * 1.25f, unit * 1.25f);
    writer.write(scene->selectedCampaign.isEmpty() ? none : scene->selectedCampaign);
    writer.setHCentered(false);
}

void Menu::difficultyMenu()
{
    clampPos(5);
    for (int i = 0; i < 5; ++i)
        item(i, 80 - 40 * i, QString::fromLatin1(kDifficulties[i]));
    item(5, -140, QStringLiteral("Back to the main menu"));
}

void Menu::detailMenu()
{
    clampPos(3);
    for (int i = 0; i < 3; ++i)
        item(i, 40 - 40 * i, QString::fromLatin1(kDetails[i]));
    item(3, -100, QStringLiteral("Back to the main menu"));
}

void Menu::fileMenu(bool campaigns)
{
    // Up to 5 entries above and below the highlighted one
    const QDir dir(paths::resolve(campaigns ? QStringLiteral("campaigns") : QStringLiteral("levels")));
    files = dir.entryList({campaigns ? QStringLiteral("*.campaign") : QStringLiteral("*.xml")}, QDir::Files,
                          QDir::Name | QDir::IgnoreCase);
    writer.setHCentered(true);
    if (!dir.exists() || files.isEmpty()) {
        writer.prepare(1, 1, 1, 0.5f, cx, cy, 0, unit * 2, unit * 2);
        writer.write(!dir.exists() ? QStringLiteral("The data folder is missing")
                                   : (campaigns ? QStringLiteral("No campaigns") : QStringLiteral("No levels")));
        writer.setHCentered(false);
        return;
    }
    clampPos(int(files.size()) - 1);
    const int first = qMax(0, pos - 5);
    const int last = qMin(int(files.size()), pos + 6);
    for (int i = first; i < last; ++i) {
        const QString name = QFileInfo(files[i]).completeBaseName();
        item(i, -40 * (i - pos), name);
    }
    writer.setHCentered(false);
}

void Menu::hallOfFame()
{
    const int delta = int(20 * unit);
    writer.prepare(0.5f, 0.5f, 1, 1, cx - int(200 * unit) - delta, cy + int(180 * unit), 0, unit * 1.5f, unit * 1.5f);
    writer.write(QStringLiteral("Player"));
    writer.prepare(0.5f, 0.5f, 1, 1, cx - int(10 * unit) - delta, cy + int(180 * unit), 0, unit * 1.5f, unit * 1.5f);
    writer.write(QStringLiteral("Difficulty"));
    writer.prepare(0.5f, 0.5f, 1, 1, cx + int(120 * unit) + delta, cy + int(180 * unit), 0, unit * 1.5f, unit * 1.5f);
    writer.write(QStringLiteral("Score"));

    const QList<HighScore> list = SceneGraph::loadHighScores();
    int y = 150;
    int rank = 1;
    for (const HighScore& s : list) {
        writer.prepare(0.5f, 0.5f, 1, 1, cx - int(200 * unit) - delta, cy + int(y * unit), 0, unit * 1.5f, unit * 1.5f);
        writer.write(QStringLiteral("%1. Player %2").arg(rank).arg(s.player));
        writer.prepare(0.5f, 0.5f, 1, 1, cx - int(10 * unit) - delta, cy + int(y * unit), 0, unit * 1.5f, unit * 1.5f);
        writer.write(difficultyName(s.difficulty));
        writer.prepare(0.5f, 0.5f, 1, 1, cx + int(120 * unit) + delta, cy + int(y * unit), 0, unit * 1.5f, unit * 1.5f);
        writer.write(QString::number(s.points));
        y -= 30;
        rank++;
    }
    if (list.isEmpty()) {
        writer.setHCentered(true);
        writer.prepare(1, 1, 1, 0.5f, cx, cy + int(100 * unit), 0, unit * 1.5f, unit * 1.5f);
        writer.write(QStringLiteral("No scores yet"));
        writer.setHCentered(false);
    }
    pos = 0;
    item(0, -200, QStringLiteral("Back to the main menu"));
}

void Menu::showHallOfFame()
{
    active = true;
    state = HALL_OF_FAME;
    pos = 0;
}

void Menu::evaluate()
{
    switch (state) {
    case MAIN:
        switch (pos) {
        case 0: state = PLAYERS; pos = 0; break;
        case 1: state = CHOOSE; pos = 0; break;
        case 2: state = DIFFICULTY; pos = scene->difficulty; break;
        case 3: state = DETAIL; pos = scene->detail; break;
        case 4: state = HALL_OF_FAME; pos = 0; break;
        case 5: keepRunning = false; break;
        default: break;
        }
        break;
    case CHOOSE:
        if (pos == 0) { state = PICK_LEVEL; pos = 0; }
        else if (pos == 1) { state = PICK_CAMPAIGN; pos = 0; }
        else { state = MAIN; pos = 1; }
        break;
    case DIFFICULTY:
        if (pos < 5)
            scene->difficulty = pos;
        state = MAIN;
        pos = 2;
        break;
    case DETAIL:
        if (pos < 3)
            scene->detail = pos;
        state = MAIN;
        pos = 3;
        break;
    case PICK_LEVEL:
        if (pos < files.size()) {
            scene->selectedLevel = files[pos];
            scene->selectedCampaign.clear();
        }
        state = CHOOSE;
        pos = 0;
        break;
    case PICK_CAMPAIGN:
        if (pos < files.size()) {
            scene->selectedCampaign = files[pos];
            scene->selectedLevel.clear();
        }
        state = CHOOSE;
        pos = 1;
        break;
    case HALL_OF_FAME:
        state = MAIN;
        pos = 4;
        break;
    case PLAYERS:
        if (pos == 2) {
            state = MAIN;
            pos = 0;
            break;
        }
        scene->numPlayers = pos + 1;
        if (scene->startGame()) {
            active = false;
            gameStarted = true;
        } else {
            // Nothing to play: choose a level first
            state = CHOOSE;
            pos = 0;
        }
        break;
    }
}

} // namespace pang
