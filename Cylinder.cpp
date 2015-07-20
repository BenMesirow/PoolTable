/*
 * Cylinder.cpp
 */

#include "Cylinder.h"

void Cylinder::draw() {
    
    glBegin(GL_TRIANGLES);      

    for (int i = 0; i < m_segmentsX; ++i) {

        float lowerX = ithSliceXComponent(0.5,i,m_segmentsX);
        float upperX = ithSliceXComponent(0.5,i+1,m_segmentsX);
        float lowerZ = ithSliceZComponent(0.5,i,m_segmentsX);
        float upperZ = ithSliceZComponent(0.5,i+1,m_segmentsX);

            /* bottom base */
        glNormal3f(0, -1, 0);
        glVertex3f(0.0, -0.5, 0.0);
        glVertex3f(upperX,-0.5,upperZ);
        glVertex3f(lowerX,-0.5,lowerZ);

        for (float y = 0.0; y < m_segmentsY; ++y) {

            float lowerY = -0.5 + y / m_segmentsY;
            float upperY = -0.5 + (y + 1.0) / m_segmentsY; 

            Vector norm((lowerX+upperX)/2, 0, (lowerZ+upperZ)/2);
            norm.normalize();

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerX,lowerY,lowerZ);
            glVertex3f(upperX,lowerY,upperZ);
            glVertex3f(lowerX,upperY,lowerZ);

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerX,upperY,lowerZ);
            glVertex3f(upperX,lowerY,upperZ);
            glVertex3f(upperX,upperY,upperZ);
        }           

            /* top base */
        glNormal3f(0, 1, 0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(lowerX,0.5,lowerZ);
        glVertex3f(upperX,0.5,upperZ);
    }
    glEnd();
}

void Cylinder::drawNormal()  {
    glBegin(GL_TRIANGLES);      

    for (int i = 0; i < m_segmentsX; ++i) {

        float lowerX = ithSliceXComponent(0.5,i,m_segmentsX);
        float upperX = ithSliceXComponent(0.5,i+1,m_segmentsX);
        float lowerZ = ithSliceZComponent(0.5,i,m_segmentsX);
        float upperZ = ithSliceZComponent(0.5,i+1,m_segmentsX);

            /* bottom base */
        glNormal3f(0, -1, 0);
        glVertex3f(0.0, -0.5, 0.0);
        glVertex3f(upperX,-0.5,upperZ);
        glVertex3f(lowerX,-0.5,lowerZ);

        for (float y = 0.0; y < m_segmentsY; ++y) {

            float lowerY = -0.5 + y / m_segmentsY;
            float upperY = -0.5 + (y + 1.0) / m_segmentsY; 

            Vector norm((lowerX+upperX)/2, 0, (lowerZ+upperZ)/2);
            norm.normalize();

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerX,lowerY,lowerZ);
            glVertex3f(upperX,lowerY,upperZ);
            glVertex3f(lowerX,upperY,lowerZ);

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerX,upperY,lowerZ);
            glVertex3f(upperX,lowerY,upperZ);
            glVertex3f(upperX,upperY,upperZ);
        }           

            /* top base */
        glNormal3f(0, 1, 0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(lowerX,0.5,lowerZ);
        glVertex3f(upperX,0.5,upperZ);
    }
    glEnd();
}

double Cylinder::Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {

    Point  P = eyePointP;
    Vector d = rayV;

    double a = SQR(d[0]) + SQR(d[2]);
    double b = 2.0 * (P[0] * d[0] + P[2] * d[2]);
    double c = SQR(P[0]) + SQR(P[2]) - 0.25;

    double det = SQR(b) - 4.0 * a * c;

    if (det >= 0){
        double t = DBL_MAX;

        double t_cyl = fmin((-b + sqrt(det)) / (2.0 * a), (-b - sqrt(det)) / (2.0 * a));
        Point isect = P + t_cyl * d;

        if (t_cyl > 0 && isect[1] > -0.5 && isect[1] < 0.5) {
            t = fmin(t, t_cyl);
        }

        double t_plane = (-0.5 - P[1]) / d[1];
        isect = P + t_plane * d;

        if (t_plane >= 0 && SQR(isect[0]) + SQR(isect[2]) < 0.25) {
            t = fmin(t, t_plane);
        }
        
        t_plane = (0.5 - P[1]) / d[1];
        isect = P + t_plane * d;

        if (t_plane >= 0 && SQR(isect[0]) + SQR(isect[2]) < 0.25) {
            t = fmin(t, t_plane);
        }
        return t;
    }
    return DBL_MAX;
}

Vector Cylinder::findIsectNormal(Point eyePoint, Vector ray, double dist) {
    Point isect = eyePoint + ray * dist;
    if (isect[1] == -0.5) {
        return Vector(0, -1, 0);
    }
    else if (isect[1] == 0.5) {
        return Vector(0, 1, 0);
    }
    else {
        Vector norm = Vector(isect[0], 0, isect[2]);
        norm.normalize();
        return norm;
    }
}

