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
const int num_segments = 100; // pre kreslenie kruhu
const float anglePerSegment = 2.0 * 3.14159265358979323846 / 100;

float r, ysur0, ysur, xsur = 0, zsur,
        v0z, v0y, v0x, v,
        ymax, xmax = 0, zmax,
        tymax, txmax,
        vmax,
        t = -0.01,
        alfa, azimut;



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
    zsur = v0z * t;
    v = sqrt(vy*vy + v0x*v0x);

    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void Sikmy_Vrh(){

    //glutWireSphere

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    // Draw the sphere
    glTranslatef(zsur, xsur, ysur);
    glutWireSphere(r, 20, 20);
    glPopMatrix();

    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 50.0, 0.0, 50.0, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}





int main(int argc, char **argv){
    scanf("%f %f %f %f %f", &ysur0, &v, &alfa, &azimut, &r);
    ysur = ysur0;
    xsur = 0;

    if (v <= 0) alfa = 360 - alfa;

    float radians_alfa = alfa * M_PI / 180.0;
    float radians_azimut = azimut * M_PI / 180.0;
    v0x = fabs(v * cos(radians_alfa));
    v0y = fabs(v) * sin(radians_alfa);
    v0z = fabs(v * cos(radians_azimut));

    file_txt = fopen("uloha5.txt","w");
    file_dat = fopen("uloha5.dat","w");
    if (v0y > 0) tymax = v0y / g;   //cas za ktory kruh dosihne najvyssej pozicie
    else tymax = 0.0;




    ymax = ysur0 + (tymax*tymax*g)/2; // maximalne dosiahnutelna vyska
    txmax =  (fabs(v) * sin(radians_alfa) + sqrt(pow(v * sin(radians_alfa), 2) + 2 * g * ysur0)) / g;
    xmax = v0x * txmax;
    zmax = v0z * txmax;

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
    glutCreateWindow ("FYPH/Kokin: Sikmy Vrh v 3D");

    glutDisplayFunc(Sikmy_Vrh);
    glClearColor(0.2, 1.0, 1.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}