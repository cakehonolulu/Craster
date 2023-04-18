#pragma once

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    vec3 p[3];
} triangle;

typedef struct {
    float m[4][4];
} mat4x4;
