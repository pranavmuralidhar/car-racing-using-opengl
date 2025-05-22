#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Game state
struct {
    // Player car position and movement
    float x;
    float y;
    float speed;
    float rotation;

    // Road properties
    float roadWidth;
    float roadBounds[2]; // Left and right boundaries

    // Obstacle properties
#define MAX_OBSTACLES 10
    float obstacleX[MAX_OBSTACLES];
    float obstacleY[MAX_OBSTACLES];
    float obstacleSpeed[MAX_OBSTACLES];

    // Game metrics
    int score;
    bool gameOver;

    // Controls
    bool keyStates[256]; // For keeping track of which keys are pressed
} gameState;

// Initialize game state
void initGame() {
    // Initialize random seed
    srand(time(NULL));

    // Set player car initial position and properties
    gameState.x = 0.0f;
    gameState.y = -0.7f;
    gameState.speed = 0.0f;
    gameState.rotation = 0.0f;

    // Set road properties
    gameState.roadWidth = 1.5f;
    gameState.roadBounds[0] = -gameState.roadWidth / 2;
    gameState.roadBounds[1] = gameState.roadWidth / 2;

    // Initialize obstacles
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        gameState.obstacleX[i] = ((float)rand() / RAND_MAX) * gameState.roadWidth - gameState.roadWidth / 2;
        gameState.obstacleY[i] = 1.0f + i * 0.5f; // Stagger obstacles vertically
        gameState.obstacleSpeed[i] = 0.01f + ((float)rand() / RAND_MAX) * 0.01f; // Random speeds
    }

    // Initialize game metrics
    gameState.score = 0;
    gameState.gameOver = false;

    // Initialize key states
    memset(gameState.keyStates, 0, sizeof(gameState.keyStates));
}

// Function to handle keypresses
void keyPressed(unsigned char key, int x, int y) {
    gameState.keyStates[key] = true;

    // Reset game if R is pressed and game is over
    if (key == 'r' && gameState.gameOver) {
        initGame();
    }
}

// Function to handle key releases
void keyReleased(unsigned char key, int x, int y) {
    gameState.keyStates[key] = false;
}

// Process keyboard input and update game state
void processInput() {
    if (gameState.gameOver) return;

    // Process W key - accelerate
    if (gameState.keyStates['w'] || gameState.keyStates['W']) {
        gameState.speed += 0.001f;
        if (gameState.speed > 0.05f) gameState.speed = 0.05f; // Cap max speed
    }
    else {
        // Decelerate when not pressing W
        gameState.speed *= 0.98f;
    }

    // Process S key - brake/reverse
    if (gameState.keyStates['s'] || gameState.keyStates['S']) {
        gameState.speed -= 0.002f;
        if (gameState.speed < -0.02f) gameState.speed = -0.02f; // Cap max reverse speed
    }

    // Process A key - turn left
    if (gameState.keyStates['a'] || gameState.keyStates['A']) {
        gameState.rotation += 0.05f;
        gameState.x -= 0.01f;

        // Keep car within road bounds
        if (gameState.x < gameState.roadBounds[0]) {
            gameState.x = gameState.roadBounds[0];
        }
    }

    // Process D key - turn right
    if (gameState.keyStates['d'] || gameState.keyStates['D']) {
        gameState.rotation -= 0.05f;
        gameState.x += 0.01f;

        // Keep car within road bounds
        if (gameState.x > gameState.roadBounds[1]) {
            gameState.x = gameState.roadBounds[1];
        }
    }
}

// Update game state
void updateGame() {
    if (gameState.gameOver) return;

    // Update obstacles
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        // Move obstacles down (toward player)
        gameState.obstacleY[i] -= gameState.obstacleSpeed[i];

        // If obstacle goes off screen, reset it
        if (gameState.obstacleY[i] < -1.0f) {
            gameState.obstacleY[i] = 1.0f;
            gameState.obstacleX[i] = ((float)rand() / RAND_MAX) * gameState.roadWidth - gameState.roadWidth / 2;
            gameState.obstacleSpeed[i] = 0.01f + ((float)rand() / RAND_MAX) * 0.01f;
            gameState.score += 10; // Increase score for each passed obstacle
        }

        // Check for collisions (simple point-based collision)
        float dx = gameState.x - gameState.obstacleX[i];
        float dy = gameState.y - gameState.obstacleY[i];
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 0.1f) { // Collision threshold
            gameState.gameOver = true;
        }
    }
}

// Draw the player's car
void drawCar() {
    glPushMatrix();
    glTranslatef(gameState.x, gameState.y, 0.0f);
    glRotatef(gameState.rotation, 0.0f, 0.0f, 1.0f);

    // Car body (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, -0.1f);
    glVertex2f(0.05f, -0.1f);
    glVertex2f(0.05f, 0.1f);
    glVertex2f(-0.05f, 0.1f);
    glEnd();

    // Windows (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.04f, -0.02f);
    glVertex2f(0.04f, -0.02f);
    glVertex2f(0.04f, 0.05f);
    glVertex2f(-0.04f, 0.05f);
    glEnd();

    // Wheels (black)
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2f(-0.05f, -0.07f); // Back-left wheel
    glVertex2f(0.05f, -0.07f);  // Back-right wheel
    glVertex2f(-0.05f, 0.07f);  // Front-left wheel
    glVertex2f(0.05f, 0.07f);   // Front-right wheel
    glEnd();

    glPopMatrix();
}

// Draw the road and obstacles
void drawRoad() {
    // Road (gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(gameState.roadBounds[0], -1.0f);
    glVertex2f(gameState.roadBounds[1], -1.0f);
    glVertex2f(gameState.roadBounds[1], 1.0f);
    glVertex2f(gameState.roadBounds[0], 1.0f);
    glEnd();

    // Road markings - dashed center line (white)
    glColor3f(1.0f, 1.0f, 1.0f);
    for (float y = -1.0f; y < 1.0f; y += 0.2f) {
        glBegin(GL_QUADS);
        glVertex2f(-0.02f, y);
        glVertex2f(0.02f, y);
        glVertex2f(0.02f, y + 0.1f);
        glVertex2f(-0.02f, y + 0.1f);
        glEnd();
    }

    // Road side lines (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    // Left side
    glVertex2f(gameState.roadBounds[0], -1.0f);
    glVertex2f(gameState.roadBounds[0], 1.0f);
    // Right side
    glVertex2f(gameState.roadBounds[1], -1.0f);
    glVertex2f(gameState.roadBounds[1], 1.0f);
    glEnd();
}

// Draw the obstacles
void drawObstacles() {
    glColor3f(0.0f, 0.5f, 0.0f); // Green obstacles

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        glPushMatrix();
        glTranslatef(gameState.obstacleX[i], gameState.obstacleY[i], 0.0f);

        // Draw obstacle as a rectangle
        glBegin(GL_QUADS);
        glVertex2f(-0.05f, -0.05f);
        glVertex2f(0.05f, -0.05f);
        glVertex2f(0.05f, 0.05f);
        glVertex2f(-0.05f, 0.05f);
        glEnd();

        glPopMatrix();
    }
}

// Draw score and game over text
void drawText() {
    char scoreText[50];
#ifdef _MSC_VER
    sprintf_s(scoreText, sizeof(scoreText), "Score: %d", gameState.score);
#else
    sprintf(scoreText, "Score: %d", gameState.score);
#endif

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, 0.9f);

    for (int i = 0; scoreText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }

    if (gameState.gameOver) {
        char gameOverText[] = "Game Over! Press 'R' to Restart";
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.5f, 0.0f);

        for (int i = 0; gameOverText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOverText[i]);
        }
    }
}

// Display function - renders the game
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Process input and update game state
    processInput();
    updateGame();

    // Draw game elements
    drawRoad();
    drawObstacles();
    drawCar();
    drawText();

    glutSwapBuffers();
}

// Timer function for animation
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Reshape function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Maintain aspect ratio
    if (width <= height) {
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)height / (GLfloat)width,
            1.0 * (GLfloat)height / (GLfloat)width, -1.0, 1.0);
    }
    else {
        glOrtho(-1.0 * (GLfloat)width / (GLfloat)height,
            1.0 * (GLfloat)width / (GLfloat)height, -1.0, 1.0, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Car Racing Game");

    // Set up OpenGL state
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);
    glutTimerFunc(0, timer, 0);

    // Initialize game state
    initGame();

    // Enter the GLUT main loop
    glutMainLoop();

    return 0;
}