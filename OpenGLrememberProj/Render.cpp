#include "Render.h"
#include "Spheres.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <GL/glut.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"
#include "math.h"


bool textureMode = true;
bool lightMode = true;
double* Normal(double* a, double* b, double* c)
{
	double ABx = b[0] - a[0];
	double ABy = b[1] - a[1];
	double ABz = b[2] - a[2];
	double ACx = c[0] - a[0];
	double ACy =c[1] - a[1];
	double ACz = c[2] - a[2];
	double aa[] = { ABx,ABy,ABz };
	double bb[] = { ACx,ACy,ACz };
	double nx = aa[1] * bb[2] - bb[1] * aa[2];
	double ny = -aa[0] * bb[2] + bb[0] * aa[2];
	double nz = aa[0] * bb[1] - bb[0] * aa[1];
	double n = sqrt(nx * nx + ny * ny + nz * nz);
	nx /= n;
	ny /= n;
	nz /= n;
	double nn[] = { nx,ny,nz };
	return nn;
}
//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			//Circus �;
			//�.pos.setCoords(pos.X(), pos.Y(), 0);
			//c.scale = c.scale*1.5;
			//c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

bool flag = 0;
bool tflag = 1;


void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

	if (key == 'N')
	{
		if (flag) {
			flag = 0;
		}
		else {
			flag = 1;

		}
	}
	if (key == 'P')
	{
		if (tflag) {
			tflag = 0;
		}
		else {
			tflag = 1;

		}
	}

}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//���������� �� ��� ��������
	glGenTextures(1, &texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}


void Circus(double* c, double r) {
	double d = 0;
	while (d <= 180) {
		glVertex3dv(c);
		glVertex3d(r * cos(d) + c[0], r * sin(d) + c[1], c[2]);
		glVertex3d(r * cos(d + 0.1) + c[0], r * sin(d + 0.1) + c[1], c[2]);
		d += 0.1;
	}

}

void Round(double* a, double* b, double h) {
	glColor3d(0.3, 0.5, 0.2);
	glBegin(GL_TRIANGLES);
	double middle[] = { (a[0] + b[0]) / 2, (a[1] + b[1]) / 2, (a[2] + b[2]) / 2 };
	double radius = sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2])) / 2;
	Circus(middle, radius);
	double middle1[] = { middle[0], middle[1], middle[2] + h };
	Circus(middle1, radius);
	glEnd();
	glColor3d(0.2, 0.2, 0.5);
	glBegin(GL_QUADS);
	double d = 0;
	while (d <= 180) {
		double Z[] = { radius * cos(d) + middle[0], radius * sin(d) + middle[1], middle[2] };
		double X[] = { radius * cos(d) + middle1[0], radius * sin(d) + middle1[1], middle1[2] };
		double C[] = { radius * cos(d + 0.1) + middle1[0], radius * sin(d + 0.1) + middle1[1], middle1[2] };
		glNormal3dv(Normal(C,X, Z));
		glVertex3d(radius * cos(d) + middle[0], radius * sin(d) + middle[1], middle[2]);
		glVertex3d(radius * cos(d) + middle1[0], radius * sin(d) + middle1[1], middle1[2]);
		glVertex3d(radius * cos(d + 0.1) + middle1[0], radius * sin(d + 0.1) + middle1[1], middle1[2]);
		glVertex3d(radius * cos(d + 0.1) + middle[0], radius * sin(d + 0.1) + middle[1], middle[2]);
		d += 0.1;
	}
	glEnd();
	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3dv(a);
	glVertex3dv(b);
	glEnd();
}




double f(double a, double b, double c, double d, double e, double t)
{
	return (1 - t) * (1 - t) * (1 - t) * (1 - t) * a + 4 * t * (1 - t) * (1 - t) * (1 - t) * b + 6 * t * t * (1 - t) * (1 - t) * c + t * t * t * (1 - t)*4 * d + t * t * t * t *  d;
}
double f2(double a, double b, double c, double d, double t) {//a - p1, b - p4,c - r1,d - r4
	return(a * (2 * t * t * t - 3 * t * t + 1) + b * (-2 * t * t * t + 3 * t * t) + c * (t * t * t - 2 * t * t + t) + d * (t * t * t - t * t));
}
double* beze(double* P1, double* P2, double* P3, double* P4, double* P5, double t) {
	double p[6];
	p[0] = f(P1[0], P2[0], P3[0], P4[0], P5[0], t);
	p[1] = f(P1[1], P2[1], P3[1], P4[1], P5[1], t);
	p[2] = f(P1[2], P2[2], P3[2], P4[2], P5[2], 0.5*t);
	return p;
}
double* ermyt(double* P1, double* P2, double* P3, double* P4, double t) {
	double R1[3] = { (P1[0] - P2[0]),(P1[1] - P2[1]) ,(P1[2] - P2[2]) };
	double R4[3] = { (P3[0] - P4[0]),(P3[1] - P4[1]) ,(P3[2] - P4[2]) };
	double p[3];
	p[0] = f2(P1[0], P3[0], R1[0], R4[0], t);
	p[1] = f2(P1[1], P3[1], R1[1], R4[1], t);
	p[2] = f2(P1[2], P3[2], R1[2], R4[2], t);
	return p;
}


double t_max = 0;
double ttime = 0.01;



//float angle = 0.0f; // ���� �������� �����
//float radius = 2.0f; // ������ �������� ����� ������ ����
//
//void init() {
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ������� ����� ���� (������)
//	glEnable(GL_DEPTH_TEST); // ��������� ����� �������
//}
//
//void display() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ������ � ������ �������
//	glLoadIdentity(); // ����� ������� �������
//
//	// ������������ ������
//
//	// ������ ��� � ������ �����
//	glPushMatrix();
//	glColor3f(1.0f, 0.0f, 0.0f); // ���� ���� (�������)
//	glutSolidCube(1.0f);
//	glPopMatrix();
//
//	// ��������� ��������� �����
//	float sphereX = radius * cos(angle);
//	float sphereZ = radius * sin(angle);
//
//	// ������ �����
//	glPushMatrix();
//	glTranslatef(sphereX, 0.0f, sphereZ);
//	glColor3f(0.0f, 0.0f, 1.0f); // ���� ����� (�����)
//	glutSolidSphere(0.5, 20, 20);
//	glPopMatrix();
//
//	glutSwapBuffers(); // ����� �������
//}
//
//void update(int value) {
//	angle += 0.01f; // ����������� ���� ��������
//	if (angle > 2 * M_PI) {
//		angle -= 2 * M_PI; // ������������ ���� �������� � �������� 0 �� 2*PI
//	}
//	glutPostRedisplay(); // �������������� �����
//	glutTimerFunc(16, update, 0); // ������������� ������ ��� ���������� ����������
//}

//struct Vec3 {
//	float x, y, z;
//};
//
//Vec3 calculateSpherePosition(float angle, float radius) {
//	Vec3 position;
//	position.x = radius * cos(angle);
//	position.z = radius * sin(angle);
//	position.y = 0.0f; // ����� ��������� � �������������� ���������
//	return position;
//}

void Render(OpenGL *ogl)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  
	//

	t_max += ttime;
	if (t_max > 1) {
		ttime *= -1;
	} //����� ����������
	if (t_max < 0) {
		ttime *= -1;
	} //����� ����������
	double P1[] = { 0,-15,0 }; //���� �����, ��������� double
	double P2[] = { 10,0,-5 };
	double P3[] = { 0,10,0 };
	double P4[] = { -10,0,10 };
	double P5[] = { 5,-10,0 };


	double down[8][3] = {
	{0, 0, 0},
	{0, 7, 0},
	{8, 3, 0},
	{2, -1, 0},
	{6, -7, 0},
	{1, -2, 0},
	{-3, -6, 0},
	{-5, -2, 0}
	};
	double up[8][3] = {
	{0, 0, 1},
	{0, 7, 1},
	{8, 3, 1},
	{2, -1, 1},
	{6, -7, 1},
	{1, -2, 1},
	{-3, -6, 1},
	{-5, -2, 1}
	};

	glBindTexture(GL_TEXTURE_2D, texId);

	//glColor3d(0.7, 0.7, 0.7);
	//glBegin(GL_QUADS);
	//glNormal3d(0, 0, 1);
	//glTexCoord2d(0, 0);
	//glVertex3dv(D);
	//glTexCoord2d(1, 0);
	//glVertex3dv(C);
	//glTexCoord2d(1, 1);
	//glVertex3dv(B);
	//glTexCoord2d(0, 1);
	//glVertex3dv(A);
	//glEnd();



	double lastend[] = { 0,0,0 };

	glBegin(GL_LINES); //�������� ������� P1P2 � P2P3
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P3);
	glVertex3dv(P4);
	glVertex3dv(P5);


	glEnd();
	glLineWidth(3); //������ �����
	glColor3d(0, 1, 0);
	if (!tflag) {
		if (flag) {
			glBegin(GL_LINE_STRIP);
			for (double t = 0; t <= 1.0001; t += 0.01) {
				glVertex3dv(ermyt(P1, P2, P3, P4, t));
			}
			glEnd();
			double p[] = { ermyt(P1, P2, P3, P4, t_max)[0], ermyt(P1, P2, P3, P4, t_max)[1],ermyt(P1, P2, P3, P4, t_max)[2] };
			glTranslated(p[0], p[1], p[2]);
		}
		else {
			glBegin(GL_LINE_STRIP);
			for (double t = 0; t <= 1.0001; t += 0.01) {
				glVertex3dv(beze(P1, P2, P3, P4, P5, t));
			}
			glEnd();
			double p[] = { beze(P1, P2, P3, P4, P5, t_max)[0], beze(P1, P2, P3, P4, P5, t_max)[1],beze(P1, P2, P3, P4, P5, t_max)[2] };
			glTranslated(p[0], p[1], p[2]);

		}

	}
	glBegin(GL_POLYGON);
	glColor3d(0.2, 0.7, 0.7);

	//glVertex3dv(A);
	//glVertex3dv(B);
	//glVertex3dv(C);
	//glVertex3dv(D);

	for (int i = 0; i < 8; i++) {
		glVertex3dv(up[i]);
	}
	for (int i = 0; i < 8; i++) {
		glVertex3dv(down[i]);
	}
	for (int i = 0; i < 8; i++) {
		glVertex3dv(up[i]);
		glVertex3dv(down[i]);
	}
	//double A[3] = { 1, -2, 1.1 };
	//double B[3] = { -3, -6, 1.1 };
	//double C[3] = { -5, -2, 1.1 };
	//double D[3] = { 0, 0, 1.1 };
	glBindTexture(GL_TEXTURE_2D, texId);

	glColor3d(0.7, 0.7, 0.7);
	//glBegin(GL_QUADS);

	//glNormal3d(0, 0, 1);
	//glTexCoord2d(0, 0);
	//glVertex3dv(D);
	//glTexCoord2d(1, 0);
	//glVertex3dv(C);
	//glTexCoord2d(1, 1);
	//glVertex3dv(B);
	//glTexCoord2d(0, 1);
	//glVertex3dv(A);

	//glEnd();


	glEnd();
	glColor3d(0.7, 0.7, 0.7);
	glLineWidth(1); //���������� ������ ����� = 1
	glBegin(GL_LINES); //�������� ������� AB
	glEnd();

	glBegin(GL_QUADS);
	
	
	double A1[3] = { 0, 0, 1.005 };
	double B1[3] = { 0, 7, 1.005 };
	double C1[3] = { 8, 3, 1.005 };
	double D1[3] = { 2, -1, 1.005 };

	glNormal3d(0, 0, 1);
	glTexCoord2d(0, 0);
	glVertex3dv(D1);
	glTexCoord2d(1, 0);
	glVertex3dv(C1);
	glTexCoord2d(1, 1);
	glVertex3dv(B1);
	glTexCoord2d(0, 1);
	glVertex3dv(A1);

	glEnd();

	
	//float angle = 0.0f; // ��������� ����
	//float radius = 2.0f; // ������ ������

	//for (int i = 0; i < 360; ++i) {
	//	angle = i * (M_PI / 180.0f); // �������������� �������� � �������
	//	Vec3 spherePos = calculateSpherePosition(angle, radius);

	//	std::cout << "Angle: " << angle << " radians, ";
	//	std::cout << "Sphere Position: (" << spherePos.x << ", " << spherePos.y << ", " << spherePos.z << ")\n";
	//}

	//����� ��������� ���������� ��������*/


   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "P - �����" << std::endl;
	ss << "N - ������� ���������� ��������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}
void Render(double delta_time)
{

}

