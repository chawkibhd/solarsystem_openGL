#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib> 

// === Caméra FPV ===
float camX = 0.0f, camY = 5.0f, camZ = 25.0f;
float angleX = 0.0f, angleY = 0.0f;
float moveSpeed = 0.5f;
float mouseSensitivity = 0.2f;
bool isDragging = false;
int lastMouseX = 0, lastMouseY = 0;

// === Lumière ===
GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f}; 
float lightIntensity = 2.0f;                   // Intensité globale de la lumière

// === Angles des planètes ===
float angleMercure = 0.0f, angleVenus = 0.0f, angleTerre = 0.0f;
float angleMars = 0.0f, angleJupiter = 0.0f, angleSaturne = 0.0f;
float angleUranus = 0.0f, angleNeptune = 0.0f, anglelune = 0.0f;

// === Initialisation lumière et matériau ===
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat ambient[]  = {0.2f*lightIntensity, 0.2f*lightIntensity, 0.2f*lightIntensity, 1.0f};
    GLfloat diffuse[]  = {1.0f*lightIntensity, 1.0f*lightIntensity, 1.0f*lightIntensity, 1.0f};
    GLfloat specular[] = {1.0f*lightIntensity, 1.0f*lightIntensity, 1.0f*lightIntensity, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
}
void drawOrbit(float radius) {
    glColor3f(1.0f, 1.0f, 1.0f); // couleur blanche pour l'orbite
    glBegin(GL_LINE_LOOP);
        for(int i=0; i<100; i++) { // 100 segments pour un cercle lisse
            float angle = 2 * M_PI * i / 100;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            glVertex3f(x, 0, z);
        }
    glEnd();
}
// === Dessin du système solaire ===
void drawSolarSystem() {
    // Orbites des planètes
    drawOrbit(3);  // Mercure
    drawOrbit(5);  // Vénus
    drawOrbit(7);  // Terre
    drawOrbit(9);  // Mars
    drawOrbit(12); // Jupiter
    drawOrbit(16); // Saturne
    drawOrbit(19); // Uranus
    drawOrbit(22); // Neptune
    // Soleil
    glPushMatrix();
        GLfloat sunEmission[] = {1.0, 1.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, sunEmission);
        glColor3f(1.0, 1.0, 0.0);
        glutSolidSphere(2.0, 50, 50);
        GLfloat noEmission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    glPopMatrix();

    // Mercure
    glPushMatrix();
        glRotatef(angleMercure, 0, 1, 0);
        glTranslatef(3, 0, 0);
        glRotatef(angleMercure*5, 0, 1, 0);
        glColor3f(0.5, 0.5, 0.5);
        glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    // Vénus
    glPushMatrix();
        glRotatef(angleVenus, 0, 1, 0);
        glTranslatef(5, 0, 0);
        glRotatef(angleVenus*3, 0, 1, 0);
        glColor3f(1.0, 0.8, 0.0);
        glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    // Terre
    glPushMatrix();
        glRotatef(angleTerre, 0, 1, 0);
        glTranslatef(7, 0, 0);
        glRotatef(angleTerre*10, 0, 1, 0);
        glColor3f(0.0, 0.0, 1.0);
        glutSolidSphere(0.5, 20, 20);


            glPushMatrix();
            glRotatef(anglelune, 0, 1, 0);      
            glTranslatef(0.7, 0, 0);             
            glRotatef(anglelune*5, 0, 1, 0);     
            glColor3f(1.0, 1.0, 1.0);
            glutSolidSphere(0.1, 10, 10);     
            glPopMatrix();
    glPopMatrix();

    // Mars
    glPushMatrix();
        glRotatef(angleMars, 0, 1, 0);
        glTranslatef(9, 0, 0);
        glRotatef(angleMars*8, 0, 1, 0);
        glColor3f(1.0, 0.0, 0.0);
        glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // Jupiter
    glPushMatrix();
        glRotatef(angleJupiter, 0, 1, 0);
        glTranslatef(12, 0, 0);
        glRotatef(angleJupiter*3, 0, 1, 0);
        glColor3f(1.0, 0.5, 0.0);
        glutSolidSphere(1.0, 30, 30);
    glPopMatrix();

    // Saturne
    glPushMatrix();
        glRotatef(angleSaturne, 0, 1, 0);
        glTranslatef(16, 0, 0);
        glRotatef(angleSaturne*3, 0, 1, 0);
        glColor3f(1.0, 1.0, 0.5);
        glutSolidSphere(0.9, 30, 30);
        glRotatef(90, 1, 0, 0);
        glutWireTorus(0.1, 1.2, 10, 30);
    glPopMatrix();

    // Uranus
    glPushMatrix();
        glRotatef(angleUranus, 0, 1, 0);
        glTranslatef(19, 0, 0);
        glRotatef(angleUranus*3, 0, 1, 0);
        glColor3f(0.0, 1.0, 1.0);
        glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    // Neptune
    glPushMatrix();
        glRotatef(angleNeptune, 0, 1, 0);
        glTranslatef(22, 0, 0);
        glRotatef(angleNeptune*3, 0, 1, 0);
        glColor3f(0.0, 0.0, 0.8);
        glutSolidSphere(0.7, 20, 20);
    glPopMatrix();
}
struct Star {
    float x, y, z;
};
std::vector<Star> stars;
int starCount = 10000;

// Appelle une seule fois au début
void initStars() {
    for(int i = 0; i < starCount; i++) {
        Star s;
        s.x = (rand() % 2000 - 1000) / 100.0f;
        s.y = (rand() % 1000 - 500) / 100.0f;
        s.z = (rand() % 2000 - 1000) / 100.0f;
        stars.push_back(s);
    }
}

// Dessin des étoiles fixes
void drawStars() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0f);

    glBegin(GL_POINTS);
    for(auto &s : stars) {
        glVertex3f(s.x, s.y, s.z);
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
// === Affichage ===
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    drawStars();

    float dirX = cos(angleX * M_PI / 180.0f) * sin(angleY * M_PI / 180.0f);
    float dirY = sin(angleX * M_PI / 180.0f);
    float dirZ = -cos(angleX * M_PI / 180.0f) * cos(angleY * M_PI / 180.0f);

    gluLookAt(camX, camY, camZ,camX + dirX, camY + dirY, camZ + dirZ,0.0, 1.0, 0.0);

    drawSolarSystem();
    glutSwapBuffers();
}

// === Animation planètes ===
void update(int value) {
    angleMercure += 4.0f;
    angleVenus   += 1.6f;
    angleTerre   += 1.0f;
    anglelune   += 0.15f;
    angleMars    += 0.8f;
    angleJupiter += 0.4f;
    angleSaturne += 0.3f;
    angleUranus  += 0.2f;
    angleNeptune += 0.15f;

    if(angleMercure>360) angleMercure-=360;
    if(angleVenus>360) angleVenus-=360;
    if(angleTerre>360) angleTerre-=360;
    if(anglelune>360) anglelune-=360;
    if(angleMars>360) angleMars-=360;
    if(angleJupiter>360) angleJupiter-=360;
    if(angleSaturne>360) angleSaturne-=360;
    if(angleUranus>360) angleUranus-=360;
    if(angleNeptune>360) angleNeptune-=360;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// === Caméra clavier + contrôle lumière ===
void keyboard(unsigned char key, int x, int y) {
    float radY = angleY * M_PI / 180.0f;
    float forwardX = sin(radY);
    float forwardZ = -cos(radY);
    float rightX = cos(radY);
    float rightZ = sin(radY);

    switch(key) {
        case 'z': camX += forwardX*moveSpeed; camZ += forwardZ*moveSpeed; break;
        case 's': camX -= forwardX*moveSpeed; camZ -= forwardZ*moveSpeed; break;
        case 'q': camX -= rightX*moveSpeed; camZ += rightZ*moveSpeed; break;
        case 'd': camX += rightX*moveSpeed; camZ -= rightZ*moveSpeed; break;
        case 'a': camY -= moveSpeed; break;
        case 'e': camY += moveSpeed; break;
        case ' ': camX=0; camY=5; camZ=25; angleX=0; angleY=0; break;
        case 27: exit(0); break;
        case '+': lightIntensity += 0.2f; initLighting(); break;
        case '-': lightIntensity -= 0.2f; if(lightIntensity<0) lightIntensity=0; initLighting(); break;
    }
    glutPostRedisplay();
}

// === Caméra souris ===
void mouseMotion(int x, int y) {
    if(!isDragging) return;
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    angleY += dx*mouseSensitivity;
    angleX -= dy*mouseSensitivity;
    if(angleX>89) angleX=89;
    if(angleX<-89) angleX=-89;
    lastMouseX = x; lastMouseY = y;
    glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
    if(button==GLUT_LEFT_BUTTON) {
        isDragging = (state==GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
}

// === Redimensionnement ===
void reshape(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// === Initialisation OpenGL ===
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    initLighting();
    initStars();
}

// === Main ===
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Système Solaire 3D");

    init();
     
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}