//
//  Flocking.h
//
//  Applies functions to all the boids
//
//

#ifndef FLOCKING_H
#define FLOCKING_H

#include "ofMain.h"
#include "Boids.h"


class Flocking {
public:
	Flocking();
	void update();
	void draw();
	void addBoid();
	
    void followMouse(ofPoint mouse);
	vector<Boid*> boids;
    
};

#endif