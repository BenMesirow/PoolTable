/*
 * balls.cpp
 */
#include "ball.h"


Ball::Ball() {
    velocity = Vector(0,0,0);
    radius = BALL_RADIUS;
    mass = BALL_MASS;
    vAngular = Vector(0,0,0);
    inHole = false;
    setMaterial(0.25,0,1.0);
};

Ball::Ball(int color) {
    velocity = Vector(0,0,0);
    radius = BALL_RADIUS;
    mass = BALL_MASS;
    vAngular = Vector(0,0,0);
    inHole = false;

    double SQRT_3 = sqrt(3.0);

    switch(color) {
        case SOLID_YELLOW:
            position = Point(-SQRT_3,0.5,-2);
            setMaterial(1.0,1.0,0.0);
            break;
        case SOLID_BLUE:
            position = Point(-SQRT_3,0.5,-1);
            setMaterial(0.0,0.0,1.0);
            break;
        case SOLID_RED:
            position = Point(-SQRT_3,0.5,0);
            setMaterial(1.0,0,0);
            break;
        case SOLID_PURPLE:
            position = Point(-SQRT_3,0.5,1);
            setMaterial(0.5,0,1.0);
            break;
        case SOLID_ORANGE:
            position = Point(-SQRT_3,0.5,2);
            setMaterial(1.0,0.5,0);
            break;
        case SOLID_GREEN:
            position = Point(-SQRT_3 * 0.5,0.5,-1.5);
            setMaterial(0,1,0);
            break;
        case SOLID_BURGUNDY:
            position = Point(-SQRT_3 * 0.5,0.5,-0.5);
            setMaterial(1.0,.4,.4);
            break;
        case STRIPED_YELLOW:
            position = Point(-SQRT_3 * 0.5,0.5,0.5);
            setMaterial(0.5,0.5,0);
            break;
        case STRIPED_BLUE:
            position = Point(-SQRT_3 * 0.5,0.5,1.5);
            setMaterial(0,0,0.5);
            break;
        case STRIPED_RED:
            position = Point(0,0.5,-1);
            setMaterial(0.5,0,0);
            break;
        case STRIPED_PURPLE:
            position = Point(0,0.5,0);
            setMaterial(0.25,0,0.5);
            break;
        case STRIPED_ORANGE:
            position = Point(0,0.5,1);
            setMaterial(0.5,0.25,0);
            break;
        case STRIPED_GREEN:
            position = Point(SQRT_3*0.5,0.5,-0.5);
            setMaterial(0,0.5,0);
            break;
        case STRIPED_BURGUNDY:
            position = Point(SQRT_3*0.5,0.5,0.5);
            setMaterial(0.5,0.2,0.2);
            break;
        case SOLID_BLACK:
            position = Point(SQRT_3,0.5,0);
            setMaterial(0,0,0);
            break;
        case SOLID_WHITE:
            position = Point(6,0.5,0);
            setMaterial(1,1,1);
            break;
    };
};

Point Ball::getPosition() {
    return position;
};

void Ball::setMaterial(double r, double g, double b) {
    material.cAmbient.r = material.cDiffuse.r = r;
    material.cAmbient.g = material.cDiffuse.g = g;
    material.cAmbient.b = material.cDiffuse.b = b;

    material.cSpecular.r = material.cSpecular.g = material.cSpecular.b = 1.0;
    material.cEmissive.r = material.cEmissive.g = material.cEmissive.b = 0.0;
    material.shininess = 3;

};

bool Ball::isInHole() {
    return inHole;
};

void Ball::fallIntoHole() {
    inHole = true;
};