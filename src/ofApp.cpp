#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);

	box2d.init();
	box2d.enableEvents();
	box2d.setGravity(0, 10);
	box2d.createBounds();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
	
	// register the listener so that we get the events
	ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &ofApp::contactEnd);

	playerInit();

}

//--------------------------------------------------------------
void ofApp::update() {
	box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	for (int i = 0; i < circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		circles[i].get()->draw();
	}

	for (int i = 0; i < boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].get()->draw();
	}

	ofFill();
	ofSetHexColor(0xBF2545);
	player->draw();

	// draw the ground
	box2d.drawGround();

	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: " + ofToString(box2d.getBodyCount()) + "\n";
	info += "Total Joints: " + ofToString(box2d.getJointCount()) + "\n\n";
	info += "FPS: " + ofToString(ofGetFrameRate(), 1) + "\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void ofApp::playerInit() {
	player = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
	player.get()->setPhysics(8.0, 0.0, 1);
	player.get()->setup(box2d.getWorld(), width / 2, height / 2, 25);
	player->setFixedRotation(true);
	player->setData("player");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'c') {
		float r = ofRandom(4, 20);
		circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
		circles.back().get()->setPhysics(3.0, 0.53, 0.1);
		circles.back().get()->setup(box2d.getWorld(), mouseX, mouseY, r);
		boxes.back().get()->enableGravity(true);
	}

	if (key == 'b') {
		float w = ofRandom(4, 20);
		float h = ofRandom(4, 20);
		boxes.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
		boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
		boxes.back().get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.back().get()->enableGravity(false);
		boxes.back().get()->setData("ground");
	}

	if (key == 't') ofToggleFullscreen();

	if ((key == 'w' || key == ' ') && grounded) {
		player->addForce(ofVec2f(0, -100), 100);
		grounded = false;
	}
	if (key == 'd') {
		player->setVelocity(10, player->getVelocity().y);
	}
	if (key == 'a') {
		player->setVelocity(-10, player->getVelocity().y);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::contactStart(ofxBox2dContactArgs& e) {
	if (e.a != NULL && e.b != NULL) {
		if (e.a->GetType() == b2Shape::e_circle || e.b->GetType() == b2Shape::e_circle) {
			grounded = true;
		}
	}
}

void ofApp::contactEnd(ofxBox2dContactArgs& e) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
