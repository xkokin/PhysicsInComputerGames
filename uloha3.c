//Hlib Kokin id:117991


#include <GL/glu.h>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<stdlib.h>
#include<stdio.h>
#include <math.h>
#include <unistd.h>

const int icaskrok = 10; // 10 milisekund
const float g = 9.80665; //m/s^2
float a, ysur, vy, ymax, tmax, td, sy; //ymax je najvyssia pointa objektu, sy je startova suradnica pre maticu
int theta; // uhol ktory zavisi od toho kam bol vyhodeny objekt
int atHighest; // ako keby boolean premenna ktora povie ci objekt uz bol v najvyssom bode

FILE* file_txt; // zapisujem data do dvoch suborov, txt na odovzdanie a dat na vytvorenie grafu kedze
// ja nemozem spravit gnuplot z txt
FILE* file_dat;

const float Lmax = 4.0;

void aktualizuj(const int ihod){

    float t = 0.01*(ihod); //cas od zaciatku;

    if (t >= tmax && atHighest != 1) atHighest = 1;

    if(ysur < 0) {
        fclose(file_txt);
        fclose(file_dat);
        return;
    }


    printf("time in seconds: %.2fs, was at highest %d\n", t, atHighest);



    if (atHighest == 0 && ysur < ymax) {
        //ysur -= (vy * sin(theta) * t) - (0.5 * g * t*t); //t je cas od spustenia
        //vy = -sqrt(2 * g * ysur + vy*vy); // podla rovnice musime tam mat sin^2(theta) ale ked
        ysur += vy * 0.01;
        vy = vy - g*0.01;
        // mozeme mat theta bud 90 alebo 180 tak mozeme vynechat ten stvorec pretoze hodnoty sa nemenia
    }
    else {
        //ysur = ymax - (vmax * t-tmax) + (0.5 * g * pow(t-tmax, 2));
        //vy = sqrt(2 * g * (ymax - ysur) + pow(vmax, 2));
        ysur += vy * 0.01; // kedze vy bude zaporna
        vy = vy - g*0.01;
    }


    fprintf(file_txt, "%.2f\t%.2f\t%.2f\n", t, ysur, vy);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\n", t, ysur, vy);
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}

void Zvisly_Vrh(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(a, a, 1);
    glTranslatef(0, ysur/ymax, 0);


    glBegin(GL_QUADS);

    glColor3f(1.0,0.0,0.0);
    glVertex2f(-0.1, sy+0.2);

    glColor3f(0.5,0.5,0.0);
    glVertex2f(-0.1 , sy);

    glColor3f(0.0,0.5,0.5);
    glVertex2f(0.1 , sy);

    glColor3f(0.0,0.0,1.0);
    glVertex2f(0.1 , sy+0.2);


    glEnd();

    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska){
    glViewport(0, 0, sirka, vyska);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(sirka == 0) sirka++;
    const float pomstr = (GLfloat)vyska/(GLfloat)sirka;
    if (sirka >= vyska) gluOrtho2D(-1.0*pomstr*a, 1.0*pomstr*a, -1.0*a, 1.0*a);
        //else gluOrtho2D(-0.375*a, 0.375*a, -0.375*pomstr*a, 0.375*pomstr*a);
    else gluOrtho2D(-0.5, 0.5, -0.5*pomstr*(-1), 0.5*pomstr*ymax);
    //else gluOrtho2D(-0.5, 0.5, -0.5*pomstr, 0.5*pomstr);

}



int main(int argc, char **argv){

    scanf("%f %f %f", &ysur, &vy, &a);

    if (vy < 0) theta = 0; // ked objekt je vyhodeni hore tak rychlost bude zaporna
    if (vy >= 0) theta = 1;

    file_txt = fopen("uloha3.txt","w");
    file_dat = fopen("uloha3.dat","w");
    if (vy > 0) tmax = vy / g;   //cas za ktory stvorec dosihne najvyssej pozicie
    else tmax = 0.0;

    td = (1/g)*(vy+sqrt(vy*vy+2*g*ysur));
    if (td < 0) td = (1/g)*(vy-sqrt(vy*vy+2*g*ysur));

    ymax = (vy*vy * theta) / (2*g) + ysur; // maximalne dosiahnutelna vyska
    //vmax = sqrt(vy*vy - 2 * g * ymax * sin(theta)); // rychlost v najvyssem bode

    sy = -1 ; // startove koordinaty

    if(ysur == ymax) atHighest = 1;

    printf ("Najvyssia pozicia bude: %.2f\n"
            "V case %.2f\n"
            "sy = %.2f\n"
            "td = %.2f\n", ymax, tmax, sy+ ysur/ymax*2, td);

    fprintf(file_txt, "#-Hlavicka-#\n"
                      "# y0 = %.2fm, v0y = %.2fm/s, ymax = %.2fm, tmax = %.2fs\n#----------#\n", ysur, vy, ymax, tmax);
    fprintf(file_txt, "%.2f\t%.2f\t%.2f\n", 0.00, ysur, vy);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\n", 0.00, ysur, vy);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(300, 600);
    glutInitWindowPosition(200,150);
    glutCreateWindow ("FYPH/Kokin: Zvisly Vrh");

    glutDisplayFunc(Zvisly_Vrh);
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}