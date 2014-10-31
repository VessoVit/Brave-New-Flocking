#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"

//Custom classes
#include "Boids.h"
#include "Flocking.h"
//#include "Ctrl.h"


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
		
	ofxLeapMotion leap;
    
// Settings
    bool fullscreen = true;

//  Custom Methods
    
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
    ofEasyCam cam;
    ofNode lookat;

    
//  #LeapMotion Controller
    
//Hands variables
    vector <ofxLeapMotionSimpleHand> simpleHands;
    vector <int> fingersFound;
    vector <ofVect3f> fingerPos;
    vector <ofVect3f> spherePos;
    vector <float> SphereSize;
    
    map <int, ofPolyline> fingerTrails;
    
//Flocking
    Boid boid;
    vector<Boid*> boids;
    Flocking flock;
};
