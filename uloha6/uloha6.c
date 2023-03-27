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


float r, ysur0, ysur, xsur = 0, zsur,
        v0z, v0y, v0x, v, o,
        ymax, xmax = 0, zmax,
        thmax, tdmax,
        vmax,
        t = -0.01,
        alfa, azimut, omega = 0.0;


FILE* file_txt; // zapisujem data do dvoch suborov, txt na odovzdanie a dat na vytvorenie grafu kedze
// ja nemozem spravit gnuplot z txt
FILE* file_dat;


void aktualizuj(const int ihod){
    float vy = 0;
    t += 0.01; //cas od zaciatku;
    omega+=1;
    if (omega > 360) omega = 0;

    //if (t >= tmax && atHighest != 1) atHighest = 1;

    if(ysur < 0) {
        fclose(file_txt);
        fclose(file_dat);
        printf("td = %.2f s, xmax = %.2f m, ymax = %.2f m, zmax = %.2f m\n", tdmax, xmax, ymax, zmax);
        return;
    }


    printf("time in seconds: %.2fs\n", t);

    vy = v0y + ay*t;
    ysur = ysur0 + v0y * t + ay * t * t / 2;
    xsur = v0x * t;
    zsur = v0z * t;
    v = sqrt(v0z*v0z + vy*vy + v0x*v0x);

    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur, v0x, vy, v);
    glutPostRedisplay();

    glutTimerFunc(icaskrok, aktualizuj, ihod+1);
}


void Sikmy_Vrh(){


    GLfloat vertices[][3] = { {-1,-1,-1},
                              {1,-1,-1},
                              {1,1,-1},
                              {-1,1,-1},
                              {-1,-1,1},
                              {1,-1,1},
                              {1,1,1},
                              {-1,1,1},

                              {0, 0, 0},
                              {xmax, 0, 0},
                              {xmax, 0, zmax},
                              {0, 0, zmax}
    };

    GLfloat colors[][3] = {{1.0, 0.0, 0.0},
                           {0.0, 0.0, 1.0},
                           {1.0, 0.0, 1.0},
                           {1.0, 1.0, 0.0},
                           {0.0, 1.0, 0.0},
                           {0.0, 1.0, 1.0},
                           {1.0, 1.0, 1.0}
    };
    GLfloat col_lines[][3] = {
            {1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0},
            {1.0, 1.0, 0.0}
    };
    GLubyte indices[][4] = { {0,3,2,1},
                             {2,3,7,6},
                             {0,4,7,3},
                             {1,2,6,5},
                             {4,5,6,7},
                             {0,1,5,4},


                             {11, 10, 9, 8}
    };


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    gluLookAt(xmax/2, ymax, zmax*2, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
//    gluLookAt(xmax/2, ymax/2, zmax/2, xmax, ymax, zmax, 1.0, 1.0, 1.0);

    glLineWidth(1.0f);


    glTranslatef(xsur, ysur, zsur);
    glRotatef(omega, 1.0, 1.0, 1.0);
    glScalef(r, r, r);
    for(int i=0; i<6; i++)
    {
        glBegin(GL_POLYGON);

        glColor3fv(colors[i]);

        glVertex3fv(vertices[indices[i][0]]);
        glVertex3fv(vertices[indices[i][1]]);
        glVertex3fv(vertices[indices[i][2]]);
        glVertex3fv(vertices[indices[i][3]]);
        glEnd();
    }

    glPopMatrix();

//    glLoadIdentity();
    glPushMatrix();

    glBegin(GL_POLYGON);
    glColor3fv(colors[6]);

    glVertex3fv(vertices[indices[6][0]]);
    glVertex3fv(vertices[indices[6][1]]);
    glVertex3fv(vertices[indices[6][2]]);
    glVertex3fv(vertices[indices[6][3]]);
    glEnd();
    glPopMatrix();

//    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_LINE);
    glLineWidth(6.0f);
    glBegin(GL_LINES);


    glColor3fv(col_lines[1]);
    glVertex3f(0, ymax+r, 0);
    glColor3fv(col_lines[1]);
    glVertex3f(0, 0, 0);

    glColor3fv(col_lines[2]);
    glVertex3f(xmax+r, 0, 0);
    glColor3fv(col_lines[2]);
    glVertex3f(0, 0, 0);

    glColor3fv(col_lines[0]);
    glVertex3f(xmax+r, 0, zmax+r);
    glColor3fv(col_lines[0]);
    glVertex3f(0, 0, 0);

    glEnd();



    glutSwapBuffers();

}

void obsluhaResize(int sirka, int vyska){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float pomstr = (float)sirka/(float)vyska;

//    if (ymax / vyska > xmax / sirka) {
//        glOrtho(0.0, (ymax+r) / pomstr, 0.0, ymax+r, -zmax, zmax);
//    }
//    else if (ymax / vyska < xmax / sirka) {
//        glOrtho(0.0, xmax+r, 0.0, (xmax+r)* pomstr, -zmax, zmax);
//    }
    float theta = atan(ymax / zmax) * 180 / M_PI * 2;
    printf("theta: %.2f\n", theta);
    gluPerspective(theta, pomstr, zmax, 0);
    glViewport(0, 0, sirka, vyska);

}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set clear color to black
    glEnable(GL_DEPTH_TEST); // Enable depth testing
}


int main(int argc, char **argv){
    scanf("%f %f %f %f %f %f", &ysur, &v, &alfa, &azimut, &r, &o);
    ysur0 = ysur;
    xsur = 0;

    if (v <= 0) alfa = 360 - alfa;

    float radians_alfa = alfa * M_PI / 180.0;
    float radians_azimut = azimut * M_PI / 180.0;
    v0z = fabs(v * cos(radians_alfa) * cos(radians_azimut));
    v0x = fabs(v * cos(radians_alfa) * sin(radians_azimut));
    v0y = fabs(v) * sin(radians_alfa);

    file_txt = fopen("uloha6.txt","w");
    file_dat = fopen("uloha6.dat","w");
    if (v0z > 0) thmax = v0y / g;   //cas za ktory kruh dosihne najvyssej pozicie
    else thmax = 0.0;




    ymax = ysur + (thmax*thmax*g)/2; // maximalne dosiahnutelna vyska
    tdmax =  (v0y + sqrt(pow(v0y, 2) + 2 * g * ysur)) / g;
    zmax = v0z * tdmax;
    xmax = v0x * tdmax;

    printf("ysur = %.2fm, alfa = %.2f, v0y = %.2fm/s, v0x = %.2fm/s, v0z = %.2fm/s, tymax = %.2f s, "
           "ymax = %.2fm, txmax = %.2f s, xmax = %.2f m\n", ysur0, alfa, v0y, v0x, v0z, thmax, ymax, tdmax, xmax);

    printf ("Najvyssia pozicia bude: %.2f\n"
            "V case %.2f\n"
            "\n", zmax, thmax);

    fprintf(file_txt, "#-Hlavicka-#\n"
                      "# y0 = %.2fm, v0 = %.2fm/s, alfa = %.0f degrees, azimut = %.0f degrees, r = %.2fm,\n"
                      "# tD = %.2fs, ymax = %.2fm, tm = %.2fs, xmax = %.2f\n#----------#\n", ysur0, v, alfa, azimut, r,
            tdmax, ymax, thmax, xmax);

    fprintf(file_txt, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur0, v0x, v0y, v);
    fprintf(file_dat, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", t, xsur, ysur0, v0x, v0y, v);



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1000, 500);
    glutInitWindowPosition(0, 100);
    glutCreateWindow ("FYPH/Kokin: Sikmy Vrh v 3D");
    init();
    glutDisplayFunc(Sikmy_Vrh);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutReshapeFunc(obsluhaResize);
    glutTimerFunc(icaskrok, aktualizuj, 1);
    glutMainLoop();
    return 0;

}