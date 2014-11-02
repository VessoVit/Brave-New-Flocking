#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"

//Custom classes
#include "Boids.h"
#include "Flocking.h"

#define kNumTestNodes 4
#define kNumCameras 2
#define kNumLights 3

class testApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
//  Custom Methods
    void resetCam();
	ofxLeapMotion leap;
    
// Settings
    bool fullscreen = true;

    
//  Variables
    ofPoint acc;
    ofImage texture;

    
//  3D
    ofSpherePrimitive sphere;
    ofLight pointLight;
    ofMaterial material;
    
// Views
    float eyeX, eyeY, eyeZ;
    float centerX, centerY;
    bool setCenter = false;
    
// Camera
//  1. EasyCam
    ofEasyCam cam;
    ofNode lookat;
//  2. ofCamera
    ofNode testNodes[kNumTestNodes];
    ofLight light[kNumLights];
    ofCamera camMain[kNumCameras];
//  which test node to lookat
//  one for each camera
    int lookatIndex[kNumCameras];
// which test node to link parent to
    int parentIndex[kNumCameras];
// whether camera is in mouseOrbit mode or not
    bool doMouseOrbit[kNumCameras];
    float orbitRadius;
//  which camera index are we looking through
    int camToView;
//  which camera index we are configuring
    int camToConfig;
    
//  #LeapMotion Controller

//Hands variables
    vector <ofxLeapMotionSimpleHand> simpleHands;
    vector <int> fingersFound;
    
    map <int, ofPolyline> fingerTrails;
    
//Flocking
    Boid boid;
    vector<Boid*> boids;
    Flocking flock;
};
