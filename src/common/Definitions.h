#pragma once
// Constants and enums shared by the player and the editor.

#include "GlInclude.h"

#include "VcgMath.h"

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QStringList>
#include <QTextStream>

namespace pang {

using namespace vcg;

// Ball tessellation (slices/stacks) per detail level
constexpr int BALL_DETAIL_LOW = 6;
constexpr int BALL_DETAIL_MEDIUM = 12;
constexpr int BALL_DETAIL_HIGH = 24;

// Number of sides of the three shadow circles
constexpr int SHADOW_DETAIL_LOW = 4;
constexpr int SHADOW_DETAIL_MEDIUM = 8;
constexpr int SHADOW_DETAIL_HIGH = 16;

// How high a ball may be (in multiples of its radius) above a surface
// for its shadow to be drawn with high/medium/low detail
constexpr float SHADOW_HIGH_MAX_HEIGHT = 5.0f;
constexpr float SHADOW_MEDIUM_MAX_HEIGHT = 7.5f;
constexpr float SHADOW_LOW_MAX_HEIGHT = 10.0f;

constexpr float PI = 3.14159265358979323846f;

// Difficulty levels (menu order). Many gameplay formulas use the number directly.
enum Difficulty { EASY = 0, MEDIUM = 1, HARD = 2, NIGHTMARE = 3, SUPERHUMAN = 4 };

// Detail levels (menu order)
enum Detail { DETAIL_LOW = 0, DETAIL_MEDIUM = 1, DETAIL_HIGH = 2 };

// Weapons
enum Weapon { HARPOON, QUAD_HARPOON, BOMB_BLAST, GUN };

// Power-ups
enum BonusType { BONUS_LIFE, BONUS_FRUIT, BONUS_WEAPON, BONUS_TORNADO, BONUS_BOMB };

// Values returned by the getType() methods of nodes, groups and objects
enum NodeType {
    GROUP, BALL_GROUP, PLAYER_GROUP, ENV_GROUP, NODE, TRANSFORMATION,
    TRANSLATION, ROTATION, SCALING, FLOOR, BOUNCE, MOVEMENT, FALL,
    PLAYER, BALL, WALL, TWISTER, RANDOM_MOVE, ENV_OBJECT,
    CELL, LADDER, ATTRACTION, BONUS_GROUP, BONUS, CLIMB
};

// Helpers to write level files
inline QDomElement addChild(QDomElement& parent, const QString& tag)
{
    QDomElement e = parent.ownerDocument().createElement(tag);
    parent.appendChild(e);
    return e;
}
inline void setAttr(QDomElement& e, const char* name, double value)
{
    e.setAttribute(QLatin1String(name), QString::number(value, 'g', 6));
}
inline void setAttr(QDomElement& e, const char* name, int value)
{
    e.setAttribute(QLatin1String(name), QString::number(value));
}
inline void setAttr(QDomElement& e, const char* name, bool value)
{
    e.setAttribute(QLatin1String(name), value ? QStringLiteral("1") : QStringLiteral("0"));
}
inline void setAttr(QDomElement& e, const char* name, const QString& value)
{
    e.setAttribute(QLatin1String(name), value);
}
// Without this overload a string literal would silently pick the bool one
inline void setAttr(QDomElement& e, const char* name, const char* value)
{
    e.setAttribute(QLatin1String(name), QLatin1String(value));
}

// Default background colour of the game (dark blue)
constexpr float CLEAR_COLOR[4] = {0.0f, 0.0f, 0.5f, 0.5f};

} // namespace pang
