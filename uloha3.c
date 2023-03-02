//Hlib Kokin id:117991

#include<stdlib.h>
#include<stdio.h>
#include <math.h>
#include <unistd.h>




int main(int argc, char **argv){

    const float icaskrok = 0.5;
    float v1, a1, v2, a2, dst;
    scanf("%f %f %f %f %f", &v1, &a1, &v2, &a2, &dst);
    FILE* file;
    int closed = 0;

    file = fopen("uloha2.dat","w");
    float b = (v1+v2);
    float a = (a1/2 + a2/2);
    float c = -dst;

    float d = b*b - 4 * a * c;
    if (d < 0) printf("They will never meet\n");
    float root1 = ( -b + sqrt(d)) / (2* a);
    float root2 = ( -b - sqrt(d)) / (2* a);
    float res = 0;
    if(root1 < 0) res = root2;
    else if(root2 < 0) res = root1;

    printf("They will meet in %.2f secs\n"
           "At x = %.2f meters", res, res*v1);

    fprintf(file, "# Hlavicka #\n"
                  "# v1=%.2fm/s, a1=%.2fm/s, v2=%.2fm/s, a2=%.2fm/s,\n"
                  "# dst=%.2fm t=%.2fm, rx = %.2f\n#----------#\n"
                  ,v1, a2, v2, a2, dst, res, res * v1);

    int x1 = 0;
    int x2 = 100;
    float t = 0;
    while(x2>x1){
        fprintf(file, "%.1f\t%d\t%d\n", t, x1, x2);
        sleep(icaskrok);
        t+=0.5;
        x1 = v1*t + a1/2*t*t;
        x2 = 100 - v2*t + a2/2*t*t;
    }

    fclose(file);
    return 0;
}