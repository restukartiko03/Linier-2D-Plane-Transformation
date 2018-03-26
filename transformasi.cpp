#include "transformasi.h"
#include <bits/stdc++.h>

using namespace std;

#define PI 3.14159265

void tulisMatriks(GLfloat current[NMax][3], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++)
			cout << current[i][j] << " ";
		cout << "\n";
	}
}

void copyMatriks(GLfloat M1[][3], GLfloat (&M2)[NMax][3], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++)
			M2[i][j] = M1[i][j];
	}
}

void translasi(GLfloat (&current)[NMax][3], float dx, float dy, float dz, int n) {
	// Matriks current akan di translasi sesuai parameter yang ada
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++) {
			if (j == 0)
				current[i][j] += dx;
			else if (j == 1)
				current[i][j] += dy;
			else
				current[i][j] += dz;
		}
	}
}

void rotate(GLfloat (&current)[NMax][3], float angle, float ax, float ay, float az, int n) {
	// Matriks current akan di rotasi sesuai parameter yang ada
	GLfloat Original2[NMax][3];
	
	angle = (360-angle)*PI/180;
	
	copyMatriks(current, Original2, n);
	translasi(current, -ax, -ay, -az, n);
	translasi(Original2, -ax, -ay, -az, n);	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++) {
			if (j == 0)
				current[i][j] = cos(angle)*Original2[i][0] - sin(angle)*Original2[i][1];
			else if (j == 1)
				current[i][j] = cos(angle)*Original2[i][j] + sin(angle)*Original2[i][0];
		}
	}
	translasi(current, ax, ay, az, n);
}

void dilate(GLfloat (&current)[NMax][3] ,float x, int n) {
	// Matriks current akan di dilatasi sesuai parameter yang ada
	for(int i = 0; i < n; i++)
		for(int j = 0; j < 3; j++)
			current[i][j] *= x;
}

void custom(GLfloat (&Prev_current)[NMax][3], GLfloat a, GLfloat b, GLfloat c, GLfloat d, int n) {
	// Matriks current akan di transformasi sesuai matriks yang ada
	GLfloat Original2[NMax][3];
	
	copyMatriks(Prev_current, Original2, n);
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++) {
			if (j == 0)
				Prev_current[i][j] = a*Original2[i][0] + b*Original2[i][1];
			else if (j == 1)
				Prev_current[i][j] = c*Original2[i][0] + d*Original2[i][1];
		}
	}
	
}

void refleksi(GLfloat (&Prev_current)[NMax][3], int pilihan, int n, string Param_Refleksi) {
	// Matriks current akan di cerminkan sesuai parameter yang ada
	switch (pilihan){
		case 1 : custom(Prev_current, 1, 0, 0, -1, n);
				 break;
		case 2 : custom(Prev_current,-1, 0, 0, 1, n);
				 break;
		case 3 : custom(Prev_current,0, 1, 1, 0, n);
				 break;
		case 4 : custom(Prev_current,0, -1, -1, 0, n);
				 break;
		case 5 : 
				 GLfloat a, b;
				 int idx = 1;
				 string pars1 = "";
				 string pars2 = "";
				 while(Param_Refleksi[idx] != ','){
					 pars1 += Param_Refleksi[idx];
					 idx++;
				 }
				 idx++;
				 while(Param_Refleksi[idx] != ')'){
					 pars2 += Param_Refleksi[idx];
					 idx++;
				 }
				 string::size_type sz;
				 a = stof(pars1,&sz);
				 b = stof(pars2,&sz);
				 custom(Prev_current,-1, 0, 0, -1, n);
				 translasi(Prev_current, 2*a, 2*b, 0, n);
				 break;
	}
}

void Update_Difference ( GLfloat (current)[NMax][3], GLfloat (Prev_current)[NMax][3], GLfloat (&diference)[NMax][3], int n){
	// mengisi matriks difference sesuai parameter yang ada
	// matriks different berisi perubahan yang diperlukan dari matriks Prev_current ke matriks current.
	// matriks different berguna untuk menciptakan animasi transformasi karena perubahan yang disimpan nilainya sangat kecil
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++) {
			diference[i][j] = current[i][j]-Prev_current[i][j];
			diference[i][j] /= 200;
		}
	}
}

