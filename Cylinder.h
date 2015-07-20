#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include "tess.h"

class Cylinder : public Shape {
public:
	void draw();
	void drawNormal();
    double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix);
    Vector findIsectNormal(Point eyePoint, Vector ray, double dist);
};

#endif
