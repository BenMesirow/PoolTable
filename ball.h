/*
 * ball.h
 */
#ifndef POOL_BALL_H
#define POOL_BALL_H

#include "physics.h"
#include "SceneData.h"

#define BALL_RADIUS 0.5
#define BALL_MASS 0.170 //kg

enum {
    /* solid balls */
    SOLID_YELLOW,
    SOLID_BLUE,
    SOLID_RED,
    SOLID_PURPLE,
    SOLID_ORANGE,
    SOLID_GREEN,
    SOLID_BURGUNDY,
    /* striped balls */
    STRIPED_YELLOW,
    STRIPED_BLUE,
    STRIPED_RED,
    STRIPED_PURPLE,
    STRIPED_ORANGE,
    STRIPED_GREEN,
    STRIPED_BURGUNDY,
    /* special balls */
    SOLID_WHITE,
    SOLID_BLACK
};

class Ball {
public:
    Ball();
    Ball(int color);

    Vector velocity;
    Vector vAngular;

    Point getPosition();

    void setMaterial(double r, double g, double b);
    bool isInHole();
    void fallIntoHole();

    // make private later
    SceneMaterial material;
    Point position;

private:
    bool inHole;
    double mass;
    double radius;
};

#endif