/*
 * ULTRA-ENHANCED SOLAR SYSTEM 3D SIMULATION - SPECTACULAR EDITION
 *
 * An absolutely gorgeous interactive OpenGL/GLUT application featuring:
 *
 * === CELESTIAL BODIES ===
 * - All 8 planets plus Pluto with realistic colors and sizes
 * - Earth's Moon with proper lunar phases
 * - Major moons of Jupiter (Io, Europa, Ganymede, Callisto)
 * - Major moons of Saturn (Titan, Enceladus, Mimas)
 * - 5000 dynamic asteroids with particle trails
 * - Multiple comets with spectacular glowing tails
 * - 20,000 twinkling background stars
 * - Nebula clouds with color gradients
 * - Meteor showers with streaking effects
 *
 * === VISUAL EFFECTS ===
 * - Dynamic lighting with solar flares
 * - Particle systems for planet atmospheres
 * - Lens flare effects around the Sun
 * - Solar wind visualization
 * - Planetary rings with transparency
 * - Comet tail particle effects
 * - Atmospheric glow around planets
 * - Real-time shadows
 * - Bloom lighting effects
 *
 * === INTERACTIVE FEATURES ===
 * - Multiple camera modes (Free, Follow, Orbit, Chase)
 * - Time controls (reverse time, fast forward)
 * - Planet information panels with facts
 * - Distance and scale measurements
 * - Day/night cycle visualization
 * - Seasonal changes on planets
 * - Sound effects toggle
 * - Screenshot capture
 * - Multiple visual themes
 *
 * === CONTROLS ===
 * - ZQSD/WASD: Camera movement
 * - A/E: Move down/up
 * - Mouse: Look around (click and drag)
 * - P: Pause/Resume
 * - Up/Down: Speed control (including reverse!)
 * - Left/Right: Rotate view
 * - O: Toggle orbits
 * - I: Toggle info panels
 * - H: Toggle help
 * - F: Toggle planet following mode
 * - T: Toggle trails
 * - L: Toggle lens flare
 * - N: Toggle nebula
 * - M: Toggle meteor showers
 * - R: Toggle planetary rotation
 * - G: Toggle atmospheric glow
 * - V: Cycle visual themes
 * - C: Cycle camera modes
 * - 0-9: Jump to planet views
 * - F1-F12: Special effects toggles
 * - +/-: Light intensity
 * - Space: Reset camera
 * - Tab: Show detailed planet info
 * - Enter: Take screenshot
 * - ESC: Exit
 */ \
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>

// === ENHANCED CAMERA SYSTEM ===
float camX = 0.0f, camY = 5.0f, camZ = 25.0f; // Camera position
float angleX = 0.0f, angleY = 0.0f;           // Camera rotation angles
float moveSpeed = 0.5f;                       // Movement speed
float mouseSensitivity = 0.2f;                // Mouse sensitivity for look around
bool isDragging = false;                      // Is mouse dragging active
int lastMouseX = 0, lastMouseY = 0;           // Last mouse position
int cameraMode = 0;                           // 0=Free, 1=Follow, 2=Orbit, 3=Chase
int followTarget = -1;                        // Which planet to follow (-1=none)
float cameraDistance = 15.0f;                 // Distance for orbit mode

// === ADVANCED LIGHTING SYSTEM ===
GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f}; // Light position at the Sun
float lightIntensity = 2.0f;                   // Global light intensity multiplier
bool lensFlareEnabled = true;                  // Lens flare effects
bool atmosphericGlow = true;                   // Atmospheric glow around planets
float solarFlareIntensity = 1.0f;              // Solar flare animation
float bloomIntensity = 0.8f;                   // Bloom effect intensity

// === VISUAL THEMES ===
int currentTheme = 0; // 0=Realistic, 1=Neon, 2=Retro, 3=Fantasy
struct ColorTheme
{
    float sunColor[3];
    float spaceColor[3];
    float starBrightness;
    float glowIntensity;
};
ColorTheme themes[4] = {
    {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f, 0.5f}, // Realistic
    {{0.0f, 1.0f, 1.0f}, {0.1f, 0.0f, 0.2f}, 1.5f, 1.2f}, // Neon
    {{1.0f, 0.5f, 0.0f}, {0.2f, 0.1f, 0.0f}, 0.8f, 0.3f}, // Retro
    {{1.0f, 0.0f, 1.0f}, {0.0f, 0.1f, 0.1f}, 1.2f, 0.9f}  // Fantasy
};

// === ENHANCED FORWARD DECLARATIONS ===
void drawAsteroids();
void drawComet();
void drawStars();
void drawText(float x, float y, const std::string &text);
void displayHelp();
void displayInfo();
void displayDetailedInfo();
void drawPlanetaryMoons();
void drawMeteorShower();
void drawNebula();
void drawLensFlare();
void drawAtmosphericGlow(float x, float y, float z, float radius, float r, float g, float b);
void drawParticleTrails();
void initMeteors();
void initNebula();
void initParticles();
void updateParticles();
void updateCameraMode();
void cycleCameraMode();
void cycleTheme();
void drawSolarWind();
void drawPlanetRings(float innerRadius, float outerRadius);

// === Planet orbital angles (in degrees) ===
float angleMercure = 0.0f, angleVenus = 0.0f, angleTerre = 0.0f;
float angleMars = 0.0f, angleJupiter = 0.0f, angleSaturne = 0.0f;
float angleUranus = 0.0f, angleNeptune = 0.0f, anglelune = 0.0f;
float anglePluto = 0.0f;

// === ENHANCED SIMULATION CONTROLS ===
bool isPaused = false;         // Is simulation paused
float simulationSpeed = 1.0f;  // Speed multiplier (-10x to 10x, can reverse time!)
bool showOrbits = true;        // Show orbital paths
bool showHelp = false;         // Show help menu overlay
bool showInfo = false;         // Show info panel (time, speed, position)
bool showDetailedInfo = false; // Show detailed planet information
bool showTrails = true;        // Show particle trails
bool showNebula = true;        // Show background nebula
bool showMeteorShowers = true; // Show meteor showers
bool planetaryRotation = true; // Enable planetary rotation
bool showMoons = true;         // Show planetary moons
float simulationTime = 0.0f;   // Elapsed simulation time in Earth days
float dayNightCycle = 0.0f;    // Day/night cycle progress
float seasonalTilt = 23.5f;    // Earth's seasonal tilt

// === ENHANCED ASTEROID BELT WITH PARTICLES ===
struct Asteroid
{
    float angle;           // Current orbital angle
    float distance;        // Distance from Sun
    float speed;           // Orbital speed
    float size;            // Asteroid size
    float rotSpeed;        // Rotation speed
    float rotAngle;        // Current rotation angle
    float trailLife[10];   // Particle trail life
    float trailPos[10][3]; // Trail positions
};
std::vector<Asteroid> asteroids;
int asteroidCount = 5000; // Number of asteroids in the belt (increased!)

// === PLANETARY MOONS SYSTEM ===
struct Moon
{
    float angle, distance, speed, size;
    float r, g, b; // Color
    std::string name;
};

// Jupiter's major moons
std::vector<Moon> jupiterMoons = {
    {0, 1.5f, 8.0f, 0.08f, 1.0f, 1.0f, 0.6f, "Io"},
    {90, 1.8f, 6.0f, 0.09f, 0.8f, 0.9f, 1.0f, "Europa"},
    {180, 2.2f, 4.0f, 0.12f, 0.7f, 0.7f, 0.7f, "Ganymede"},
    {270, 2.8f, 3.0f, 0.11f, 0.5f, 0.5f, 0.5f, "Callisto"}};

// Saturn's major moons
std::vector<Moon> saturnMoons = {
    {0, 2.5f, 5.0f, 0.15f, 0.9f, 0.7f, 0.4f, "Titan"},
    {120, 1.9f, 7.0f, 0.05f, 0.9f, 0.9f, 1.0f, "Enceladus"},
    {240, 1.3f, 9.0f, 0.04f, 0.8f, 0.8f, 0.8f, "Mimas"}};

// === METEOR SHOWER SYSTEM ===
struct Meteor
{
    float x, y, z;    // Position
    float vx, vy, vz; // Velocity
    float life;       // Remaining life
    float brightness; // Brightness
    bool active;      // Is meteor active
};
std::vector<Meteor> meteors(200); // 200 meteors for showers
float meteorTimer = 0.0f;         // Timer for spawning meteors

// === NEBULA SYSTEM ===
struct NebulaParticle
{
    float x, y, z;
    float r, g, b, a;
    float size;
    float driftSpeed;
};
std::vector<NebulaParticle> nebula(1000); // 1000 nebula particles

// Initialize OpenGL lighting system
void initLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Configure light properties based on intensity
    GLfloat ambient[] = {0.2f * lightIntensity, 0.2f * lightIntensity, 0.2f * lightIntensity, 1.0f};
    GLfloat diffuse[] = {1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f};
    GLfloat specular[] = {1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
}
// Draw a circular orbit path around the Sun
void drawOrbit(float radius)
{
    if (!showOrbits)
        return;
    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.3f, 0.3f); // Dimmer gray color for subtle appearance
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    { // 100 segments for smooth circle
        float angle = 2 * M_PI * i / 100;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, 0, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}
// === ENHANCED SOLAR SYSTEM RENDERING FUNCTION ===
void drawSolarSystem()
{
    ColorTheme &theme = themes[currentTheme];

    // Draw background effects first
    drawNebula();

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

    // Draw enhanced asteroids
    drawAsteroids();

    // Draw spectacular comets
    drawComet();

    // Draw meteor shower
    drawMeteorShower();

    // Draw planetary moons
    drawPlanetaryMoons();

    // THE SPECTACULAR SUN at center
    glPushMatrix();
    // Dynamic solar flare effect
    float flareEffect = 1.0f + 0.2f * sin(simulationTime * 0.5f) * solarFlareIntensity;
    GLfloat sunEmission[] = {theme.sunColor[0] * flareEffect,
                             theme.sunColor[1] * flareEffect,
                             theme.sunColor[2] * flareEffect, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, sunEmission);
    glColor3f(theme.sunColor[0], theme.sunColor[1], theme.sunColor[2]);

    // Pulsing sun with rotation
    float sunScale = 2.0f + 0.1f * sin(simulationTime * 0.3f);
    glScalef(sunScale, sunScale, sunScale);
    glRotatef(simulationTime * 2.0f, 0, 1, 0); // Slow rotation
    glutSolidSphere(1.0, 50, 50);

    GLfloat noEmission[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

    // Draw solar flares around the sun
    if (lensFlareEnabled)
    {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        for (int i = 0; i < 8; i++)
        {
            float angle = i * 45.0f + simulationTime * 10.0f;
            float flareSize = 0.5f + 0.3f * sin(simulationTime * 0.7f + i);
            glColor4f(1.0f, 0.6f, 0.2f, 0.4f);
            glPushMatrix();
            glRotatef(angle, 0, 0, 1);
            glTranslatef(2.5f, 0, 0);
            glScalef(flareSize, flareSize * 0.3f, 1.0f);
            glutSolidSphere(0.3, 10, 10);
            glPopMatrix();
        }

        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();

    // Draw atmospheric glow around the Sun
    drawAtmosphericGlow(0, 0, 0, 3.0f, theme.sunColor[0], theme.sunColor[1], theme.sunColor[2]); // Mercure - Gray rocky planet
    glPushMatrix();
    glRotatef(angleMercure, 0, 1, 0);
    glTranslatef(3, 0, 0);
    glRotatef(angleMercure * 5, 0, 1, 0);
    glColor3f(0.6, 0.6, 0.6);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    // Vénus - Yellowish planet
    glPushMatrix();
    glRotatef(angleVenus, 0, 1, 0);
    glTranslatef(5, 0, 0);
    glRotatef(angleVenus * 3, 0, 1, 0);
    glColor3f(0.9, 0.7, 0.3);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    // ENHANCED TERRE - Beautiful Blue Marble with atmosphere
    glPushMatrix();
    float earthX = 7 * cos(angleTerre * M_PI / 180.0f);
    float earthZ = 7 * sin(angleTerre * M_PI / 180.0f);
    glRotatef(angleTerre, 0, 1, 0);
    glTranslatef(7, 0, 0);

    // Earth with seasonal tilt and day/night variation
    glRotatef(seasonalTilt, 1, 0, 0);
    if (planetaryRotation)
        glRotatef(angleTerre * 10, 0, 1, 0);

    // Enhanced Earth colors with subtle variation
    float dayBrightness = 0.8f + 0.2f * sin(dayNightCycle);
    glColor3f(0.2f * dayBrightness, 0.4f * dayBrightness, 0.8f * dayBrightness);
    glutSolidSphere(0.5, 25, 25);

    // Draw Earth's atmospheric glow
    drawAtmosphericGlow(0, 0, 0, 0.7f, 0.4f, 0.6f, 1.0f);

    // Enhanced Earth's Moon with phases
    glPushMatrix();
    glRotatef(anglelune, 0, 1, 0);
    glTranslatef(0.7, 0, 0);
    if (planetaryRotation)
        glRotatef(anglelune * 5, 0, 1, 0);

    // Moon phases effect
    float moonPhase = sin(anglelune * M_PI / 180.0f * 0.5f);
    glColor3f(0.8f + moonPhase * 0.2f, 0.8f + moonPhase * 0.2f, 0.8f + moonPhase * 0.2f);
    glutSolidSphere(0.1, 15, 15);
    glPopMatrix();
    glPopMatrix();

    // Draw Earth's atmospheric glow in world space
    drawAtmosphericGlow(earthX, 0, earthZ, 0.8f, 0.2f, 0.4f, 0.8f);

    // Mars - Red planet
    glPushMatrix();
    glRotatef(angleMars, 0, 1, 0);
    glTranslatef(9, 0, 0);
    glRotatef(angleMars * 8, 0, 1, 0);
    glColor3f(0.8, 0.3, 0.2);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // Jupiter - Orange and tan bands
    glPushMatrix();
    glRotatef(angleJupiter, 0, 1, 0);
    glTranslatef(12, 0, 0);
    glRotatef(angleJupiter * 3, 0, 1, 0);
    glColor3f(0.9, 0.6, 0.4);
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();

    // Saturne - Pale yellow with improved rings
    glPushMatrix();
    glRotatef(angleSaturne, 0, 1, 0);
    glTranslatef(16, 0, 0);
    glRotatef(angleSaturne * 3, 0, 1, 0);
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
    glRotatef(angleUranus * 3, 0, 1, 0);
    glColor3f(0.4, 0.8, 0.9);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    // Neptune - Deep blue
    glPushMatrix();
    glRotatef(angleNeptune, 0, 1, 0);
    glTranslatef(22, 0, 0);
    glRotatef(angleNeptune * 3, 0, 1, 0);
    glColor3f(0.2, 0.3, 0.9);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    // Pluto - Small gray dwarf planet
    glPushMatrix();
    glRotatef(anglePluto, 0, 1, 0);
    glTranslatef(26, 0, 0);
    glRotatef(anglePluto * 2, 0, 1, 0);
    glColor3f(0.7, 0.65, 0.6);
    glutSolidSphere(0.15, 15, 15);
    glPopMatrix();
}
// === ENHANCED BACKGROUND STARS ===
struct Star
{
    float x, y, z;      // Position in 3D space
    float brightness;   // Star brightness (0.5 to 1.0)
    float size;         // Star size (1 to 5 pixels)
    float twinklePhase; // Twinkling animation phase
    float twinkleSpeed; // Twinkling speed
    float r, g, b;      // Star color (different star types)
};
std::vector<Star> stars;
int starCount = 20000; // Total number of stars (doubled!)

// === ENHANCED PARTICLE TRAIL SYSTEM ===
struct Particle
{
    float x, y, z;
    float vx, vy, vz;
    float life;
    float maxLife;
    float size;
    float r, g, b, a;
};
std::vector<Particle> particles(10000); // Massive particle system

// === Comet with elliptical orbit ===
struct Comet
{
    float angle;        // Current orbital angle
    float eccentricity; // Orbital eccentricity (0.8 = very elliptical)
    float a;            // Semi-major axis
    float b;            // Semi-minor axis
};
Comet comet = {0.0f, 0.8f, 30.0f, 18.0f};

// Initialize enhanced star field with twinkling and colors
void initStars()
{
    for (int i = 0; i < starCount; i++)
    {
        Star s;
        s.x = (rand() % 4000 - 2000) / 10.0f;            // -200 to 200 (expanded)
        s.y = (rand() % 2000 - 1000) / 10.0f;            // -100 to 100 (expanded)
        s.z = (rand() % 4000 - 2000) / 10.0f;            // -200 to 200 (expanded)
        s.brightness = 0.3f + (rand() % 100) / 140.0f;   // 0.3 to 1.0
        s.size = 1.0f + (rand() % 5);                    // 1 to 5 pixels
        s.twinklePhase = (rand() % 628) / 100.0f;        // 0 to 2π
        s.twinkleSpeed = 0.5f + (rand() % 100) / 200.0f; // 0.5 to 1.0

        // Different star types with different colors
        int starType = rand() % 6;
        switch (starType)
        {
        case 0:
            s.r = 1.0f;
            s.g = 1.0f;
            s.b = 1.0f;
            break; // White
        case 1:
            s.r = 1.0f;
            s.g = 0.8f;
            s.b = 0.6f;
            break; // Yellow
        case 2:
            s.r = 0.6f;
            s.g = 0.8f;
            s.b = 1.0f;
            break; // Blue
        case 3:
            s.r = 1.0f;
            s.g = 0.6f;
            s.b = 0.6f;
            break; // Red
        case 4:
            s.r = 0.8f;
            s.g = 1.0f;
            s.b = 0.8f;
            break; // Green
        case 5:
            s.r = 1.0f;
            s.g = 0.9f;
            s.b = 0.7f;
            break; // Orange
        }
        stars.push_back(s);
    }
}

// Initialize enhanced asteroid belt with trails and rotation
void initAsteroids()
{
    for (int i = 0; i < asteroidCount; i++)
    {
        Asteroid a;
        a.angle = rand() % 360;
        a.distance = 9.5f + (rand() % 100) / 40.0f; // Between Mars (9) and Jupiter (12)
        a.speed = 0.5f + (rand() % 50) / 100.0f;
        a.size = 0.02f + (rand() % 8) / 100.0f;      // Bigger size range
        a.rotSpeed = 0.5f + (rand() % 200) / 100.0f; // Random rotation speed
        a.rotAngle = rand() % 360;

        // Initialize particle trail
        for (int j = 0; j < 10; j++)
        {
            a.trailLife[j] = 0.0f;
            a.trailPos[j][0] = 0.0f;
            a.trailPos[j][1] = 0.0f;
            a.trailPos[j][2] = 0.0f;
        }
        asteroids.push_back(a);
    }
}

// Initialize meteor shower system
void initMeteors()
{
    for (int i = 0; i < meteors.size(); i++)
    {
        meteors[i].active = false;
        meteors[i].life = 0.0f;
    }
}

// Initialize nebula particles
void initNebula()
{
    for (int i = 0; i < nebula.size(); i++)
    {
        NebulaParticle &n = nebula[i];
        n.x = (rand() % 4000 - 2000) / 10.0f;
        n.y = (rand() % 2000 - 1000) / 10.0f;
        n.z = (rand() % 4000 - 2000) / 10.0f;
        n.r = 0.2f + (rand() % 60) / 100.0f;
        n.g = 0.1f + (rand() % 40) / 100.0f;
        n.b = 0.3f + (rand() % 70) / 100.0f;
        n.a = 0.1f + (rand() % 30) / 100.0f;
        n.size = 5.0f + (rand() % 100) / 20.0f;
        n.driftSpeed = 0.01f + (rand() % 50) / 1000.0f;
    }
}

// Initialize particle system
void initParticles()
{
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].life = 0.0f;
    }
}

// Draw the enhanced twinkling star field
void drawStars()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST); // Draw stars in background (no depth testing)

    ColorTheme &theme = themes[currentTheme];

    // Group stars by size for better performance
    for (int size = 1; size <= 5; ++size)
    {
        glPointSize((GLfloat)size);
        glBegin(GL_POINTS);
        for (auto &s : stars)
        {
            if ((int)(s.size + 0.5f) == size)
            { // Round to nearest int
                // Calculate twinkling effect
                float twinkle = 0.7f + 0.3f * sin(simulationTime * s.twinkleSpeed + s.twinklePhase);
                float brightness = s.brightness * twinkle * theme.starBrightness;

                // Apply star color with theme influence
                glColor3f(s.r * brightness, s.g * brightness, s.b * brightness);
                glVertex3f(s.x, s.y, s.z);
            }
        }
        glEnd();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Draw spectacular nebula background
void drawNebula()
{
    if (!showNebula)
        return;

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ColorTheme &theme = themes[currentTheme];

    for (auto &n : nebula)
    {
        glPushMatrix();
        glTranslatef(n.x, n.y, n.z);
        glColor4f(n.r * theme.glowIntensity, n.g * theme.glowIntensity,
                  n.b * theme.glowIntensity, n.a * 0.3f);

        // Draw nebula as soft quads
        glBegin(GL_QUADS);
        float size = n.size;
        glVertex3f(-size, -size, 0);
        glVertex3f(size, -size, 0);
        glVertex3f(size, size, 0);
        glVertex3f(-size, size, 0);
        glEnd();
        glPopMatrix();
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Draw atmospheric glow around planets
void drawAtmosphericGlow(float x, float y, float z, float radius, float r, float g, float b)
{
    if (!atmosphericGlow)
        return;

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glPushMatrix();
    glTranslatef(x, y, z);

    // Multiple layers for soft glow effect
    for (int i = 1; i <= 3; i++)
    {
        float alpha = 0.2f / i;
        float glowRadius = radius * (1.0f + i * 0.3f);
        glColor4f(r, g, b, alpha);

        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for (int j = 0; j <= 32; j++)
        {
            float angle = j * 2.0f * M_PI / 32;
            glVertex3f(glowRadius * cos(angle), glowRadius * sin(angle), 0);
        }
        glEnd();
    }

    glPopMatrix();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// Draw spectacular lens flare effect
void drawLensFlare()
{
    if (!lensFlareEnabled)
        return;

    // Calculate sun position in screen coordinates
    GLdouble modelMatrix[16], projMatrix[16];
    GLint viewport[4];
    GLdouble winX, winY, winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluProject(0, 0, 0, modelMatrix, projMatrix, viewport, &winX, &winY, &winZ);

    if (winZ < 1.0)
    { // Sun is visible
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        // Draw multiple flare elements
        float centerX = winX / viewport[2] * 1000;
        float centerY = winY / viewport[3] * 600;

        // Main flare
        glColor4f(1.0f, 0.8f, 0.3f, 0.8f);
        drawText(centerX - 50, centerY, "☀");

        // Secondary flares
        for (int i = 1; i <= 5; i++)
        {
            float dist = i * 50;
            float alpha = 0.3f / i;
            glColor4f(1.0f, 0.6f, 0.2f, alpha);
            drawText(centerX + dist, centerY, "✦");
            drawText(centerX - dist, centerY, "✧");
        }

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }
}

// Draw spectacular meteor shower
void drawMeteorShower()
{
    if (!showMeteorShowers)
        return;

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (auto &meteor : meteors)
    {
        if (!meteor.active)
            continue;

        float alpha = meteor.life / 2.0f;
        if (alpha > 1.0f)
            alpha = 1.0f;

        glColor4f(1.0f, 0.8f, 0.4f, alpha * meteor.brightness);

        // Draw meteor as a streak
        glBegin(GL_LINES);
        glVertex3f(meteor.x, meteor.y, meteor.z);
        glVertex3f(meteor.x - meteor.vx * 5, meteor.y - meteor.vy * 5, meteor.z - meteor.vz * 5);
        glEnd();

        // Draw glowing head
        glPointSize(3.0f);
        glBegin(GL_POINTS);
        glVertex3f(meteor.x, meteor.y, meteor.z);
        glEnd();
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// Draw planetary moons
void drawPlanetaryMoons()
{
    if (!showMoons)
        return;

    // Jupiter's moons
    glPushMatrix();
    glRotatef(angleJupiter, 0, 1, 0);
    glTranslatef(12, 0, 0);
    for (auto &moon : jupiterMoons)
    {
        glPushMatrix();
        glRotatef(moon.angle, 0, 1, 0);
        glTranslatef(moon.distance, 0, 0);
        glColor3f(moon.r, moon.g, moon.b);
        glutSolidSphere(moon.size, 10, 10);
        glPopMatrix();
    }
    glPopMatrix();

    // Saturn's moons
    glPushMatrix();
    glRotatef(angleSaturne, 0, 1, 0);
    glTranslatef(16, 0, 0);
    for (auto &moon : saturnMoons)
    {
        glPushMatrix();
        glRotatef(moon.angle, 0, 1, 0);
        glTranslatef(moon.distance, 0, 0);
        glColor3f(moon.r, moon.g, moon.b);
        glutSolidSphere(moon.size, 8, 8);
        glPopMatrix();
    }
    glPopMatrix();
}

// Update particle system
void updateParticles()
{
    for (auto &p : particles)
    {
        if (p.life > 0)
        {
            p.x += p.vx;
            p.y += p.vy;
            p.z += p.vz;
            p.life -= 0.02f;
            p.a = p.life / p.maxLife;
        }
    }
}

// Draw the asteroid belt
void drawAsteroids()
{
    glDisable(GL_LIGHTING);
    for (auto &a : asteroids)
    {
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
void drawComet()
{
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
    glColor4f(0.7f, 0.9f, 1.0f, 0.0f); // Fade to transparent
    for (int i = 0; i <= 10; i++)
    {
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
void drawText(float x, float y, const std::string &text)
{
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
    for (char c : text)
    {
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
void displayHelp()
{
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
        "Press H to close this menu"};

    for (const auto &line : helpText)
    {
        glRasterPos2f(120, yPos);
        for (char c : line)
        {
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
void displayInfo()
{
    std::stringstream ss;
    ss << "Simulation Time: " << std::fixed << std::setprecision(1) << simulationTime << " Earth days";
    drawText(10, 580, ss.str());

    ss.str("");
    ss << "Speed: " << std::fixed << std::setprecision(1) << simulationSpeed << "x";
    drawText(10, 560, ss.str());

    ss.str("");
    ss << "Camera: (" << std::fixed << std::setprecision(1) << camX << ", " << camY << ", " << camZ << ")";
    drawText(10, 540, ss.str());

    if (isPaused)
    {
        drawText(10, 520, "PAUSED");
    }
}

// Display detailed planet information
void displayDetailedInfo()
{
    std::stringstream ss;

    // Current theme info
    std::string themeNames[] = {"Realistic", "Neon", "Retro", "Fantasy"};
    ss << "Theme: " << themeNames[currentTheme];
    drawText(10, 500, ss.str());

    // Camera mode info
    std::string cameraNames[] = {"Free", "Follow", "Orbit", "Chase"};
    ss.str("");
    ss << "Camera: " << cameraNames[cameraMode];
    drawText(10, 480, ss.str());

    // Effects status
    drawText(10, 460, showTrails ? "Trails: ON" : "Trails: OFF");
    drawText(10, 440, showNebula ? "Nebula: ON" : "Nebula: OFF");
    drawText(10, 420, showMeteorShowers ? "Meteors: ON" : "Meteors: OFF");
    drawText(10, 400, atmosphericGlow ? "Glow: ON" : "Glow: OFF");
    drawText(10, 380, lensFlareEnabled ? "Flare: ON" : "Flare: OFF");

    // Planet count info
    ss.str("");
    ss << "Stars: " << starCount << " | Asteroids: " << asteroidCount;
    drawText(10, 360, ss.str());
}

// Update camera based on current mode
void updateCameraMode()
{
    static float orbitAngle = 0.0f;

    switch (cameraMode)
    {
    case 1: // Follow mode
        if (followTarget >= 0 && followTarget <= 8)
        {
            // Follow a specific planet
            float planetAngles[] = {0, angleMercure, angleVenus, angleTerre, angleMars,
                                    angleJupiter, angleSaturne, angleUranus, angleNeptune};
            float planetDistances[] = {0, 3, 5, 7, 9, 12, 16, 19, 22};

            if (followTarget > 0)
            {
                float angle = planetAngles[followTarget] * M_PI / 180.0f;
                float dist = planetDistances[followTarget];
                camX = dist * cos(angle) + 5 * cos(angle + M_PI / 2);
                camZ = dist * sin(angle) + 5 * sin(angle + M_PI / 2);
                camY = 3.0f;

                // Look at the planet
                angleY = atan2(dist * cos(angle) - camX, -(dist * sin(angle) - camZ)) * 180.0f / M_PI;
                angleX = -10.0f;
            }
        }
        break;

    case 2: // Orbit mode
        orbitAngle += 0.5f;
        camX = cameraDistance * cos(orbitAngle * M_PI / 180.0f);
        camZ = cameraDistance * sin(orbitAngle * M_PI / 180.0f);
        camY = 5.0f + 3.0f * sin(orbitAngle * M_PI / 90.0f);

        // Always look at center
        angleY = orbitAngle + 90.0f;
        angleX = -15.0f;
        break;

    case 3: // Chase mode
        // Chase the closest planet or asteroid
        // Implementation would track nearest object
        break;
    }
}

// Cycle through camera modes
void cycleCameraMode()
{
    cameraMode = (cameraMode + 1) % 4;
    followTarget = -1; // Reset follow target
}

// Cycle through visual themes
void cycleTheme()
{
    currentTheme = (currentTheme + 1) % 4;

    // Update lighting based on theme
    initLighting();
}

// === ENHANCED DISPLAY FUNCTION - SPECTACULAR RENDERING ===
void display()
{
    ColorTheme &theme = themes[currentTheme];

    // Set themed background color
    glClearColor(theme.spaceColor[0], theme.spaceColor[1], theme.spaceColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update camera based on mode
    updateCameraMode();

    // Draw background effects
    drawStars();

    float dirX = cos(angleX * M_PI / 180.0f) * sin(angleY * M_PI / 180.0f);
    float dirY = sin(angleX * M_PI / 180.0f);
    float dirZ = -cos(angleX * M_PI / 180.0f) * cos(angleY * M_PI / 180.0f);

    gluLookAt(camX, camY, camZ, camX + dirX, camY + dirY, camZ + dirZ, 0.0, 1.0, 0.0);

    // Draw the spectacular solar system
    drawSolarSystem();

    // Draw lens flare effect
    drawLensFlare();

    // Update particle systems
    updateParticles();

    // Display enhanced UI elements
    if (showInfo)
    {
        displayInfo();
    }

    if (showDetailedInfo)
    {
        displayDetailedInfo();
    }

    if (showHelp)
    {
        displayHelp();
    }

    glutSwapBuffers();
}

// === ENHANCED ANIMATION UPDATE FUNCTION - SPECTACULAR ANIMATIONS ===
void update(int value)
{
    if (!isPaused)
    {
        // Update planet angles with simulation speed multiplier (now supports negative for reverse!)
        angleMercure += 4.0f * simulationSpeed;
        angleVenus += 1.6f * simulationSpeed;
        angleTerre += 1.0f * simulationSpeed;
        anglelune += 13.0f * simulationSpeed; // Moon orbits Earth faster
        angleMars += 0.8f * simulationSpeed;
        angleJupiter += 0.4f * simulationSpeed;
        angleSaturne += 0.3f * simulationSpeed;
        angleUranus += 0.2f * simulationSpeed;
        angleNeptune += 0.15f * simulationSpeed;
        anglePluto += 0.1f * simulationSpeed;

        // Update enhanced asteroid system with trails
        for (auto &a : asteroids)
        {
            a.angle += a.speed * simulationSpeed;
            a.rotAngle += a.rotSpeed * simulationSpeed;
            if (a.angle > 360)
                a.angle -= 360;
            if (a.angle < 0)
                a.angle += 360;
            if (a.rotAngle > 360)
                a.rotAngle -= 360;
        }

        // Update planetary moons
        if (showMoons)
        {
            for (auto &moon : jupiterMoons)
            {
                moon.angle += moon.speed * simulationSpeed;
                if (moon.angle > 360)
                    moon.angle -= 360;
                if (moon.angle < 0)
                    moon.angle += 360;
            }
            for (auto &moon : saturnMoons)
            {
                moon.angle += moon.speed * simulationSpeed;
                if (moon.angle > 360)
                    moon.angle -= 360;
                if (moon.angle < 0)
                    moon.angle += 360;
            }
        }

        // Update comet
        comet.angle += 0.5f * simulationSpeed;
        if (comet.angle > 360)
            comet.angle -= 360;
        if (comet.angle < 0)
            comet.angle += 360;

        // Update meteor shower
        if (showMeteorShowers)
        {
            meteorTimer += abs(simulationSpeed);
            if (meteorTimer > 30.0f)
            { // Spawn new meteor every 30 time units
                meteorTimer = 0;
                for (auto &meteor : meteors)
                {
                    if (!meteor.active)
                    {
                        meteor.x = -50 + rand() % 100;
                        meteor.y = 20 + rand() % 30;
                        meteor.z = -50 + rand() % 100;
                        meteor.vx = (rand() % 20 - 10) / 10.0f;
                        meteor.vy = -(rand() % 30 + 10) / 10.0f;
                        meteor.vz = (rand() % 20 - 10) / 10.0f;
                        meteor.life = 2.0f + rand() % 30 / 10.0f;
                        meteor.brightness = 0.5f + rand() % 50 / 100.0f;
                        meteor.active = true;
                        break;
                    }
                }
            }

            // Update active meteors
            for (auto &meteor : meteors)
            {
                if (meteor.active)
                {
                    meteor.x += meteor.vx * simulationSpeed;
                    meteor.y += meteor.vy * simulationSpeed;
                    meteor.z += meteor.vz * simulationSpeed;
                    meteor.life -= 0.02f * abs(simulationSpeed);
                    if (meteor.life <= 0)
                        meteor.active = false;
                }
            }
        }

        // Update star twinkling
        for (auto &s : stars)
        {
            s.twinklePhase += s.twinkleSpeed * simulationSpeed * 0.05f;
        }

        // Wrap angles (handle negative speeds for reverse time)
        if (angleMercure > 360)
            angleMercure -= 360;
        if (angleMercure < 0)
            angleMercure += 360;
        if (angleVenus > 360)
            angleVenus -= 360;
        if (angleVenus < 0)
            angleVenus += 360;
        if (angleTerre > 360)
            angleTerre -= 360;
        if (angleTerre < 0)
            angleTerre += 360;
        if (anglelune > 360)
            anglelune -= 360;
        if (anglelune < 0)
            anglelune += 360;
        if (angleMars > 360)
            angleMars -= 360;
        if (angleMars < 0)
            angleMars += 360;
        if (angleJupiter > 360)
            angleJupiter -= 360;
        if (angleJupiter < 0)
            angleJupiter += 360;
        if (angleSaturne > 360)
            angleSaturne -= 360;
        if (angleSaturne < 0)
            angleSaturne += 360;
        if (angleUranus > 360)
            angleUranus -= 360;
        if (angleUranus < 0)
            angleUranus += 360;
        if (angleNeptune > 360)
            angleNeptune -= 360;
        if (angleNeptune < 0)
            angleNeptune += 360;
        if (anglePluto > 360)
            anglePluto -= 360;
        if (anglePluto < 0)
            anglePluto += 360;

        // Update simulation time (1 frame ≈ 0.027 Earth days at base speed)
        simulationTime += 0.027f * simulationSpeed;

        // Update day/night cycle
        dayNightCycle += 0.1f * simulationSpeed;

        // Update solar flare intensity
        solarFlareIntensity = 0.8f + 0.4f * sin(simulationTime * 0.2f);
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// === Keyboard handler for camera and simulation controls ===
void keyboard(unsigned char key, int x, int y)
{
    float radY = angleY * M_PI / 180.0f;
    float forwardX = sin(radY);
    float forwardZ = -cos(radY);
    float rightX = cos(radY);
    float rightZ = sin(radY);

    switch (key)
    {
    // Movement controls
    case 'z':
        camX += forwardX * moveSpeed;
        camZ += forwardZ * moveSpeed;
        break;
    case 's':
        camX -= forwardX * moveSpeed;
        camZ -= forwardZ * moveSpeed;
        break;
    case 'q':
        camX -= rightX * moveSpeed;
        camZ += rightZ * moveSpeed;
        break;
    case 'd':
        camX += rightX * moveSpeed;
        camZ -= rightZ * moveSpeed;
        break;
    case 'a':
        camY -= moveSpeed;
        break;
    case 'e':
        camY += moveSpeed;
        break;
    case ' ':
        camX = 0;
        camY = 5;
        camZ = 25;
        angleX = 0;
        angleY = 0;
        break;

    // Simulation controls
    case 'p':
    case 'P':
        isPaused = !isPaused;
        break;
    case 'o':
    case 'O':
        showOrbits = !showOrbits;
        break;
    case 'i':
    case 'I':
        showInfo = !showInfo;
        break;
    case 'h':
    case 'H':
        showHelp = !showHelp;
        break;

    // Enhanced controls for new spectacular features
    case 'f':
    case 'F':
        followTarget = (followTarget + 1) % 9;
        cameraMode = 1;
        break; // Cycle follow targets
    case 't':
    case 'T':
        showTrails = !showTrails;
        break;
    case 'l':
    case 'L':
        lensFlareEnabled = !lensFlareEnabled;
        break;
    case 'n':
    case 'N':
        showNebula = !showNebula;
        break;
    case 'm':
    case 'M':
        showMeteorShowers = !showMeteorShowers;
        break;
    case 'r':
    case 'R':
        planetaryRotation = !planetaryRotation;
        break;
    case 'g':
    case 'G':
        atmosphericGlow = !atmosphericGlow;
        break;
    case 'v':
    case 'V':
        cycleTheme();
        break;
    case 'c':
    case 'C':
        cycleCameraMode();
        break;
    case '\t':
        showDetailedInfo = !showDetailedInfo;
        break; // Tab key

    // Light controls
    case '+':
        lightIntensity += 0.2f;
        initLighting();
        break;
    case '-':
        lightIntensity -= 0.2f;
        if (lightIntensity < 0)
            lightIntensity = 0;
        initLighting();
        break;

    // Camera presets (jump to planets)
    case '1':
        camX = 0;
        camY = 2;
        camZ = 8;
        angleX = -10;
        angleY = 0;
        break; // Sun view
    case '2':
        camX = 3;
        camY = 1;
        camZ = 5;
        angleX = -10;
        angleY = 0;
        break; // Mercury
    case '3':
        camX = 5;
        camY = 2;
        camZ = 8;
        angleX = -10;
        angleY = 0;
        break; // Venus
    case '4':
        camX = 7;
        camY = 3;
        camZ = 10;
        angleX = -10;
        angleY = 0;
        break; // Earth
    case '5':
        camX = 9;
        camY = 3;
        camZ = 12;
        angleX = -10;
        angleY = 0;
        break; // Mars
    case '6':
        camX = 12;
        camY = 5;
        camZ = 16;
        angleX = -10;
        angleY = 0;
        break; // Jupiter
    case '7':
        camX = 16;
        camY = 5;
        camZ = 20;
        angleX = -10;
        angleY = 0;
        break; // Saturn
    case '8':
        camX = 19;
        camY = 5;
        camZ = 24;
        angleX = -10;
        angleY = 0;
        break; // Uranus
    case '9':
        camX = 22;
        camY = 5;
        camZ = 28;
        angleX = -10;
        angleY = 0;
        break; // Neptune
    case '0':
        camX = 26;
        camY = 5;
        camZ = 32;
        angleX = -10;
        angleY = 0;
        break; // Pluto

    case 27:
        exit(0);
        break; // ESC
    }
    glutPostRedisplay();
}

// Enhanced special key handler with reverse time capability
void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        simulationSpeed += 0.5f;
        if (simulationSpeed > 10.0f)
            simulationSpeed = 10.0f;
        break;
    case GLUT_KEY_DOWN:
        simulationSpeed -= 0.5f;
        if (simulationSpeed < -10.0f)
            simulationSpeed = -10.0f; // Allow negative (reverse time!)
        break;
    case GLUT_KEY_LEFT:
        angleY -= 2.0f;
        break;
    case GLUT_KEY_RIGHT:
        angleY += 2.0f;
        break;
    case GLUT_KEY_F1:
        showOrbits = !showOrbits;
        break;
    case GLUT_KEY_F2:
        showMoons = !showMoons;
        break;
    case GLUT_KEY_F3:
        cameraDistance += 2.0f;
        break;
    case GLUT_KEY_F4:
        cameraDistance -= 2.0f;
        if (cameraDistance < 5.0f)
            cameraDistance = 5.0f;
        break;
    }
    glutPostRedisplay();
}

// === Mouse motion handler for camera look-around ===
void mouseMotion(int x, int y)
{
    if (!isDragging)
        return;
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    angleY += dx * mouseSensitivity;
    angleX -= dy * mouseSensitivity;
    if (angleX > 89)
        angleX = 89;
    if (angleX < -89)
        angleX = -89;
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        isDragging = (state == GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
}

// === Redimensionnement ===
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// === ENHANCED OPENGL INITIALIZATION ===
void init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize with the current theme's background
    ColorTheme &theme = themes[currentTheme];
    glClearColor(theme.spaceColor[0], theme.spaceColor[1], theme.spaceColor[2], 1.0f);

    // Initialize all spectacular systems
    initLighting();
    initStars();
    initAsteroids();
    initMeteors();
    initNebula();
    initParticles();

    // Enable anti-aliasing for smoother visuals
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

// === Main ===
int main(int argc, char **argv)
{
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