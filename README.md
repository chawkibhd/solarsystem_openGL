# Enhanced Solar System 3D Simulation

A realistic and interactive 3D simulation of our solar system built with OpenGL and GLUT. This simulation features all 8 planets, Pluto (dwarf planet), Earth's moon, an asteroid belt, a comet with an elliptical orbit, and thousands of stars.

![Solar System Simulation](https://img.shields.io/badge/OpenGL-Simulation-blue)

## Features

### Visual Elements
- ✨ **All celestial bodies**: Sun, Mercury, Venus, Earth (with Moon), Mars, Jupiter, Saturn (with detailed rings), Uranus, Neptune, and Pluto
- 🌌 **Dynamic star field**: 10,000 stars with varying brightness and sizes for a realistic space backdrop
- 🪨 **Asteroid belt**: 1,000 asteroids orbiting between Mars and Jupiter
- ☄️ **Comet**: A comet with an eccentric elliptical orbit and a glowing tail
- 🎨 **Realistic colors**: Improved planet appearances with accurate color schemes
- 💍 **Enhanced Saturn rings**: Multiple ring layers for a more detailed appearance
- 🔆 **Dynamic lighting**: The Sun emits light that illuminates all planets

### Interactive Controls

#### Camera Movement
- **Z/S**: Move forward/backward
- **Q/D**: Move left/right (strafe)
- **A/E**: Move down/up
- **Mouse**: Click and drag to look around
- **Space**: Reset camera to default position

#### Simulation Controls
- **P**: Pause/Resume the simulation
- **Up/Down Arrow**: Increase/Decrease simulation speed (0.1x to 10x)
- **O**: Toggle orbit lines visibility
- **I**: Toggle information display (shows time, speed, camera position, pause status)
- **H**: Toggle help menu

#### Planet Camera Presets
Instantly jump to view specific celestial bodies:
- **1**: Sun view
- **2**: Mercury
- **3**: Venus
- **4**: Earth
- **5**: Mars
- **6**: Jupiter
- **7**: Saturn
- **8**: Uranus
- **9**: Neptune
- **0**: Pluto

#### Lighting Controls
- **+**: Increase light intensity
- **-**: Decrease light intensity

#### General
- **ESC**: Exit the simulation

## Building and Running

### Prerequisites

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install freeglut3-dev libglu1-mesa-dev mesa-common-dev build-essential
```

#### macOS
OpenGL and GLUT come pre-installed on macOS. You just need Xcode command line tools:
```bash
xcode-select --install
```

### Build Instructions

#### Using Make
```bash
make
```

#### Manual Compilation

**Linux:**
```bash
g++ -o main mainprojet.cpp -lGL -lGLU -lglut -lm -std=c++11
```

**macOS:**
```bash
g++ -o main mainprojet.cpp -framework OpenGL -framework GLUT -std=c++11
```

### Running the Simulation

#### Using Make
```bash
make run
```

#### Direct Execution
```bash
./main
```

## Technical Details

### Physics
- Planets orbit at different speeds reflecting their relative orbital periods
- The Moon orbits Earth while Earth orbits the Sun
- Asteroid belt particles have randomized orbits between Mars and Jupiter
- Comet follows an eccentric elliptical orbit with high eccentricity (0.8)
- Simulation time is tracked in Earth days

### Rendering
- Uses OpenGL fixed-function pipeline with GLUT
- Real-time 3D rendering at 60 FPS (16ms frame time)
- Smooth lighting model with adjustable intensity
- Depth testing for proper occlusion
- Alpha blending for UI elements and effects

### Performance
- Efficiently renders 10,000+ objects (stars, asteroids, planets)
- Optimized rendering with proper use of OpenGL state management
- Responsive camera controls with FPS-style movement

## Project Structure

```
solarsystem_openGL/
├── mainprojet.cpp          # Main source code with all functionality
├── Makefile                # Build automation
├── README.md               # This file
├── .gitignore              # Git ignore rules
├── c_cpp_properties.json   # VS Code C++ configuration
├── launch.json             # VS Code launch configuration
└── tasks.json              # VS Code build tasks
```

## Future Enhancements

Potential improvements for future versions:
- Textures for planets with real NASA imagery
- Planetary rings for Jupiter, Uranus, and Neptune
- More moons for outer planets
- Realistic orbital mechanics with Kepler's laws
- Planet rotation on tilted axes
- Meteor showers
- Milky Way galaxy background
- Planet selection with detailed information panel
- Path tracing to show historical planet positions
- Save/load camera positions

## Educational Value

This simulation demonstrates:
- 3D graphics programming with OpenGL
- Celestial mechanics and orbital dynamics
- Interactive camera systems
- User interface design in 3D applications
- C++ programming practices

## Credits

Created as an educational project to demonstrate solar system dynamics and OpenGL programming.

## License

This project is open source and available for educational purposes.

## Screenshots

The simulation features:
- A glowing yellow Sun at the center
- Planets in accurate relative positions (not to scale for visibility)
- Dimmed gray orbit lines (toggleable)
- Realistic planet colors and appearances
- A star-filled background
- On-screen information display
- Interactive help menu

Explore the cosmos! 🚀🌍🪐
