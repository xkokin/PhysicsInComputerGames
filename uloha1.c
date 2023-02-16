#include <GL/glu.h>
#include<GL/glut.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<stdio.h>

const int icaskrok = 25;
float posunX = 0.0;
float posunY = 0.0;

void aktualizuj(const int ihod){
	
	if(posunX >= 2.0 || posunY >= 2.0) {
		posunX = 0.0;
		posunY = 0.0;
	}
	
    printf("aktualizuj ihod: %d\n", ihod);
    posunX += 0.02;
    posunY += 0.02;
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void myFunc(){
    glClear(GL_COLOR_BUFFER_BIT);




    //gluOrtho2D(-2.0, 2.0, -1.0, 1.0);

    glBegin(GL_QUADS);
    	glColor3f(1.0,0.0,0.0);
    	glVertex2f(-1.0 + posunX, -1.0 + posunY);

    	glColor3f(0.5,0.5,0.0);
    	glVertex2f(-0.5 + posunX, -1.0 + posunY);

    	glColor3f(0.0,0.5,0.5);
    	glVertex2f(-0.5 + posunX, -0.5 + posunY);

	glColor3f(0.0,0.0,1.0);
    	glVertex2f(-1.0 + posunX, -0.5 + posunY);

    glEnd();

    glutSwapBuffers();

}

const float Lmax = 4.0;

void obsluhaResize(int sirka, int vyska){
    glViewport(0, 0, sirka, vyska);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(sirka == 0) sirka++;
    const float pomstr = ((float)vyska)/sirka;
    gluOrtho2D(-0.2*Lmax, 0.2*Lmax, -0.2*Lmax*pomstr, 0.2*Lmax*pomstr);

}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200,150);
    glutCreateWindow ("OpenGL: Ahoj, ja som okno!");

    glutDisplayFunc(myFunc);
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 0);
    glutMainLoop();
    return 0;
}