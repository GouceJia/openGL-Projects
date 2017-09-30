#ifndef DRAW_HPP
#define DRAW_HPP

#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glu.h>
//#include <OpenGL/glu.h>
#include <glm/vec3.hpp>
using glm::vec3;
#define PI 3.1415926535897932384626433832795

namespace Draw {

    void axes();
    void unitCube();
    void unitSphere();
    // functions for carSoccer
    void drawFeild();
    void drawCar(vec3 position);
    void drawSoccer(vec3 position);
    void drawShadow(vec3 position);

    // Definitions below

    inline void axes() {
        glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex3f(0,-0.2,0);
        glVertex3f(1, 0.0,0);
        glVertex3f(0,+0.2,0);
        glColor3f(0,1,0);
        glVertex3f(0,0,-0.2);
        glVertex3f(0,1, 0.0);
        glVertex3f(0,0,+0.2);
        glColor3f(0,0,1);
        glVertex3f(-0.2,0,0);
        glVertex3f( 0.0,0,1);
        glVertex3f(+0.2,0,0);
        glEnd();
    }

    inline void unitCube() {
        vec3 verts[8] = {
            vec3(0,0,0),
            vec3(1,0,0),
            vec3(0,1,0),
            vec3(1,1,0),
            vec3(0,0,1),
            vec3(1,0,1),
            vec3(0,1,1),
            vec3(1,1,1)
        };
        int faces[6][4] = {
            {0,2,3,1},
            {0,4,6,2},
            {0,1,5,4},
            {4,5,7,6},
            {1,3,7,5},
            {2,6,7,3}
        };
        vec3 normals[6] = {
            vec3(0,0,-1),
            vec3(-1,0,0),
            vec3(0,-1,0),
            vec3(0,0,+1),
            vec3(+1,0,0),
            vec3(0,+1,0)
        };
        glBegin(GL_QUADS);
        for (int f = 0; f < 6; f++) {
            vec3 n = normals[f];
            glNormal3f(n.x,n.y,n.z);
            for (int i = 0; i < 4; i++) {
                vec3 v = verts[faces[f][i]];
                glVertex3f(v.x,v.y,v.z);
            }
        }
        glEnd();
    }

    inline void unitSphere() {
        static GLUquadric* quadric = gluNewQuadric();
        gluSphere(quadric, 1, 40, 40);
    }

    // draw the filed outlines
    inline void drawField() {
        // field lines
        glLineWidth(2.0);
        glColor3f(1.0, 1.0, 1.0);
        glLineStipple(1, 0x5555);

        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-40, 35, 50);
        glVertex3f(-40, 35, -50);
        glVertex3f(40, 35, -50);
        glVertex3f(40, 35, 50);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(-40, 0, 50);
        glVertex3f(-40, 35, 50);
        glVertex3f(40, 0, 50);
        glVertex3f(40, 35, 50);
        glVertex3f(40, 0, -50);
        glVertex3f(40, 35, -50);
        glVertex3f(-40, 0, -50);
        glVertex3f(-40, 35, -50);
        glEnd();
        glDisable(GL_LINE_STIPPLE);

        glBegin(GL_LINES);
        glVertex3f(-40, 0, 0);
        glVertex3f(40, 0, 0);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(-40, 0, 50);
        glVertex3f(-40, 0, -50);
        glVertex3f(40, 0, -50);
        glVertex3f(40, 0, 50);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex3f(-20, 0, -50);
        glVertex3f(-20, 0, -35);
        glVertex3f(20, 0, -35);
        glVertex3f(20, 0, -50);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex3f(-20, 0, 50);
        glVertex3f(-20, 0, 35);
        glVertex3f(20, 0, 35);
        glVertex3f(20, 0, 50);
        glEnd();

        //        glColor3f(0.2, 0.8, 0.6);
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i <= 100; i++){
            float angle = 2 * PI * i / 100;
            float x = cosf(angle) * 10;
            float y = sinf(angle) * 10;
            glVertex3f(x, 0, y);
        }
        glEnd();

        // the goals
        glColor3f(0.3, 0.6, 0.7);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_POLYGON);
        glVertex3f(-10, 0, -50);
        glVertex3f(-10, 10, -50);
        glVertex3f(10, 10, -50);
        glVertex3f(10, 0, -50);
        glEnd();
        glBegin(GL_LINES);
        for (int i = 0; i < 10; i++) {
            glVertex3f(-10, i, -50);
            glVertex3f(10, i, -50);
        }
        for (int j = 0; j < 10; j ++) {
            glVertex3f(j, 0, -50);
            glVertex3f(j, 10, -50);
            glVertex3f(-j, 0, -50);
            glVertex3f(-j, 10, -50);
        }
        glEnd();

        glColor3f(0.6, 0.6, 0.0);
        glBegin(GL_POLYGON);
        glVertex3f(-10, 0, 50);
        glVertex3f(-10, 10, 50);
        glVertex3f(10, 10, 50);
        glVertex3f(10, 0, 50);
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_LINES);
        for (int i = 0; i < 10; i++) {
            glVertex3f(-10, i, 50);
            glVertex3f(10, i, 50);
        }
        for (int j = 0; j < 10; j ++) {
            glVertex3f(j, 0, 50);
            glVertex3f(j, 10, 50);
            glVertex3f(-j, 0, 50);
            glVertex3f(-j, 10, 50);
        }
        glEnd();

    }

    inline void drawCar(vec3 position){
        // bottom
        glColor3f(0.8,0.2,0.2);
        //glColor3f(1, 1, 0);
        glBegin(GL_QUADS);
        glVertex3f(position.x+1.5, 0.1, position.z+2);
        glVertex3f(position.x-1.5, 0.1, position.z+2);
        glVertex3f(position.x-1.5, 0.1, position.z-2);
        glVertex3f(position.x+1.5, 0.1, position.z-2);
        glEnd();

        // top
        //glColor3f(0.8,0.2,0.2);
        glBegin(GL_QUADS);
        glVertex3f(position.x+1.5, 2, position.z+2);
        glVertex3f(position.x-1.5, 2, position.z+2);
        glVertex3f(position.x-1.5, 2, position.z-2);
        glVertex3f(position.x+1.5, 2, position.z-2);
        glEnd();

        //sides
        glColor3f(0, 0.3, 0.7);
        glBegin(GL_QUADS);
        glVertex3f(position.x+1.5, 0, position.z+2);
        glVertex3f(position.x+1.5, 2, position.z+2);
        glVertex3f(position.x+1.5, 2, position.z-2);
        glVertex3f(position.x+1.5, 0, position.z-2);
        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(position.x-1.5, 0, position.z+2);
        glVertex3f(position.x-1.5, 2, position.z+2);
        glVertex3f(position.x-1.5, 2, position.z-2);
        glVertex3f(position.x-1.5, 0, position.z-2);
        glEnd();

        //glColor3f(0.2, 0.9, 0.3);
        glBegin(GL_QUADS);
        glVertex3f(position.x-1.5, 0, position.z+2);
        glVertex3f(position.x+1.5, 0, position.z+2);
        glVertex3f(position.x+1.5, 2, position.z+2);
        glVertex3f(position.x-1.5, 2, position.z+2);
        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(position.x-1.5, 0, position.z-2);
        glVertex3f(position.x+1.5, 0, position.z-2);
        glVertex3f(position.x+1.5, 2, position.z-2);
        glVertex3f(position.x-1.5, 2, position.z-2);
        glEnd();
    }

    inline void drawSoccer(vec3 position){
        glTranslatef(position.x, position.y, position.z);
        GLUquadric* ball = gluNewQuadric();
        gluSphere(ball, 2, 40, 40);
        //glTranslated(-position.x, -position.y, -position.z);
    }

    void drawCircle(int radius, vec3 position) {
        glBegin(GL_TRIANGLE_FAN);
        for(int i = 0; i <= 60; i++){
            float angle = 2 * PI * i / 60;
            float x = cosf(angle) * radius;
            float y = sinf(angle) * radius;
            glVertex3f(x, 0, y);
        }
        glEnd();
    }

    // need fix
    inline void drawShadow(vec3 position) {
        glColor3f(0.7, 0.7, 0.7);
        glTranslated(0, -position.y + 0.3, 0);
        vec3 temp = vec3(position.x, 0, position.z);
        drawCircle(2, temp);

//        glPointSize(10);
//        glBegin(GL_POINT);
//        glVertex3f(position.x, 10, position.z);
//        glEnd();
    }

}

#endif
