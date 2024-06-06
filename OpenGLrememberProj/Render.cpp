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
//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
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
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
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
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			//Circus с;
			//с.pos.setCoords(pos.X(), pos.Y(), 0);
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

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//двигаем свет по плоскости, в точку где мышь
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

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	

	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

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



//float angle = 0.0f; // Угол вращения сферы
//float radius = 2.0f; // Радиус вращения сферы вокруг куба
//
//void init() {
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Очистка цвета фона (черный)
//	glEnable(GL_DEPTH_TEST); // Включение теста глубины
//}
//
//void display() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экрана и буфера глубины
//	glLoadIdentity(); // Сброс текущей матрицы
//
//	// Расположение камеры
//
//	// Рисуем куб в центре сцены
//	glPushMatrix();
//	glColor3f(1.0f, 0.0f, 0.0f); // Цвет куба (красный)
//	glutSolidCube(1.0f);
//	glPopMatrix();
//
//	// Вычисляем положение сферы
//	float sphereX = radius * cos(angle);
//	float sphereZ = radius * sin(angle);
//
//	// Рисуем сферу
//	glPushMatrix();
//	glTranslatef(sphereX, 0.0f, sphereZ);
//	glColor3f(0.0f, 0.0f, 1.0f); // Цвет сферы (синий)
//	glutSolidSphere(0.5, 20, 20);
//	glPopMatrix();
//
//	glutSwapBuffers(); // Обмен буферов
//}
//
//void update(int value) {
//	angle += 0.01f; // Увеличиваем угол вращения
//	if (angle > 2 * M_PI) {
//		angle -= 2 * M_PI; // Ограничиваем угол вращения в пределах 0 до 2*PI
//	}
//	glutPostRedisplay(); // Перерисовываем экран
//	glutTimerFunc(16, update, 0); // Устанавливаем таймер для следующего обновления
//}

//struct Vec3 {
//	float x, y, z;
//};
//
//Vec3 calculateSpherePosition(float angle, float radius) {
//	Vec3 position;
//	position.x = radius * cos(angle);
//	position.z = radius * sin(angle);
//	position.y = 0.0f; // Сфера вращается в горизонтальной плоскости
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


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут  
	//

	t_max += ttime;
	if (t_max > 1) {
		ttime *= -1;
	} //после обнуляется
	if (t_max < 0) {
		ttime *= -1;
	} //после обнуляется
	double P1[] = { 0,-15,0 }; //Наши точки, массивчик double
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

	glBegin(GL_LINES); //построим отрезки P1P2 и P2P3
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P3);
	glVertex3dv(P4);
	glVertex3dv(P5);


	glEnd();
	glLineWidth(3); //ширина линии
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
	glLineWidth(1); //возвращаем ширину линии = 1
	glBegin(GL_LINES); //построим отрезок AB
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

	
	//float angle = 0.0f; // Начальный угол
	//float radius = 2.0f; // Радиус орбиты

	//for (int i = 0; i < 360; ++i) {
	//	angle = i * (M_PI / 180.0f); // Преобразование градусов в радианы
	//	Vec3 spherePos = calculateSpherePosition(angle, radius);

	//	std::cout << "Angle: " << angle << " radians, ";
	//	std::cout << "Sphere Position: (" << spherePos.x << ", " << spherePos.y << ", " << spherePos.z << ")\n";
	//}

	//конец рисования квадратика станкина*/


   //Сообщение вверху экрана

	
	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
	                                //(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "P - Пауза" << std::endl;
	ss << "N - Сменить траекторию движения" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}
void Render(double delta_time)
{

}

