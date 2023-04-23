//
// Created by glebk on 30.03.2023.
//
#include <stdio.h>
#include <math.h>

int main() {
    // Constants and parameters (replace with actual values)
    double m = 1500;
    double alpha = 15;
    double Cx = 0.31;
    double S = 2162;
    double rho = 1.26;
    double g = 9.61;
//    double M = 30000;
    double M = 9000;

    // Initial conditions
    double t = 0;
    double v = 0;
    double a = (M - m * g * sin(alpha)) / m;

    double time_step = 0.05;  // 50 ms in seconds
    double total_time = 5;  // desired total time range

    FILE *outfile = fopen("uloha7_1.dat", "w");
    if (outfile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(outfile, "Time (s)\tAcceleration (m/s²)\tVelocity (m/s)\n");

    while (t <= total_time) {
        fprintf(outfile, "%.2f\t%.2f\t%.2f\n", t, a, v);

        // Update acceleration and velocity using the Euler method
        a = (M - m * g * sin(alpha) - 0.5 * Cx * S * rho * pow(v, 2)) / m;
        v += a * time_step;
        t += time_step;
    }

    fclose(outfile);

    return 0;
}