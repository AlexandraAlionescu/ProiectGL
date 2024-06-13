#include "glut.h"
#include <math.h>
#include <stdio.h>

// Function prototypes
void myInit();
void display();
void myReshape(GLsizei w, GLsizei h);
void rotatieDreapta();
void rotatieStanga();
void rotatieSus();
void rotatieJos();
void timer(int value);
void ceas();
void limbi(float length, float width, float angle);
void nume();
void textura();
void setMaterial(const GLfloat* ambient, const GLfloat* diffuse, const GLfloat* specular, GLfloat shininess);
void rotire(int key, int x, int y);
void toggleClipping();

// Global variables
float hourAngle = 0.0f;
float minuteAngle = 0.0f;
float secondAngle = 0.0f;
float rotationAngleY = 0.0f;
float rotationAngleX = 0.0f;
GLuint texture;
bool clippingEnabled = false;

void textura() {
    const int width = 64;
    const int height = 64;
    GLubyte checkerboard[height][width][3];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            GLubyte c = ((((x & 8) == 0) ^ ((y & 8) == 0))) * 255;
            if (c == 255) {
                checkerboard[y][x][0] = 255; // Red
                checkerboard[y][x][1] = 182; // Green
                checkerboard[y][x][2] = 193; // Blue
            }
            else {
                checkerboard[y][x][0] = 255; // Red
                checkerboard[y][x][1] = 255; // Green
                checkerboard[y][x][2] = 255; // Blue
            }
        }
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerboard);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void myInit() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    textura();

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 1.0f, 0.0f, 2.0f, 1.0f }; // Adjusted light position from right and slightly in front

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void rotatieSus() {
    rotationAngleX += 5.0f;
}

void rotatieJos() {
    rotationAngleX -= 5.0f;
}

void rotatieDreapta() {
    rotationAngleY += 5.0f;
}

void rotatieStanga() {
    rotationAngleY -= 5.0f;
}

void toggleClipping() {
    clippingEnabled = !clippingEnabled;
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(rotationAngleX, 1.0, 0.0, 0.0);
    glRotatef(rotationAngleY, 0.0, 1.0, 0.0);

    if (clippingEnabled) {
        GLdouble plane[] = { 0.0, 1.0, 0.0, 0.0 };
        glClipPlane(GL_CLIP_PLANE0, plane);
        glEnable(GL_CLIP_PLANE0);
    }
    else {
        glDisable(GL_CLIP_PLANE0);
    }

    ceas();
    nume();

    glutSwapBuffers();
}

void myReshape(GLsizei w, GLsizei h) {
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    secondAngle -= 6.0f;
    if ((int)secondAngle % 360 == 0) {
        minuteAngle -= 6.0f / 60.0f;
        if ((int)minuteAngle % 360 == 0) {
            hourAngle -= 6.0f / 3600.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(1000, timer, 0);
}

void setMaterial(const GLfloat* ambient, const GLfloat* diffuse, const GLfloat* specular, GLfloat shininess) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void ceas() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat shininess = 100.0f;
    setMaterial(ambient, diffuse, specular, shininess);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.05);
    gluDisk(quad, 0, 1.0, 50, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.1);
    gluCylinder(quad, 1.05, 1.05, 0.3, 50, 1);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    GLfloat hand_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat hand_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat hand_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat hand_shininess = 120.0f;
    setMaterial(hand_ambient, hand_diffuse, hand_specular, hand_shininess);

    limbi(0.5f, 0.04f, hourAngle);
    limbi(0.7f, 0.03f, minuteAngle);
    limbi(0.9f, 0.02f, secondAngle);
}

void limbi(float length, float width, float angle) {
    glPushMatrix();
    glRotatef(angle, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 0.0, 0.11);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, 0.0f, 0.0f);
    glVertex3f(width / 2, 0.0f, 0.0f);
    glVertex3f(width / 2, length, 0.0f);
    glVertex3f(-width / 2, length, 0.0f);
    glEnd();
    glPopMatrix();
}

void nume() {
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos3f(-0.5f, -1.4f, 0.0f);
    const char* name = "Alionescu Alexandra";
    while (*name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *name);
        name++;
    }
    glPopMatrix();
}

void rotire(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        rotatieStanga();
        break;
    case GLUT_KEY_RIGHT:
        rotatieDreapta();
        break;
    case GLUT_KEY_UP:
        rotatieSus();
        break;
    case GLUT_KEY_DOWN:
        rotatieJos();
        break;
    case GLUT_KEY_F1:
        toggleClipping();
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Clock");

    myInit();

    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutSpecialFunc(rotire);
    glutTimerFunc(1000, timer, 0);

    glutMainLoop();
    return 0;
}
