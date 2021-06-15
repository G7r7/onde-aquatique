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

    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 5.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

// ************************ Called in the drawing function ********** **************** /
    GLfloat ctrlpoints[6][5][3] = {
    {{-3,0,0}, {-1,1,0}, {0,0,0}, {1,-1,0}, {3,0,0}},
    {{-3,0,-1},{-1,1,-1},{0,0,-1},{1,-1,-1},{3,0,-1}},
    {{-3,0,-3},{-1,1,-3},{0,0,-3},{1,1,-3},{3,0,-3}},
    {{-3,1,-3},{-1,1,-3},{0,0,-3},{1,-1,-3},{3,0,-3}},
    {{-3,0,-4},{-1,1,-4},{0,0,-4},{1,-1,-4},{3,0,-4}},
    {{-3,2,-5},{-1,1,-5},{0,0,-5},{1,-1,-5},{3,1,-5}} };

// influence parameter setting of each control point
    GLfloat knots1[12] = { 0.0, 0.0, 0.0, 0.0,0.0,0.0,
1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // Control vector of NURBS surface
    GLfloat knots2[10] = { 0.0, 0.0, 0.0, 0.0,0.0,
1.0, 1.0, 1.0, 1.0, 1.0}; // Control vector of NURBS surface

    gluBeginSurface(theNurb); // Start surface drawing
    gluNurbsSurface(theNurb, 12, knots1, 10, knots2, 5 * 3, 3, & ctrlpoints [0] [0] [0], 6, 5, GL_MAP2_VERTEX_3); // Define the surface Mathematical model to determine its shape
    Form::render();
    gluEndSurface(theNurb); // End surface drawing

}
