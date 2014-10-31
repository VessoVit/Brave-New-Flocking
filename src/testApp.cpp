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
    
//  setup camera (look at)
//    cam.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, -650));
//    lookat.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0));
//    cam.disableMouseInput();
    cam.setOrientation(ofPoint(-20, 0, 0));
    cam.setDistance(5);

// [TODO]: EVENTS:
// --------- Listeners ------
//    ofAddListener(boid.clickableEvent, this, &ofApp::onEventClicked);

// we enable our gesture detection here
	leap.open();
	leap.setupGestures();
}


//--------------------------------------------------------------
void testApp::update()
{
    flock.update();
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
//update Camera
    cam.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, -650));
    lookat.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0));
    cam.lookAt(lookat);
//update leap hand
    fingersFound.clear();
    
//## Leap Motion Setup
//    -------------------------
    vector <Hand> hands = leap.getLeapHands();
    if(leap.isFrameNew() && hands.size() ){
        //return the data in mm and mapping it to our world space.
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
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

    cam.begin();
    
        ofEnableDepthTest();
        ofEnableLighting();
        pointLight.enable();
    
        ofColor(180);
        ofNoFill();

        ofDrawSphere(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth()*2);
//  Call the flocking
        flock.draw();

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

    cam.end();
    
//    ### Debug Screen Output
    string distanceString;
    distanceString = "Distance: "+ofToString(cam.getDistance());
    ofDrawBitmapString(distanceString, 20, 40);
    
   
    
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
    
    switch (leap.iGestures) {
        case 1:
            msg = "Screen Tap";
            msg += "\nPosition: "+ofToString(leap.screenTapPosition);
            break;
            
        case 2:
            msg = "Key Tap";
            msg += "\nPosition: " + ofToString(leap.keyTapPosition);
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
            break;
            
        case 10:
            msg = "Circle Right";
            msg += "\nCircle Radius: " + ofToString(leap.circleRadius) + "\nCircle Center: "+ofToString(leap.circleCenter);
            break;
            
        default:
            msg = "Waiting for hand movement...";
            break;
    }
//  let's draw the debug message to the screen
    ofDrawBitmapString(msg, 20, 20);
}

//  --------------------------------------------------------------
void testApp::keyPressed(int key){
    if( key == 'f' ) {
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
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
