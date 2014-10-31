#include "testApp.h"


// ###Leap Note:
// Note on OS X, you must have this in the Run Script Build Phase of your project.
// where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon.

//      cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";

//   If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib

//  --------------------------------------------------------------
void testApp::setup(){
    
    ofSetLogLevel(OF_LOG_WARNING);
    ofBackground(20);
    
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

//ctrl.init();
    
    ofSetFrameRate(60);
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
    
//# Leap Motion Cntrl ver.1
//    -------------------------
    //## simple access to the hand
//    simpleHands = leap.getSimpleHands();
//    
//    if(leap.isFrameNew() && simpleHands.size() ){
//        //Map the view in which the hand can operate
//        //min and max of the viewpoint and mapped values on the screen
//        leap.setMappingX(-500, 500, -ofGetWidth(), ofGetWidth());
//        leap.setMappingY(90, 490, -ofGetHeight(), ofGetHeight());
//        leap.setMappingZ(-150, 150, -150, 650);
//        
//        for(int i = 0; i < simpleHands.size(); i++){
//            for(int j = 0; j < simpleHands[i].fingers.size(); j++){
//                //Keep an id for all the fingers of each hand in the scene
//                int id = simpleHands[i].fingers[j].id;
//                //Combine the multiple into a single vector data object
//                ofPolyline & polyline = fingerTrails[id];
//                ofPoint pt = simpleHands[i].fingers[j].pos;
//                
//                //if the distance between the last point and the current is too big- lets clear the line
//                //this stops us connecting to an old drawing
//                if(polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50){
//                    polyline.clear();
//                }
//                //add our point to our trail
//                polyline.addVertex(pt);
//                //store fingers seen this frame for drawing
//                fingersFound.push_back(id);
//            }
//        }
//    }
//# Leap Motion Cntrl ver.2
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
    
//    texture.loadImage("bnf-background.jpg");
//    texture.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
//    ofBackground(55);
//    glMatrixMode(GL_TEXTURE);
//    glPushMatrix();
//    ofScale(2048, 1024);
//    glMatrixMode(GL_MODELVIEW);
//    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    

    cam.begin();
    
    ofDrawSphere(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth()*2);

//    texture.getTextureReference().bind();
    
//    cam.setDistance(10);
//    ofRotateX(ofRadToDeg(.5));
//    ofRotateY(ofRadToDeg(-.5));

//    float spinX = sin(ofGetElapsedTimef()*.35f);
//    float spinY = cos(ofGetElapsedTimef()*.075f);

    ofEnableDepthTest();

    flock.draw();
    ofEnableLighting();
    pointLight.enable();

    ofColor(180);
    ofNoFill();


// Centrel object Sphere
    ofSpherePrimitive deathStar;
//    sphere.setPosition(ofGetWidth()*.8f, ofGetHeight()*.25, 0);
    deathStar.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
//    deathStar.rotate(spinX, 1.0, 0.0, 0.0);
//    deathStar.rotate(spinY, 0, 1.0, 0.0);

// set Sphere.Wireframe
    deathStar.setScale(1.01f);
    deathStar.drawWireframe();
    deathStar.setScale(1.f);

    ofDisableDepthTest();
   
//    for (int i = 0; i < fingersFound.size(); i++) {
//      int id = fingersFound[i];
//      ofPolyline & polyline = fingerTrails[id];
//    }
    
    for(int i = 0; i < simpleHands.size(); i++){
        simpleHands[i].debugDraw();
    }
    
//      for(int i = 0; i < fingersFound.size(); i++){
//          int id = fingersFound[i];
//          ofPolyline & polyline = fingerTrails[id];
//          // Do something with it
//          ofDrawSphere(simpleHands.pos, 20);
//
//      }

    cam.end();
//    ### Leap Hands draw debug
//      for(int i = 0; i < simpleHands.size(); i++){
//          simpleHands[i].debugDraw();
//          simpleHands[i].handPos;
//          ofSpherePrimitive handTracker;
//          handTracker.setPosition(simpleHands[i].handPos);
//    }

    string distanceString;
    
//    distanceString = "Distance: "+ofToString(cam.getDistance());
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
