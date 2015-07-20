/*
 * pool.cpp
 */
#include "pool.h"

PoolGame::PoolGame() {

    //double SQRT_3 = sqrt(3.0);

    for (int i = 0; i < NUM_BALLS; ++i) {
        balls.push_back(Ball(i));
    }

    /*
    balls[SOLID_YELLOW].position = Point(-SQRT_3,0.5,-2);
    balls[SOLID_YELLOW].setMaterial(1.0,1.0,0.0);

    balls[SOLID_BLUE].position = Point(-SQRT_3,0.5,-1);
    balls[SOLID_BLUE].setMaterial(0.0,0.0,1.0);

    balls[SOLID_RED].position = Point(-SQRT_3,0.5,0);
    balls[SOLID_RED].setMaterial(1.0,0,0);

    balls[SOLID_PURPLE].position = Point(-SQRT_3,0.5,1);
    balls[SOLID_PURPLE].setMaterial(0.5,0,1.0);

    balls[SOLID_ORANGE].position = Point(-SQRT_3,0.5,2);
    balls[SOLID_ORANGE].setMaterial(1.0,0.5,0);

    balls[SOLID_GREEN].position = Point(-SQRT_3 * 0.5,0.5,-1.5);
    balls[SOLID_GREEN].setMaterial(0,1,0);

    balls[SOLID_BURGUNDY].position = Point(-SQRT_3 * 0.5,0.5,-0.5);
    balls[SOLID_BURGUNDY].setMaterial(1.0,.4,.4);

    balls[STRIPED_YELLOW].position = Point(-SQRT_3 * 0.5,0.5,0.5);
    balls[STRIPED_YELLOW].setMaterial(0.5,0.5,0);

    balls[STRIPED_BLUE].position = Point(-SQRT_3 * 0.5,0.5,1.5);
    balls[STRIPED_BLUE].setMaterial(0,0,0.5);
    balls[STRIPED_RED].position = Point(0,0.5,-1);
    balls[STRIPED_RED].setMaterial(0.5,0,0);

    balls[STRIPED_PURPLE].position = Point(0,0.5,0);
    balls[STRIPED_PURPLE].setMaterial(0.25,0,0.5);

    balls[STRIPED_ORANGE].position = Point(0,0.5,1);
    balls[STRIPED_ORANGE].setMaterial(0.5,0.25,0);
    balls[STRIPED_GREEN].position = Point(SQRT_3*0.5,0.5,-0.5);
    balls[STRIPED_GREEN].setMaterial(0,0.5,0);

    balls[STRIPED_BURGUNDY].position = Point(SQRT_3*0.5,0.5,0.5);
    balls[STRIPED_BURGUNDY].setMaterial(0.5,0.2,0.2);
    balls[SOLID_WHITE].position = Point(6,0.5,0);
    balls[SOLID_WHITE].setMaterial(1,1,1);

    balls[SOLID_BLACK].position = Point(SQRT_3,0.5,0);
    balls[SOLID_BLACK].setMaterial(0,0,0);*/
}

void PoolGame::reset() {

    for (int i = 0; i < NUM_BALLS; ++i) {
        balls[i] = Ball(i);
    }
}

void PoolGame::strikeBall(int i, Vector applied_force, double time) {
    Vector total_momentum = BALL_MASS * balls[i].velocity + time * applied_force;
    balls[i].velocity = total_momentum / BALL_MASS;
    //balls[i].vAngular = cross(Vector(0,BALL_RADIUS,0),balls[i].velocity);
    updateMotion(time);
}

void PoolGame::updateMotion(double secs) {
    for (int i = 0; i < balls.size(); ++i) {
            if (balls[i].isInHole())
                continue;
 
            balls[i].position = balls[i].position + (balls[i].velocity * secs);
            if (checkHole(i))
                continue;

            Vector friction = -balls[i].velocity;
            friction.normalize();

            friction = MU_K * friction;

            balls[i].velocity = balls[i].velocity + friction * secs;
    }
    for (int i = 0; i < balls.size(); ++i) {
        checkCollision(i);
    }
}

bool PoolGame::checkCollision(int ball) {
    tableCollision(ball);
    for (int i = 0; i < balls.size(); ++i) {
        if (ball == i || balls[i].isInHole())
            continue;
        ballCollision(ball,i);
    }
    return true;
}

bool PoolGame::ballCollision(int ball1, int ball2) {

    if (ball1 == ball2) return false;

    Point x1 = Point(balls[ball1].position);
    Point x2 = Point(balls[ball2].position);

    Vector v1 = Vector(balls[ball1].velocity);
    Vector v2 = Vector(balls[ball2].velocity);

    Vector d1 = x1 - x2;
    Vector d2 = x2 - x1;

    if (d1.length() <= 2.0 * BALL_RADIUS && dot(v1,d2) > 0) {
        
        balls[ball1].velocity = v1 - dot(v1-v2,d1) / SQR(d1.length()) * d1;
        balls[ball1].vAngular = balls[ball1].vAngular + cross(Vector(0,BALL_RADIUS,0),balls[ball1].velocity);

        balls[ball2].velocity = v2 - dot(v2-v1,d2) / SQR(d2.length()) * d2;
        balls[ball2].vAngular = balls[ball2].vAngular + cross(Vector(0,BALL_RADIUS,0),balls[ball2].velocity);
        return true;
    }
    return false;
}

bool PoolGame::tableCollision(int ball) {
    Point p = balls[ball].position;
    Vector v = balls[ball].velocity;
    
    bool collision = false;

    if (p[0] >= 7.5 && v[0] > 0) {
        balls[ball].velocity[0] = -v[0];
        collision = true;
    } else if (p[0] <= -7.5 && v[0] < 0) {
        collision = true;
        balls[ball].velocity[0] = -v[0];
    } else if (p[2] >= 3.5 && v[2] > 0) {
        collision = true;
        balls[ball].velocity[2] = -v[2];
    } else if (p[2] <= -3.5 && v[2] < 0) {
        collision = true;
        balls[ball].velocity[2] = - v[2];
    }

    if (collision) {
        balls[ball].velocity = balls[ball].velocity * 0.75; // 75% elasticity
        balls[ball].vAngular = balls[ball].vAngular + cross(Vector(0,BALL_RADIUS,0),balls[ball].velocity);
        return true;
    }

    return false;
}

bool PoolGame::checkHole(int ball) {
    if (   (balls[ball].position - Point(0,1,-3.25)).length() < 0.75
        || (balls[ball].position - Point(0,1,3.25)).length() < 0.75
        || (balls[ball].position - Point(-7.25,1,-3.25)).length() < 0.75
        || (balls[ball].position - Point(-7.25,1,3.25)).length() < 0.75
        || (balls[ball].position - Point(7.25,1,-3.25)).length() < 0.75
        || (balls[ball].position - Point(7.25,1,3.25)).length() < 0.75 )
    {
        balls[ball].fallIntoHole();
        return true;
    }
    return false;
}
