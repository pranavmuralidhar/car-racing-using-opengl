OpenGL Car Racing Game

A simple 3D Car Racing Game built using OpenGL.
This project demonstrates basic game development concepts such as rendering, transformations, user input handling, collision detection, and real-time animation using OpenGL.

Features

3D car model with smooth movement

Racing track with curves and obstacles

Keyboard controls for steering, acceleration, and braking

Dynamic camera view (third-person chase camera)

Collision detection with boundaries and obstacles

Score / lap system (if implemented)

Lighting, textures, and shading for realism

Controls
Key	Action
↑	Accelerate
↓	Brake / Reverse
←	Steer Left
→	Steer Right
Esc	Exit Game

Requirements
C++ Compiler (GCC / MinGW / MSVC)

Clone this repository:
git clone https://github.com/your-username/opengl-car-racing.git
cd opengl-car-racing

Compile the project (example with g++ and freeGLUT):
g++ main.cpp -lfreeglut -lglu32 -lopengl32 -o CarRacing

Run the executable:
./CarRacing
