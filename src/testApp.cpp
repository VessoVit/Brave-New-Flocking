#include "testApp.h"


// # Brave New Flocking
//
//  --------------------------------------------------------------
void testApp::setup(){
    
    ofSetLogLevel(OF_LOG_WARNING);
    ofBackground(20);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    float width = ofGetWidth() * .12;
    
    ofSpherePrimitive galaxy;
    galaxy.setRadius(width);
    galaxy.setPosition(ofGetHeight()/2, ofGetHeight()/2, 0);
    
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .85) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f) );
    
    ofSetSphereResolution(24);
    for(int i = 0; i < 50; i++){
        flock.addBoid();
    }
    
//    ### EasyCam Setup
//  setup camera (look at)
//    cam.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, -650));
//    lookat.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0));
//    cam.disableMouseInput();
//    cam.setOrientation(ofPoint(-20, 0, 0));
//    cam.setDistance(5);
//    -----------------------
    
// [TODO]: EVENTS:
// --------- Listeners ------
//    ofAddListener(boid.clickableEvent, this, &ofApp::onEventClicked);

//    ### ofCamera

    ofEnableDepthTest();
    ofSetVerticalSync(true);
    ofEnableLighting();
    
    for(int i = 0; i < kNumLights; i++){
        light[i].enable();
    }
    
    resetCam();
    
    for (int i = 0; i < kNumTestNodes; i++) {
        if(i>0) testNodes[i].setParent(testNodes[i-1]);
    }
//    -----------------------
//    ### LEAP MOTION
// we enable our gesture detection here
	leap.open();
	leap.setupGestures();
}


//--------------------------------------------------------------
void testApp::update()
{
    flock.update();
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
//update easyCam
//    cam.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, -650));
//    lookat.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0));
//    cam.lookAt(lookat);
    
//    ### ofCamera
//(ofGetWidth()/2, ofGetHeight()/2, 0
//    testNodes[i].setPosition
//    for (int i = 0; i < kNumTestNodes; i++) {
        testNodes[0].setPosition(ofVec3f(ofGetWidth()/2,ofGetHeight()/2, 250));
        testNodes[0].setOrientation(ofVec3f(ofGetWidth(),ofGetHeight(), 650));
        testNodes[0].setScale(1);
//        Second Node
        testNodes[1].setPosition(ofVec3f(ofGetWidth(),ofGetHeight()*4, -650));
        testNodes[1].setOrientation(ofVec3f(ofGetWidth(),ofGetHeight(), 650));
        testNodes[1].setScale(1);
//  Third Node
        testNodes[2].setPosition(ofVec3f(ofGetWidth(),ofGetHeight()*2, -650));
        testNodes[2].setOrientation(ofVec3f(ofGetWidth(),ofGetHeight(), 0));
        testNodes[2].setScale(1);
//    Last Node
        testNodes[3].setPosition(ofVec3f(ofGetWidth()*2,ofGetHeight()*2, 0));
        testNodes[3].setOrientation(ofVec3f(ofGetWidth(),ofGetHeight(), 650));
        testNodes[3].setScale(1);
//    }
    
//    testNodes[0].setPosition(ofVec3f(ofGetWidth()*2), ofGetHeight()*2, 650));
//    testNodes[0].setOrientation();
//    testNodes[0].setScale();
    //
//    
//update leap hand
    fingersFound.clear();
    
//## Leap Motion Setup
//    -------------------------
    vector <Hand> hands = leap.getLeapHands();
    if(leap.isFrameNew() && hands.size() ){
        //return the data in mm and mapping it to our world space.
        leap.setMappingX(-230, 230, ofGetWidth()/2, ofGetWidth());
        leap.setMappingY(90, 490, ofGetHeight()/2, ofGetHeight());
        leap.setMappingZ(-150, 150, 0, 650);
        
        for (int i = 0; i < hands.size(); i++) {
            for (int j = 0; j < hands[i].fingers().count(); j++) {
                ofPoint pt;
                const Finger & finger = hands[i].fingers()[j];
                pt = leap.getMappedofPoint(finger.tipPosition());
                ofPolyline & polyline = fingerTrails[finger.id()];
                
                if (polyline.size() && (pt-polyline[polyline.size()-1]).length() > 50) {
                    polyline.clear();
                }
                polyline.addVertex(pt);
                fingersFound.push_back(finger.id());
            }
        }
    }
// check for Leap Gestures updates
	leap.updateGestures();
//  IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    for (int i = 0; i < kNumCameras; i++) {
        if (lookatIndex[i] >= 0) camMain[i].lookAt(testNodes[lookatIndex[i]]);
    }

    camMain[camToView].begin();
        ofDrawAxis(100);
        ofEnableDepthTest();
        ofEnableLighting();
        pointLight.enable();
    
        ofColor(180);
        ofNoFill();

        ofDrawSphere(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth()*2);
//  Call the flocking
        flock.draw();
    
    

// draw testNodes
    for(int i=0; i<kNumTestNodes; i++) {
        ofSetColor(255, 128, 255);
        testNodes[i].draw();
    }
    
// draw cameras
    for(int i=0; i<kNumCameras; i++) {
        ofSetColor(255, 255, 0);
        camMain[i].draw();
        
        // draw line from cam to its lookat
        if(lookatIndex[i] >= 0) {
            ofSetColor(0, 255, 255);
            ofVec3f v1 = camMain[i].getGlobalPosition();
            ofVec3f v2 = testNodes[lookatIndex[i]].getGlobalPosition();
            ofLine(v1,v2);
        }
        
        // draw line from cam to its parent
        if(parentIndex[i] >= 0) {
            ofSetColor(255, 255, 0);
            ofVec3f v1 = camMain[i].getGlobalPosition();
            ofVec3f v2 = testNodes[parentIndex[i]].getGlobalPosition();
            ofLine(v1,v2);
        }
    }
    
// Centrel object Sphere
        ofSpherePrimitive deathStar;
        deathStar.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
//    deathStar.rotate(spinX, 1.0, 0.0, 0.0);
//    deathStar.rotate(spinY, 0, 1.0, 0.0);
        deathStar.setScale(1.01f);
        deathStar.drawWireframe();
        deathStar.setScale(1.f);

        ofDisableDepthTest();
   
//    for (int i = 0; i < fingersFound.size(); i++) {
//      int id = fingersFound[i];
//      ofPolyline & polyline = fingerTrails[id];
//    }
//    ### Leap Hands draw debug
        for(int i = 0; i < simpleHands.size(); i++){
            simpleHands[i].debugDraw();
        }
//    Add finger/hands trails to a single vector for drawing
//      for(int i = 0; i < fingersFound.size(); i++){
//          int id = fingersFound[i];
//          ofPolyline & polyline = fingerTrails[id];
//          // Do something with it
//      }

    camMain[camToView].end();
    for(int i = 0; i < simpleHands.size(); i++){
        simpleHands[i].debugDraw();
    }
//    ### Debug Screen Output
//    string distanceString;
//    distanceString = "Distance: "+ofToString(cam.getDistance());
//    ofDrawBitmapString(distanceString, 20, 40);
//    
   
    
//  ###Leap iGesture Key
//  --------------------------------
//  1.  = Screen Tap
//  2.  = Key Tap
//  3.  = Swipe Right
//  4.  = Swipe Left
//  5.  = Swipe Down
//  6.  = Swipe Up
//  7.  = Swipe Forward
//  8.  = Swipe Backward (towards yourself)
//  9.  = Circle Left (counter-clockwise)
//  10. = Circle Right (clockwise)
// --------------------------------
    
    string msg;
    ofNode *n = &camMain[camToConfig];

    switch (leap.iGestures) {
        case 1:
            msg = "Screen Tap";
            msg += "\nPosition: "+ofToString(leap.screenTapPosition);
            break;
            
        case 2:
            msg = "Key Tap";
            msg += "\nPosition: " + ofToString(leap.keyTapPosition);

            boid.setTarget(leap.keyTapPosition);
            deathStar.setPosition(leap.keyTapPosition);
            break;
            
        case 3:
            msg = "Swipe Right";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 4:
            msg = "Swipe Left";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            cam.pan(leap.swipeSpeed*100);
            break;
            
        case 5:
            msg = "Swipe Down";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 6:
            msg = "Swipe Up";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 7:
            msg = "Swipe Forward";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 8:
            msg = "Swipe Backwards";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 9:
            msg = "Circle Left";
            msg += "\nCircle Radius: " + ofToString(leap.circleRadius) + "\nCircle Center: "+ofToString(leap.circleCenter);

//            if(ofGetElapsedTimeMillis() - startedGesture >= 5000)
//            {
//                lookatIndex[camToConfig]--;
//                if(lookatIndex[camToConfig]<=-1){
//                    lookatIndex[camToConfig] = 3;
//                }
//            }
//
            
            break;
            
        case 10:
            msg = "Circle Right";
            msg += "\nCircle Radius: " + ofToString(leap.circleRadius) + "\nCircle Center: "+ofToString(leap.circleCenter);
            
            if (ofGetFrameNum()%60 == 0) {
                lookatIndex[camToConfig] = lookatIndex[camToConfig]+1;
                if(lookatIndex[camToConfig]>=kNumTestNodes){
                    lookatIndex[camToConfig] = -1;
                }
            }
            break;
            
        default:
            msg = "Waiting for hand movement...";
            break;
    }
//  let's draw the debug message to the screen
    ofDrawBitmapString(msg, 20, 20);
}

//  --------------------------------------------------------------

void testApp::resetCam(){
    camToView = 0;
    camToConfig = 1;
    
    orbitRadius = 200;
    
    for(int i = 0; i < kNumCameras; i++){
        camMain[i].resetTransform();
        camMain[i].setFov(60);
        camMain[i].clearParent();
//        Don't look at any node
        lookatIndex[i] = -1;
//        Don't parent any node
        parentIndex[i] = -1;
        doMouseOrbit[i] = false;
    }
    
    camMain[0].setPosition(40,40,190);
    doMouseOrbit[0] = true;
    
    camMain[1].setPosition(80, 40, 30);
//    Look at smallest node
    lookatIndex[1] = kNumTestNodes-1;
}

//  --------------------------------------------------------------
void testApp::keyPressed(int key){
    ofNode *n = &camMain[camToConfig];
    switch (key) {
        case 'f':
            fullscreen = !fullscreen;
            ofSetFullscreen(fullscreen);
            break;
        case 't':
            lookatIndex[camToConfig]++;
            if(lookatIndex[camToConfig]>=kNumTestNodes){
                lookatIndex[camToConfig] = -1;
            }
            break;
        case 'v':
            camToView = 1 - camToView;
            break;
        case 'r':
            resetCam();
            break;
        default:
            break;
    }
}

//  --------------------------------------------------------------
void testApp::keyReleased(int key){
}

//  --------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//  --------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//  --------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}
//  --------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//  --------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//  --------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
}

//  --------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
}

//  --------------------------------------------------------------
void testApp::exit(){
// let's close down Leap and kill the controller
    leap.close();
}
