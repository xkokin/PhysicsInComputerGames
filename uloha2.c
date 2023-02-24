//Hlib Kokin id:117991

#include <GL/glu.h>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<stdio.h>

const int icaskrok = 25;
float posunXa = 0.0;
float posunXk = 0.0;
float va = 0.02;
float vk = 0.01;
FILE* file;
int closed = 0;
const float Lmax = 4.0;
/*float matrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        posunX, posunY, 0, 1
};*/


void aktualizuj(const int ihod){
    // najedeme cas za ktory achilles stretne korytnacku
    //kde 0.7 je naskok korytnacky a va - vk je rozdiel medzi rychlostiami Achilesa a korytnacky
    if(ihod==(0.7/(va-vk)) && closed != 1){
        fclose(file);
        closed = 1;
    }
    // kazdych 100 ms vypiseme do suboru udaje
    if(ihod % 4 == 0 && closed != 1){
        //zapiseme cas, nech nasa scena je 100 metrov, vtedy polohu achilesa a korytnacky nasobime 50m
        fprintf(file, "%f\t%f\t%f\n",0.025*ihod, (posunXa)*50, (0.7+posunXk)*50);
    }

    if(posunXk >= 1.3 || posunXa >= 2.0) {
        posunXa = 0.0;
        posunXk = 0.0;
    }

    printf("aktualizuj ihod: %d\t", ihod);
    printf("rychlost pohybu: %f\n", (posunXa*1000*Lmax)/icaskrok);
    posunXa += va;
    posunXk += vk;
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void Achilles_Korytnacka(){
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(posunXa, 0, 0);
    glScalef(1, 1, 1);

    //glLoadMatrix(mat);
    //gluOrtho2D(-2.0, 2.0, -1.0, 1.0);
    //Achilles
    glBegin(GL_QUADS);

    glColor3f(1.0,0.0,0.0);
    glVertex2f(-1.0, 0.0 );

    glColor3f(0.5,0.5,0.0);
    glVertex2f(-0.9 , 0.0 );

    glColor3f(0.0,0.5,0.5);
    glVertex2f(-0.9 , 0.5 );

    glColor3f(0.0,0.0,1.0);
    glVertex2f(-1.0 , 0.5 );


    glEnd();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(posunXk, 0, 0);
    glScalef(1, 1, 1);

    //Korytnacka
    glBegin(GL_QUADS);

    glColor3f(1.0,0.0,0.0);
    glVertex2f(-0.3, 0.0 );

    glColor3f(0.5,0.5,0.0);
    glVertex2f(-0.1 , 0.0 );

    glColor3f(0.0,0.5,0.5);
    glVertex2f(-0.1 , 0.2 );

    glColor3f(0.0,0.0,1.0);
    glVertex2f(-0.3 , 0.2 );

    glEnd();

    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska){
    glViewport(0, 0, sirka, vyska);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(sirka == 0) sirka++;
    const float pomstr = ((float)vyska)/sirka;
    gluOrtho2D(-0.2*Lmax, 0.2*Lmax, -0.2*Lmax*pomstr, 0.2*Lmax*pomstr);

}

int main(int argc, char **argv){
    file = fopen("uloha2.txt","w");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(200,150);
    glutCreateWindow ("FYPH/Kokin: Achilles a Korytnacka");

    glutDisplayFunc(Achilles_Korytnacka);
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 0);
    glutMainLoop();
    return 0;
}