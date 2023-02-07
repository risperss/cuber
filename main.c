#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float v1;
    float v2;
    float v3;
} vec3;

typedef struct {
    float x;
    float y;
} vec2;

vec3 cartesian_to_polar(vec3 p) {
    float r = sqrt(p.v1 * p.v1 + p.v2 * p.v2 + p.v3 * p.v3);
    float theta = acos(p.v3 / r);

    int sgn_y;
    if (p.v2 > 0) {
        sgn_y = 1;
    } else if (p.v2 < 0) {
        sgn_y = -1;
    } else {
        sgn_y = 0;
    }
    float phi = sgn_y * acos(p.v1 / (sqrt(p.v1 * p.v1 + p.v2 * p.v2)));

    vec3 point = {r, theta, phi};

    return point;
}

vec3 polar_to_cartesian(vec3 p) {
    float temp = p.v1 * sin(p.v2);

    float x = temp * cos(p.v3);
    float y = temp * sin(p.v3);
    float z = p.v1 * cos(p.v2);

    vec3 point = {x, y, z};

    return point;
}

int main(void) {
    const int N = 11 * 11 * 11;

    vec3* cartesian_points = malloc(N * sizeof(vec3));
    vec3* polar_points = malloc(N * sizeof(vec3));

    int c = 0;
    for (int x = -5; x <= 5; x++) {
        for (int y = -5; y <= 5; y++) {
            for (int z = -5; z <= 5; z++) {
                cartesian_points[c].v1 = x;
                cartesian_points[c].v2 = y;
                cartesian_points[c].v3 = z;
                c++;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        polar_points[i] = cartesian_to_polar(cartesian_points[i]);
        printf("(%.3f, %.3f, %.3f)\n", polar_points[i].v1, polar_points[i].v2,
               polar_points[i].v3);
        printf("(%.3f, %.3f, %.3f)\n", cartesian_points[i].v1,
               cartesian_points[i].v2, cartesian_points[i].v3);
    }

    free(cartesian_points);
    free(polar_points);

    return 0;
}