//
//  Flocking.cpp
//
//  Applies functions to all the boids
//  The data structure for the flock.
//


#include <iostream>
#include "Flocking.h"

Flocking::Flocking() {
    
}

void Flocking::update() {
	for(int i=0; i<boids.size(); i++) {
		boids[i]->update(boids);
	}    
}

void Flocking::draw() {
	for(int i=0; i<boids.size(); i++) {
		boids[i]->draw();
	}
}

void Flocking::addBoid() {
	boids.push_back( new Boid() );
}


void Flocking::followMouse(ofPoint mouse) {
    for (int i=0; i<boids.size();i++) {
        boids[i]-> followMouse(mouse);
    }
}