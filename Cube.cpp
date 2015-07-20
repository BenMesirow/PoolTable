/*
 * Cube.cpp
 */
#include "Cube.h"

 void Cube::draw()  {
    Point p1, p2, p3, p4;

    glBegin(GL_QUADS);

                    /* front face */
            p1 = Point(-0.5,-0.5,0.5);
            p2 = Point(0.5,-0.5,0.5);
            p3 = Point(0.5,0.5,0.5);
            p4 = Point(-0.5,0.5,0.5);

            glNormal3f(0,0,1);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p3[0], p3[1], p3[2]);
            glVertex3f(p4[0], p4[1], p4[2]);

                    /* right face */
            p1 = Point(0.5,-0.5,0.5);
            p2 = Point(0.5,-0.5,-0.5);
            p3 = Point(0.5,0.5,-0.5);
            p4 = Point(0.5,0.5,0.5);

            glNormal3f(1,0,0);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p3[0], p3[1], p3[2]);
            glVertex3f(p4[0], p4[1], p4[2]);

                   /* back face */
            p1 = Point(0.5,-0.5,-0.5);
            p2 = Point(-0.5,-0.5,-0.5);
            p3 = Point(-0.5,0.5,-0.5);
            p4 = Point(0.5,0.5,-0.5);

            glNormal3f(0,0,-1);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p3[0], p3[1], p3[2]);
            glVertex3f(p4[0], p4[1], p4[2]);

                    /* left face */
            p1 = Point(-0.5,-0.5,-0.5);
            p2 = Point(-0.5,-0.5,0.5);
            p3 = Point(-0.5,0.5,0.5);
            p4 = Point(-0.5,0.5,-0.5);

            glNormal3f(-1,0,0);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p3[0], p3[1], p3[2]);
            glVertex3f(p4[0], p4[1], p4[2]);

                    /* top face */
            p1 = Point(-0.5,0.5,0.5);
            p2 = Point(0.5,0.5,0.5);
            p3 = Point(0.5,0.5,-0.5);
            p4 = Point(-0.5,0.5,-0.5);

            glNormal3f(0,1,0);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p3[0], p3[1], p3[2]);
            glVertex3f(p4[0], p4[1], p4[2]);

                    /* bottom face */
            p1 = Point(-0.5,-0.5,0.5);
            p2 = Point(-0.5,-0.5,-0.5);
            p3 = Point(0.5,-0.5,-0.5);
            p4 = Point(0.5,-0.5,0.5);

            glNormal3f(0,-1,0);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glVertex3f(p3[0], p3[1], p3[2]);
            glVertex3f(p4[0], p4[1], p4[2]);

    glEnd();
}

void Cube::drawNormal()  {
    glBegin(GL_LINES);
    for (float x = 0; x < m_segmentsX; ++x) {
        for (float y = 0; y < m_segmentsY; ++y) {
                    /* front face */
            glVertex3f(-0.5+x/m_segmentsX,-0.5+y/m_segmentsY,0.5);
            glVertex3f(-0.5+x/m_segmentsX,-0.5+y/m_segmentsY,0.5);
                    /* right face */
            glVertex3f(0.5,-0.5+y/m_segmentsY,0.5-x/m_segmentsX);
            glVertex3f(0.5,-0.5+y/m_segmentsY,0.5-x/m_segmentsX);
                    /* back face */
            glVertex3f(0.5-x/m_segmentsX,-0.5+y/m_segmentsY,-0.5);
            glVertex3f(0.5-x/m_segmentsX,-0.5+y/m_segmentsY,-0.5);
                    /* left face */
            glVertex3f(-0.5,-0.5+y/m_segmentsY,-0.5+x/m_segmentsX);
            glVertex3f(-0.5,-0.5+y/m_segmentsY,-0.5+x/m_segmentsX);
        }
        for (float z = 0; z < m_segmentsX; ++z) {
                    /* top face */
            glVertex3f(-0.5+x/m_segmentsX,0.5,0.5-z/m_segmentsX);
            glVertex3f(-0.5+x/m_segmentsX,0.5,0.5-z/m_segmentsX);
                    /* bottom face */
            glVertex3f(-0.5+x/m_segmentsX,-0.5,-0.5+z/m_segmentsX);
            glVertex3f(-0.5+x/m_segmentsX,-0.5,-0.5+z/m_segmentsX);
        }
    }
    glEnd();
}

double Cube::Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {

    Point P = transformMatrix * eyePointP;
    Vector d = transformMatrix * rayV;

    double t = MIN_ISECT_DISTANCE;
    Point isect;
    Vector norm;

    /* right face, x = 0.5 */
    double t_plane = (0.5 - P[0]) / d[0];
    isect = P + t_plane * d;

    if (t_plane >= 0 &&
        isect[1] < 0.5 && isect[1] > -0.5 && 
        isect[2] < 0.5 && isect[2] > -0.5) {
        t = fmin(t, t_plane);
    }

    /* top face, y = 0.5 */
    t_plane = (0.5 - P[1]) / d[1];
    isect = P + t_plane * d;

    if (t_plane >= 0 &&
        isect[0] < 0.5 && isect[0] > -0.5 && 
        isect[2] < 0.5 && isect[2] > -0.5) {
        t = fmin(t, t_plane);
    }

    /* back face, z = 0.5 */
    t_plane = (0.5 - P[2]) / d[2];
    isect = P + t_plane * d;

    if (t_plane >= 0 &&
        isect[1] < 0.5 && isect[1] > -0.5 && 
        isect[0] < 0.5 && isect[0] > -0.5) {
        t = fmin(t, t_plane);
    }

    /* left face, x = -0.5 */
    t_plane = (-0.5 - P[0]) / d[0];
    isect = P + t_plane * d;

    if (t_plane >= 0 &&
        isect[1] < 0.5 && isect[1] > -0.5 && 
        isect[2] < 0.5 && isect[2] > -0.5) {
        t = fmin(t, t_plane);
    }

    /* bottom face, y = -0.5 */
    t_plane = (-0.5 - P[1]) / d[1];
    isect = P + t_plane * d;

    if (t_plane >= 0 &&
        isect[0] < 0.5 && isect[0] > -0.5 && 
        isect[2] < 0.5 && isect[2] > -0.5) {
        t = fmin(t, t_plane);
    }

    /* front face, z = -0.5 */
    t_plane = (-0.5 - P[2]) / d[2];
    isect = P + t_plane * d;

    if (t_plane >= 0 &&
        isect[1] < 0.5 && isect[1] > -0.5 && 
        isect[0] < 0.5 && isect[0] > -0.5) {
        t = fmin(t, t_plane);
    }
    return t;
}

Vector Cube::findIsectNormal(Point eyePoint, Vector ray, double dist) {

    Point isect = eyePoint + dist * ray;
    Vector norm = isect - ORIGIN;
    int longest = 0;

    for (int i = 1; i < 3; ++i) {
        if (fabs(norm[i]) > fabs(norm[longest])) {
            norm[longest] = 0;
            longest = i;
        }
        else {
            norm[i] = 0;
        }
    }
    return norm;
}
