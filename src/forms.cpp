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
    Point org = anim.getPos();

    GLUquadric *quad;
    quad = gluNewQuadric();
    gluQuadricTexture(quad, 0);
    glColor3f(col.r, col.g, col.b);
    glTranslated(org.x, org.y, org.z);
    glRotated(getAnim().getPhi(), 1, 0, 0);
    glRotated(getAnim().getTheta(), 0, 1, 0);
    gluSphere(quad, radius, 10, 10);
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
//    // Complete this part
//    double currentPhi = anim.getPhi();
//    anim.setPhi(currentPhi + 1);
//
//
//    double currentTheta = anim.getTheta();
//    anim.setTheta(currentTheta + 1);
//
//    Point currentPos = anim.getPos();
//    currentPos.x += 0.001;
//    anim.setPos(currentPos);

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

Triangle::Triangle(Point p1, Point p2, Point p3, Color cl)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->col = cl;
}


void Triangle::update(double delta_t)
{
//    // Complete this part
//    double currentPhi = anim.getPhi();
//    anim.setPhi(currentPhi + 1);
//
//
//    double currentTheta = anim.getTheta();
//    anim.setTheta(currentTheta + 1);
//
//    Point currentPos = anim.getPos();
//    currentPos.x += 0.001;
//    anim.setPos(currentPos);

}


void Triangle::render()
{
    glColor3f(col.r, col.g, col.b);

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    }
    glEnd();
}

Surface::Surface(GLfloat *points, int nbPointsX, int nbPointsZ)
{
    //Allocate 3d array of correct size
    GLfloat *ctrlPoints = new GLfloat[nbPointsX*nbPointsZ*3];
    // Store pointer in object
    this->ctrlPoints = ctrlPoints;

    // Copying array
    std::copy(points, points + nbPointsX * nbPointsZ * 3, ctrlPoints);

    this->nbPointsX = nbPointsX;
    this->nbPointsZ = nbPointsZ;

    this->nbNoeudsX = nbPointsX * 2;
    this->nbNoeudsZ = nbPointsZ * 2;

    int degreX = nbPointsX;
    int degreZ = nbPointsZ;
//
//     influence parameter setting of each control point
//     Les valeurs des noeuds doivent alterner au minimum tout les degrés fois
    GLfloat *noeudsX = new GLfloat[nbNoeudsX];
    {
        int k = 0;
        int j = 0;
        for(int h = 0; k < nbNoeudsX; h++) {
            for(int i = 0; i < degreX; i++) {
                j = 0;
                noeudsX[k] = h;
                k = k+1;
            }
            j=j+1;
        }
    }
    this->NoeudsX = noeudsX;
    NoeudsX[nbNoeudsX] = 2;

    GLfloat *noeudsZ = new GLfloat[nbNoeudsZ];
    {
        int k = 0;
        int j = 0;
        for(int h = 0; k < nbNoeudsZ; h++) {
            for(int i = 0; i < degreZ; i++) {
                j = 0;
                noeudsZ[k] = h;
                k = k+1;
            }
            j=j+1;
        }
    }
    this->NoeudsZ = noeudsZ;

}


void Surface::update(double delta_t)
{
    double currentPhi = anim.getPhi();
    anim.setPhi(currentPhi + 1);


    double currentTheta = anim.getTheta();
    anim.setTheta(currentTheta + 1);

    Point currentPos = anim.getPos();
    currentPos.x += 0.001;
    anim.setPos(currentPos);
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

// Sampling fault tolerance tolerance


//
//    if (ctrlPoints[(3*6*3)+3*3+1] <= -10 || ctrlPoints[(3*6*3)+3*3+1] < 10) {
//        ctrlPoints[(3*6*3)+3*3+1] += 0.1;
//    }
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 2000);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

//    std::cout<<"Nb noeuds X : "<<nbNoeudsX<<"\n";
//    std::cout<<"Nb noeuds Z : "<<nbNoeudsZ<<"\n";
//    std::cout<<"Nb pts X : "<<nbPtsCtrlX<<"\n";
//    std::cout<<"Nb pts Z : "<<nbPtsCtrlZ<<"\n";

    glColor3f( 0.2, 0.7, 0.4 );
    gluBeginSurface(theNurb); // Start surface drawing
    gluNurbsSurface(theNurb, nbNoeudsX, NoeudsX, nbNoeudsZ, NoeudsZ, nbPointsX * 3, 3, ctrlPoints, nbPointsX, nbPointsZ, GL_MAP2_VERTEX_3); // Define the surface Mathematical model to determine its shape
    Form::render();
    gluEndSurface(theNurb); // End surface drawing

}

Maillage::Maillage(int nbPointsX, int nbPointsZ)
{
    this->nbPointsX = nbPointsX;
    this->nbPointsZ = nbPointsZ;

    initControlPoints();
    initSpheres();
    initTriFaces();
}

void Maillage::updateFormList(Form **form_list, unsigned short *number_of_forms) {
    for(int i = 0; i < this->spheres.size(); i++) {
        Sphere *pSphere = NULL;
        pSphere = &spheres[i];
        form_list[*number_of_forms]=pSphere;
        *number_of_forms = *number_of_forms+1;
    }
    for(int i = 0; i < this->triFaces.size(); i++) {
        Triangle *pTriangle = NULL;
        pTriangle = &triFaces[i];
        form_list[*number_of_forms]=pTriangle;
        *number_of_forms = *number_of_forms+1;
    }
}

void Maillage::initControlPoints() {
    //Flat plane of control points
    {
    for (int i = 0; i < nbPointsZ; i++) { // On itère les lignes
        for (int j = 0; j < nbPointsX; j++) { // On itère les valeurs des lignes
            Point monPoint = Point(j, 0, i);
            basePoints.push_back(monPoint);
        }
    }
    }

    pointsToRender = basePoints;

    // Null speed vectors
    {
    for (int i = 0; i < nbPointsZ; i++) { // On itère les lignes
        for (int j = 0; j < nbPointsX; j++) { // On itère les valeurs des lignes
            Vector monVecteur = Vector(0,0,0);
            speedVectors.push_back(monVecteur);
        }
    }
    }

    // Null acceleration vectors
    {
    for (int i = 0; i < nbPointsZ; i++) { // On itère les lignes
        for (int j = 0; j < nbPointsX; j++) { // On itère les valeurs des lignes
            Vector monVecteur = Vector(0,0,0);
            accelerationVectors.push_back(monVecteur);
        }
    }
    }
}

void Maillage::setPointsToRender(std::vector<Point> pointsToRender) {
    this->pointsToRender=pointsToRender;
    this->initSpheres();
    this->initTriFaces();
}

void Maillage::setSpeedVectors(std::vector<Vector> speedVectors) {
    this->speedVectors=speedVectors;
    this->initSpheres();
    this->initTriFaces();
}

void Maillage::setAccelerationVectors(std::vector<Vector> accelerationVectors) {
    this->accelerationVectors=accelerationVectors;
    this->initSpheres();
    this->initTriFaces();
}

void Maillage::initSpheres() {
    this->spheres.clear();

    //Creating a sphere for each control point
    for(int ligne = 0; ligne < nbPointsZ; ligne ++) { // On itère les lignes
        for(int colonne = 0; colonne < nbPointsX; colonne++) { // On itère les valeurs des lignes
            // Origine
            Point Origine = pointsToRender[ligne*nbPointsX + colonne];

            //Sphere
            Sphere sphere = Sphere(0.05, YELLOW);
            sphere.getAnim().setPos(Origine);

            this->spheres.push_back(sphere);
        }
    }
}

void Maillage::initTriFaces() {
    this->triFaces.clear();

    //Upper triFaces
    for(int ligne = 0; ligne < nbPointsZ; ligne ++) { // On itère les lignes
        for(int colonne = 0; colonne < nbPointsX; colonne++) { // On itère les valeurs des lignes
            if(colonne < nbPointsX-1 && ligne < nbPointsZ-1) { // On ne fait pas de surface à partir du bord droit et bas
                // Origine
                Point Origine = pointsToRender[ligne*nbPointsX + colonne];
                // Point X
                Point PointX1 = pointsToRender[ligne*nbPointsX + colonne+1];
                //Point Z
                Point PointZ1 = pointsToRender[(ligne+1)*nbPointsX + colonne];
                //Triangle face
                Triangle face = Triangle(Origine, PointX1, PointZ1, RED);

                this->triFaces.push_back(face);
            }
        }
    }

    //Lower triFaces
    for(int ligne = 0; ligne < nbPointsZ; ligne ++) { // On itère les lignes
        for(int colonne = 0; colonne < nbPointsX; colonne++) { // On itère les valeurs des lignes
            if(colonne < nbPointsX-1 && ligne != 0) { // On ne fait pas de surface à partir du bord haut et droit
                // Origine
                Point Origine = pointsToRender[ligne*nbPointsX + colonne];
                // Point X
                Point PointX1 = pointsToRender[ligne*nbPointsX + colonne+1];
                //Point Z
                Point PointZ1 = pointsToRender[(ligne-1)*nbPointsX + colonne+1];
                //Triangle face
                Triangle face = Triangle(Origine, PointX1, PointZ1, GREEN);

                this->triFaces.push_back(face);
            }
        }
    }
}

void Maillage::update(double delta_t)
{
    std::vector<Point> mesPoints = basePoints;

    //Moving wave origin
    for(int i = 0; i < waves.size(); i++) {
        Point origin = waves[i].getWaveOrigin();
        origin.translate(waves[i].getWaveSpeed()*delta_t);
        waves[i].setWaveOrigin(origin);

        //Deforming basePoints with each wave

        for(int j = 0; j < basePoints.size(); j++) {
            //Searching points in the radius
            GLfloat distanceToOrigin = pow(pow(basePoints[j].x-origin.x,2) + pow(basePoints[j].z-origin.z,2),0.5);

            if(distanceToOrigin <= waves[i].getWaveRadius()) {
                    mesPoints[j].y = - pow(
                                         (
                                                pow(basePoints[j].x-origin.x,2)
                                            +   pow(basePoints[j].z-origin.z,2)
                                          )
                        /   pow(waves[i].getWaveRadius()/waves[i].getWaveHeight(),2)
                                         ,0.5) + waves[i].getWaveHeight();
            }
        }
    }

    this->setPointsToRender(mesPoints);

}

void Maillage::render()
{
    for(int i = 0; i < this->spheres.size(); i++) {
        this->spheres[i].render();
    }
    for(int i = 0; i < this->triFaces.size(); i++) {
        this->triFaces[i].render();
    }
}

void Maillage::addWave(Wave myWave)
{
    waves.push_back(myWave);
}


Wave::Wave(Point waveOrigin, GLfloat waveHeight, GLfloat waveRadius, Vector waveSpeed, Vector waveAcceleration) {
    this->waveOrigin = waveOrigin;
    this->waveHeight = waveHeight;
    this->waveRadius = waveRadius;
    this->waveSpeed = waveSpeed;
    this->waveAcceleration = waveAcceleration;
}
