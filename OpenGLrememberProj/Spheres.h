//#pragma once
//#include <iostream>
//
//#include <windows.h>
//#include <GL\GL.h>
//#include <GL\GLU.h>
////#include <SOIL\SOIL.h>
//
//#include "MyOGL.h"
//
//#include "Camera.h"
//#include "Light.h"
//#include "Primitives.h"
//
//#include "GUItextRectangle.h"
//#include "math.h"
//
////class Spheres {
////public:
////	Spheres(const char* textureImagePath, const char* alphaImagePath)
////		: texturePath(textureImagePath), alphaPath(alphaImagePath),
////		rotationAngle(0), sphereRadius(1.0), rotationRadius(3.0) {}
////
////	bool init();
////	void run();
////
////private:
////	GLuint loadTexture(const char* imagePath, bool alpha);
////	void setupLighting();
////	void drawSphere(GLuint texture, bool alpha);
////	void render();
////
////	const char* texturePath;
////	const char* alphaPath;
////	GLuint texture;
////	GLuint alphaTexture;
////	float rotationAngle;
////	float sphereRadius;
////	float rotationRadius;
////};
////
////bool Spheres::init() {
////	if (!glfwInit()) {
////		return false;
////	}
////
////	window = glfwCreateWindow(800, 600, "Rotating Spheres", nullptr, nullptr);
////	if (!window) {
////		glfwTerminate();
////		return false;
////	}
////
////	glfwMakeContextCurrent(window);
////	glewExperimental = GL_TRUE;
////	glewInit();
////
////	glEnable(GL_DEPTH_TEST);
////
////	texture = loadTexture(texturePath, false);
////	alphaTexture = loadTexture(alphaPath, true);
////
////	setupLighting();
////
////	return true;
////}
////
////GLuint Spheres::loadTexture(const char* imagePath, bool alpha) {
////	int width, height;
////	unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
////	GLuint textureID;
////	glGenTextures(1, &textureID);
////	glBindTexture(GL_TEXTURE_2D, textureID);
////	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	SOIL_free_image_data(image);
////
////	return textureID;
////}
////
////void Spheres::setupLighting() {
////	glEnable(GL_LIGHTING);
////	glEnable(GL_LIGHT0);
////	GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
////	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
////	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
////	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
////	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
////	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
////}
////
////void Spheres::drawSphere(GLuint texture, bool alpha) {
////	glBindTexture(GL_TEXTURE_2D, texture);
////	glEnable(GL_TEXTURE_2D);
////	if (alpha) {
////		glEnable(GL_BLEND);
////		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
////	}
////	GLUquadric* quadric = gluNewQuadric();
////	gluQuadricTexture(quadric, GL_TRUE);
////	gluSphere(quadric, sphereRadius, 32, 32);
////	gluDeleteQuadric(quadric);
////	if (alpha) {
////		glDisable(GL_BLEND);
////	}
////	glDisable(GL_TEXTURE_2D);
////}
////
////void Spheres::render() {
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////	glLoadIdentity();
////	gluPerspective(45.0, 800.0 / 600.0, 0.1, 50.0);
////	glTranslatef(0.0, 0.0, -10.0);
////
////	// Draw central sphere with alpha texture
////	drawSphere(alphaTexture, true);
////
////	// Calculate position for rotating sphere
////	float x = rotationRadius * std::cos(rotationAngle * M_PI / 180.0);
////	float z = rotationRadius * std::sin(rotationAngle * M_PI / 180.0);
////
////	glPushMatrix();
////	glTranslatef(x, 0, z);
////	drawSphere(texture);
////	glPopMatrix();
////
////	rotationAngle += 1.0f;
////
////	glfwSwapBuffers(window);
////}
//
//
//
//class Spheres {
//public:
//    Spheres(const char* textureImagePath, const char* alphaImagePath)
//        : texturePath(textureImagePath), alphaPath(alphaImagePath),
//        rotationAngle(0), sphereRadius(1.0), rotationRadius(3.0) {}
//
//    bool init();
//    void renderToFBO();
//
//private:
//    GLuint loadTexture(const char* imagePath, bool alpha);
//    void setupLighting();
//    void drawSphere(GLuint texture, bool alpha);
//    void render();
//
//    const char* texturePath;
//    const char* alphaPath;
//    GLuint texture;
//    GLuint alphaTexture;
//    GLuint fbo;
//    GLuint fboTexture;
//    float rotationAngle;
//    float sphereRadius;
//    float rotationRadius;
//};
//
//bool Spheres::init() {
//    //glewExperimental = GL_TRUE;
//    //if (glewInit() != GLEW_OK) {
//    //    std::cerr << "Failed to initialize GLEW" << std::endl;
//    //    return false;
//    //}
//
//    glEnable(GL_DEPTH_TEST);
//
//    texture = loadTexture(texturePath, false);
//    alphaTexture = loadTexture(alphaPath, true);
//
//    setupLighting();
//
//    // Setup FBO
//    //glGenFramebuffers(1, &fbo);
//    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//
//    glGenTextures(1, &fboTexture);
//    glBindTexture(GL_TEXTURE_2D, fboTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
//
//    GLuint rbo;
//    //glGenRenderbuffers(1, &rbo);
//    //glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
//    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
//
//    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//    //    std::cerr << "Failed to create framebuffer" << std::endl;
//    //    return false;
//    //}
//
//    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    return true;
//}
//
////GLuint Spheres::loadTexture(const char* imagePath, bool alpha) {
////    int width, height;
////    //unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
////    GLuint textureID;
////    glGenTextures(1, &textureID);
////    glBindTexture(GL_TEXTURE_2D, textureID);
////    //glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////    //SOIL_free_image_data(image);
////
////    return textureID;
////}
//
//void Spheres::setupLighting() {
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
//    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
//}
//
//void Spheres::drawSphere(GLuint texture, bool alpha) {
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glEnable(GL_TEXTURE_2D);
//    if (alpha) {
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    }
//    GLUquadric* quadric = gluNewQuadric();
//    gluQuadricTexture(quadric, GL_TRUE);
//    gluSphere(quadric, sphereRadius, 32, 32);
//    gluDeleteQuadric(quadric);
//    if (alpha) {
//        glDisable(GL_BLEND);
//    }
//    glDisable(GL_TEXTURE_2D);
//}
//
//void Spheres::render() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//    gluPerspective(45.0, 800.0 / 600.0, 0.1, 50.0);
//    glTranslatef(0.0, 0.0, -10.0);
//
//    // Draw central sphere with alpha texture
//    drawSphere(alphaTexture, true);
//
//    // Calculate position for rotating sphere
//    float x = rotationRadius * std::cos(rotationAngle * M_PI / 180.0);
//    float z = rotationRadius * std::sin(rotationAngle * M_PI / 180.0);
//
//    glPushMatrix();
//    glTranslatef(x, 0, z);
//    //drawSphere(texture);
//    glPopMatrix();
//
//    rotationAngle += 1.0f;
//}
//
//void Spheres::renderToFBO() {
//    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//    render();
//    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    // Optionally, save the FBO texture to an image file using SOIL
//    glBindTexture(GL_TEXTURE_2D, fboTexture);
//    unsigned char* data = new unsigned char[800 * 600 * 3];
//    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    //SOIL_save_image("output.png", SOIL_SAVE_TYPE_PNG, 800, 600, 3, data);
//    delete[] data;
//}