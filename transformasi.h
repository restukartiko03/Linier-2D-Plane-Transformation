#ifndef _transformasi_H
#define _transformasi_H

#include <gl/glut.h>
#include <gl/freeglut.h>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

const int NMax = 1e3;

void tulisMatriks(GLfloat current[NMax][3], int n);

void copyMatriks(GLfloat M1[][3], GLfloat (&M2)[NMax][3], int n);

void translasi(GLfloat (&current)[NMax][3], float dx, float dy, float dz, int n);

void rotate(GLfloat (&current)[NMax][3], float angle, float ax, float ay, float az, int n);

void dilate(GLfloat (&current)[NMax][3] ,float x, int n);

void custom(GLfloat (&Prev_current)[NMax][3], GLfloat a, GLfloat b, GLfloat c, GLfloat d, int n);

void refleksi(GLfloat (&Prev_current)[NMax][3], int pilihan, int n, string Param_Refleksi);

void Update_Difference ( GLfloat (current)[NMax][3], GLfloat (Prev_current)[NMax][3], GLfloat (&diference)[NMax][3], int n);
#endif