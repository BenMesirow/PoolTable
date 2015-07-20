#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
public:
	
	void draw();
	void drawNormal();
    double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix);
    Vector findIsectNormal(Point eyePoint, Vector ray, double dist);
};
#endif
