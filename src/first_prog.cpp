// Using SDL, SDL OpenGL and standard IO
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>

// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"


/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 60000;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;


// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t);

// Renders scene to the screen
void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos);

// Frees media and shuts down SDL
void close(SDL_Window** window);


/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow( "TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if( *context == NULL )
            {
                std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if( !initGL() )
                {
                    std::cout << "Unable to initialize OpenGL!"  << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}


bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0, 1000.0);


    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);


    // Lighting basic configuration and activation
    const GLfloat light_ambient[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    // Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cout << "Error initializing OpenGL!  " << gluErrorString( error ) << std::endl;
        success = false;
    }

    return success;
}

void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t)
{
    // Update the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        formlist[i]->update(delta_t);
        i++;
    }
}

void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos, double rho, double theta)
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Set the camera position and parameters
        gluLookAt(  cam_pos.x, cam_pos.y, cam_pos.z,
                    0, 0, 0,
			0.0f, 1.0f,  0.0f);
    // Isometric view
    glRotated(rho, 0, 1, 0);
    glRotated(theta, 1, 0, -1);

    // X, Y and Z axis
    glPushMatrix(); // Preserve the camera viewing point for further forms
    // Render the coordinates system
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(1, 0, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 1, 0);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, 1);
    }
    glEnd();
    glPopMatrix(); // Restore the camera viewing point for next object

    // Render the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        glPushMatrix(); // Preserve the camera viewing point for further forms
        formlist[i]->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
}

void close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}


/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;



    // Start up SDL and create window
    if( !init(&gWindow, &gContext))
    {
        std::cout << "Failed to initialize!" << std::endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time, elapsed_time;

        // Event handler
        SDL_Event event;

        // Camera position
        double xcam = 0;
        double ycam = 50;
        double zcam = 150;

        float vitesse = 2; // vitesse de deplacement de la camera

        // vecteur repr???sentant la direction de la camera
        float lx=0.0f,lz=-1.0f;

        double rho = 0;
        double theta = 0;
        Point camera_position(0, 0, 5.0);

        // The forms to render
        Form* forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i=0; i<MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }

        Maillage *pMaillage = NULL;
        pMaillage = new Maillage(100, 100);

        ConicWave *pConic1 = new ConicWave(Point(7,0,2),0,10,Vector(0,0,0),Vector(0,0,0));
        ConicWave *pConic2 = new ConicWave(Point(15,0,5),0,3,Vector(-1,0,1),Vector(0,0,0));
        CircularWave *pCircular1 = new CircularWave(Point(0,0,0),0,30,4,10,0);
        CircularWave *pCircular2 = new CircularWave(Point(0,0,0),0,30,4,10,0);
        pMaillage->addWave(pCircular1);
        pMaillage->addWave(pCircular2);
        pMaillage->addWave(pConic1);
        pMaillage->addWave(pConic2);
        pMaillage->updateFormList(forms_list, &number_of_forms);



        // Get first "current time"
        previous_time = SDL_GetTicks();
        // While application is running
        while(!quit)
        {
            // Handle events on queue
            while(SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;

                switch(event.type)
                {
                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    // Handle key pressed with current mouse position
                    SDL_GetMouseState( &x, &y );

                    switch(key_pressed)
                    {
                    // Quit the program when 'q' or Escape keys are pressed

                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    case SDLK_1:
                        pConic1->setWaveOrigin(Point(0,0,0));
                        pConic1->setWaveHeight(15);
                        pConic1->setWaveSpeed(Vector(0,0,0));

                        pConic2->setWaveOrigin(Point(0,0,0));
                        pConic2->setWaveHeight(0);
                        pConic2->setWaveSpeed(Vector(0,0,0));

                        pCircular1->setWaveOrigin(Point(0,0,0));
                        pCircular1->setWaveHeight(0);
                        pCircular1->setWaveSpeed(0);

                        pCircular2->setWaveOrigin(Point(0,0,0));
                        pCircular2->setWaveHeight(0);
                        pCircular2->setWaveSpeed(0);
                        break;

                    case SDLK_2:
                        pConic1->setWaveOrigin(Point(-20,0,-20));
                        pConic1->setWaveHeight(15);
                        pConic1->setWaveSpeed(Vector(5,0,5));

                        pConic2->setWaveOrigin(Point(20,0,-20));
                        pConic2->setWaveHeight(10);
                        pConic2->setWaveSpeed(Vector(-5,0,5));

                        pCircular1->setWaveOrigin(Point(0,0,0));
                        pCircular1->setWaveHeight(0);
                        pCircular1->setWaveSpeed(0);

                        pCircular2->setWaveOrigin(Point(0,0,0));
                        pCircular2->setWaveHeight(0);
                        pCircular2->setWaveSpeed(0);
                        break;

                    case SDLK_3:
                        pConic1->setWaveOrigin(Point(-20,0,-20));
                        pConic1->setWaveHeight(0);
                        pConic1->setWaveSpeed(Vector(5,0,5));

                        pConic2->setWaveOrigin(Point(20,0,-20));
                        pConic2->setWaveHeight(0);
                        pConic2->setWaveSpeed(Vector(-5,0,5));

                        pCircular1->setWaveOrigin(Point(0,0,0));
                        pCircular1->setWaveHeight(20);
                        pCircular1->setWaveWidth(20);
                        pCircular1->setWaveSpeed(10);
                        pCircular1->setWaveRadius(0);

                        pCircular2->setWaveOrigin(Point(0,0,0));
                        pCircular2->setWaveHeight(0);
                        pCircular2->setWaveSpeed(0);
                        pCircular2->setWaveRadius(0);
                        break;


                    case SDLK_4:
                        pConic1->setWaveOrigin(Point(-20,0,-20));
                        pConic1->setWaveHeight(0);
                        pConic1->setWaveSpeed(Vector(5,0,5));

                        pConic2->setWaveOrigin(Point(20,0,-20));
                        pConic2->setWaveHeight(0);
                        pConic2->setWaveSpeed(Vector(-5,0,5));

                        pCircular1->setWaveOrigin(Point(0,0,0));
                        pCircular1->setWaveHeight(25);
                        pCircular1->setWaveWidth(4);
                        pCircular1->setWaveSpeed(10);
                        pCircular1->setWaveRadius(0);

                        pCircular2->setWaveOrigin(Point(0,0,0));
                        pCircular2->setWaveHeight(0);
                        pCircular2->setWaveSpeed(0);
                        pCircular2->setWaveRadius(0);
                        break;

                    case SDLK_5:
                        pConic1->setWaveOrigin(Point(-20,0,-20));
                        pConic1->setWaveHeight(0);
                        pConic1->setWaveSpeed(Vector(5,0,5));

                        pConic2->setWaveOrigin(Point(20,0,-20));
                        pConic2->setWaveHeight(0);
                        pConic2->setWaveSpeed(Vector(-5,0,5));

                        pCircular1->setWaveOrigin(Point(-15,0,0));
                        pCircular1->setWaveHeight(15);
                        pCircular1->setWaveSpeed(20);
                        pCircular1->setWaveWidth(5);
                        pCircular1->setWaveRadius(0);

                        pCircular2->setWaveOrigin(Point(15,0,0));
                        pCircular2->setWaveHeight(15);
                        pCircular2->setWaveSpeed(20);
                        pCircular2->setWaveWidth(5);
                        pCircular2->setWaveRadius(0);
                        break;

                    case SDLK_6:
                        pConic1->setWaveOrigin(Point(-20,0,-20));
                        pConic1->setWaveHeight(20);
                        pConic1->setWaveSpeed(Vector(9,0,4));

                        pConic2->setWaveOrigin(Point(20,0,-20));
                        pConic2->setWaveHeight(10);
                        pConic2->setWaveSpeed(Vector(-7,0,8));

                        pCircular1->setWaveOrigin(Point(-15,0,0));
                        pCircular1->setWaveHeight(5);
                        pCircular1->setWaveSpeed(20);
                        pCircular1->setWaveWidth(6);
                        pCircular1->setWaveRadius(0);

                        pCircular2->setWaveOrigin(Point(15,0,0));
                        pCircular2->setWaveHeight(4);
                        pCircular2->setWaveSpeed(20);
                        pCircular2->setWaveWidth(8);
                        pCircular2->setWaveRadius(0);
                        break;


                    case SDLK_UP :
                        xcam += lx * vitesse;
                        zcam += lz * vitesse;
                        break;

                    case SDLK_DOWN :
                        xcam -= lx * vitesse;
                        zcam -= lz * vitesse;
                        break;

                    case SDLK_q:
                        rho += 5;
                        break;

                    case SDLK_d:
                        rho -= 5;
                        break;

                    case SDLK_a:
                        theta += 5;
                        break;

                    case SDLK_e:
                        theta -= 5;
                        break;

                    case SDLK_z:
                        ycam += 2;
                        break;

                    case SDLK_s:
                        ycam -= 2;
                        break;

                    case SDLK_r:
                        ycam = 50;
                        xcam = 0;
                        zcam = 150;
                        rho = 0;
                        theta=0;
                        break;

                    case SDLK_f:
                        ycam = 0;
                        xcam = 0;
                        zcam = 150;
                        rho = 0;
                        break;

                    case SDLK_t:
                        ycam = 150;
                        xcam = 2;
                        zcam = 0;
                        rho = 90;
                        theta = 0;
                        break;

                    case SDLK_o:
                        pConic1->setWaveSpeed(Vector(pConic1->getWaveSpeed().x,0,pConic1->getWaveSpeed().z-1));
                        break;

                    case SDLK_l:
                        pConic1->setWaveSpeed(Vector(pConic1->getWaveSpeed().x,0,pConic1->getWaveSpeed().z+1));
                        break;

                    case SDLK_k:
                        pConic1->setWaveSpeed(Vector(pConic1->getWaveSpeed().x-1,0,pConic1->getWaveSpeed().z));
                        break;

                    case SDLK_m:
                        pConic1->setWaveSpeed(Vector(pConic1->getWaveSpeed().x+1,0,pConic1->getWaveSpeed().z));
                        break;
                    case SDLK_SPACE:
                          pConic1->setWaveSpeed(Vector(0,0,0));
                        break;
                    case SDLK_c:
                          pMaillage->setColorType(false);
                        break;
                    case SDLK_v:
                          pMaillage->setColorType(true);
                        break;
                    default:

                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            // Update the scene
            current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
            elapsed_time = current_time - previous_time;
            if (elapsed_time > ANIM_DELAY)
            {
                previous_time = current_time;
                update(forms_list, 1e-3 * elapsed_time); // International system units : seconds
                pMaillage->update(1e-3 * elapsed_time);
            }

            // Render the scene
            camera_position = Point(xcam, ycam, zcam);
            render(forms_list, camera_position, rho, theta);


            // Update window screen
            SDL_GL_SwapWindow(gWindow);
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
