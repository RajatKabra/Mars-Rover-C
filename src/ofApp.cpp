// X, Y, Z are buttons. once pressed, they will be on till 
// user presses the button again to switch them of
//  CS235 - User Interface Design
//
//  Assignment 5 -  Mars HiRise Project
// 
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter

//  Student Name:   < Rajat Kabra >
//  Date: <11/24/2017>


#include "ofApp.h"
#include "math.h"



void ofApp::setup(){
	background.loadImage("space.jpg");
	background.draw(0, 0);
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bPointSelected = false;
	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();
	selectmodel = false;
	initLightingAndMaterials();
	mars.loadModel("mars-low-v2.obj");
	mesh = mars.getMesh(0);
	mars.setScaleNormalization(false);

}
// functions for arrow key activation taken from openframeworks examples
void ofApp::update(){
	if (bMoveCameraForward && !bCtrlKeyDown) moveCamera(CamMoveForward, .05);
	else if (bMoveCameraBackward && !bCtrlKeyDown) moveCamera(CamMoveBackward, .05);

	else if (bRotateCameraLeft) rotateCamera(-1.0);   // rotate left/right
	else if (bRotateCameraRight) rotateCamera(1.0); 
	else if (bMoveCameraForward && bCtrlKeyDown) rotateCamera(-1.0);  // rotate up/down
	else if (bMoveCameraBackward && bCtrlKeyDown) rotateCamera(1.0);
	if (tel == true) telep();
	if (bTeleport==true) teleportCamera();
	if (cTeleport == true) teleportRover();
	vel += acceleration;
	vel *= 0.1;
	acceleration *= 0.99;
	auto velVector = cam.getZAxis() * -vel;
	cam.move(velVector);
}

//contains mapping and function calling for drawing axis
void ofApp::draw(){

//	ofBackgroundGradient(ofColor(20), ofColor(0));   // pick your own backgroujnd
	
ofBackground(ofColor::black);
ofEnableLighting();

ofDisableDepthTest();
///draw the backdrop
ofPushMatrix();
ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
ofSetColor(255); //will be black if this is not included 

background.getTexture().bind();
backdrop.resizeToTexture(background.getTexture());
backdrop.set(ofGetWidth(), ofGetHeight());
backdrop.draw();
background.getTexture().unbind();
ofPopMatrix();
ofEnableDepthTest();

//ofEnableAlphaBlending();
ofEnableDepthTest();
//
	cam.begin();
	ofPushMatrix();
	if (xtrue == false && ytrue == false && ztrue == false)
		drawAxis(0);
	else if (xtrue == true && ytrue == true && ztrue == true)
		drawAxis(7);
	else if (xtrue == true)
	{
		if (ytrue == true)
			drawAxis(4);
		else if (ztrue == true)
			drawAxis(5);
		else
			drawAxis(1);
	}
	else if (ytrue == true)
	{
		if (ztrue == true)
			drawAxis(6);
		else
			drawAxis(2);
	}
	else if (ztrue == true)
	{
			drawAxis(3);
	}

	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		rover.drawWireframe();

	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
		rover.drawFaces();
	}

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
		rover.drawVertices();
	}
	if (drawrover)
	{
		//rover.setScaleNormalization(false);
		
		rover.setScale(0.005, 0.005, 0.005);
		rover.drawFaces();
	}
	if (bPointSelected) {
		
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}

	ofPopMatrix();
	cam.end();
}

// Draw an XYZ axis around rover or terrain and for making selected axis bold

void ofApp::drawAxis(int i) { 

	if (selectmodel==false)
	{
		if (i == 0)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
			//ofDrawSphere(1.0, 0.0, 0.0, 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));
			//ofDrawSphere(0, 1.0, 0.0, 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));
			//ofDrawSphere(0, 0.0, 1.0, 0.1);
			ofPopMatrix();
		}

	}
	else if(selectmodel==true)
	{
		ofPoint x = rover.getPosition();
		//no axis selected
		if (i == 0)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x+2, x.y, x.z));
			//ofDrawSphere(x.x +2, x.y, x.z, 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x , x.y+2, x.z));
			//ofDrawSphere(x.x, x.y+2, x.z, 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z+2));
			//ofDrawSphere(x.x, x.y, x.z+2, 0.1);
			ofPopMatrix();
		}
		//x axis
		if (i == 1)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofSetLineWidth(5.0f);
			
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x +2, x.y, x.z));
			//ofDrawCone(x.x+2, x.y, x.z, 0.1, 0.3);
			ofDrawSphere(x.x + 2, x.y, x.z, 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y+2, x.z));
			//ofDrawSphere(x.x, x.y+2, x.z, 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z+2));
			//ofDrawSphere(x.x, x.y, x.z+2, 0.1);
			ofPopMatrix();
		}
		//y axis
		if (i == 2)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x +2, x.y, x.z));
			//ofDrawSphere(x.x+2, x.y, x.z, 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y+2, x.z));
			ofDrawSphere(x.x, x.y+2, x.z, 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z+2));
			//ofDrawSphere(x.x, x.y, x.z+2, 0.1);
			ofPopMatrix();
		}

		//z axis
		if (i == 3)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x+2, x.y, x.z));
			//ofDrawSphere(x.x +2, x.y, x.z, 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y+2, x.z));
			//ofDrawSphere(x.x, x.y +2, x.z, 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z+2));
			ofDrawSphere(x.x, x.y, x.z +2, 0.1);
			ofSetLineWidth(1.0f);

			ofPopMatrix();
		}
		// x y axis
		if (i == 4)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x+2, x.y, x.z));
			ofDrawSphere(x.x+2, x.y, x.z , 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y+2, x.z));
			ofDrawSphere(x.x, x.y+2, x.z , 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z+2));
			//ofDrawSphere(x.x, x.y, x.z +2, 0.1);
			ofSetLineWidth(1.0f);

			ofPopMatrix();
		}
		//x z axis
		if (i == 5)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x+2, x.y, x.z));			
			ofSetColor(ofColor(0, 255, 0));
			//ofDrawSphere(x.x+2, x.y, x.z, 0.1);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y+2, x.z));
			ofDrawSphere(x.x, x.y+2, x.z , 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z+2));
			ofDrawSphere(x.x, x.y, x.z +2, 0.1);
			ofSetLineWidth(1.0f);

			ofPopMatrix();
		}
	// y z axis
		if (i == 6)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofSetLineWidth(1.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x+2, x.y, x.z));
			//ofDrawSphere(x.x+2, x.y, x.z, 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y+2, x.z));
			ofDrawSphere(x.x, x.y+2, x.z , 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z+2));
			ofDrawSphere(x.x, x.y, x.z +2, 0.1);
			ofSetLineWidth(1.0f);

			ofPopMatrix();
		}
		// all three axis
		if (i == 7)
		{
			ofPushMatrix();
			ofSetColor(ofColor(255, 0, 0));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x +2, x.y, x.z));
			ofDrawSphere(x.x +2, x.y, x.z, 0.1);
			ofSetColor(ofColor(0, 255, 0));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y +2, x.z));
			ofDrawSphere(x.x, x.y +2, x.z, 0.1);
			ofSetColor(ofColor(0, 0, 255));
			ofSetLineWidth(5.0f);
			ofDrawLine(ofPoint(x.x, x.y, x.z), ofPoint(x.x, x.y, x.z + 2));
			ofDrawSphere(x.x, x.y, x.z + 2, 0.1);
			ofSetLineWidth(1.0f);

			ofPopMatrix();
		}
	}
}
//for side arrow keys
void ofApp::steer(float dir) {
	cam.rotate(dir, 0, 1, 0); 
}
// x,y,z are like activation keys which needs to be pressed again to turn them off
void ofApp::keyPressed(int key) {

	switch (key) {
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'r':
		cam.reset();
		
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'p':
		setRoverTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'm':
		selectmodel = !selectmodel;
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case 'x':
		xtrue = !xtrue;
		break;
	case 'y':
		ytrue = !ytrue;
		break;
	case 'z':
		ztrue = !ztrue;
		break;
	case OF_KEY_ALT:
		if (selectmodel == false)
		{cam.enableMouseInput();
		bAltKeyDown = true; }
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_LEFT:
		steer(+1);
		break;
	case OF_KEY_RIGHT:
		steer(-1);
		break;
	case OF_KEY_UP:
		bMoveCameraForward = true;
		acc();
		break;
	case OF_KEY_DOWN:
		bMoveCameraBackward = true;
		back();
		break;
	case OF_KEY_F1:
		cam.reset();
		nextpoint.x = 0;
		nextpoint.y = 0;
		nextpoint.z = 0;
		teleportStartPoint = cam.getPosition();		
		bTeleport = true;
		xdegrees = 90;
		totaldis = teleportStartPoint.distance(selectedPoint);
		teledes = nextpoint.distance(selectedPoint);
		//teleportCamera();
		break;
	case OF_KEY_F2:
		v = rover.getPosition() - selectedPoint;
		//cam.reset();
		nextpoint.x = 0;
		nextpoint.y = 0;
		nextpoint.z = 0;
		teleportStartPoint = rover.getPosition();
		cTeleport = true;
		xdegrees = 90;
		totaldis = teleportStartPoint.distance(selectedPoint);
		teledes = nextpoint.distance(selectedPoint);
		//teleportCamera();
		break;
	}
}
// Useless function. No memory consumed since its never called. Used during
// debugging
void ofApp::acc1() {
	cout << "F1";
	bPointSelected = false;
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	ofVec3f nearestVertex;
	int nearestIndex = 0;
	float distance;
	ofVec2f mouse(mouseX, mouseY);
	for (int i = 0; i < n; i++) {
		ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
		distance = cur.distance(mouse);
		if (distance<36000)
			if (i == 0 || distance < nearestDistance) {
				nearestDistance = distance;
				nearestVertex = cur;
				nearestIndex = i;
				bPointSelected = true;
			}
	}
	selectedPoint = mesh.getVertex(nearestIndex);
	
	v = selectedPoint - cam.getPosition();
	v=v.normalize();
}

//Switching wireframe on/off
void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

//for front arrow key
void ofApp::acc() {
	acceleration += 0.1;
}

//for back arrow key
void ofApp::back() {
	acceleration -= 0.1;
}
void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_LEFT:
		bRotateCameraLeft = false;
		break;
	case OF_KEY_RIGHT:
		bRotateCameraRight = false;
		break;
	case OF_KEY_UP:
		bMoveCameraForward = false;
		break;
	case OF_KEY_DOWN:
		bMoveCameraBackward = false;
		break;
	case OF_KEY_F1:
		//tel = false;
		//bTeleport = false;
		break;
	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//checking whether rover is selected or the terrain. False means terrain
//true means rover
void ofApp::mouseDragged(int x, int y, int button){
	if (selectmodel == true)
		doTranslation(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	if (bAltKeyDown == false && tel==false) doPointSelection(x,y);
	lastMouse = ofVec2f(x, y);
}

//
//  Select Target Point on Terrain
//
void ofApp::doPointSelection(int x, int y) {
	if(selectmodel==false)
	{
		bPointSelected = false;
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	ofVec3f nearestVertex;
	int nearestIndex = 0;
	float distance;
	ofVec2f mouse(mouseX, mouseY);
	for (int i = 0; i < n; i++) {
		ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
		distance = cur.distance(mouse);
		if (distance < 36000)
			if (i == 0 || distance < nearestDistance) {
				nearestDistance = distance;
				nearestVertex = cur;
				nearestIndex = i;
				bPointSelected = TRUE;
			}
	}
	//selectedPoint = nearestVertex;
	selectedPoint = mesh.getVertex(nearestIndex);
	v = cam.getGlobalPosition() - selectedPoint;
	ofVec3f c = cam.getPosition();
	float d = c.z - selectedPoint.z;
	//dis = d;
	halfdis = d / 2;
	}
}

// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {
	cam.setTarget(selectedPoint);
}
// Set the camera to use the Rover as it's new target
void ofApp::setRoverTarget() {
	cam.setTarget(rover.getPosition());
}
// Animate camera by moving it to the selected points (using update())
//
void ofApp::moveCamera(CamMoveDirection dir, float step) {
	
}

// Rotate Camera Left/Right
//
void ofApp::rotateCamera(float step) {
}


// For teleporting camera towards the blue dot. F1 for activating
void ofApp::teleportCamera()
{
	cam.setTarget(selectedPoint);
	teledes = nextpoint.distance(selectedPoint);
	if (teledes >= (totaldis / 2))
	{
		nextpoint.x = (teleportStartPoint.x - (v.x / (100 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.y = (teleportStartPoint.y - (v.y / (100 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.z = (teleportStartPoint.z - (v.z / (100 * abs(sin((xdegrees*3.14159 / 180))))));
		xdegrees = xdegrees - 1;
	}
	else if (teledes < (totaldis / 2) && teledes>3.5)
	{
		xdegrees = xdegrees + 1;
		nextpoint.x = (teleportStartPoint.x - (v.x / (250 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.y = (teleportStartPoint.y - (v.y / (250 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.z = (teleportStartPoint.z - (v.z / (250 * abs(sin((xdegrees*3.14159 / 180))))));
	}
	else
	{
		bTeleport = false;
		xdegrees = 0;
	}

	cam.setGlobalPosition(nextpoint);
	teleportStartPoint.x = nextpoint.x;
	teleportStartPoint.y = nextpoint.y;
	teleportStartPoint.z = nextpoint.z;

}





// Teleporting Rover towards the blue dot. F2 for activation
void ofApp::teleportRover()
{

	//cam.setTarget(selectedPoint);
	teledes = nextpoint.distance(selectedPoint);
	if (teledes >= (totaldis / 2))
	{
		nextpoint.x = (teleportStartPoint.x - (v.x / (100 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.y = (teleportStartPoint.y - (v.y / (100 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.z = (teleportStartPoint.z - (v.z / (100 * abs(sin((xdegrees*3.14159 / 180))))));
		xdegrees = xdegrees - 1;
	}
	else if (teledes < (totaldis / 2) && teledes>2.0)
	{
		xdegrees = xdegrees + 1;
		nextpoint.x = (teleportStartPoint.x - (v.x / (200 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.y = (teleportStartPoint.y - (v.y / (200 * abs(sin((xdegrees*3.14159 / 180))))));
		nextpoint.z = (teleportStartPoint.z - (v.z / (200 * abs(sin((xdegrees*3.14159 / 180))))));
	}
	else
	{
		cTeleport = false;
		xdegrees = 0;
	}

	rover.setPosition(nextpoint.x, nextpoint.y, nextpoint.z);
	teleportStartPoint.x = nextpoint.x;
	teleportStartPoint.y = nextpoint.y;
	teleportStartPoint.z = nextpoint.z;
}

// Moving rover freely or on controlled axis
void ofApp::doTranslation(int x, int y) {
	if (selectmodel==true)
	{
		ofVec3f r = rover.getPosition();
		ofDragInfo dragInfo;
		dragInfo.position.x = x;
		dragInfo.position.y = y;
		ofVec3f temppoint;
		temppoint.x = 0;
		temppoint.z = 0;
		temppoint.y = 0;
		ofVec3f my_point = cam.worldToScreen(temppoint);
		dragInfo.position.z = my_point.z;
		ofPoint my_worldPoint = cam.screenToWorld(dragInfo.position);
		if (xtrue == false && ytrue == false && ztrue == false)
			rover.setPosition(my_worldPoint.x, my_worldPoint.y, my_worldPoint.z);
		if (xtrue == true)
		{
		if (ytrue == true && ztrue == true)
			rover.setPosition(my_worldPoint.x, my_worldPoint.y, my_worldPoint.z);
			else if(ytrue ==true)
				rover.setPosition(my_worldPoint.x, my_worldPoint.y, r.z);
			else if(ztrue==true)
				rover.setPosition(my_worldPoint.x, r.y, my_worldPoint.z);
			else
				rover.setPosition(my_worldPoint.x, r.y, r.z);

		}
		else if (ytrue == true)
		{
			if (ztrue == true)
				rover.setPosition(r.x, my_worldPoint.y, my_worldPoint.z);
			else
				rover.setPosition(r.x, my_worldPoint.y, r.z);
		}
		else if (ztrue == true)
		{
				rover.setPosition(r.x, r.y, my_worldPoint.z);
		}
	}
}



void ofApp::telep()
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//For rover drag and drop
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	ofxAssimpModelLoader *rovObj = new ofxAssimpModelLoader();
	rover.loadModel(dragInfo.files[0]);
	ofPoint mouse(mouseX, mouseY);
	cout << mouse.x << "    " << mouse.y << "      " << endl;
	ofPoint bingopoint = cam.screenToWorld(mouse);
	dragInfo.position.z = 9.8;
	ofVec3f v = cam.screenToWorld(dragInfo.position);
	ofVec3f world2cam = cam.worldToCamera(v);
	ofVec3f temppoint;
	temppoint.x = 0;
	temppoint.z = 0;
	temppoint.y = 0;
	ofVec3f my_point = cam.worldToScreen(temppoint);
	dragInfo.position.z = my_point.z;
	ofPoint my_worldPoint = cam.screenToWorld(dragInfo.position);
	rover.setPosition(my_worldPoint.x, my_worldPoint.y, my_worldPoint.z);
	drawrover = true;
	
}

//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 
// Save canvas function
void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}
