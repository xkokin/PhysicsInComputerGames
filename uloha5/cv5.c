//
// Created by glebk on 16.03.2023.
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const float Lmax = 0.8;

void obsluhaResize(int sirka, int vyska)
{
    glViewport(0, 0, sirka, vyska);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (sirka == 0) sirka++;
    const float pomstr = ((float)sirka)/vyska;
    // doplniť
    gluPerspective(85, pomstr, 10, 20);
    glOrtho(-0.4, 0.4, 0, 0.8, -10, 20);
}

void kresliRovnobezky3D()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glColor3f(0.0, 0.0, 0.0);

    // doplniť
    glBegin(GL_LINES);
    glVertex3f(-0.2, -0.4, -10);
    glVertex3f(-0.2, -0.4, -12);
    glVertex3f(0.2, -0.4, -10);
    glVertex3f(0.2, -0.4, -12);
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(200, 150);
    glutCreateWindow("OpenGL: rovnobezky");
    glutDisplayFunc(kresliRovnobezky3D);
    glutReshapeFunc(obsluhaResize);
    glClearColor(1.0, 1.0, 1.0, 0);
    glutMainLoop();
    return 0;
}