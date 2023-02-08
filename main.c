#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int S = 23;
const int S_2 = S / 2;
const int N = 6 * (S - 2) * (S - 2);

const int ROWS_DISPLAY = 50;
const int COLS_DISPLAY = 50;

const int N_DISPLAY = ROWS_DISPLAY * COLS_DISPLAY;
const int MID_ROW = ROWS_DISPLAY / 2;
const int MID_COL = COLS_DISPLAY / 2;

typedef struct {
    float v1;
    float v2;
    float v3;
    int face;  // 0 for empty [1, 6] for faces
} vec3;

void print_vec(vec3 p) {
    printf("(%.2f, %.2f, %.2f, %d)\n", p.v1, p.v2, p.v3, p.face);
}

vec3 cartesian_to_spherical(vec3 p) {
    float x = p.v1;
    float x_2 = x * x;
    float y = p.v2;
    float y_2 = y * y;
    float z = p.v3;
    float z_2 = z * z;

    float r, theta, phi;

    r = sqrt(x_2 + y_2 + z_2);

    if (z > 0) {
        theta = atan(sqrt(x_2 + y_2) / z);
    } else if (z < 0) {
        theta = M_PI + atan(sqrt(x_2 + y_2) / z);
    } else if (x + y != 0) {
        theta = M_PI_2;
    } else {
        theta = NAN;
    }

    if (x > 0) {
        phi = atan(y / x);
    } else if (x < 0 && y >= 0) {
        phi = atan(y / x) + M_PI;
    } else if (x < 0 && y < 0) {
        phi = atan(y / x) - M_PI;
    } else if (x == 0 && y > 0) {
        phi = M_PI_2;
    } else if (x == 0 && y < 0) {
        phi = -M_PI_2;
    } else {
        // phi = NAN; Final decision TBD
        phi = 0;
    }

    vec3 point = {r, theta, phi, p.face};

    return point;
}

vec3 spherical_to_cartesian(vec3 p) {
    float r = p.v1;
    float theta = p.v2;
    float phi = p.v3;

    float x, y, z;

    x = r * sin(theta) * cos(phi);
    y = r * sin(theta) * sin(phi);
    z = r * cos(theta);

    vec3 point = {x, y, z, p.face};

    return point;
}

void clear_display_points(vec3* display_points) {
    for (int i = 0; i < N_DISPLAY; i++) {
        display_points[i] = (vec3){0.0f, 0.0f, 0.0f, 0};
    }
}

void render_display_points(vec3* cartesian_points, vec3* display_points) {
    for (int i = 0; i < N; i++) {
        float x = cartesian_points[i].v1;
        float y = cartesian_points[i].v2;
        float z = cartesian_points[i].v3;

        int idx = (MID_ROW + z) * ROWS_DISPLAY + (MID_COL + y);

        if (!display_points[idx].face) {
            display_points[idx] = cartesian_points[i];
        } else if (x > display_points[idx].v1) {
            // This 'closer to you' function needs to become matrix projection
            display_points[idx] = cartesian_points[i];
        }
    }
}

void print_cube(vec3* display_points) {
    for (int i = 0; i < N_DISPLAY; i++) {
        if (display_points[i].face) {
            if (display_points[i].face == 1) {
                printf("\033[0;31m11\033[0m");
            } else if (display_points[i].face == 2) {
                printf("\033[0;32m22\033[0m");
            } else if (display_points[i].face == 3) {
                printf("\033[0;33m33\033[0m");
            } else if (display_points[i].face == 4) {
                printf("\033[0;34m44\033[0m");
            } else if (display_points[i].face == 5) {
                printf("\033[0;35m55\033[0m");
            } else {
                printf("\033[0;36m66\033[0;0m");
            }
        } else {
            printf("  ");
        }

        if (i % ROWS_DISPLAY == ROWS_DISPLAY - 1) {
            printf("\n");
        }
    }
}

int main(void) {
    vec3* cartesian_points = malloc(N * sizeof(vec3));
    vec3* spherical_points = malloc(N * sizeof(vec3));
    vec3* display_points = malloc(N_DISPLAY * sizeof(vec3));

    int c = 0;

    for (int i = -S_2 + 1; i < S_2; i++) {
        for (int j = -S_2 + 1; j < S_2; j++) {
            cartesian_points[c] = (vec3){i, j, S_2, 1};
            c++;

            cartesian_points[c] = (vec3){i, j, -S_2, 2};
            c++;

            cartesian_points[c] = (vec3){i, S_2, j, 3};
            c++;

            cartesian_points[c] = (vec3){i, -S_2, j, 4};
            c++;

            cartesian_points[c] = (vec3){S_2, i, j, 5};
            c++;

            cartesian_points[c] = (vec3){-S_2, i, j, 6};
            c++;
        }
    }

    // Setting initial angle
    for (int i = 0; i < N; i++) {
        spherical_points[i] = cartesian_to_spherical(cartesian_points[i]);
    }

    int millis = 50;

    while (1) {
        render_display_points(cartesian_points, display_points);
        print_cube(display_points);
        clear_display_points(display_points);

        for (int i = 0; i < N; i++) {
            spherical_points[i].v2 += 0.05;
            // spherical_points[i].v3 += 0.05;
            cartesian_points[i] = spherical_to_cartesian(spherical_points[i]);
        }

        usleep(millis * 1000);
        system("clear");
    }

    free(cartesian_points);
    free(spherical_points);
    free(display_points);

    return 0;
}
