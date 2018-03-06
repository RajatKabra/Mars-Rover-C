#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"
#include <assimp/scene.h>
#include "ofxGui.h"


typedef enum { CamMoveForward, CamMoveBackward } CamMoveDirection;

class ofApp : public ofBaseApp{
	ofVec2f		lastMouse;
	public:
		ofImage background;
		ofPlanePrimitive backdrop;
		ofVec3f nextpoint;
		int i=1;
		float teledes = 0;
		float xdegrees = 0;
		float sinval=0.00000001;
		int flag = 0;
		float halfdis;
		void setup();
		void update();
		void draw();
		bool tel;
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void doTranslation(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void steer(float dir);
		float totaldis;
		void drawAxis(int i);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void setCameraTarget();
		void doPointSelection(int x, int y);
		void moveCamera(CamMoveDirection dir, float step);
		void rotateCamera(float step);
		void teleportCamera();
		void acc();
		void acc1();
		void back();
		ofEasyCam cam;
		bool drawrover=false;
		ofxAssimpModelLoader mars;
		//ofxAssimpModelLoader rover;
		ofLight light;
		ofMesh mesh,mesh1;
		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bPointSelected1;
		ofVec3f v;
		bool bMoveCameraForward;
		bool bMoveCameraBackward;
		bool bRotateCameraLeft;
		bool bRotateCameraRight;
		bool bTeleport;
		bool cTeleport;
		void telep();
		void teleportRover();
		void setRoverTarget();
		ofVec3f selectedPoint;
		ofVec3f teleportStartPoint;
		float acceleration;
		float vel;
		const float selectionRange = 4.0;
		ofPolyline polyline;
		ofPolyline polylineControl;
		ofVec3f trans;
		bool xselected, yselected, zselected;
		ofxAssimpModelLoader rover;
		float lookAtPointLength = 0.0;
		float camPosLength = 0.0;
		ofVec3f lookAtPoint;
		ofVec3f camPosition;
		bool selectmodel;
		bool xtrue, ytrue, ztrue;
//public:
		//ofxPanel gui;
		//ofxToggle one;
		//ofxToggle two;
		//ofxToggle three;
};
