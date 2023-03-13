//Hlib Kokin id:117991


#include <GL/glu.h>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<stdio.h>
#include <math.h>
#include <unistd.h>

const int icaskrok = 10; // 10 milisekund cas onovenia
const float g = -9.80665; //m/s^2
const float PI = 3.14159265358979323846;
const int num_segments = 100; // pre kreslenie kruhu
const float anglePerSegment = 2.0 * PI / num_segments;

float r, ysur, xsur = 0,
        v0, v0y, v0x, v,
        ymax, xmax = 0,
        tymax, txmax,
        vmax,
        t = -0.01;



int alfa; // uhol ktory zavisi od toho kam bol vyhodeny objekt
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

    vy = v0y - g*t;
    ysur = vy * t;
    xsur = v0x * t;
    v = sqrt(vy*vy + v0x*v0x);

//    if (atHighest == 0 && t < tymax) {
//        //ysur -= (vy * sin(theta) * t) - (0.5 * g * t*t); //t je cas od spustenia
//        //vy = -sqrt(2 * g * ysur + vy*vy); // podla rovnice musime tam mat sin^2(theta) ale ked
//        vy = v0y - g*t;
//        ysur = vy * t;
//        xsur = v0x * t;
//
//        // mozeme mat theta bud 90 alebo 180 tak mozeme vynechat ten stvorec pretoze hodnoty sa nemenia
//    }
//    else {
//        //ysur = ymax - (vmax * t-tmax) + (0.5 * g * pow(t-tmax, 2));
//        //vy = sqrt(2 * g * (ymax - ysur) + pow(vmax, 2));
//        ysur += vy*0.01; // kedze vy bude zaporna
//        vy = vy - g*0.01;
//    }


    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void Sikmy_Vrh(){

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glTranslatef(xsur/xmax, ysur/ymax, 0);


    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        float theta = i / anglePerSegment;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();

    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska){
    glViewport(0, 0, sirka, vyska);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

//    if(sirka == 0) sirka++;
//  const float pomstr = (GLfloat)vyska/(GLfloat)sirka;
//    if (sirka >= vyska) gluOrtho2D(-1.0*pomstr*a, 1.0*pomstr*a, -1.0*a, 1.0*a);
//    else gluOrtho2D(0, xmax, 0, ymax);
    gluOrtho2D(0, xmax, 0, ymax);

}



int main(int argc, char **argv){

    scanf("%f %f %d %f", &ysur, &v, &alfa, &r);

    float radians = alfa * PI / 180.0;

    v0x = v0 * cos(radians);
    v0y = v0 * sin(radians);

    file_txt = fopen("uloha4.txt","w");
    file_dat = fopen("uloha4.dat","w");
    if (v0y > 0) tymax = v0y / g;   //cas za ktory kruh dosihne najvyssej pozicie
    else tymax = 0.0;


    ymax = tymax * g / 2 + ysur; // maximalne dosiahnutelna vyska
    txmax = 2 * v0y / g;
    xmax = v0x * txmax;


    if(ysur == ymax) atHighest = 1;

    printf ("Najvyssia pozicia bude: %.2f\n"
            "V case %.2f\n"
            "\n", ymax, tymax );

    fprintf(file_txt, "#-Hlavicka-#\n"
                      "# y0 = %.2fm, v0 = %.2fm/s, alfa = %ddegrees, r = %.2fm,\n"
                      "# tD = %.2fs, xmax = %.2fm, tm = %.2fs, ymax = %.2f\n#----------#\n", ysur, v, alfa, r,
                                                                                        txmax, xmax, tymax, ymax);

    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, v0y, v);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, v0y, v);



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    int windowWidth, windowHeight;

    if (xmax >= ymax) {
        windowWidth = 1080;
        windowHeight = 1080 * (ymax / xmax);
    }
    else  {
        windowWidth = 1080 * (xmax / ymax);
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