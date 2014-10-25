//
//  Boid.cpp
//
//  Defines Boid behaviour:
//  - application of the Flocking Rules,
//  - steering,
//  - limit speed,
//  - draw and update
//
//


#include <iostream>
#include "Boids.h"


float ofDist(float x1, float y1, float z1, float x2, float y2, float z2) {
	return sqrt(double(( x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2))); //rick
}

Boid::Boid() {
    
    loc.x = ofRandom(-ofGetWidth(), ofGetWidth());
	loc.y = ofRandom(-ofGetHeight(), ofGetHeight());
	loc.z = ofRandom(-ofGetHeight(), ofGetHeight());
    
	vel.set(0);
	acc.set(0);
    r = 5.0;
    
    maxspeed = 4;
    maxforce = 0.1;
    
    //Event
//    if(!bRegisteredEvents){
//        ofRegisterMouseEvents(this);
//        bRegisteredEvents = true;
//    }
}

//----------------------------------------------
// Method to update location
void Boid::update(vector<Boid*> boids) {
	
	flock(boids);
    
    vel += acc;   // Update velocity
    vel.x = ofClamp(vel.x, -maxspeed, maxspeed);  // Limit speed
	vel.y = ofClamp(vel.y, -maxspeed, maxspeed);  // Limit speed
    vel.z = ofClamp(vel.z, -maxspeed, maxspeed);
    
    loc += vel;
    acc.set(0);  // Reset accelertion to 0 each cycle
	
	if (loc.x < -ofGetWidth()*2-r) loc.x = ofGetWidth()+r;
    if (loc.y < -ofGetHeight()*2-r) loc.y = ofGetHeight()+r;
    if (loc.z < -ofGetWidth()*2-r) loc.z = ofGetHeight()+r; //rick
    
    if (loc.x > ofGetWidth()*2+r) loc.x = -r;
    if (loc.y > ofGetHeight()*2+r) loc.y = -r;
    if (loc.z > ofGetHeight()*2+r) loc.z = -r; //rick
    
}
//----------------------------------------------
//
//----------------------------------------------

void Boid::setTarget(ofPoint target){
    //[TODO]
    acc += steer(target, false);
}

void Boid::seek(ofPoint target) {
    acc += steer(target, false);
}

void Boid::arrive(ofPoint target) {
    acc += steer(target, true);
}


//----------------------------------------------
// A method that calculates a steering vector towards a target
// Takes a second argument, if true, it slows down as it approaches the target
ofPoint Boid::steer(ofPoint target, bool slowdown) {
    ofPoint steer;  // The steering vector
    ofPoint desired = target - loc;  // A vector pointing from the location to the target
    
	float d = ofDist(target.x, target.y, target.z, loc.x, loc.y, loc.z); // Distance from the target is the magnitude of the vector
	
	
	// If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0) {
		
		desired /= d; // Normalize desired
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if ((slowdown) && (d < 100.0f)) {
			desired *= maxspeed * (d/100.0f); // This damping is somewhat arbitrary
		} else {
			desired *= maxspeed;
		}
		// Steering = Desired minus Velocity
		steer = desired - vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce); // Limit to maximum steering force
		steer.y = ofClamp(steer.y, -maxforce, maxforce);
		steer.z = ofClamp(steer.z, -maxforce, maxforce);  //rick
		
    }
    return steer;
}


//----------------------------------------------
void Boid::draw() {
    // Draw a triangle rotated in the direction of velocity
//	float angle = (float)atan2(-vel.y, vel.x);
//    float theta =  -1.0*angle;
//	float heading2D = ofRadToDeg(theta)+90;
	
	//ofEnableAlphaBlending();
//    ofSetColor(255);
//	ofPushStyle();
//    ofFill();
//    
//    ofSetColor(50,boid_bright, boid_sat, 5);
//    
//    boid_bright = ofMap(loc.x,0,ofGetWidth(), 200,0);
//    boid_sat = ofMap(loc.y,0,ofGetHeight(), 200,0);
    
//----------------------------------------------
//      Target (TEST)
//----------------------------------------------
    
//    ofPoint target(ofGetWidth()/2, ofGetHeight()/2, ofGetHeight()/2);
    ofPoint target(ofRandomWidth(), ofRandomHeight(), ofRandomHeight());
    acc += steer(target, false);
    
    //3D
    glPushMatrix();
    glTranslated(loc.x, loc.y, loc.z);
    
    vel.normalize();
//    ofVec3f forward(0,0,1);
//    ofVec3f axis = forward.crossed(vel);
//    float theta = acosf(forward.dot(vel));
//    
//    float heading3D = ofRadToDeg(theta);
//    glRotatef(heading3D, axis.x, axis.y, axis.z);
    
//    ofEnableLighting();
//    pointLight.enable();
    
    ofColor(255);
    sphere.setRadius(30);
    ofSetSphereResolution(24);
    
    ofDrawSphere(100,100, r);
    sphere.setScale(1.f);
    
    //2D
//    ofPushMatrix();
//    ofTranslate(loc.x, loc.y, loc.z);
//    ofRotateZ(heading2D);
//    ofBeginShape();
//    ofVertex(0, -r*2);
//    ofVertex(-r, r*2);
//    ofVertex(r, r*2);
//    ofEndShape(true);
//    ofPopMatrix();
    
    
//	ofPopStyle();
	//ofDisableAlphaBlending();
    
}


//----------------------------------------------
void Boid::flock(vector<Boid*> boids) {
	ofPoint sep = separate(boids);
	ofPoint ali = align(boids);
	ofPoint coh = cohesion(boids);
	
	// Arbitrarily weight these forces
	sep *= 1.5;
	ali *= 1.0;
	coh *= 1.0;
	
	acc += sep + ali + coh;
	
}


//----------------------------------------------
// Separation
// Method checks for nearby boids and steers away
ofPoint Boid::separate(vector<Boid*> boids) {
    float desiredseparation = 100.0f;
    ofPoint steer;
    int count = 0;
	
    // For every boid in the system, check if it's too close
    for (int i = 0 ; i < boids.size(); i++) {
		Boid* other = boids[i];
		
		float d = ofDist(loc.x, loc.y, loc.z, other->loc.x, other->loc.y, other->loc.z);
		
		
		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if ((d > 0) && (d < desiredseparation)) {
			// Calculate vector pointing away from neighbor
			ofPoint diff = loc - other->loc;
			diff /= d;			// normalize
			diff /= d;        // Weight by distance
			steer += diff;
			count++;            // Keep track of how many
		}
    }
    // Average -- divide by how many
    if (count > 0) {
		steer /= (float)count;
    }
	
	
    // As long as the vector is greater than 0
	//float mag = sqrt(steer.x*steer.x + steer.y*steer.y);
	
	float mag = sqrt(steer.x*steer.x + steer.y*steer.y + steer.z*steer.z); //rick
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce);
		steer.y = ofClamp(steer.y, -maxforce, maxforce);
		steer.z = ofClamp(steer.z, -maxforce, maxforce); //rick
    }
    return steer;
}


//----------------------------------------------
// Alignment
// For every nearby boid in the system, calculate the average velocity
ofPoint Boid::align(vector<Boid*> boids) {
    float neighbordist = 100.0f;
    ofPoint steer;
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid* other = boids[i];
		
		float d = ofDist(loc.x, loc.y, loc.z, other->loc.x, other->loc.y, other->loc.z);
		if ((d > 0) && (d < neighbordist)) {
			steer += (other->vel);
			count++;
		}
    }
    if (count > 0) {
		steer /= (float)count;
    }
	
    // As long as the vector is greater than 0
	float mag = sqrt(steer.x*steer.x + steer.y*steer.y + steer.z*steer.z);
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce);
		steer.y = ofClamp(steer.y, -maxforce, maxforce);
		steer.z = ofClamp(steer.z, -maxforce, maxforce);
        
    }
    return steer;
}


//----------------------------------------------
// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
ofPoint Boid::cohesion(vector<Boid*> boids) {
    float neighbordist = 100.0f;
    ofPoint sum;   // Start with empty vector to accumulate all locations
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid* other = boids[i];
		float d = ofDist(loc.x, loc.y, loc.z, other->loc.x, other->loc.y, other->loc.z);
		if ((d > 0) && (d < neighbordist)) {
			sum += other->loc; // Add location
			count++;
		}
    }
    if (count > 0) {
		sum /= (float)count;
		return steer(sum, false);  // Steer towards the location
    }
    return sum;
}

//----------------------------------------------
//void Boid::followMouse(ofPoint target) {
//    acc += steer(target, false);
//}

//void Boid::setTarget(ofPoint target){
//    targetLoc = 
//    acc += steer(target, false);
//}
//--------------------------------------------------------------
//          CLICKABLE
//--------------------------------------------------------------

//void Boid::mouseReleased(ofMouseEventArgs & args){
//    // if the mouse is pressed over the circle an event will be notified (broadcasted)
//    // the circleEvent object will contain the mouse position, so this values are accesible to any class that is listening.
//    ofVec2f mousePos = ofVec2f(args.x, args.y);
//    ofNotifyEvent(clickableEvent, mousePos, this);
//}
//----------------------------------------------
