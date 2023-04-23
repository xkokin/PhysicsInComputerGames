//
// Created by glebk on 20.04.2023.
//
//Hlib Kokin id:117991


#include <GL/glu.h>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<stdio.h>
#include <math.h>
#include <unistd.h>

const int icaskrok = 10; // 10 milisekund cas onovenia
const float g = 9.80665; //m/s^2
const float ay = -9.80665;
const float PI = 3.14159265358979323846;
const int num_segments = 200; // pre kreslenie kruhu
const float anglePerSegment = 2.0 * 3.14159265358979323846 / 100;
const float Lxmax = 100;
const float Lymax = 70;


float r, ysur0, xsur0, ysur, xsur,
        v0, vy, vx, v,
        ymax, xmax,
        tymax, txmax,
        vmax,
        t = 0.00, tn[4],
        dt = 0.1;


int atHighest; // boolean premenna ktora povie ci objekt uz bol v najvyssom bode

FILE* file_txt; // zapisujem data do dvoch suborov, txt na odovzdanie a dat na vytvorenie grafu kedze
// ja nemozem spravit gnuplot z txt
FILE* file_dat;
void calculateCollision() {
    if (vx > 0 && vy > 0) {
        tn[0] = (0.5 * Lxmax - (xsur + r)) / vx; // right side
        tn[1] = (0.5 * Lymax - (xsur + r))/ vy; // top side
        tn[2] = -1;
        tn[3] = -1;
    }
    else if (vx > 0 && vy < 0) {
        tn[0] = (0.5 * Lxmax - (xsur + r)) / vx; // right side
        tn[3] = (-0.5 * Lymax - (ysur - r)) / vy; // bottom side
        tn[2] = -1;
        tn[1] = -1;
    }
    else if (vx < 0 && vy > 0) {
        tn[2] = (-0.5 * Lxmax - (xsur - r)) / vx; // left side
        tn[1] = (0.5 * Lymax - (xsur + r)) / vy; // top side
        tn[0] = -1;
        tn[3] = -1;
    }
    else if (vx < 0 && vy < 0) {
        tn[2] = (-0.5 * Lxmax - (xsur - r)) / vx; // left side
        tn[3] = (-0.5 * Lymax - (ysur - r)) / vy; // bottom side
        tn[0] = -1;
        tn[1] = -1;
    }
}


void aktualizuj(const int ihod){
    float closest = 999999;
    t += dt; //cas od zaciatku


    printf("time in seconds: %.2fs\n", t);

    xsur = xsur + vx*dt;
    ysur = ysur + vy*dt;

    if (xsur + r >= 0.5 * Lxmax || xsur - r <= -0.5 * Lxmax) {
        vx = -1 * vx;
        calculateCollision();
        for (int i = 0; i < 4; i ++) {
            if (tn[i] != -1 && tn[i] < closest) {
                closest = tn[i];
            }
        }
        dt = round(closest / dt) + 1;
        dt = closest / dt;
    }
    if (ysur + r >= 0.5 * Lymax || ysur - r <= -0.5 * Lymax) {
        vy = -1 * vy;
        calculateCollision();
        for (int i = 0; i < 4; i ++) {
            if (tn[i] != -1 && tn[i] < closest) {
                closest = tn[i];
            }
        }
        dt = round(closest / dt) + 1;
        dt = closest / dt;

    }


    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void Sikmy_Vrh(){

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(6.0f);
    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex2f(0.5*Lxmax, 0.5*Lymax);
    glVertex2f(-0.5*Lxmax, 0.5*Lymax);

    glVertex2f(-0.5*Lxmax, 0.5*Lymax);
    glVertex2f(-0.5*Lxmax, -0.5*Lymax);

    glVertex2f(-0.5*Lxmax, -0.5*Lymax);
    glVertex2f(0.5*Lxmax, -0.5*Lymax);

    glVertex2f(0.5*Lxmax, -0.5*Lymax);
    glVertex2f(0.5*Lxmax, 0.5*Lymax);

    glEnd();

    glLoadIdentity();

    glTranslatef(xsur, ysur, 0);

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        float theta = i / anglePerSegment;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glColor3f(0, 0, 0);
        glVertex2f(x, y);
    }
    glEnd();

    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska) {


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float pomstr = (float)vyska/(float)sirka;

    if ((0.5 * Lymax) / vyska > (0.5 * Lxmax) / sirka) {
        gluOrtho2D((-.5 * Lymax) / pomstr, (0.5 * Lymax) / pomstr, -0.5 * Lymax, 0.5 * Lymax);
    }
    else if ((0.5 * Lymax) / vyska < (0.5 * Lxmax) / sirka) {
        gluOrtho2D(-0.5 * Lxmax, 0.5 * Lxmax, (-0.5 * Lxmax) * pomstr, (0.5 * Lxmax) * pomstr);
    }
    //gluOrtho2D(0, 2, 0, 2);
    glViewport(0, 0, sirka, vyska);
}



int main(int argc, char **argv){

    scanf("%f %f %f %f %f", &ysur, &xsur, &vy, &vx, &r);
    ysur0 = ysur;
    xsur0 = xsur;
//
//    file_txt = fopen("cv10.txt","w");
//    file_dat = fopen("cv10.dat","w");



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 100);
    glutCreateWindow ("FYPH/Kokin: Cv10");

    glutDisplayFunc(Sikmy_Vrh);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}