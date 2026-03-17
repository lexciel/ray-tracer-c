#ifndef MAT4_H

#define MAT4_H
#include <math.h>
#include <stdio.h>
#include "vec3.h"
typedef double scalar;

#ifdef USE_FLOAT_PRECISION
    #define m_sin sinf
    #define m_cos cosf
#else
    #define m_sin sin
    #define m_cos cos
#endif

typedef union {
    scalar m[4][4];
    scalar flat_m[16];
} mat4;

static inline mat4 mat4_identity() {
    mat4 identity = {};
    for (int i = 0; i<16;i+=4) {
        identity.flat_m[i] = 1;
    }
    return identity;
}


static inline mat4 mat4_add(mat4 a, mat4 b) {
    mat4 result;
    for (int i = 0; i < 16; i++) {
        result.flat_m[i] = a.flat_m[i]+b.flat_m[i];
    }
    return result;
}

static inline mat4 mat4_scale(mat4 a, scalar b) {
    for (int i = 0; i < 16; i++) {
        a.flat_m[i] *= b;
    }
    return a;
}

static inline mat4 mat4_scale(vec3 v) {
    mat4 m = mat4_identity();
    m.m[0][0] = v.x;
    m.m[1][1] = v.y;
    m.m[2][2] = v.z;
    return m;
}

static inline mat4 mat4_translate(vec3 v) {
    mat4 m = mat4_identity();
    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;
    return m;
}



static inline mat4 mat4_rotate_z(scalar angle_radian) {
    mat4 m = mat4_identity();

    scalar a,b;

    a = m_sin(angle_radian);
    b = m_cos(angle_radian);

    m.m[0][0] =  b;
    m.m[0][1] = -a;
    m.m[1][0] =  a;
    m.m[1][1] =  b;

    return m;
};


static inline mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 res;

    res.m[0][0] = a.m[0][0]*b.m[0][0] + a.m[0][1]*b.m[1][0] + a.m[0][2]*b.m[2][0] + a.m[0][3]*b.m[3][0];
    res.m[0][1] = a.m[0][0]*b.m[0][1] + a.m[0][1]*b.m[1][1] + a.m[0][2]*b.m[2][1] + a.m[0][3]*b.m[3][1];
    res.m[0][2] = a.m[0][0]*b.m[0][2] + a.m[0][1]*b.m[1][2] + a.m[0][2]*b.m[2][2] + a.m[0][3]*b.m[3][2];
    res.m[0][3] = a.m[0][0]*b.m[0][3] + a.m[0][1]*b.m[1][3] + a.m[0][2]*b.m[2][3] + a.m[0][3]*b.m[3][3];

    res.m[1][0] = a.m[1][0]*b.m[0][0] + a.m[1][1]*b.m[1][0] + a.m[1][2]*b.m[2][0] + a.m[1][3]*b.m[3][0];
    res.m[1][1] = a.m[1][0]*b.m[0][1] + a.m[1][1]*b.m[1][1] + a.m[1][2]*b.m[2][1] + a.m[1][3]*b.m[3][1];
    res.m[1][2] = a.m[1][0]*b.m[0][2] + a.m[1][1]*b.m[1][2] + a.m[1][2]*b.m[2][2] + a.m[1][3]*b.m[3][2];
    res.m[1][3] = a.m[1][0]*b.m[0][3] + a.m[1][1]*b.m[1][3] + a.m[1][2]*b.m[2][3] + a.m[1][3]*b.m[3][3];

    res.m[2][0] = a.m[2][0]*b.m[0][0] + a.m[2][1]*b.m[1][0] + a.m[2][2]*b.m[2][0] + a.m[2][3]*b.m[3][0];
    res.m[2][1] = a.m[2][0]*b.m[0][1] + a.m[2][1]*b.m[1][1] + a.m[2][2]*b.m[2][1] + a.m[2][3]*b.m[3][1];
    res.m[2][2] = a.m[2][0]*b.m[0][2] + a.m[2][1]*b.m[1][2] + a.m[2][2]*b.m[2][2] + a.m[2][3]*b.m[3][2];
    res.m[2][3] = a.m[2][0]*b.m[0][3] + a.m[2][1]*b.m[1][3] + a.m[2][2]*b.m[2][3] + a.m[2][3]*b.m[3][3];

    res.m[3][0] = a.m[3][0]*b.m[0][0] + a.m[3][1]*b.m[1][0] + a.m[3][2]*b.m[2][0] + a.m[3][3]*b.m[3][0];
    res.m[3][1] = a.m[3][0]*b.m[0][1] + a.m[3][1]*b.m[1][1] + a.m[3][2]*b.m[2][1] + a.m[3][3]*b.m[3][1];
    res.m[3][2] = a.m[3][0]*b.m[0][2] + a.m[3][1]*b.m[1][2] + a.m[3][2]*b.m[2][2] + a.m[3][3]*b.m[3][2];
    res.m[3][3] = a.m[3][0]*b.m[0][3] + a.m[3][1]*b.m[1][3] + a.m[3][2]*b.m[2][3] + a.m[3][3]*b.m[3][3];

    return res;
}

static inline mat4 mat4_transform(vec3 sc, scalar rot, vec3 tr) {
    mat4 s = mat4_scale(sc);
    mat4 r = mat4_rotate_z(rot);
    mat4 t = mat4_translate(tr);
    return mat4_mul(t, mat4_mul(r, s));

}



#endif