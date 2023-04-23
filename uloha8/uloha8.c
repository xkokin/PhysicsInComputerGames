//
// Created by glebk on 13.04.2023.
//

#include <GL/glu.h>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<stdio.h>
#include <math.h>
#include <unistd.h>

const int icaskrok = 100; // 10 milisekund cas onovenia
const float g = 9.80665; //m/s^2
const float ay = -9.80665;
const float rho = 1.26;
const float m = 10.0;


float S, r, C, z0, zsur, zmax,
        vz0, o = 75.0,
        td = 0, vzd = 0,
        vter,
        t = 0,
        omega = 0;


FILE* file_txt; // zapisujem data do dvoch suborov, txt na odovzdanie a dat na vytvorenie grafu kedze
// ja nemozem spravit gnuplot z txt
FILE* file_dat;

float zOdT(float tz) {
//    if (vz0 == 0) {
//        return z0 - ((vter * vter) / g) * log(cosh((g * tz) / vter));
//    }
//    else{
//        return z0 + ((vter * vter) / g) * log(cosh((g * tz) / vter));
//    }
    return z0 - ((vter * vter) / g) * log(cosh((g * tz) / vter));
}


float vOdTaZ(){
    if (vz0 != 0 && vz0 < 0) {
        zsur = zOdT(t);
        return (vz0 - vter * tanh((g * t) / vter)) / (1 - (vz0 / vter) * tanh((g * t) / vter));
    }
    else if (vz0 != 0 && vz0 > 0){
        double v = vz0;
        double z = z0;
        for ( double curr_t = 0; curr_t < t; curr_t += 0.1){
            double a = -g - copysign(0.5 * C * rho * S * v * v / m, v);
            v += a * 0.1;
            z += v * 0.1;
        }
        zsur = z;
        return v;
    }
    else {

        zsur = zOdT(t);
        //return -1 * vter * tanh((g * t) / vter);
        return -1 * sqrt(1 - exp(-1 * ((C * S * rho) / m) * (z0 - zsur))) * vter;
    }
}


void aktualizuj(const int ihod){
    float vz = 0;
    t += 0.1; //cas od zaciatku;
    omega += o * 0.1;
    omega = fmod(omega, 360.0f);

    //if (t >= tmax && atHighest != 1) atHighest = 1;

    if(zsur < 0) {
        fclose(file_txt);
        fclose(file_dat);
        printf("td = %.2f s, vter = %.2f m\n", td, vter);
        return;
    }


    printf("time in seconds: %.2fs, zsur = %.2f m\n", t, zsur);

    vz = vOdTaZ();
    //zsur = zOdT(t);
    if (vz0 != 0) {
        fprintf(file_txt, "%.2f\t%.2f\t%.2f\n", t, vz, zsur);
        fprintf(file_dat, "%.2f\t%.2f\t%.2f\n", t, vz, zsur);
    }
    else {
        fprintf(file_txt, "%.2f\t%.2f\t%.2f\n", t, vz, zsur);
        fprintf(file_dat, "%.2f\t%.2f\t%.2f\n", t, vz, zsur);
    }
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void Aerodynamics(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDisable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(40, -40, zmax/2, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    //------------------------------------------------------

    glPopMatrix();


    //------------------------------------------------------
    glLineWidth(6.0f);
    glBegin(GL_LINES);


    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 100+S, 0);
    glVertex3f(0, -100-r, 0);

    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(100, 0, 0);
    glVertex3f(-100-r, 0, 0);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, zmax*2+r);
    glVertex3f(0, 0, -zmax*2-r);

    glEnd();


    glPushMatrix();

    glLineWidth(1.0f);


    glTranslatef(25, -25, zsur);
    glRotatef(omega, 0.0, 0.0, 1.0);
    glScalef(r, r, r);

    glBegin(GL_QUADS);

    // Front face (red) to z
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face (blue) to 0 from z
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Left face (cyan) to - x
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Right face (red) to x
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Top face (magenta) to +y
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Bottom face (cyan)to 0 from y
    glColor3f(0.7f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();

    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska){
    glViewport(0, 0, sirka, vyska);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float pomstr = (float)sirka/(float)vyska;

    float theta = atan(100 / zmax) * 180 / M_PI * 2;
    //printf("theta: %.2f\n", theta);
    gluPerspective(theta, pomstr, 0.1, zmax*3);
//    gluPerspective(theta, pomstr, zmax, zmax*3);


}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set clear color to black
    glEnable(GL_DEPTH_TEST); // Enable depth testing
}


int main(int argc, char **argv){
    scanf("%f %f %f %f", &z0, &vz0, &S, &C);
    r = sqrt(S);
    zsur = z0;
    if (vz0 == 0) zmax = zsur;
    else zmax = zsur*2;

    file_txt = fopen("uloha8_2.txt","w");
    file_dat = fopen("uloha8_2.dat","w");

    vter = sqrt((2*m*g) / (C * S * rho)); //terminalna rychlost

    td = (vter / g) * acosh(exp((g * z0) / (vter * vter)));

    vzd = -1 * vter * sqrt(1 - exp(-1 * ((2 * g * z0) / (vter * vter))));

    printf("zsur = %.2fm, vz0 = %.2fm/s, C = %.2f, S = %.2f, m = %.2f,\n"
           "td = %.2f s, vzd = %.2f m/s, vter = %.2f m/s\n", zsur, vz0, C, S, m, td, vzd, vter);

    fprintf(file_txt, "#-Hlavicka-#\n"
                      "# z0 = %.2fm, vz0 = %.2fm/s, C = %.2f, S = %.2f, m = %.2f,\n"
                      " td = %.2f s, vzd = %.2f m/s, vter = %.2f m/s"
                      "\n#----------#\n", zsur, vz0, C, S, m, td, vzd, vter);


    if (vz0 != 0) {
        fprintf(file_txt, "%.2f\t%.2f\n", t, vz0);
        fprintf(file_dat, "%.2f\t%.2f\n", t, vz0);
    }
    else {
        fprintf(file_txt, "%.2f\t%.2f\t%.2f\n", t, vz0, zsur);
        fprintf(file_dat, "%.2f\t%.2f\t%.2f\n", t, vz0, zsur);
    }



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1000, 500);
    glutInitWindowPosition(0, 100);
    glutCreateWindow ("FYPH/Kokin: Sikmy Vrh v 3D");
    init();
    glutDisplayFunc(Aerodynamics);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}