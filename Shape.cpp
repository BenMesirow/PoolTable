/*
 * Shape.cpp
 */
#include "Shape.h"

void Shape::setSegments(int x, int y) {
        m_segmentsX = x;
        m_segmentsY = y;
}

void Shape::normalizeNormal (float x, float y, float z) {
    normalizeNormal (Vector(x, y, z));
}

void Shape::normalizeNormal (Vector v) {
    v.normalize();
    glNormal3dv(v.unpack());
}
