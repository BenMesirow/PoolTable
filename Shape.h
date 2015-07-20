#ifndef SHAPE_H
#define SHAPE_H

#include <GLUI/glui.h>
#include "Algebra.h"
#include <iostream>

#define MIN_ISECT_DISTANCE 30
/*
#define RAD 0.0174532925199432954743716805978692718782
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#define DEG_TO_RAD(a)	(a*RAD)
*/
class Shape {
public:

	Shape() {
		setSegments(20,20);
	};
	~Shape() {};

	void setSegments(int x, int y);

	virtual void draw() {};
	virtual void drawNormal() {};

	virtual double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix)=0;
	virtual Vector findIsectNormal(Point eyePoint, Vector ray, double dist)=0;

protected:
	void normalizeNormal (float x, float y, float z);
	void normalizeNormal (Vector v);

	int m_segmentsX, m_segmentsY;
};

#endif