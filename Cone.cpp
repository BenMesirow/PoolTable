/*
 * Cone.cpp
 */
#include "Cone.h"

 void Cone::draw() {
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
    }
    for (float y = 0; y < m_segmentsY; ++y) {
        float lowerY = 0.5 - y / m_segmentsY;
        float upperY = 0.5 - (y+1.0) / m_segmentsY;
        float lowerR = transverseSliceRadiusCone(0.5,1.0,lowerY+0.5);
        float upperR = transverseSliceRadiusCone(0.5,1.0,upperY+0.5);
        for (int i = 0; i < m_segmentsX; ++i) {

            float lowerXL = ithSliceXComponent(lowerR,i,m_segmentsX);
            float upperXL = ithSliceXComponent(lowerR,i+1,m_segmentsX);
            float lowerZL = ithSliceZComponent(lowerR,i,m_segmentsX);
            float upperZL = ithSliceZComponent(lowerR,i+1,m_segmentsX);

            float lowerXU = ithSliceXComponent(upperR,i,m_segmentsX);
            float upperXU = ithSliceXComponent(upperR,i+1,m_segmentsX);
            float lowerZU = ithSliceZComponent(upperR,i,m_segmentsX);
            float upperZU = ithSliceZComponent(upperR,i+1,m_segmentsX);

            Vector norm((ithSliceXComponent(0.5,i,m_segmentsX) + 
                ithSliceXComponent(0.5,i+1,m_segmentsX))/2,
            0.25,
            (ithSliceZComponent(0.5,i,m_segmentsX) +
                ithSliceZComponent(0.5,i+1,m_segmentsX))/2);
            norm.normalize();

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerXL,lowerY,lowerZL);
            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(upperXL,lowerY,upperZL);
            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerXU,upperY,lowerZU);

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerXU,upperY,lowerZU);
            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(upperXL,lowerY,upperZL);
            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(upperXU,upperY,upperZU);
        }
    }
    glEnd();
}

void Cone::drawNormal()  {
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
    }
    for (float y = 0; y < m_segmentsY; ++y) {
        float YL = 0.5 - y / m_segmentsY;
        float YU = 0.5 - (y+1.0) / m_segmentsY;
        float lowerR = transverseSliceRadiusCone(0.5,1.0,YL+0.5);
        float upperR = transverseSliceRadiusCone(0.5,1.0,YU+0.5);
        for (int i = 0; i < m_segmentsX; ++i) {

            float lowerXL = ithSliceXComponent(lowerR,i,m_segmentsX);
            float upperXL = ithSliceXComponent(lowerR,i+1,m_segmentsX);
            float lowerZL = ithSliceZComponent(lowerR,i,m_segmentsX);
            float upperZL = ithSliceZComponent(lowerR,i+1,m_segmentsX);

            float lowerXU = ithSliceXComponent(upperR,i,m_segmentsX);
            float upperXU = ithSliceXComponent(upperR,i+1,m_segmentsX);
            float lowerZU = ithSliceZComponent(upperR,i,m_segmentsX);
            float upperZU = ithSliceZComponent(upperR,i+1,m_segmentsX);

            Vector norm((ithSliceXComponent(0.5,i,m_segmentsX) + 
                ithSliceXComponent(0.5,i+1,m_segmentsX))/2,
            0.25,
            (ithSliceZComponent(0.5,i,m_segmentsX) +
                ithSliceZComponent(0.5,i+1,m_segmentsX))/2);
            norm.normalize();

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerXL,YL,lowerZL);
            glVertex3f(upperXL,YL,upperZL);
            glVertex3f(lowerXU,YU,lowerZU);

            glNormal3f(norm[0],norm[1],norm[2]);
            glVertex3f(lowerXU,YU,lowerZU);
            glVertex3f(upperXL,YL,upperZL);
            glVertex3f(upperXU,YU,upperZU);
        }
    }
    glEnd();
}

double Cone::Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {
    Point  P = eyePointP;
    Vector d = rayV;

    double a = SQR(d[0]) + SQR(d[2]) - 0.25 * SQR(d[1]);
    double b = 2.0 * (P[0] * d[0] + P[2] * d[2] + 0.25 * (0.5 - P[1]) * d[1]);
    double c = SQR(P[0]) + SQR(P[2]) - 0.25 * SQR(0.5 - P[1]);

    double det = SQR(b) - 4.0 * a * c;

    double t = DBL_MAX;
    if (det >= 0){

        double t_cone = fmin((-b + sqrt(det)) / (2.0 * a), (-b - sqrt(det)) / (2.0 * a));
        Point isect = P + t_cone * d;

        if (t_cone > 0 && isect[1] > -0.5 && isect[1] < 0.5) {
            t = fmin(t, t_cone);
        }
    }

    double t_base = (-0.5 - P[1]) / d[1];
    Point isect = P + t_base * d;

    if (t_base > 0 && SQR(isect[0]) + SQR(isect[2]) < 0.25) {
        t = fmin(t, t_base);
    }

    return t;
}
Vector Cone::findIsectNormal(Point eyePoint, Vector ray, double dist) {
    Point isect = eyePoint + ray * dist;
    if (isect[1] == -0.5) {
        return Vector(0, -1, 0);
    }
    Vector norm = Vector(isect[0], 0.25 * (0.5 - isect[1]), isect[2]);
    norm.normalize();
    return norm;
}

