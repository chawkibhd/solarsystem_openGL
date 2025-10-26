/*
 * Enhanced Solar System 3D Simulation
 * 
 * An interactive OpenGL/GLUT application that simulates our solar system with:
 * - All 8 planets plus Pluto (dwarf planet)
 * - Earth's Moon
 * - 1000 asteroids in a belt between Mars and Jupiter
 * - A comet with eccentric elliptical orbit
 * - 10,000 stars with varying brightness
 * - Interactive camera controls (FPS-style movement)
 * - Simulation controls (pause, speed adjustment)
 * - Visual toggles (orbits, info display, help menu)
 * 
 * Controls:
 * - ZQSD/WASD: Camera movement
 * - A/E: Move down/up
 * - Mouse: Look around (click and drag)
 * - P: Pause/Resume
 * - Up/Down: Speed control
 * - O: Toggle orbits
 * - I: Toggle info
 * - H: Toggle help
 * - 0-9: Jump to planet views
 * - +/-: Light intensity
 * - Space: Reset camera
 * - ESC: Exit
 */

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime> 

// === Camera FPS (First Person Shooter style) ===
float camX = 0.0f, camY = 5.0f, camZ = 25.0f;  // Camera position
float angleX = 0.0f, angleY = 0.0f;             // Camera rotation angles
float moveSpeed = 0.5f;                          // Movement speed
float mouseSensitivity = 0.2f;                   // Mouse sensitivity for look around
bool isDragging = false;                         // Is mouse dragging active
int lastMouseX = 0, lastMouseY = 0;              // Last mouse position

// === Lighting ===
GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};  // Light position at the Sun
float lightIntensity = 2.0f;                      // Global light intensity multiplier

// Forward declarations
void drawAsteroids();
void drawComet();
void drawStars();
void drawText(float x, float y, const std::string& text);
void displayHelp();
void displayInfo();

// === Planet orbital angles (in degrees) ===
float angleMercure = 0.0f, angleVenus = 0.0f, angleTerre = 0.0f;
float angleMars = 0.0f, angleJupiter = 0.0f, angleSaturne = 0.0f;
float angleUranus = 0.0f, angleNeptune = 0.0f, anglelune = 0.0f;
float anglePluto = 0.0f;

// === Simulation controls ===
bool isPaused = false;              // Is simulation paused
float simulationSpeed = 1.0f;       // Speed multiplier (0.1x to 10x)
bool showOrbits = true;             // Show orbital paths
bool showHelp = false;              // Show help menu overlay
bool showInfo = false;              // Show info panel (time, speed, position)
float simulationTime = 0.0f;        // Elapsed simulation time in Earth days

// === Asteroid belt ===
struct Asteroid {
    float angle;      // Current orbital angle
    float distance;   // Distance from Sun
    float speed;      // Orbital speed
    float size;       // Asteroid size
};
std::vector<Asteroid> asteroids;
int asteroidCount = 1000;  // Number of asteroids in the belt

// Initialize OpenGL lighting system
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Configure light properties based on intensity
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
// Draw a circular orbit path around the Sun
void drawOrbit(float radius) {
    if(!showOrbits) return;
    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.3f); // Dimmer gray color for subtle appearance
    glBegin(GL_LINE_LOOP);
        for(int i=0; i<100; i++) { // 100 segments for smooth circle
            float angle = 2 * M_PI * i / 100;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            glVertex3f(x, 0, z);
        }
    glEnd();
    glEnable(GL_LIGHTING);
}
// === Main solar system rendering function ===
void drawSolarSystem() {
    // Draw planetary orbits
    drawOrbit(3);  // Mercure
    drawOrbit(5);  // Vénus
    drawOrbit(7);  // Terre
    drawOrbit(9);  // Mars
    drawOrbit(12); // Jupiter
    drawOrbit(16); // Saturne
    drawOrbit(19); // Uranus
    drawOrbit(22); // Neptune
    drawOrbit(26); // Pluto
    
    // Draw asteroids
    drawAsteroids();
    
    // Draw comet
    drawComet();
    
    // The Sun at center
    glPushMatrix();
        GLfloat sunEmission[] = {1.0, 1.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, sunEmission);
        glColor3f(1.0, 1.0, 0.0);
        glutSolidSphere(2.0, 50, 50);
        GLfloat noEmission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    glPopMatrix();

    // Mercure - Gray rocky planet
    glPushMatrix();
        glRotatef(angleMercure, 0, 1, 0);
        glTranslatef(3, 0, 0);
        glRotatef(angleMercure*5, 0, 1, 0);
        glColor3f(0.6, 0.6, 0.6);
        glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    // Vénus - Yellowish planet
    glPushMatrix();
        glRotatef(angleVenus, 0, 1, 0);
        glTranslatef(5, 0, 0);
        glRotatef(angleVenus*3, 0, 1, 0);
        glColor3f(0.9, 0.7, 0.3);
        glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    // Terre - Blue and green
    glPushMatrix();
        glRotatef(angleTerre, 0, 1, 0);
        glTranslatef(7, 0, 0);
        glRotatef(angleTerre*10, 0, 1, 0);
        glColor3f(0.2, 0.4, 0.8);
        glutSolidSphere(0.5, 20, 20);

        // Earth's Moon
        glPushMatrix();
            glRotatef(anglelune, 0, 1, 0);      
            glTranslatef(0.7, 0, 0);             
            glRotatef(anglelune*5, 0, 1, 0);     
            glColor3f(0.8, 0.8, 0.8);
            glutSolidSphere(0.1, 10, 10);     
        glPopMatrix();
    glPopMatrix();

    // Mars - Red planet
    glPushMatrix();
        glRotatef(angleMars, 0, 1, 0);
        glTranslatef(9, 0, 0);
        glRotatef(angleMars*8, 0, 1, 0);
        glColor3f(0.8, 0.3, 0.2);
        glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // Jupiter - Orange and tan bands
    glPushMatrix();
        glRotatef(angleJupiter, 0, 1, 0);
        glTranslatef(12, 0, 0);
        glRotatef(angleJupiter*3, 0, 1, 0);
        glColor3f(0.9, 0.6, 0.4);
        glutSolidSphere(1.0, 30, 30);
    glPopMatrix();

    // Saturne - Pale yellow with improved rings
    glPushMatrix();
        glRotatef(angleSaturne, 0, 1, 0);
        glTranslatef(16, 0, 0);
        glRotatef(angleSaturne*3, 0, 1, 0);
        glColor3f(0.9, 0.8, 0.5);
        glutSolidSphere(0.9, 30, 30);
        
        // Multiple ring layers for better appearance
        glRotatef(90, 1, 0, 0);
        glColor3f(0.8, 0.7, 0.4);
        glutWireTorus(0.05, 1.2, 10, 30);
        glColor3f(0.7, 0.6, 0.3);
        glutWireTorus(0.08, 1.4, 10, 30);
        glColor3f(0.6, 0.5, 0.2);
        glutWireTorus(0.06, 1.6, 10, 30);
    glPopMatrix();

    // Uranus - Cyan/light blue
    glPushMatrix();
        glRotatef(angleUranus, 0, 1, 0);
        glTranslatef(19, 0, 0);
        glRotatef(angleUranus*3, 0, 1, 0);
        glColor3f(0.4, 0.8, 0.9);
        glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    // Neptune - Deep blue
    glPushMatrix();
        glRotatef(angleNeptune, 0, 1, 0);
        glTranslatef(22, 0, 0);
        glRotatef(angleNeptune*3, 0, 1, 0);
        glColor3f(0.2, 0.3, 0.9);
        glutSolidSphere(0.7, 20, 20);
    glPopMatrix();
    
    // Pluto - Small gray dwarf planet
    glPushMatrix();
        glRotatef(anglePluto, 0, 1, 0);
        glTranslatef(26, 0, 0);
        glRotatef(anglePluto*2, 0, 1, 0);
        glColor3f(0.7, 0.65, 0.6);
        glutSolidSphere(0.15, 15, 15);
    glPopMatrix();
}
// === Background stars ===
struct Star {
    float x, y, z;       // Position in 3D space
    float brightness;    // Star brightness (0.5 to 1.0)
    float size;          // Star size (1 to 3 pixels)
};
std::vector<Star> stars;
int starCount = 10000;  // Total number of stars

// === Comet with elliptical orbit ===
struct Comet {
    float angle;         // Current orbital angle
    float eccentricity;  // Orbital eccentricity (0.8 = very elliptical)
    float a;             // Semi-major axis
    float b;             // Semi-minor axis
};
Comet comet = {0.0f, 0.8f, 30.0f, 18.0f};

// Initialize star field with random positions and properties
void initStars() {
    for(int i = 0; i < starCount; i++) {
        Star s;
        s.x = (rand() % 2000 - 1000) / 10.0f;    // -100 to 100
        s.y = (rand() % 1000 - 500) / 10.0f;     // -50 to 50
        s.z = (rand() % 2000 - 1000) / 10.0f;    // -100 to 100
        s.brightness = 0.5f + (rand() % 100) / 200.0f; // 0.5 to 1.0
        s.size = 1.0f + (rand() % 3);            // 1 to 3 pixels
        stars.push_back(s);
    }
}

// Initialize asteroid belt with random orbital parameters
void initAsteroids() {
    for(int i = 0; i < asteroidCount; i++) {
        Asteroid a;
        a.angle = rand() % 360;
        a.distance = 9.5f + (rand() % 100) / 40.0f; // Between Mars (9) and Jupiter (12)
        a.speed = 0.5f + (rand() % 50) / 100.0f;
        a.size = 0.02f + (rand() % 5) / 100.0f;
        asteroids.push_back(a);
    }
}

// Draw the background star field
void drawStars() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);  // Draw stars in background (no depth testing)

    // Group stars by size for better performance
    for(int size = 1; size <= 3; ++size) {
        glPointSize((GLfloat)size);
        glBegin(GL_POINTS);
        for(auto &s : stars) {
            if((int)(s.size + 0.5f) == size) {  // Round to nearest int
                glColor3f(s.brightness, s.brightness, s.brightness);
                glVertex3f(s.x, s.y, s.z);
            }
        }
        glEnd();
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Draw the asteroid belt
void drawAsteroids() {
    glDisable(GL_LIGHTING);
    for(auto &a : asteroids) {
        glPushMatrix();
            glRotatef(a.angle, 0, 1, 0);
            glTranslatef(a.distance, 0, 0);
            glColor3f(0.5f, 0.4f, 0.3f); // Brown-gray rocky color
            glutSolidSphere(a.size, 8, 8);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

// Draw comet with glowing tail
void drawComet() {
    // Calculate elliptical orbit position
    float angleRad = comet.angle * M_PI / 180.0f;
    float cosAngle = cos(angleRad);
    float sinAngle = sin(angleRad);
    // Optimize: use multiplication instead of pow() for better performance
    float bCos = comet.b * cosAngle;
    float aSin = comet.a * sinAngle;
    float r = (comet.a * comet.b) / sqrt(bCos * bCos + aSin * aSin);
    float x = r * cosAngle;
    float z = r * sinAngle;
    
    glDisable(GL_LIGHTING);
    
    // Draw tail pointing away from the Sun
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.7f, 0.9f, 1.0f, 0.8f);
    glVertex3f(x, 0, z);
    glColor4f(0.7f, 0.9f, 1.0f, 0.0f);  // Fade to transparent
    for(int i = 0; i <= 10; i++) {
        float tailAngle = (comet.angle + 180) * M_PI / 180.0f + (i - 5) * 0.2f;
        float tailDist = 2.0f;
        glVertex3f(x + tailDist * cos(tailAngle), 0, z + tailDist * sin(tailAngle));
    }
    glEnd();
    
    // Draw bright comet head
    glPushMatrix();
        glTranslatef(x, 0, z);
        glColor3f(0.9f, 0.95f, 1.0f);
        glutSolidSphere(0.15, 10, 10);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

// Draw text on screen
void drawText(float x, float y, const std::string& text) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for(char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Display help menu
void displayHelp() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Semi-transparent background
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(100, 100);
        glVertex2f(900, 100);
        glVertex2f(900, 500);
        glVertex2f(100, 500);
    glEnd();
    glDisable(GL_BLEND);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    int yPos = 460;
    std::vector<std::string> helpText = {
        "=== CONTROLS ===",
        "Movement: Z/S (forward/back), Q/D (left/right), A/E (down/up)",
        "Mouse: Click and drag to look around",
        "Space: Reset camera",
        "P: Pause/Resume simulation",
        "Up/Down Arrow: Increase/Decrease speed",
        "O: Toggle orbit lines",
        "I: Toggle info display",
        "H: Toggle this help menu",
        "+/-: Increase/Decrease light intensity",
        "1-9: Jump to planet view",
        "ESC: Exit",
        "",
        "Press H to close this menu"
    };
    
    for(const auto& line : helpText) {
        glRasterPos2f(120, yPos);
        for(char c : line) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
        yPos -= 20;
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Display simulation info
void displayInfo() {
    std::stringstream ss;
    ss << "Simulation Time: " << std::fixed << std::setprecision(1) << simulationTime << " Earth days";
    drawText(10, 580, ss.str());
    
    ss.str("");
    ss << "Speed: " << std::fixed << std::setprecision(1) << simulationSpeed << "x";
    drawText(10, 560, ss.str());
    
    ss.str("");
    ss << "Camera: (" << std::fixed << std::setprecision(1) << camX << ", " << camY << ", " << camZ << ")";
    drawText(10, 540, ss.str());
    
    if(isPaused) {
        drawText(10, 520, "PAUSED");
    }
}
// === Display function - renders the entire scene ===
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    drawStars();

    float dirX = cos(angleX * M_PI / 180.0f) * sin(angleY * M_PI / 180.0f);
    float dirY = sin(angleX * M_PI / 180.0f);
    float dirZ = -cos(angleX * M_PI / 180.0f) * cos(angleY * M_PI / 180.0f);

    gluLookAt(camX, camY, camZ, camX + dirX, camY + dirY, camZ + dirZ, 0.0, 1.0, 0.0);

    drawSolarSystem();
    
    // Display UI elements
    if(showInfo) {
        displayInfo();
    }
    
    if(showHelp) {
        displayHelp();
    }
    
    glutSwapBuffers();
}

// === Animation update function - updates all moving objects ===
void update(int value) {
    if(!isPaused) {
        // Update planet angles with simulation speed multiplier
        angleMercure += 4.0f * simulationSpeed;
        angleVenus   += 1.6f * simulationSpeed;
        angleTerre   += 1.0f * simulationSpeed;
        anglelune    += 13.0f * simulationSpeed; // Moon orbits Earth faster
        angleMars    += 0.8f * simulationSpeed;
        angleJupiter += 0.4f * simulationSpeed;
        angleSaturne += 0.3f * simulationSpeed;
        angleUranus  += 0.2f * simulationSpeed;
        angleNeptune += 0.15f * simulationSpeed;
        anglePluto   += 0.1f * simulationSpeed;

        // Update asteroid angles
        for(auto &a : asteroids) {
            a.angle += a.speed * simulationSpeed;
            if(a.angle > 360) a.angle -= 360;
        }
        
        // Update comet
        comet.angle += 0.5f * simulationSpeed;
        if(comet.angle > 360) comet.angle -= 360;

        // Wrap angles
        if(angleMercure>360) angleMercure-=360;
        if(angleVenus>360) angleVenus-=360;
        if(angleTerre>360) angleTerre-=360;
        if(anglelune>360) anglelune-=360;
        if(angleMars>360) angleMars-=360;
        if(angleJupiter>360) angleJupiter-=360;
        if(angleSaturne>360) angleSaturne-=360;
        if(angleUranus>360) angleUranus-=360;
        if(angleNeptune>360) angleNeptune-=360;
        if(anglePluto>360) anglePluto-=360;
        
        // Update simulation time (1 frame ≈ 0.027 Earth days at base speed)
        simulationTime += 0.027f * simulationSpeed;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// === Keyboard handler for camera and simulation controls ===
void keyboard(unsigned char key, int x, int y) {
    float radY = angleY * M_PI / 180.0f;
    float forwardX = sin(radY);
    float forwardZ = -cos(radY);
    float rightX = cos(radY);
    float rightZ = sin(radY);

    switch(key) {
        // Movement controls
        case 'z': camX += forwardX*moveSpeed; camZ += forwardZ*moveSpeed; break;
        case 's': camX -= forwardX*moveSpeed; camZ -= forwardZ*moveSpeed; break;
        case 'q': camX -= rightX*moveSpeed; camZ += rightZ*moveSpeed; break;
        case 'd': camX += rightX*moveSpeed; camZ -= rightZ*moveSpeed; break;
        case 'a': camY -= moveSpeed; break;
        case 'e': camY += moveSpeed; break;
        case ' ': camX=0; camY=5; camZ=25; angleX=0; angleY=0; break;
        
        // Simulation controls
        case 'p': 
        case 'P': isPaused = !isPaused; break;
        case 'o':
        case 'O': showOrbits = !showOrbits; break;
        case 'i':
        case 'I': showInfo = !showInfo; break;
        case 'h':
        case 'H': showHelp = !showHelp; break;
        
        // Light controls
        case '+': lightIntensity += 0.2f; initLighting(); break;
        case '-': lightIntensity -= 0.2f; if(lightIntensity<0) lightIntensity=0; initLighting(); break;
        
        // Camera presets (jump to planets)
        case '1': camX=0; camY=2; camZ=8; angleX=-10; angleY=0; break;  // Sun view
        case '2': camX=3; camY=1; camZ=5; angleX=-10; angleY=0; break;  // Mercury
        case '3': camX=5; camY=2; camZ=8; angleX=-10; angleY=0; break;  // Venus
        case '4': camX=7; camY=3; camZ=10; angleX=-10; angleY=0; break; // Earth
        case '5': camX=9; camY=3; camZ=12; angleX=-10; angleY=0; break; // Mars
        case '6': camX=12; camY=5; camZ=16; angleX=-10; angleY=0; break; // Jupiter
        case '7': camX=16; camY=5; camZ=20; angleX=-10; angleY=0; break; // Saturn
        case '8': camX=19; camY=5; camZ=24; angleX=-10; angleY=0; break; // Uranus
        case '9': camX=22; camY=5; camZ=28; angleX=-10; angleY=0; break; // Neptune
        case '0': camX=26; camY=5; camZ=32; angleX=-10; angleY=0; break; // Pluto
        
        case 27: exit(0); break; // ESC
    }
    glutPostRedisplay();
}

// Special key handler for arrow keys
void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            simulationSpeed += 0.25f;
            if(simulationSpeed > 10.0f) simulationSpeed = 10.0f;
            break;
        case GLUT_KEY_DOWN:
            simulationSpeed -= 0.25f;
            if(simulationSpeed < 0.1f) simulationSpeed = 0.1f;
            break;
    }
    glutPostRedisplay();
}

// === Mouse motion handler for camera look-around ===
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

// === OpenGL initialization ===
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    initLighting();
    initStars();
    initAsteroids();
}

// === Main ===
int main(int argc, char** argv) {
    srand(time(NULL)); // Initialize random seed for star and asteroid generation
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Enhanced Solar System 3D Simulation");

    init();
     
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}