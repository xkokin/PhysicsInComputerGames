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

float r, ysur0, ysur, xsur = 0,
        v0, v0y, v0x, v,
        ymax, xmax,
        tymax, txmax,
        vmax,
        t = -0.01,
        alfa;



int atHighest; // boolean premenna ktora povie ci objekt uz bol v najvyssom bode

FILE* file_txt; // zapisujem data do dvoch suborov, txt na odovzdanie a dat na vytvorenie grafu kedze
// ja nemozem spravit gnuplot z txt
FILE* file_dat;


void aktualizuj(const int ihod){
    float vy = 0;
    t += 0.01; //cas od zaciatku;

    //if (t >= tmax && atHighest != 1) atHighest = 1;

    if(ysur < 0) {
        fclose(file_txt);
        fclose(file_dat);
        return;
    }


    printf("time in seconds: %.2fs\n", t);

    vy = v0y + ay*t;
    ysur = ysur0 + v0y * t + ay * t * t / 2;
    xsur = v0x * t;
    v = sqrt(vy*vy + v0x*v0x);

    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
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
    glVertex2f(0, ymax);
    glColor3f(0, 0, 1);
    glVertex2f(0, 0);

    glColor3f(1, 1, 0);
    glVertex2f(xmax, 0);
    glColor3f(1, 1, 0);
    glVertex2f(0, 0);
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

    if (ymax / vyska > xmax / sirka) {
        gluOrtho2D(0.0, (ymax+r) / pomstr, 0.0, ymax+r);
    }
    else if (ymax / vyska < xmax / sirka) {
            gluOrtho2D(0.0, xmax+r, 0.0, (xmax+r) * pomstr);
        }
    //gluOrtho2D(0, 2, 0, 2);
    glViewport(0, 0, sirka, vyska);
}



int main(int argc, char **argv){

    scanf("%f %f %f %f", &ysur0, &v, &alfa, &r);
    ysur = ysur0;
    xsur = 0;

    if (v <= 0) alfa = 360 - alfa;

    float radians = alfa * M_PI / 180.0;
    v0x = fabs(v * cos(radians));
    v0y = fabs(v) * sin(radians);

    file_txt = fopen("uloha4.txt","w");
    file_dat = fopen("uloha4.dat","w");
    if (v0y > 0) tymax = v0y / g;   //cas za ktory kruh dosihne najvyssej pozicie
    else tymax = 0.0;




    ymax = ysur0 + (tymax*tymax*g)/2; // maximalne dosiahnutelna vyska
    txmax =  (fabs(v) * sin(radians) + sqrt(pow(v * sin(radians), 2) + 2 * g * ysur0)) / g;
    xmax = v0x * txmax;

    printf("ysur = %.2fm, alfa = %.2f, v0y = %.2fm/s, v0x = %.2fm/s, tymax = %.2f s, "
           "ymax = %.2fm, txmax = %.2f s, xmax = %.2f m\n", ysur0, alfa, v0y, v0x, tymax, ymax, txmax, xmax);

    printf ("Najvyssia pozicia bude: %.2f\n"
            "V case %.2f\n"
            "\n", ymax, tymax );

    fprintf(file_txt, "#-Hlavicka-#\n"
                      "# y0 = %.2fm, v0 = %.2fm/s, alfa = %.0f degrees, r = %.2fm,\n"
                      "# tD = %.2fs, xmax = %.2fm, tm = %.2fs, ymax = %.2f\n#----------#\n", ysur0, v, alfa, r,
                                                                                        txmax, xmax, tymax, ymax);

    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur0, v0x, v0y, v);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur0, v0x, v0y, v);



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);

    // Determine the screen size
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Calculate the window position to center it
    int windowPosX = (screenWidth - 1000) / 2;
    int windowPosY = (screenHeight - 500) / 2;

    glutInitWindowSize(1000, 500);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow ("FYPH/Kokin: Sikmy Vrh");

    glutDisplayFunc(Sikmy_Vrh);
    glClearColor(0.2, 1.0, 1.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}