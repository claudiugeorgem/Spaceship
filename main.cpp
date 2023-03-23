#include <iostream>
#include <gl\glut.h>

/*
    Declare the state of the game:
    state = 0 => open startScreen()
    state = 1 => open playScreen()
    state = 2 => open endScreen()
*/ 
int state = 0;

// Initialize score and highscore with value 0
int score = 0;
int highscore = 0;

// Initialize variables in game (enemy collide, bullet shot, bullet hit)
bool collide = false;
bool bulletshot = false;
bool bulletHit = false;
char buffer[10];

// Set initial position of spaceship
// Declare obstacle and bullet position
int spaceshipX = 200, spaceshipY = 70;
int obstacleX[4], obstacleY[4];
int bulletX, bulletY;

// Prints messages on screen
void printMessage(char* ch, int xpos, int ypos)
{
    int numofchar = strlen(ch);
    glLoadIdentity();
    glRasterPos2f(xpos, ypos);
    for (int i = 0; i < numofchar; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ch[i]);
    }
}

// Counts and prints the score
void printMessageNum(char ch[], int numtext, int xpos, int ypos)
{
    int len = numtext - strlen(ch);

    glLoadIdentity();
    glRasterPos2f(xpos, ypos);

    int k;
    k = 0;
    for (int i = 0; i < numtext; i++)
    {
        if (i < len)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0');
        else
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ch[k]);
            k++;
        }
    }
}

// Draws the background of the game
void drawSpace()
{
    for (int i = 0; i < 10; i++)                    // Add random white dots (small-sized stars)
    {
        glPointSize(1.0);
        glBegin(GL_POINTS);
        glVertex2f(rand() % 800, rand() % 500);
        glEnd();
    }
    for (int i = 0; i < 10; i++)                    // Add random white dots (medium-sized stars)
    {
        glPointSize(2.0);   
        glBegin(GL_POINTS);
        glVertex2f(rand() % 800, rand() % 500);
        glEnd();
    }
    for (int i = 0; i < 10; i++)                    // Add random white dots (big-sized stars)
    {
        glPointSize(3.0);
        glBegin(GL_POINTS);
        glVertex2f(rand() % 800, rand() % 500);
        glEnd();
    }
}

// Draws the player character
void drawSpaceship()
{
    glBegin(GL_TRIANGLES);                          // Trigger
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(spaceshipX, spaceshipY + 60);
    glVertex2f(spaceshipX - 18, spaceshipY + 40);
    glVertex2f(spaceshipX + 18, spaceshipY + 40);
    glEnd();

    glBegin(GL_TRIANGLES);                          // Tail
    glColor3f(0.8f, 0.8f, 1.0f);
    glVertex2f(spaceshipX, spaceshipY + 40);
    glVertex2f(spaceshipX - 23, spaceshipY - 35);
    glVertex2f(spaceshipX + 23, spaceshipY - 35);
    glEnd();

    glBegin(GL_QUADS);                              // Middle Body
    glColor3f(0.0f, 0.5f, 0.5f);  
    glVertex2f(spaceshipX - 25, spaceshipY + 25);
    glVertex2f(spaceshipX - 25, spaceshipY - 25);
    glVertex2f(spaceshipX + 25, spaceshipY - 25);
    glVertex2f(spaceshipX + 25, spaceshipY + 25);
    glEnd();

    glBegin(GL_TRIANGLES);                          // Right wing
    glColor3f(0.0f, 0.2f, 0.8f);
    glVertex2f(spaceshipX + 25, spaceshipY + 25);
    glVertex2f(spaceshipX + 25, spaceshipY - 25);
    glVertex2f(spaceshipX + 45, spaceshipY - 35);
    glEnd();

    glBegin(GL_TRIANGLES);                          // Left wing
    glColor3f(0.0f, 0.2f, 1.0f);
    glVertex2f(spaceshipX - 25, spaceshipY + 25);
    glVertex2f(spaceshipX - 25, spaceshipY - 25);
    glVertex2f(spaceshipX - 45, spaceshipY - 35);
    glEnd();

    glBegin(GL_QUADS);                              // Upper body
    glColor3f(0.0f, 0.5f, 0.5f);
    glVertex2f(spaceshipX - 25, spaceshipY + 25);
    glVertex2f(spaceshipX - 18, spaceshipY + 40);
    glVertex2f(spaceshipX + 18, spaceshipY + 40);
    glVertex2f(spaceshipX + 25, spaceshipY + 25);
    glEnd();
}

// Draws the incoming obstacles
void drawObstacle()
{
    for (int i = 0; i < 4; i++)
    {
        glBegin(GL_TRIANGLES);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex2f(obstacleX[i], obstacleY[i] + 20);
        glVertex2f(obstacleX[i] - 23, obstacleY[i] - 35);
        glVertex2f(obstacleX[i] + 23, obstacleY[i] - 35);
        glEnd();

        obstacleY[i] = obstacleY[i] - 5;    // Obstacles approach the spaceship

        if (obstacleY[i] > spaceshipY && obstacleY[i] < spaceshipY + 45 + 45 && obstacleX[i] == spaceshipX)     // Obstacle was hit by spaceship
        {
            collide = true;
        }
        if (obstacleY[i] - bulletY < 20 && obstacleX[i] == bulletX)     // Obstacle was hit by bullet
        {
            bulletHit = true;
            score += 100;
            obstacleY[i] = -100;
            obstacleX[i] = -100;
            bulletY = -100;
            bulletX = -100;

        }
        if (obstacleY[i] < -25)     // Spawn another obstacle
        {
            if (rand() % 2 == 0)
            {
                obstacleX[i] = 200;
            }
            else
            {
                obstacleX[i] = 300;
            }
            obstacleY[i] = 800;
        }
    }
}

// Shooting animation
void shotObstacle()
{
    if (bulletHit == false) // Spawn a bullet if there is none on the screen
    {
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glColor3f(255.0f, 255.0f, 255.0f);
        glVertex2f(bulletX, bulletY);
        bulletY += 10;
        glEnd();
    }
    else
    {
        bulletshot = false;
        bulletHit = false;
    }
}

// Initializes obstacles when starting a new game
void obstaclePos()
{
    glClearColor(0, 0, 0, 0);
    for (int i = 0; i < 4; i++)
    {
        if (rand() % 2 == 0)
        {
            obstacleX[i] = 200;   // Left obstacle
        }
        else
        {
            obstacleX[i] = 300;   // Right obstacle
        }
        obstacleY[i] = 1000 - i * 200;
    }
}

// Start screen
void startScreen()
{
    state = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Instructions
    glColor3f(1.0f, 0.0f, 1.0f);
    printMessage((char*)"Start the game by pressing \"1\" or by clicking anywhere on the game window.", 45,340);
    printMessage((char*)"You can leave the game by pressing esc key.", 135, 320);
    glColor3f(0.75f, 0.0f, 0.75f);
    printMessage((char*)"Avoid crashing into obstacles.", 165, 280);
    printMessage((char*)"Use left and right arrow keys to steer the spaceship.", 100, 260);
    printMessage((char*)"Press the space bar to shoot obstacles in your path.", 103, 240);
    printMessage((char*)"Your game is over when you crash into an obstacle.", 108, 220); 
    glColor3f(0.5f, 0.0f, 0.5f);
    printMessage((char*)"Have fun and try to beat your highscore!", 135, 180);
    
    glFlush();
    glutSwapBuffers();
}

// End screen
void endScreen()
{
    state = 2;
    score = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Instructions
    glColor3f(0.0f, 1.0f, 0.0f);
    printMessage((char*)"The spaceship has been damaged.", 165, 320);
    glColor3f(0.0f, 0.75f, 0.0f);
    printMessage((char*)"Try again to beat your highscore!", 160, 280);
    printMessage((char*)"You can retry by pressing \"1\" or by clicking anywhere on the game window.", 45, 260);
    glColor3f(0.0f, 0.5f, 0.0f);
    printMessage((char*)"You can leave the game by pressing esc key.", 135, 220);

    glFlush();
    glutSwapBuffers();
}

// Main screen
void playScreen()
{
    state = 1;
    if (collide != true)    // Spaceship has not been hit by the obstacle
    {
        glClear(GL_COLOR_BUFFER_BIT);
        drawSpace();
        drawSpaceship();
        drawObstacle();

        if (bulletshot == true)
        {
            shotObstacle();
        }

        score = score + 1;

        glColor3f(1.0f, 1.0f, 1.0f);
        printMessage((char*)"Score: ", 360, 455);
        _itoa_s(score, buffer, 10);
        printMessageNum(buffer, 6, 420, 455);

        printMessage((char*)"High Score: ", 10, 455);
        if (score > highscore)
        {
            highscore = score;
            _itoa_s(highscore, buffer, 10);
            printMessageNum(buffer, 6, 100, 455);
        }
        else
        {
            _itoa_s(highscore, buffer, 10);
            printMessageNum(buffer, 6, 100, 455);
        }

        glutSwapBuffers();
        for (int q = 0; q <= 10000000; q++)
        {
            ;
        }
    }
    else
    {
        collide = false;
        state = 2;
    }
}

// Initializes program
void init()
{
    glClearColor(37 / 255, 33 / 255, 40 / 255, 1);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

// Switches between states
void display()
{
    if (state == 0) 
    {
        startScreen();
    }
    else if (state == 1) 
    {
        playScreen();
    }
    else if (state == 2) 
    {
        endScreen();
    }
    glFlush();
}

// Starts the game whenever the player clicks anywhere on game window
void mouse(int button, int st, int x, int y) 
{
        if (button == GLUT_LEFT_BUTTON && st == GLUT_DOWN && state != 1) 
        {
            obstaclePos();
            state = 1;
        }
}

// Switches between states when using keyboard
void keyboard(unsigned char key, int x, int y)
{
    if (state == 0 && key == '1') 
    {
        state = 1;
    }
    if (state == 1 && key == ' ' && bulletshot==false)  // Spawn a bullet if there is none on the screen
    {
        bulletX = spaceshipX;
        bulletY = spaceshipY + 60;
        bulletshot = true;
        if (bulletY > 400)
        {
            bulletHit = false;
            bulletshot = false;
        }
    }
    if (state == 2 && key == '1')
    {
        obstaclePos();
        state = 1;
    }
    if (key == 27) 
    {
        exit(EXIT_SUCCESS);
    }
    glutPostRedisplay();
}

// Moves the spaceship
void Specialkey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        spaceshipX = 200;
        break;
    case GLUT_KEY_RIGHT:
        spaceshipX = 300;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 500);
    glutCreateWindow("Spaceship Game");
    obstaclePos();
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(Specialkey);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}