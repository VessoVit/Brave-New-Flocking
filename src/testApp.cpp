#include "testApp.h"

/* Note on OS X, you must have this in the Run Script Build Phase of your project. 
where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon. 

cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";

   If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
*/

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetLogLevel(OF_LOG_WARNING);
    ofBackground(20);
    
    ofSetVerticalSync(true);
    
    float width = ofGetWidth() * .12;
    
    ofSpherePrimitive galaxy;
    galaxy.setRadius(width);
    
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .85) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f) );
    
    ofSetSphereResolution(24);
    for(int i = 0; i < 50; i++){
        flock.addBoid();
        
    }
    // EVENTS:
    // --------- Listeners ------
    //    ofAddListener(boid.clickableEvent, this, &ofApp::onEventClicked);
    
    //ctrl.init();
    ofSetFrameRate(60);
    
	leap.open();
	leap.setupGestures();   // we enable our gesture detection here
}


//--------------------------------------------------------------
void testApp::update()
{
    flock.update();
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    
	leap.updateGestures();  // check for gesture updates
	leap.markFrameAsOld();	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new. 
}

//--------------------------------------------------------------
void testApp::draw(){
    cam.begin();

    ofRotateX(ofRadToDeg(.5));
    ofRotateY(ofRadToDeg(-.5));

    float spinX = sin(ofGetElapsedTimef()*.35f);
    float spinY = cos(ofGetElapsedTimef()*.075f);

    ofEnableDepthTest();

    flock.draw();
    ofEnableLighting();
    pointLight.enable();

    ofColor(180);
    ofNoFill();

    ofDrawSphere(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth());

    // Sphere
    ofSpherePrimitive deathStar;
    //    sphere.setPosition(ofGetWidth()*.8f, ofGetHeight()*.25, 0);
    deathStar.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    deathStar.rotate(spinX, 1.0, 0.0, 0.0);
    deathStar.rotate(spinY, 0, 1.0, 0.0);

    // Sphere.Wireframe
    deathStar.setScale(1.01f);
    deathStar.drawWireframe();
    deathStar.setScale(1.f);

    ofDisableDepthTest();



    cam.end();

     /* Leap iGesture Key
     --------------------------------
     1 = Screen Tap
     2 = Key Tap
     3 = Swipe Right
     4 = Swipe Left
     5 = Swipe Down
     6 = Swipe Up
     7 = Swipe Forward
     8 = Swipe Backward (towards yourself)
     9 = Circle Left (counter-clockwise)
     10 = Circle Right (clockwise)
     --------------------------------
     */
    
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
    
    ofDrawBitmapString(msg, 20, 20);  // let's draw our message to the screen
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
}

//--------------------------------------------------------------
void testApp::exit(){
    leap.close();  // let's close down Leap and kill the controller
}
