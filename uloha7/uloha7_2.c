//
// Created by glebk on 30.03.2023.
//
#include <stdio.h>
#include <math.h>

#define M 1500.0
#define G 9.81
#define RHO 1.26
#define CX 0.31
#define S 2162.0
#define ALPHA_DEG 15.0
#define DT 0.1
#define TOLERANCE 1e-3

double terminal_velocity();
double acceleration(double v);

int main() {
    double v = 0, a = 0, t = 0;
    double v_term = terminal_velocity();

    FILE *file = fopen("uloha7_1a.dat", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(file, "t (s)\ta (m/s^2)\tv (m/s)\n");

    while (v <= v_term) {
        a = acceleration(v);
        fprintf(file, "%.2f\t%.2f\t%.2f\n", t, a, v);
        v += a * DT;
        t += DT;
        if (fabs(v_term - v) < TOLERANCE) {
            break;
        }
    }

    fclose(file);

    return 0;
}

double terminal_velocity() {
    double alpha_rad = ALPHA_DEG * (M_PI / 180);
    return sqrt((M * G * sin(alpha_rad)) / (0.5 * RHO * CX * S));
}

double acceleration(double v) {
    double alpha_rad = ALPHA_DEG * (M_PI / 180);
    double F_parallel = M * G * sin(alpha_rad);
    double D = 0.5 * RHO * CX * S * v * v;
    double F_net = F_parallel - D;
    return F_net / M;
}