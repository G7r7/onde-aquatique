#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"
#include <vector>

class Color
{
public:
    float r, g, b;
    Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) {r=rr; g=gg; b=bb;}
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);
const Color ORANGE(1.0f, 0.65f, 0.0f);
const Color DODGERBLUE(0.11, 0.56, 1.0);


// Generic class to render and animate an object
class Form
{
protected:
    Color col;
    Animation anim;
public:
    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}
    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(double delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();
};


// A particular Form
class Sphere : public Form
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
public:
    Sphere(double r = 1.0, Color cl = Color());
    double getRadius() const {return radius;}
    void setRadius(double r) {radius = r;}
    void update(double delta_t);
    void render();
};


// A face of a cube
class Cube_face : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
public:
    Cube_face(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double l = 1.0, double w = 1.0,
          Color cl = Color());
    void update(double delta_t);
    void render();
};

// A face of a cube
class Triangle : public Form
{
private:
    Point p1;
    Point p2;
    Point p3;
public:
    Triangle(Point p1, Point p2, Point p3 ,Color cl = Color());
    void update(double delta_t);
    void render();
};

class Surface : public Form
{
private:
    GLfloat *ctrlPoints;
    int nbPointsX;
    int nbPointsZ;
    GLfloat *NoeudsX;
    int nbNoeudsX;
    GLfloat *NoeudsZ;
    int nbNoeudsZ;
public:
    Surface(GLfloat *ctrlPoints, int nbPointsX, int nbPointsZ);
    void update(double delta_t);
    void render();
};

class Wave
{
protected:
    Point waveOrigin;
public:
    //Wave(Point waveOrigin);

    Point getWaveOrigin() {return waveOrigin;}
    void setWaveOrigin(Point p) {waveOrigin = p;}
    virtual std::vector<Point> deformGrid(std::vector<Point>) = 0;
    virtual void updateWave(double delta_t, int nbPointsX, int nbPointsZ) = 0;
};


class ConicWave : public Wave
{
private:
    GLfloat waveHeight;
    GLfloat waveRadius;
    Vector waveSpeed;
    Vector waveAcceleration;
public:
    ConicWave(Point waveOrigin, GLfloat waveHeight, GLfloat waveRadius, Vector waveSpeed, Vector waveAcceleration);
    GLfloat getWaveHeight() {return waveHeight;}
    GLfloat getWaveRadius() {return waveRadius;}
    Vector getWaveSpeed() {return waveSpeed;}
    Vector getWaveAcceleration() {return waveAcceleration;}
    void setWaveHeight(GLfloat h) {waveHeight = h;}
    void setWaveRadius(GLfloat r) {waveRadius = r;}
    void setWaveSpeed(Vector v) {waveSpeed = v;}
    void setWaveAcceleration(Vector v) {waveSpeed = v;}
    std::vector<Point> deformGrid(std::vector<Point>);
    void updateWave(double delta_t, int nbPointsX, int nbPointsZ);
};

class CircularWave : public Wave
{
private:
    GLfloat waveHeight;
    GLfloat waveRadius;
    GLfloat waveWidth;
    GLfloat waveSpeed;
    GLfloat waveAcceleration;
public:
    CircularWave(Point waveOrigin, GLfloat waveHeight, GLfloat waveWidth, GLfloat waveRadius, GLfloat waveSpeed, GLfloat waveAcceleration);
    GLfloat getWaveHeight() {return waveHeight;}
    GLfloat getWaveWidth() {return waveWidth;}
    GLfloat getWaveRadius() {return waveRadius;}
    GLfloat getWaveSpeed() {return waveSpeed;}
    GLfloat getWaveAcceleration() {return waveAcceleration;}
    void setWaveOrigin(Point p) {waveOrigin = p;}
    void setWaveHeight(GLfloat h) {waveHeight = h;}
    void setWaveRadius(GLfloat r) {waveRadius = r;}
    void setWaveWidth(GLfloat w) {waveWidth = w;}
    void setWaveSpeed(GLfloat v) {waveSpeed = v;}
    void setWaveAcceleration(GLfloat a) {waveAcceleration = a;}
    std::vector<Point> deformGrid(std::vector<Point>);
    void updateWave(double delta_t, int nbPointsX, int nbPointsZ);
};

class Maillage : public Form
{
private:
    std::vector<Wave*> waves;
    std::vector<Point> pointsToRender;
    std::vector<Point> basePoints;
    std::vector<Vector> speedVectors;
    std::vector<Vector> accelerationVectors;
    int nbPointsX;
    int nbPointsZ;
    std::vector<Sphere> spheres;
    std::vector<Triangle> triFaces;
    bool colorType;
public:
    Maillage(int nbPointsX, int nbPointsZ);
    int getNbPointsX() {return nbPointsX;};
    int getNbPointsZ() {return nbPointsZ;};
    void initControlPoints();
    void initSpheres();
    void initTriFaces();
    std::vector<Point> getControlPoints() {return pointsToRender;};
    std::vector<Vector> getSpeedVectors() {return speedVectors;};
    std::vector<Vector> getAccelerationVectors() {return accelerationVectors;};
    void setPointsToRender(std::vector<Point> pointsToRender);
    void setSpeedVectors(std::vector<Vector> speedVectors);
    void setAccelerationVectors(std::vector<Vector> AccelerationVectors);
    void addWave(Wave *myWave);
    void updateFormList(Form **form_list, unsigned short *number_of_forms);
    void update(double delta_t);
    void render();
    void setColorType ( bool choice) {colorType = choice;};
};


#endif // FORMS_H_INCLUDED
