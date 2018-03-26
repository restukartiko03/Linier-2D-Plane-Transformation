#include <gl/glut.h>
#include <gl/freeglut.h>
#include <bits/stdc++.h>
#include <time.h>
#include "transformasi.h"

#define fi first
#define se second
#define mp make_pair
using namespace std;

GLfloat Original[NMax][3], current[NMax][3], Prev_current[NMax][3],
Multiple[100][NMax][3]; GLfloat AxisX[NMax][3], AxisY[NMax][3]; string param,
Jenis_Operasi, Param_Refleksi; queue < pair< int, pair< float , pair< float,
pair < float, float > > > > > Rotasi_Multiple; float diference[NMax][3]; float
dAngle,angle, ax, ay, az, dx, dy, dz, k; int Tipe_Operasi,tipe, N,
start,BanyakProses; int cnt, idx; bool baca = true, multiple, Proses = false,
Selesai_Multiple;

void initGLUT(int &argc, char **argv) {
	// Prosedur ini untuk membuat tampilan layar berbentuk kotak ukuran 500x500 pixel
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	
	glutInitWindowSize(500,500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("ALGEO");	
}

void initGL() {
	// Prosedur ini untuk menginisiasi background pada tampilan layar
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_PROJECTION);
	glMatrixMode(GL_PROJECTION);      // Select the Projection matrix for operation
	glLoadIdentity();                 // Reset Projection matrix
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set clipping area's left, right, bottom, top	
}

void Gambar_Axis() {
	// Prosedur ini untuk menggambar garis sumbu-x dan sumbu-y pada tampilan layar
	for(int i = 1; i <= 50; i++) {
		AxisX[i][0] = (float) i/50;
		AxisX[i][1] = 0;
		AxisX[i][2] = 0;
		AxisY[i][1] = (float) i/50;
		AxisY[i][0] = 0;
		AxisY[i][2] = 0;		
	}

	for(int i = 51; i <= 100; i++) {
		AxisX[i][0] = (float) -(i-50)/50;
		AxisX[i][1] = 0;
		AxisX[i][2] = 0;
		AxisY[i][1] = (float) -(i-50)/50;
		AxisY[i][0] = 0;
		AxisY[i][2] = 0;		
	}	
}

void draw() {
	// Prosedur ini untuk menggambar bangun datar sesuai dengan matriks current
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_POLYGON);
		for(int i = 0; i < N; i++) {
			glColor3f((float)(i+1)*0.9f, (float)i*0.3f, 0.3f); // menentukan warna bangun datar
			for(int j = 0; j < 3; j++) {
				if (Jenis_Operasi == "input")
					current[i][j] = Original[i][j]/500;
				else
					current[i][j] /= 500;				
			}
			glVertex3fv(current[i]); // menggambar titik pada layar
		}
	glEnd();
	
	glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, 0.0f);
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -1.0f, 0.0f);
	glEnd();
	
	glBegin(GL_POINTS);
		for(int i = 1; i <= 100; i++) {
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(AxisX[i]);
		}
		
		for(int i = 1; i <= 100; i++) {
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(AxisY[i]);
		}		
	glEnd();
	
	glFlush();
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < 3; j++)
			current[i][j] *= 500;
	}
}

void input() {
	// F.S
	// matriks original dan matriks current berisi masukan array of titik yang dimasukan oleh pengguna
	cout << "Masukkan banyak sisi : ";
	cin >> N;
	cout<< "(dalam format x y z)\n";
	for(int i = 0; i < N; i++) {
		printf("Masukan koordinat titik ke-%d: ",i+1 );
		for(int j = 0; j < 3; j++) {
			cin >> Original[i][j];
			current[i][j] = Original[i][j];
		}
	}
}
void Animated(GLfloat (&current)[NMax][3], GLfloat (diference)[NMax][3]) {
	//F.S
	// matriks current berubah, sehingga gambar juga berubah. Dengan prosedur ini, kami membuat animasi dengan cara merubah gambar sedikit demi sedikit
	for(int i = 0; i < N; i++) 
		for(int j = 0; j < 3; j++) 
			current[i][j] += diference[i][j];
}

void Inisiasi(){
	// Menginisiasi nilai matriks Prev_current pada setiap perintah transformasi yang baru.
	cnt = 0;					
	Proses = true;
	copyMatriks(current, Prev_current, N);
}

void Inisiasi_Multiple(){
	// Menginisiasi nilai matriks Prev_current pada setiap perintah multiple
	copyMatriks(Prev_current, Multiple[idx], N);
	idx++;
	Proses = false;
	if(idx == BanyakProses){
		start = 0;
		Tipe_Operasi = 11;
		cnt = 0;
	}
}

void Update_Current(){
	// membuat gerakan animasi
	Animated(current, diference);
	cnt++;
	if(cnt == 200)
		Proses = false;				
}

void idle() {
	if (baca || multiple) {
		if (!Proses) { // Program tidak sedang menjalankan suatu proses transformasi
			cin >> Jenis_Operasi;
			memset(diference,0,sizeof(diference));	//inisiasi matriks differenc dengan nilai 0					
			if (Jenis_Operasi == "translate")
				Tipe_Operasi = 2;
			else if (Jenis_Operasi == "dilate")
				Tipe_Operasi = 4;
			else if (Jenis_Operasi == "rotate")
				Tipe_Operasi = 3;
			else if (Jenis_Operasi == "reflect")
				Tipe_Operasi = 6;
			else if (Jenis_Operasi == "shear")
				Tipe_Operasi = 7;
			else if (Jenis_Operasi == "input")
				Tipe_Operasi = 1;
			else if (Jenis_Operasi == "custom")
				Tipe_Operasi = 5;
			else if (Jenis_Operasi == "exit")
				Tipe_Operasi = 10;
			else if (Jenis_Operasi == "stretch")
				Tipe_Operasi = 8;
			else if (Jenis_Operasi == "reset")
				Tipe_Operasi = 9;
			else if (Jenis_Operasi == "multiple")
				Tipe_Operasi = 11;
			else
				printf("Perintah tidak terdefinisi.\n");
		}
		switch(Tipe_Operasi) {
			case 1 : input();
					break;
			case 2 : if (!Proses) {
						if(!multiple) Inisiasi();				
						cin >> dx >> dy >> dz; // input parameter translasi tiap titik
						translasi(Prev_current, dx, dy, dz, N);
						if(!multiple) Update_Difference(Prev_current, current, diference, N); 
					}
					if(multiple) // program masih dalam proses multiple
						Inisiasi_Multiple();
					else  // program tidak dalam prosed multiple
						Update_Current();
					break;
			case 3 : if (!Proses) {
						if(!multiple) Inisiasi();				
						cin >> angle >> ax >> ay >> az;
						dAngle = angle/200;
					}
					if(multiple){// program masih dalam proses multiple
						rotate(Prev_current, angle, ax, ay, az, N);
						Rotasi_Multiple.push(mp(idx, mp(dAngle, mp(ax, mp(ay, az)))));
						Inisiasi_Multiple();
					}
					else{// program tidak dalam prosed multiple
						rotate(current, dAngle, ax, ay, az, N);
						cnt++;
						if(cnt == 200)
						 	Proses = false;
						}
					 break;
			case 4 :if(!Proses){
						if(!multiple) 
							Inisiasi();				
						float skala;
						cin >> skala;
						dilate(Prev_current, skala, N);
						if(!multiple) 
							Update_Difference(Prev_current, current, diference, N); 
					}
					if(multiple) 
						Inisiasi_Multiple();
					else // program tidak dalam prosed multiple
						Update_Current();
					 break;
			case 5 : if(!Proses){
						if(!multiple) 
							Inisiasi();				
						GLfloat a, b, c, d;
						cin >> a >> b >> c >> d;
						custom(Prev_current, a, b, c, d, N);
					 	if(!multiple) 
					 		Update_Difference(Prev_current, current, diference, N);
					 }
					if(multiple) 
						Inisiasi_Multiple();
					else // program tidak dalam prosed multiple 
						Update_Current();
					 break;
			case 6 :if(!Proses){
						if(!multiple) Inisiasi();				
						cin >> Param_Refleksi;
						if (Param_Refleksi == "x") tipe = 1;
						else if (Param_Refleksi == "y") tipe = 2;
						else if (Param_Refleksi == "y=x") tipe = 3;
						else if (Param_Refleksi == "y=-x") tipe = 4;
						else tipe = 5;
						refleksi(Prev_current, tipe, N, Param_Refleksi);
						if(!multiple) Update_Difference(Prev_current, current, diference, N); 
					}
					if(multiple) 
						Inisiasi_Multiple();
					else// program tidak dalam prosed multiple
						Update_Current();
					break;
			case 7 : if(!Proses){
						if(!multiple) Inisiasi();				
						cin >> Param_Refleksi >>  k;
						if (Param_Refleksi == "x")
							custom(Prev_current, 1, k, 0, 1, N);
						else
							custom(Prev_current, 1, 0, k, 1, N);
						if(!multiple) Update_Difference(Prev_current, current, diference, N);	
					}
					if(multiple) 
						Inisiasi_Multiple();
					else// program tidak dalam prosed multiple
					 	Update_Current();
					 break;
			case 8 : if(!Proses){
						if(!multiple) Inisiasi();				
						cin >> Param_Refleksi >> k;
						if (Param_Refleksi == "x")
							custom(Prev_current, k, 0, 0, 1, N);
						else
							custom(Prev_current, 1, 0, 0, k, N);
						if(!multiple) Update_Difference(Prev_current, current, diference, N);
					}
					if(multiple)
						Inisiasi_Multiple();
					else// program tidak dalam prosed multiple
						Update_Current();
					break;
			case 9 : if(!Proses){
						if(!multiple) Inisiasi();
						copyMatriks(Original, Prev_current, N);
					 	if(!multiple) Update_Difference(Prev_current, current, diference, N);
					 }
					 if(multiple) Inisiasi_Multiple();
					else Update_Current();
					 break;
			case 10 : glutLeaveMainLoop();
					 break;
			case 11 :if(!Proses){
						copyMatriks(current, Prev_current, N);
						cin >> BanyakProses;
						multiple = true;
						idx = 0;
						Selesai_Multiple = false;
						cnt = 0;
						memset(Multiple,0,sizeof(Multiple));
					}
					else{
						if(cnt%200 == 0){
							copyMatriks(Multiple[start], Prev_current, N);
							Update_Difference(Prev_current, current, diference, N);
						}
						if(cnt == 200*BanyakProses){
							Proses = false;
							Selesai_Multiple = true;
						}
						else if(start == Rotasi_Multiple.front().fi){
							cnt++;
							dAngle = Rotasi_Multiple.front().se.fi;
							ax = Rotasi_Multiple.front().se.se.fi;
							ay = Rotasi_Multiple.front().se.se.se.fi;
							az = Rotasi_Multiple.front().se.se.se.se;
							rotate(current, dAngle, ax, ay, az, N);
							if(cnt%200 == 0) {
								Rotasi_Multiple.pop();
								start++;
							}
						}
						else{
							cnt++;
							Animated(current,diference);
							if(cnt%200 == 0) start++;
						}
					}
					break;
		}
		if(idx == BanyakProses && !Selesai_Multiple && multiple){
		// idx adalah parameter masukan multiple. idx = BanyakProses berarti sudah ada sebanyak idx operasi yang masuk
			Proses = true;
			multiple = false;
		}
		baca = false;	
	} 
	else {
		glutPostRedisplay();
		Sleep(10);
		baca = true;
	}
}


void readme(){
	system("CLS");
	cout << "	  	   _____         _                                   \n";    
	cout << "	  	  |_   _|       | |                                  \n";
	cout << "	  	    | | ___ _ __| |__   __ _ _   _  __ _ _ __   __ _ \n";
	cout << "	  	    | |/ _ \\'__| '_  \\ / _` | | | |/ _` | '_ \\ / _` |\n";
	cout << "	  	    | |  __/ |  | |_) | (_| | |_| | (_| | | | | (_| |\n";
	cout << "	  	    \\_/\\___|_|  |_.__/ \\__,_|\\__, |\\__,_|_| |_|\\__, |\n";
	cout << "	  		  	              __/ |             __/ |\n";
	cout << "  			                     |____/            |____/ \n\n";
	cout << "----------------------------------------------------------------------------------------\n";
	cout << "----------(c) Muh. Habibi Haidir (13516085) dan Restu Wahyu Kartiko (13516155)----------\n";
	cout << "----------------------------------------------------------------------------------------\n";
	cout << "-----------------------------Perintah yang dapat dijalankan-----------------------------\n";
	cout << "input, translate, dilate, rotate, reflect, shear, custom, stretch, reset, multiple, exit\n";
	cout << "----------------------------------------------------------------------------------------\n";
	cout << "--------Syntax perintah untuk tranformasi sama seperti pada file deskripsi tugas--------\n";
	cout << "----------------------------------------------------------------------------------------\n\n";
}

int main(int argc, char** argv)
{
	readme();
	Gambar_Axis();
	initGLUT(argc, argv);
	initGL();
	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
