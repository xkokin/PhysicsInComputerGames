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
const int num_segments = 100; // pre kreslenie kruhu
const float anglePerSegment = 2.0 * 3.14159265358979323846 / 100;

float r, ysur0, ysur, xsur = 0,
        v0, v0y, v0x, v,
        ymax, xmax = 0,
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


    printf("time in seconds: %.2fs, was at highest %d\n", t, atHighest);

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

    //glutWireSphere

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glTranslatef(xsur/xmax, ysur/ymax, 0);

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        float theta = i / anglePerSegment;
        float x = r/100 * cosf(theta);
        float y = r/100 * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();

    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska){
    glViewport(0, 0, sirka, vyska);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, xmax/100, 0, 1.0);

}



int main(int argc, char **argv){

    scanf("%f %f %f %f", &ysur0, &v, &alfa, &r);

    float radians = alfa * M_PI / 180.0;
    v0x = v * cos(radians);
    v0y = v * sin(radians);

    file_txt = fopen("uloha4.txt","w");
    file_dat = fopen("uloha4.dat","w");
    if (v0y > 0) tymax = v0y / g;   //cas za ktory kruh dosihne najvyssej pozicie
    else tymax = 0.0;

    printf("ysur = %.2fm, alfa = %.2f, v0y = %.2fm/s, v0x = %.2fm/s, tymax = %.2f s\n", ysur0, alfa, v0y, v0x, tymax);


    ymax = tymax * g / 2 + ysur0; // maximalne dosiahnutelna vyska
    txmax = 2 * v0y / g;
    xmax = v0x * txmax;


    if(ysur == ymax) atHighest = 1;

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
    int windowWidth, windowHeight;

    if (xmax >= ymax) {
        windowWidth = 1080;
        windowHeight = 1080 * round(ymax / xmax);
    }
    else  {
        windowWidth = 1080 * round(xmax / ymax);
        windowHeight = 1080;
    }

    // Determine the screen size
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Calculate the window position to center it
    int windowPosX = (screenWidth - windowWidth) / 2;
    int windowPosY = (screenHeight - windowHeight) / 2;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow ("FYPH/Kokin: Sikmy Vrh");

    glutDisplayFunc(Sikmy_Vrh);
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}