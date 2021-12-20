#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Libs/FastNoiseLite.h"
#include "Libs/nlohmann_json.hpp"
#include "Libs/stb_image.h"

#include "Interfaces/Renderable.h"
#include "Interfaces/GameObject.h"
#include "Objects/Colour.h"
#include "Objects/Gradient.h"
#include "Objects/Ship/Ship.h"
#include "Objects/CelestialObjects/PlanetSettings.h"
#include "Objects/CelestialObjects/PlanetModelFactory.h"
#include "Objects/ObjectLoader/Loader.h"
#include "Objects/Shader.h"
#include "Quests/Quest.h"
#include "Quests/QuestLog.h"
#include "UI/LoadingScreen.h"
#include "UI/HUD.h"
#include "Math/Math.h"
#include "Objects/CelestialObjects/Sun/Sun.h"

#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

#ifdef __APPLE__
bool inFullScreen = false;
#endif

// Define Constants
#define MIN_NUM_PLANETS 10
#define MAX_NUM_PLANETS 20
#define MAX_PLANET_SIZE 30
#define MIN_PLANET_SIZE 15

// Define distances between planets
#define SCENE_BOUNDS_MIN_X 75
#define SCENE_BOUNDS_MIN_Z 75
#define SCENE_BOUNDS_MAX_X 150
#define SCENE_BOUNDS_MAX_Z 150

#define PROJECT_TITLE "Space Explorer - COMPSCI 3GC3 Final Project (Daniel Genkin, L. Yao, Mark Hutchison)"


// Window Properties and Global Variables
int window = -1;                  // stores id of the glut window
float screenHeight = 800;         // current height of the window
float screenWidth = 1200;         // current width of the window
float animationTime = 17;         // animation callback time
float loadingAnimationTime = 150; // animation callback time during loading (we don't need a fast refresh)
float deltaTime = animationTime / 1000.0;
float rotationIntensity = 1.0f; // rotation intensity
bool loading = true;

// Status of modifier keys
bool shiftPressed = false;

// Scene Lighting
Sun sun;

// Global Class Instances
vector<PlanetModel> celestialObjects;
QuestLog questLog = QuestLog();
Loader objectloader = Loader();
Ship ship = Ship();
HUD hud = HUD(&ship, &questLog);
LoadingScreen loadingScreen = LoadingScreen();

// Skybox
// With help from https://learnopengl.com/Advanced-OpenGL/Cubemaps
// (But modified for this older version of OpenGL)
// Textures generated from https://tools.wwwtyro.net/space-3d/index.html
Shader skyboxShader = Shader();
unsigned int skyboxBuffer;
unsigned int skyboxTexture;
float skyboxVertices[] = {
    // positions
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f};
float skyboxViewMatrix[] = {
    1, 0, 0, ship.cameraPosition.x,
    0, 1, 0, ship.cameraPosition.y,
    0, 0, 1, ship.cameraPosition.z,
    0, 0, 0, 1};

// clean everything up and exit program
void quit()
{
    glDeleteTextures(1, &skyboxTexture);

    #ifndef __APPLE__
        glutLeaveMainLoop();
    #else
        // Thankfully this does not seg fault on mac
        // unlike Ubuntu but glutLeaveMainLoop is still
        // not defined, so we must do it this way
        if (window != -1) {
            glutDestroyWindow(window);
        }
        exit(0);
    #endif
}

void set3DPerspective()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = screenWidth / screenHeight;
    gluPerspective(aspect * 12 + 45, aspect, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

// main display loop in the game (not used for loading to avoid unnecessary if)
void display(void)
{
    // Turn On 3D rendering
    set3DPerspective();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float *matrix = ship.rotation.toMatrix();

    glPushMatrix();
        // emulate gluLookAt but via the magic of quaternions
        glMultMatrixf(matrix);
        glTranslatef(-ship.position.x, -ship.position.y, -ship.position.z);

        // Sun lighting
        sun.render();

        for (PlanetModel &model : celestialObjects)
        {
            model.render();
        }
    glPopMatrix();
    glPushMatrix();
        ship.render();
    glPopMatrix();

    // Render skybox using shaders
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    // Set uniforms
    glUniform1i(glGetUniformLocation(skyboxShader.programID, "screenWidth"), screenWidth);
    glUniform1i(glGetUniformLocation(skyboxShader.programID, "screenHeight"), screenHeight);
    // Render vertices
    glPushMatrix();
        glLoadIdentity();
        glMultMatrixf(matrix);
        delete[] matrix;

        glBindBuffer(GL_ARRAY_BUFFER, skyboxBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();
    // Swap back to fixed pipeline rendering. This is safe since we're using a compatibility profile.
    glUseProgram(0);
    glDepthFunc(GL_LESS);

    glPushMatrix();
        hud.render(screenWidth, screenHeight);
    glPopMatrix();

    glutSwapBuffers();
}

// display func for the loading
void loadingDisplay(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    // Turn off the lighting
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    loadingScreen.render(screenWidth, screenHeight);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Restore the previous view
    glPopMatrix();
    glutSwapBuffers();
}
// Glut window resize handler
void reshapeHandler(int w, int h)
{
    screenHeight = h;
    screenWidth = w;

    hud.resize(w, h);

    set3DPerspective();

    glViewport(0, 0, w, h);
}

void timerFunc(int v)
{
    ship.animate(deltaTime);

    // test for collision with sun
    GameObject sunObj = sun.getGameObject();
    ship.avoidObject(sunObj, sunObj.scale.x);

    for (PlanetModel &planet : celestialObjects)
    {
        planet.animate(deltaTime);

        ship.avoidObject(planet, planet.radius);

        for (Outpost &outpost : planet.outposts)
        {
            ship.avoidObject(outpost, 3);
        }
    }

    hud.update(ship.position, deltaTime);

    glutPostRedisplay();
    glutTimerFunc(animationTime, timerFunc, 0);
}

// same as with the display we use a separate func for loading for performance
void loadingTimerFunc(int v)
{
    // if we are done loading
    if (!loading)
    {
        // finish up loading textures and other things that require context since we now have opengl context
        for (PlanetModel &m : celestialObjects)
        {
            m.init();
        }
        ship.init();

        sun.init();

        //set to the actual display and animate funcs
        glutDisplayFunc(display);
        glutTimerFunc(animationTime, timerFunc, 0);
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);

        // Create skybox
        glGenBuffers(1, &skyboxBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Load cubemap texture
        vector<string> skyboxSides{
            "assets/Skybox/right.png",
            "assets/Skybox/left.png",
            "assets/Skybox/bottom.png",
            "assets/Skybox/top.png",
            "assets/Skybox/front.png",
            "assets/Skybox/back.png",
        };
        objectloader.loadCubeMap(skyboxSides, skyboxTexture);
        // Set shader
        skyboxShader.init("assets/Shaders/skybox.vert", "assets/Shaders/skybox.frag");
        skyboxShader.use();
        glUniform1i(glGetUniformLocation(skyboxShader.programID, "texID"), 0);
        glUseProgram(0);
    }
    // otherwise show the loading
    else
    {
        loadingScreen.animate();
        glutPostRedisplay();
        glutTimerFunc(loadingAnimationTime, loadingTimerFunc, 0);
    }
}

void setupScene()
{
    loading = true;

    int numPlanets = randInt(MIN_NUM_PLANETS, MAX_NUM_PLANETS);
    int numSteps = 1 /*ship*/ + 1 /*sun*/ + 1 /*gradients*/ + 1 /*planet templates*/ + numPlanets * 2 /*planets + their outposts*/ + 1 /*first quest*/ + 1 /*clear object cache*/ + 1 /*terrain cache*/ + 1 /*planet cache*/;
    loadingScreen.setNumLoadingSteps(numSteps);

    loadingScreen.nextLoadingStep("Loading Ship...");
    ship.setup(objectloader);

    loadingScreen.nextLoadingStep("Loading Sun...");
    // center at almost origin. We can't use the actual origin as it breaks collision
    // detection's ability to find a good rotation axis. So we ofset it a tiny bit to
    // avoid 0s cancelling out our angles
    sun = Sun::basicSun(GL_LIGHT0, Vector3(0.01, 0, 0.01));

    loadingScreen.nextLoadingStep("Loading Terrain Gradients...");
    Gradient::loadGradients();

    loadingScreen.nextLoadingStep("Loading Planet Templates...");
    PlanetSettings::loadSettings();

    loadingScreen.nextLoadingStep("Creating Planets...");
    float x = 10.0f; // min distance of closest planet to ship's spawn point(0,0,0)
    float z = 10.0f;
    for (int i = 0; i < numPlanets; i++)
    {
        stringstream ss;
        ss << "Creating Planet " << i + 1 << " of " << numPlanets;
        loadingScreen.nextLoadingStep(ss.str());

        PlanetSettings pSettings = PlanetSettings::randomStoredSettings(MIN_PLANET_SIZE, MAX_PLANET_SIZE);

        PlanetModelFactory planet = PlanetModelFactory(pSettings, 64);

        x += randFloat(SCENE_BOUNDS_MIN_X, SCENE_BOUNDS_MAX_X);
        z += randFloat(SCENE_BOUNDS_MIN_Z, SCENE_BOUNDS_MAX_Z);

        planet.model.position = Vector3(x * (randInt(-100, 100) % 2 ? -1 : 1), 0.0f, z * (randInt(-100, 100) % 2 == -1 ? -1 : 1)); // this rand puts the planet in one of the 4 quadrants around the ship

        // add an outpost
        loadingScreen.nextLoadingStep("Loading outpost for the planet...");
        Outpost outpost = Outpost(planet.model.position, planet.model.radius, objectloader);
        planet.model.outposts.push_back(outpost);

        celestialObjects.push_back(planet.model);
    }

    loadingScreen.nextLoadingStep("Generating first quest!");
    questLog.setPotentialTargets(celestialObjects);
    questLog.claimQuest(questLog.generateQuest());
    hud.ForceRefreshUI();

    loadingScreen.nextLoadingStep("Clearing object loader cache...");
    objectloader.invalidateCaches();

    loadingScreen.nextLoadingStep("Clearing stored terrain gradients...");
    Gradient::clearStoredGradients();

    loadingScreen.nextLoadingStep("Clearing stored planet templates...");
    PlanetSettings::clearStoredSettings();

    loadingScreen.nextLoadingStep("Starting!");

    loading = false;
}

void keyboard(unsigned char key, int x, int y)
{
    int modifiers = glutGetModifiers();
    shiftPressed = (modifiers && GLUT_ACTIVE_SHIFT) != 0;

    switch (key)
    {
    case 27: // escape
        quit();
        break;
    case 'w':
    case 'W':
        if (shiftPressed)
        {
            ship.adjustThrottle(1);
        }
        else
        {
            ship.pitch(-rotationIntensity);
        }
        break;
    case 's':
    case 'S':
        if (shiftPressed)
        {
            ship.adjustThrottle(-1);
        }
        else
        {
            ship.pitch(rotationIntensity);
        }
        break;
    case 'a':
    case 'A':
        ship.yaw(rotationIntensity);
        break;
    case 'd':
    case 'D':
        ship.yaw(-rotationIntensity);
        break;
    case 'q':
    case 'Q':
        ship.roll(-rotationIntensity);
        break;
    case 'e':
    case 'E':
        ship.roll(rotationIntensity);
        break;
    case ' ':
        ship.setBoostMode(true);
        break;
    case 'f': {
        #ifndef __APPLE__ // MacOS doesn't support fullscreen toggle and makes life harder for everyone (as always)
            glutFullScreenToggle();
        #else
            inFullScreen = !inFullScreen;
            if (inFullScreen)
            {
                glutFullScreen();
            }
            else
            {
                // hack to get out of fullscreen (honestly Apple, why)
                glutReshapeWindow(1200, 850);
                glutPositionWindow(0, 0);
            }
        #endif
        break;
        }
    }
}

void keyboardRelease(unsigned char key, int x, int y)
{
    int modifiers = glutGetModifiers();
    shiftPressed = (modifiers && GLUT_ACTIVE_SHIFT) != 0;

    if (!shiftPressed)
    {
        ship.adjustThrottle(0);
    }

    switch (key)
    {
    case 'w':
    case 'W':
    case 's':
    case 'S':
        ship.pitch(0);
        if (shiftPressed)
        {
            ship.adjustThrottle(0);
        }
        break;
    case 'a':
    case 'A':
    case 'd':
    case 'D':
        ship.yaw(0);
        break;
    case 'q':
    case 'Q':
    case 'e':
    case 'E':
        ship.roll(0);
        break;
    case ' ':
        ship.setBoostMode(false);
        break;
    }
}

void mouseClick(int button, int state, int x, int y)
{
    hud.mouseClick(button, state, x, screenHeight - y);
}

void printControls()
{
    cout << "  _____                        ______            _                     \n"
         << " / ____|                      |  ____|          | |                    \n"
         << "| (___  _ __   __ _  ___ ___  | |__  __  ___ __ | | ___  _ __ ___ _ __ \n"
         << " \\___ \\| '_ \\ / _` |/ __/ _ \\ |  __| \\ \\/ / '_ \\| |/ _ \\| '__/ _ \\ '__|\n"
         << " ____) | |_) | (_| | (_|  __/ | |____ >  <| |_) | | (_) | | |  __/ |   \n"
         << "|_____/| .__/ \\__,_|\\___\\___| |______/_/\\_\\ .__/|_|\\___/|_|  \\___|_|   \n"
         << "       | |                                | |                          \n"
         << "       |_|                                |_|                          \n"
         << endl;
    cout << "| Action | Functionality          |\n"
         << "| ------ | -----------------------|\n"
         << "| w/s    | Adjust Ship's Pitch    |\n"
         << "| a/d    | Adjust Ship's Yaw      |\n"
         << "| q/e    | Adjust Ship's Roll     |\n"
         << "| f      | Toggle fullscreen      |\n"
         << "| W/S    | Adjust Ship's Throttle |\n"
         << "| Space  | Activate Booster       |\n"
         << "| ESC    | Close Simulator        |\n"
         << endl;

    #ifdef __APPLE__
        cout << "You are on MacOS, your experience may be limited. Please switch to Ubuntu 20 LTS if possible." << endl;
    #elif _WIN32
        cout << "You are on Windows, which we do not suppport. Exiting..." << endl;
        exit(1);
    #endif
}

/* main function - program entry point */
int main(int argc, char **argv)
{
    printControls();

    srand(time(NULL));

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(PROJECT_TITLE);

    // we use glad to load opengl instead of the base freeglut headers. This is to allow
    // for the use of shaders
    if (!gladLoadGLLoader((GLADloadproc)glutGetProcAddress))
    {
        if (!gladLoadGL())
        {
            std::cerr << "Failed to initialize the OpenGL context." << std::endl;
            exit(1);
        }
    }

    cout << "Running OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "Running GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f); // make background almost black
    glColor3f(1.0f, 1.0f, 1.0f);

    // object occlusion
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // textures for the models
    glEnable(GL_TEXTURE_2D);

    // Gouraud shading
    glShadeModel(GL_SMOOTH);

    // blending of colors (i.e. transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Link callback functions
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardRelease);
    glutMouseFunc(mouseClick);
    glutReshapeFunc(reshapeHandler);
    // set the display and aniamte func to the loading one
    glutSetCursor(GLUT_CURSOR_WAIT);
    glutDisplayFunc(loadingDisplay);
    glutTimerFunc(loadingAnimationTime, loadingTimerFunc, 0);
    glMatrixMode(GL_MODELVIEW);

    // setup the scene
    thread setupInBackground(setupScene);
    glutMainLoop();

    return 0;
}
