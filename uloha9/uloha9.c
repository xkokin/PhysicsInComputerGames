#include <GL/glu.h>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<stdio.h>
#include <math.h>
#include <unistd.h>

const int icaskrok = 10; // 10 milisekund cas onovenia
const float g = 9.80665; // m/s^2
const float ay = -9.80665;
const float M = 0.02886; // molova hmotnost pre vzduch
const float R = 8.31; // molova plynova konstanta
const float rho = 1.26; // hustota vzduchu
const float p = 101325; // normalny tlak
const float T = 293; // termodynamicka teplota

const float C = 0.3;
const float m = 10;


float r, ysur0, ysur, xsur, zsur, dy,
        v0z, v0y, v0x, v0, v, o,
        vz, vy, dvy, vx, vter, vzd, vd,
        ymax, xmax = 0, zmax,
        thmax, tdmax,
        vmax, vzmax, vxmax,
        t = 0, dt = 0.01,
        alfa, azimut, omega = 0.0,
        S, K, k;


FILE* file_txt; // zapisujem data do dvoch suborov, txt na odovzdanie a dat na vytvorenie grafu kedze
// ja nemozem spravit gnuplot z txt
FILE* file_dat;


void aktualizuj(const int ihod){
    t += 0.01; //cas od zaciatku;
    omega += o * 0.01;
    omega = fmod(omega, 360.0f);



    if(ysur < 0) {
        fclose(file_txt);
        fclose(file_dat);
        printf("td = %.2f s, xsur = %.2f m, ysur = %.2f m, zmax = %.2f m\n", tdmax, zsur, xsur, ymax);
        return;
    }


    printf("time in seconds: %.2fs\n", t);

    vz = vz + dt * (-K * exp(-k * ysur) * fabs(v) * vz);
    vx = vx + dt * (-K * exp(-k * ysur) * fabs(v) * vx);
    vy = vy + dt * (-g - K * exp(-k * ysur) * fabs(v) * vy);

    v = sqrt(vz*vz + vx*vx + vy*vy);

    zsur = zsur + dt * vz;
    xsur = xsur + dt * vx;
    ysur = ysur + dt * vy;


    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, zsur, xsur, ysur, vz, vx, vy);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, zsur, xsur, ysur, vz, vx, vy);
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void Sikmy_Vrh(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDisable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(zmax*4, xmax, ymax/2, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    //------------------------------------------------------


    glLineWidth(6.0f);
    glBegin(GL_LINES);


    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, ymax+r, 0);
    glVertex3f(0, -ymax-r, 0);

    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(xmax+r, 0, 0);
    glVertex3f(-xmax-r, 0, 0);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, zmax+r);
    glVertex3f(0, 0, -zmax-r);

    glEnd();


    glPushMatrix();

    glLineWidth(1.0f);


    glTranslatef(zsur, xsur, ysur);
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

    float theta = atan((xmax+r) / ymax) * 180 / M_PI * 2;
    printf("theta: %.2f\n", theta);
    gluPerspective(theta, pomstr, 0.1, zmax*5);
//    gluPerspective(theta, pomstr, zmax, zmax*3);


}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set clear color to black
    glEnable(GL_DEPTH_TEST); // Enable depth testing
}


int main(int argc, char **argv){
    scanf("%f %f %f %f %f", &ysur, &v, &alfa, &azimut, &r);

    v0 = v;
    S = r*r;
    K = (C * S * rho) / (2 * m);
    k = (M * g) / (R * T);


    o = 90.0; // rychlost otacania kocky
    ysur0 = dy = ysur;
    xsur = 0;
    zsur = 0;
    ymax = 0;



    if (v <= 0) alfa = 360 - alfa;

    float radians_alfa = alfa * M_PI / 180.0;
    float radians_azimut = azimut * M_PI / 180.0;
    v0z = vz = vzmax = fabs(v * cos(radians_alfa) * cos(radians_azimut));
    v0x = vx = vxmax = fabs(v * cos(radians_alfa) * sin(radians_azimut));
    v0y = vy = dvy = fabs(v) * sin(radians_alfa);

    file_txt = fopen("uloha9.txt","w");
    file_dat = fopen("uloha9.dat","w");


//    vter = sqrt((2*m*g) / (C * S * rho)); //terminalna rychlost
//
//    tdmax = (vter / g) * acosh(exp((g * ysur0) / (vter * vter)));
//    printf("td = %.2f s\n", tdmax);
//
//    vzd = -1 * vter * sqrt(1 - exp(-1 * ((2 * g * ysur0) / (vter * vter))));

    for (float i = 0.00; i < 100; i+= 0.01){
        //printf("t = %.2f\n", i);
        dvy = dvy + dt * (-g - K * exp(-k * dy) * fabs(v) * dvy);
        dy = dy + dt * dvy;
        if (ymax < dy) {
            ymax = dy;
            thmax = i;
            //printf("ymax = %.2f m, thmax = %.2f s\n", ymax, thmax);
        }

        vzmax = vzmax + dt * (-K * exp(-k * dy) * fabs(v) * vzmax);
        zmax = zmax + dt * vzmax;
        vxmax = vxmax + dt * (-K * exp(-k * dy) * fabs(v) * vxmax);
        xmax = xmax + dt * vxmax;
        v = sqrt(vxmax * vxmax + vzmax * vzmax + dvy * dvy);
        if (dy <= 0) {
            tdmax = i;
            vzd = v;
            break;
        }
    }
    v = v0;

    printf("zsur0 = %.2fm, alfa = %.2f, v0x = %.2fm/s, v0z = %.2fm/s, v0x = %.2fm/s, tzmax = %.2f s, "
           "zmax = %.2fm, td = %.2f s, xmax = %.2f m\n"
           "vzd = %.2f m/s\n", ysur0, alfa, v0y, v0x, v0z, thmax, ymax, tdmax, zmax, vzd);



    fprintf(file_txt, "#-Hlavicka-#\n"
                      "# z0 = %.2fm, v0 = %.2fm/s, alfa = %.0f degrees, azimut = %.0f degrees, r = %.2fm,\n"
                      "# tD = %.2fs, vd = %.2f m/s, xd = %.2fm\n#----------#\n", ysur0, v, alfa, azimut, r,
            tdmax, vzd, zmax);

    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, zsur, xsur, ysur, v0z, v0x, v0y);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, zsur, xsur, ysur, v0z, v0x, v0y);



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 100);
    glutCreateWindow ("FYPH/Kokin: Sikmy Vrh v 3D s aerodynamickym odporom");
    init();
    glutDisplayFunc(Sikmy_Vrh);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}