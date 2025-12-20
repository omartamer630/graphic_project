#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Function prototypes
void InitGraphics(int argc, char *argv[]);
void SetTransformations();
void OnDisplay();
void OnSpecialKeyPress(int key, int x, int y);
void DrawText(float x, float y, const char *text);

// Color variables
float colorR = 1.0f;
float colorG = 0.0f;
float colorB = 0.0f;
int currentColorIndex = 0;

// Line position variables
float lineProgress = 0.0f;
float targetLineProgress = 0.0f;
const float moveSpeed = 0.05f;

// Color presets and names
struct ColorPreset
{
  float r, g, b;
  const char *name;
};

ColorPreset colors[] = {
    {1.0f, 0.0f, 0.0f, "Red"},
    {0.0f, 1.0f, 0.0f, "Green"},
    {0.0f, 0.0f, 1.0f, "Blue"},
    {1.0f, 1.0f, 0.0f, "Yellow"},
    {1.0f, 0.0f, 1.0f, "Magenta"},
    {0.0f, 1.0f, 1.0f, "Cyan"},
    {1.0f, 0.5f, 0.0f, "Orange"},
    {0.5f, 0.0f, 0.5f, "Purple"},
    {1.0f, 0.75f, 0.8f, "Pink"},
    {0.5f, 0.5f, 0.5f, "Gray"}};

const int numColors = sizeof(colors) / sizeof(colors[0]);

int main(int argc, char *argv[])
{
  srand(time(NULL));
  InitGraphics(argc, argv);
  glutMainLoop(); // Start the main loop

  return 0;
}

// setup window and callbacks
void InitGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowPosition(500, 500);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Color Changer Game - Use Arrows Keys"); // title 

  glutDisplayFunc(OnDisplay);
  glutSpecialFunc(OnSpecialKeyPress);
  glutIdleFunc(OnDisplay);

  SetTransformations();
}


// defines coordinate system
void SetTransformations()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-100, 100, -100, 100);
  glMatrixMode(GL_MODELVIEW);
}

// contiuously called to update display
void OnDisplay()
{
  glLoadIdentity();

  // Smoothly move line progress towards target
  float diff = targetLineProgress - lineProgress;
  if (fabs(diff) > 0.01f)
  {
    lineProgress += diff * moveSpeed;
  }
  else
  {
    lineProgress = targetLineProgress;
  }

  // Calculate horizontal line position (moves left to right across rectangle)
  float lineX = -60.0f + (lineProgress * 120.0f); // Maps 0-1 to -60 to +60

  // Set background color to white
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw the colored rectangle
  glBegin(GL_QUADS);
  glColor3f(colorR, colorG, colorB);
  glVertex3f(-60, -40, 0);
  glVertex3f(60, -40, 0);
  glVertex3f(60, 40, 0);
  glVertex3f(-60, 40, 0);
  glEnd();

  // Draw border for the rectangle
  glBegin(GL_LINE_LOOP);
  glColor3f(0, 0, 0);
  glVertex3f(-60, -40, 0);
  glVertex3f(60, -40, 0);
  glVertex3f(60, 40, 0);
  glVertex3f(-60, 40, 0);
  glEnd();

  // Draw the moving vertical line (horizontal movement, vertical orientation) 
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glColor3f(0.0f, 0.0f, 0.0f); // Black line
  glVertex2f(lineX, -40.0f); // Bottom of rectangle
  glVertex2f(lineX, 40.0f);  // Top of rectangle
  glEnd();
  glLineWidth(1.0f);

  // Draw background box for color name
  glBegin(GL_QUADS);
  glColor3f(0.9f, 0.9f, 0.9f);
  glVertex3f(-40, 55, 0);
  glVertex3f(40, 55, 0);
  glVertex3f(40, 70, 0);
  glVertex3f(-40, 70, 0);
  glEnd();

  // Draw border for color name box
  glBegin(GL_LINE_LOOP);
  glColor3f(0, 0, 0);
  glVertex3f(-40, 55, 0);
  glVertex3f(40, 55, 0);
  glVertex3f(40, 70, 0);
  glVertex3f(-40, 70, 0);
  glEnd();

  // Display color name (large and centered)
  glColor3f(0, 0, 0);
  DrawText(-5, 60, colors[currentColorIndex].name);

  // Display instructions
  glColor3f(0, 0, 0);
  DrawText(-80, -80, "Arrow Keys: Change Color");
  DrawText(-80, -90, "UP/DOWN: Next/Previous");

  glFlush();
}

void DrawText(float x, float y, const char *text)
{
  glRasterPos2f(x, y);
  for (const char *c = text; *c != '\0'; c++)
  {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
}

// special
void OnSpecialKeyPress(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    // Next color
    currentColorIndex = (currentColorIndex + 1) % numColors;
    colorR = colors[currentColorIndex].r;
    colorG = colors[currentColorIndex].g;
    colorB = colors[currentColorIndex].b;
    // Move line to new random position
    targetLineProgress = (float)(rand() % 100) / 100.0f;
    break;

  case GLUT_KEY_DOWN:
    // Previous color
    currentColorIndex = (currentColorIndex - 1 + numColors) % numColors;
    colorR = colors[currentColorIndex].r;
    colorG = colors[currentColorIndex].g;
    colorB = colors[currentColorIndex].b;
    // Move line to new random position
    targetLineProgress = (float)(rand() % 100) / 100.0f;
    break;

  case GLUT_KEY_RIGHT:
    // Next color (same as UP)
    currentColorIndex = (currentColorIndex + 1) % numColors;
    colorR = colors[currentColorIndex].r;
    colorG = colors[currentColorIndex].g;
    colorB = colors[currentColorIndex].b;
    // Move line to new random position
    targetLineProgress = (float)(rand() % 100) / 100.0f;
    break;

  case GLUT_KEY_LEFT:
    // Previous color (same as DOWN)
    currentColorIndex = (currentColorIndex - 1 + numColors) % numColors;
    colorR = colors[currentColorIndex].r;
    colorG = colors[currentColorIndex].g;
    colorB = colors[currentColorIndex].b;
    // Move line to new random position
    targetLineProgress = (float)(rand() % 100) / 100.0f;
    break;
  }

  glutPostRedisplay(); // screen update 
}

// Compilation command for Linux:
// g++ main.cpp -o color_changer -lGL -lGLU -lglut
// Then run: ./color_changer
