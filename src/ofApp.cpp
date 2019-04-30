#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetLogLevel(OF_LOG_NOTICE);

	box2dArr = vector<ofxBox2d>(kworldCount);
	for (int i = 0; i < kworldCount; i++) {
		box2dArr[i].init();
		box2dArr[i].enableEvents();
		box2dArr[i].setGravity(0, 10);
		box2dArr[i].createBounds();
		box2dArr[i].setFPS(60.0);
		box2dArr[i].registerGrabbing();
		// register the listener so that we get the events
		ofAddListener(box2dArr[i].contactStartEvents, this, &ofApp::contactStart);
		ofAddListener(box2dArr[i].contactEndEvents, this, &ofApp::contactEnd);
	}

	blue = true;
	grounded = true;
	shifted = false;
	worldIndex = 0;

	update();

	playerInit();
	mapInit(map);

	update();
}

//--------------------------------------------------------------
void ofApp::update() {
	box2dArr[worldIndex * 2].update();
	box2dArr[worldIndex * 2 + 1].update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	for (int i = 0; i < boxesB.size(); i++) {
		ofFill();
		if (blue) {
			ofSetHexColor(0x0909AC);
		} else {
			ofSetHexColor(0x6FA8DC);
		}
		if (boxesB[i].get()->getWorld() == (&box2dArr[worldIndex * 2])->getWorld()) {
			boxesB[i].get()->draw();
		}
	}
	for (int i = 0; i < boxesR.size(); i++) {
		ofFill();
		if (!blue) {
			ofSetHexColor(0xAE0707);
		} else {
			ofSetHexColor(0xAE6B6B);
		}
		if (boxesR[i].get()->getWorld() == (&box2dArr[worldIndex * 2 + 1])->getWorld()) {
			boxesR[i].get()->draw();
		}
	}

	for (int i = 0; i < portals.size(); i++) {
		ofFill();
		ofSetHexColor(0x3E00DB);
		if (portals[i].get()->getWorld() == (&box2dArr[worldIndex * 2 + 1])->getWorld() ||
			portals[i].get()->getWorld() == (&box2dArr[worldIndex * 2])->getWorld()) {
			portals[i].get()->draw();
		}
	}

	ofSetHexColor(0xEEEEEE);
	player->draw();

	// draw current ground
	box2dArr[worldIndex * 2 + (blue ? 0 : 1)].drawGround();

	string info = "";
	info += "FPS: " + ofToString(ofGetFrameRate(), 1) + "\n";
	info += "Level: " + ofToString(worldIndex) + "\n";
	ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString(info, 30, 30);
	string instructions = "";
	if (worldIndex == 0) {
		instructions += "Press [q] to switch dimensions\n";
		if(!blue) {
			instructions += "Press [wasd] to move\n";
			instructions += "You can stand on dark red & blue; but not light red & blue\n";
			instructions += "if you switch, you can switch the colors of red and blue to stand on different things\n";
			instructions += "\n\nGoal: Reach the purple portal\n";
		}
	}
	ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString(instructions, 800, 300);
}

//--------------------------------------------------------------
void ofApp::playerInit() {
	player = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
	player.get()->setPhysics(8.0, 0.0, 1);
	player.get()->setup((box2dArr[worldIndex * 2 + (blue ? 0 : 1)]).getWorld(), kwidth / 2, kheight / 2, 25);
	//player.get()->setFixedRotation(true);
	player.get()->setData("p");
}

// -blue || +red
void ofApp::mapInit(vector<vector<char*>> map)
{
	for (int k = 0; k < kworldCount / 2 && k < map.size(); k++) {
		for (int i = 0; i < map[k].size(); i++) {
			for (int j = 0; map[k][i][j] != '\0'; j++) {
				int x = j * kwidth / kMapHeight;
				int y = i * kheight / kMapWidth;
				if (map[k][i][j] == '-') {
					boxBHCreate(k, x, y);
				} else if (map[k][i][j] == '+') {
					boxRHCreate(k, x, y);
				} else if(map[k][i][j] == '/') {
					boxBVCreate(k, x, y);
				} else if (map[k][i][j] == '*') {
					boxRVCreate(k, x, y);
				} else if (map[k][i][j] == '=') {
					portalCreate(x, y, k, 0);
					portalCreate(x, y, k, 1);
				} else if (map[k][i][j] == '_') {
					boxBHCreate(k, x, y);
					boxRHCreate(k, x, y);
				} else if (map[k][i][j] == '|') {
					boxBVCreate(k, x, y);
					boxRVCreate(k, x, y);
				}
			}
		}
	}
}

void ofApp::boxBHCreate(int k, int x, int y)
{
	boxesB.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
	boxesB.back().get()->setFriction(1);
	boxesB.back().get()->setup(box2dArr[k * 2].getWorld(), x, y, kboxw, kboxh);
	boxesB.back().get()->setData(box2dArr[k * 2].getWorld());
}

void ofApp::boxRHCreate(int k, int x, int y)
{
	boxesR.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
	boxesR.back().get()->setFriction(1);
	boxesR.back().get()->setup(box2dArr[k * 2 + 1].getWorld(), x, y, kboxw, kboxh);
	boxesR.back().get()->setData(box2dArr[k * 2 + 1].getWorld());
}

void ofApp::boxBVCreate(int k, int x, int y)
{
	boxesB.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
	boxesB.back().get()->setFriction(1);
	boxesB.back().get()->setup(box2dArr[k * 2].getWorld(), x, y, kboxw, kboxh);
	boxesB.back().get()->setData(box2dArr[k * 2].getWorld());
}

void ofApp::boxRVCreate(int k, int x, int y)
{
	boxesR.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
	boxesR.back().get()->setFriction(1);
	boxesR.back().get()->setup(box2dArr[k * 2 + 1].getWorld(), x, y, kboxw, kboxh);
	boxesR.back().get()->setData(box2dArr[k * 2 + 1].getWorld());
}

void ofApp::portalCreate(int x, int y, int k, int i)
{
	portals.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
	portals.back.get()->setup((box2dArr[k * 2 + i]).getWorld(), x, y, 35);
	portals.back.get()->setFixedRotation(true);
}

void ofApp::playerUpdate()
{
	ofVec2f v = player->getVelocity();
	player->setup((box2dArr[worldIndex * 2 + (blue ? 0 : 1)]).getWorld(), player->getPosition(), 25);
	player->setVelocity(v);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'p') {
		worldIndex++;
	}
	if (key == 'o') {
		worldIndex--;
	}

	if (key == 't') ofToggleFullscreen();

	//movement controls
	if ((key == 'w' || key == ' ') && grounded) {
		player.get()->addForce(ofVec2f(0, -100), 100);
		grounded = false;
	}
	if (key == 's') {
		player.get()->addForce(ofVec2f(0, 10), 100);
	}
	if (key == 'd') {
		player.get()->setVelocity(10, player->getVelocity().y);
	}
	if (key == 'a') {
		player.get()->setVelocity(-10, player->getVelocity().y);
	}
	
	//change dimension
	if (key == 'q' && !shifted) {
		shifted = true;
		blue = !blue;
		playerUpdate();
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
		s = "smt" + ofToString(e.a->GetUserData()) + ofToString(e.b->GetUserData());
		if ((e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_polygon) || 
			(e.b->GetType() == b2Shape::e_circle && e.a->GetType() == b2Shape::e_polygon)) {
			grounded = true;
			shifted = false;
		}
		//teleport to next level
		if (e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
			if (worldIndex + 1 < kworldCount / 2) {
				worldIndex += 1;
				playerUpdate();
			} else if (worldIndex + 1 == kworldCount / 2) {
				//worldIndex = 0;
				//player->setup(box2dArr[worldIndex * 2 + (blue ? 0 : 1)].getWorld(), kwidth / 8, kheight - kheight / 8, 25);
			}
		}
	}
}

void ofApp::contactEnd(ofxBox2dContactArgs& e) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
