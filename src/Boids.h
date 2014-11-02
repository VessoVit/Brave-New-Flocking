//
//  Boid.h
//
//  The class for each individual boid behaviour and the individual rules of the flock.
//
//  Defines Boid behaviour:
//  - application of the Flocking Rules,
//  - steering,
//  - limit speed,
//  - draw and update
//
//


#ifndef ofBoxExample_Boid_h
#define ofBoxExample_Boid_h


#include "ofMain.h"

// Different modes of behaviour
// [TODO]:
enum boidsMode{
    BOIDS_MODE_ATTRACT = 0,
    BOIDS_MODE_REPEL,
    BOIDS_MODE_NEAREST_POINTS,
    BOIDS_MODE_NOISE
};

class Boid {
public:
	Boid();
    
    //used for coloring the boids
    float boid_bright;
    float boid_sat;
	
	void update(vector<Boid*> boids);
	void draw();
	void seek(ofPoint target);
	void arrive(ofPoint target);
	ofPoint steer(ofPoint target, bool slowdown);
	
	void flock(vector<Boid*> boids);
	ofPoint separate(vector<Boid*> boids);
	ofPoint align(vector<Boid*> boids);
	ofPoint cohesion(vector<Boid*> boids);
	
	ofPoint loc;    //location
	ofPoint vel;    //velocity
	ofPoint acc;    //acceleration
	float r;        //radius
    
    float maxforce; //limit force
	float maxspeed; //limit speed
    
    void followMouse(ofPoint target);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseMoved();
    void mousePressed();
    void mouseDragged();
    //
    ofSpherePrimitive sphere;
    ofLight pointLight;

    //    ofPoint targetLoc;
    void setTarget(ofPoint target);

    boidsMode mode;



protected:
    bool bRegisteredEvents;
};

#endif