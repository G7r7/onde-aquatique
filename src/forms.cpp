#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"


void Form::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}


void Form::render()
{
    // Point of view for rendering
    // Common for all Forms
    Point org = anim.getPos();
    glColor3f(col.r, col.g, col.b);
    glTranslated(org.x, org.y, org.z);
    glRotated(getAnim().getPhi(), 1, 0, 0);
    glRotated(getAnim().getTheta(), 0, 1, 0);
}


Sphere::Sphere(double r, Color cl)
{
    radius = r;
    col = cl;
}


void Sphere::update(double delta_t)
{
    // Complete this part
}


void Sphere::render()
{
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 0);
    gluSphere(quad, radius, 100, 100);
    Form::render();
    gluDeleteQuadric(quad);
}


Cube_face::Cube_face(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
}


void Cube_face::update(double delta_t)
{
    // Complete this part
    double currentPhi = anim.getPhi();
    anim.setPhi(currentPhi + 1);


    double currentTheta = anim.getTheta();
    anim.setTheta(currentTheta + 1);

    Point currentPos = anim.getPos();
    currentPos.x += 0.001;
    anim.setPos(currentPos);

}


void Cube_face::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(width*vdir2);
    p4.translate(width*vdir2);

    Form::render();

    glBegin(GL_QUADS);
    {
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
        glVertex3d(p3.x, p3.y, p3.z);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();
}

Surface::Surface(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
}


void Surface::update(double delta_t)
{
}


void Surface::render()
{
// Turn on the automatic method vector switch
    glEnable(GL_AUTO_NORMAL);
// Allow regularization vector
    glEnable(GL_NORMALIZE);
    GLUnurbsObj *theNurb;
    theNurb = gluNewNurbsRenderer(); // Create a NURBS surface object

// Modify the properties of NURBS surface objects-glu library function

///// Sampling fault tolerance tolerance

    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 50);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);

// ************************ Called in the drawing function ********** **************** /
//    GLfloat ctrlpoints[6][5][3] = {
//    {{-3,0,0}, {-1,0,0}, {0,0,0}, {1,0,0}, {3,0,0}},
//    {{-3,0,-1},{-1,0,-1},{0,0,-1},{1,0,-1},{3,0,-1}},
//    {{-3,0,-2},{-1,0,-2},{0,20,-2},{1,0,-2},{3,0,-2}},
//    {{-3,0,-3},{-1,0,-3},{0,0,-3},{1,0,-3},{3,0,-3}},
//    {{-3,0,-4},{-1,0,-4},{0,0,-4},{1,0,-4},{3,0,-4}},
//    {{-3,0,-5},{-1,0,-5},{0,0,-5},{1,0,-5},{3,0,-5}} };

    //Nb Points de contrôle doit être supérieur ou égal au degré + 1
    //(sKnotCount - sOrder) x (tKnotCount -	tOrder)	control	points.
    int nbPtsCtrlX = 10;
    int nbPtsCtrlZ = 10;

    //Nb de noeuds doit être égal au degré + N - 1
    int nbNoeudsX = nbPtsCtrlX * 2;
    int nbNoeudsZ = nbPtsCtrlZ * 2;

    int degreX = nbPtsCtrlX;
    int degreZ = nbPtsCtrlZ;

    // Construction matrice points de contrôle
    GLfloat ctrlpoints[nbPtsCtrlX][nbPtsCtrlZ][3];

    GLfloat d = 0;
    for (int a = 0; a < nbPtsCtrlX; a++) {
        GLfloat c = 0;
        for (int b = 0; b < nbPtsCtrlZ; b++) {
            ctrlpoints[a][b][0] = c;
            ctrlpoints[a][b][1] = 0;
            ctrlpoints[a][b][2] = d;
            c++;
        }
        d++;
    }

    // influence parameter setting of each control point
    // Les valeurs des noeuds doivent alterner au minimum tout les degrés fois
    GLfloat knots1[nbNoeudsX]; // Control vector of NURBS surface
    {
        int k = 0;
        int j = 0;
        for(int h = 0; k < nbNoeudsX; h++) {
            for(int i = 0; i < degreX; i++) {
                j = 0;
                knots1[k] = h;
                k = k+1;
            }
            j=j+1;
        }
    }
//    {
//        for(int i = 0; i < nbNoeudsX; i++) {
//            knots1[i] = i;
//        }
//    }

    GLfloat knots2[nbNoeudsZ]; // Control vector of NURBS surface
    {
        int k = 0;
        int j = 0;
        for(int h = 0; k < nbNoeudsZ; h++) {
            for(int i = 0; i < degreZ; i++) {
                j = 0;
                knots2[k] = h;
                k = k+1;
            }
            j=j+1;
        }
    }
//    {
//        for(int i = 0; i < nbNoeudsZ; i++) {
//            knots2[i] = i;
//        }
//    }

    glColor3f( 1.0, 1.0, 1.0 );
    gluBeginSurface(theNurb); // Start surface drawing
    gluNurbsSurface(theNurb, nbNoeudsX, knots1, nbNoeudsZ, knots2, nbPtsCtrlX * 3, 3, & ctrlpoints [0] [0] [0], 10, 10, GL_MAP2_VERTEX_3); // Define the surface Mathematical model to determine its shape
    Form::render();
    gluEndSurface(theNurb); // End surface drawing

}
