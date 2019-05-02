#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetLogLevel(OF_LOG_NOTICE);

	shadowWorld.init();
	shadowWorld.setFPS(60.0);

	box2dArr = vector<ofxBox2d>(kworldCount);
	for (int i = 0; i < kworldCount; i++) {
		box2dArr[i].init();
		box2dArr[i].enableEvents();
		box2dArr[i].setGravity(0, 16);
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
	shadowInit();
	mapInit(map);

	update();
}

//--------------------------------------------------------------
void ofApp::update() {
	shadowWorld.update();
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

	ofSetHexColor(0x999999);
	shadowUpdate();
	shadow->draw();

	// draw current ground
	box2dArr[worldIndex * 2 + (blue ? 0 : 1)].drawGround();

	string info = "";
	info += "FPS: " + ofToString(ofGetFrameRate(), 1) + "\n";
	info += "Level: " + ofToString(worldIndex) + "\n";
	ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString(info, 30, 30);
	string hints = "";
	if (worldIndex == 0) {
		string instructions = "";
		instructions += "	    Press [/] to switch dimensions\n";
		instructions += "		  blue <-> light blue\n		   red <-> light red\n\n";
		instructions += "		  Press [wasd] to move\n\n";
		instructions += "   Press [<] to shift to the past (goes to the shadow)\n";
		instructions += "		shadow follows ur path\n\n";
		instructions += "	   Press [>] to shift to the future\n    (goes to the opposite direction of the shadow)\n\n";
		instructions += "		  Press [h] to get hints\n";
		instructions += "	       Goal: Reach the purple portal\n";
		ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString(instructions, 750, 750);
	} else if (worldIndex == 4 && hint) {
		vector<string> tutorial = vector<string>(4);
		tutorial[0] += "1:\nPress [/] to switch dimensions\n";
		tutorial[0] += "jump -> switch dimensions to be able \nto stand on different colors\n";
		tutorial[1] += "2:\nPress [<] to shift to the past (goes to the shadow)\n";
		tutorial[1] += "jump as high as possible -> drop down into the pit -> Press [<] when the ball reaching the top fly up";
		tutorial[2] += "3:\nPress [>] to shift to the future (goes to the opposite direction of the shadow)\n";
		tutorial[2] += "start from the right -> walk left -> Press [>] to teleport through the wall";
		ofSetHexColor(0xFFFFFF);
		ofDrawBitmapString(tutorial[0], 1650, 400);
		ofSetHexColor(0xFFFFFF);
		ofDrawBitmapString(tutorial[1], 1000, 100);
		ofSetHexColor(0xFFFFFF);
		ofDrawBitmapString(tutorial[2], 950, 1000);
	} else if (worldIndex == 1 && hint) {
		hints += "hint:\n";
		hints += "use [/] to shift between colors to climb\n";
	} else if (worldIndex == 2 && hint) {
		hints += "hint:\n";
		hints += "use [>] to project yourself through walls\n";
	} else if (worldIndex == 3 && hint) {
		hints += "hint:\n";
		hints += "use [<] & [>] to project yourself upwards and recover from falls\n";
	}
	ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString(hints, 100, 100);
}

//--------------------------------------------------------------
void ofApp::playerInit() {
	player = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
	player.get()->setPhysics(8.0, 0.0, 1);
	player.get()->setup((box2dArr[worldIndex * 2 + (blue ? 0 : 1)]).getWorld(), kwidth / 2, kheight / 2, 25);
}

void ofApp::shadowInit()
{
	shadowPos = vector<ofVec2f>();
	shadow = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
	shadow.get()->setup(shadowWorld.getWorld(), kwidth / 2, kheight / 2, 15);
}

// -blue || +red
void ofApp::mapInit(vector<vector<char*>> map)
{
	for (int k = 0; k < kworldCount / 2 && k < map.size(); k++) {
		for (int i = 0; i < map[k].size(); i++) {
			for (int j = 0; map[k][i][j] != '\0'; j++) {
				double x = j * (double)kwidth / kMapHeight;
				double y = i * (double)kheight / kMapWidth;
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
					boxBHCreate(k, x, y - kboxHh * 2.0 / 3);
					boxRHCreate(k, x, y);
				} else if (map[k][i][j] == '|') {
					boxBVCreate(k, x + kboxVw * 2.0 / 3, y);
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
	boxesB.back().get()->setup(box2dArr[k * 2].getWorld(), x, y, kboxHw, kboxHh);
	boxesB.back().get()->setData(box2dArr[k * 2].getWorld());
}

void ofApp::boxRHCreate(int k, int x, int y)
{
	boxesR.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
	boxesR.back().get()->setFriction(1);
	boxesR.back().get()->setup(box2dArr[k * 2 + 1].getWorld(), x, y, kboxHw, kboxHh);
	boxesR.back().get()->setData(box2dArr[k * 2 + 1].getWorld());
}

void ofApp::boxBVCreate(int k, int x, int y)
{
	boxesB.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
	boxesB.back().get()->setFriction(1);
	boxesB.back().get()->setup(box2dArr[k * 2].getWorld(), x, y, kboxVw, kboxVh);
	boxesB.back().get()->setData(box2dArr[k * 2].getWorld());
}

void ofApp::boxRVCreate(int k, int x, int y)
{
	boxesR.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
	boxesR.back().get()->setFriction(1);
	boxesR.back().get()->setup(box2dArr[k * 2 + 1].getWorld(), x, y, kboxVw, kboxVh);
	boxesR.back().get()->setData(box2dArr[k * 2 + 1].getWorld());
}

void ofApp::portalCreate(int x, int y, int k, int i)
{
	portals.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
	portals.back().get()->setup((box2dArr[k * 2 + i]).getWorld(), x, y, 35);
	portals.back().get()->setFixedRotation(true);
}

void ofApp::playerUpdate()
{
	ofVec2f v = player->getVelocity();
	ofxBox2dCircle temp = *player;
	player->setup((box2dArr[worldIndex * 2 + (blue ? 0 : 1)]).getWorld(), player->getPosition(), 25);
	player->setVelocity(v);
}

void ofApp::shadowUpdate()
{
	if (shadow != NULL) {
		shadowPos.insert(shadowPos.begin(), player->getPosition());
		if (shadowPos.size() >= kshadowFrameCounts) {
			shadowPos.pop_back();
		}
		ofVec2f pos = ofVec2f(0, 0);
		for (int i = 0; i < shadowPos.size(); i++) {
			pos = ofVec2f(pos.x + shadowPos[i].x, pos.y + shadowPos[i].y);
		}
		if (shadowPos.size() > 0)
			shadow->setPosition(shadowPos[shadowPos.size() - 1]);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'p') {
		worldIndex++;
	}
	if (key == 'o') {
		worldIndex--;
	}

	if (key == ',' && !projected) {
		projected = true;
		ofVec2f v = ofVec2f((shadow->getPosition().x - player->getPosition().x) / kvelScale, 
			(shadow->getPosition().y - player->getPosition().y) / kvelScale);
		ofxBox2dCircle temp = *player;
		player->setup((box2dArr[worldIndex * 2 + (blue ? 0 : 1)]).getWorld(), shadow->getPosition(), 25);
		player->setVelocity(v);
	}
	if (key == '.' && !projected) {
		projected = true;
		ofxBox2dCircle temp = *player;

		int x = 2 * player->getPosition().x - shadow->getPosition().x;
		x = x > kwidth ? kwidth - 50 : ((x < 50) ? 50 : x);
		int y = 2 * player->getPosition().y - shadow->getPosition().y;
		y = y > kheight ? kheight - 50 : ((y < 50) ? 50 : y);

		ofVec2f v = ofVec2f((x - player->getPosition().x) / kvelScale, (y- player->getPosition().y) / kvelScale);
		player->setup((box2dArr[worldIndex * 2 + (blue ? 0 : 1)]).getWorld(), ofVec2f(x,y), 25);
		player->setVelocity(v);
	}

	if (key == 'h') {
		hint = true;
	}

	//movement controls
	if ((key == 'w') && grounded) {
		player.get()->addForce(ofVec2f(0, -140), 100);
		grounded = false;
	}
	if (key == 's') {
		player.get()->addForce(ofVec2f(0, 10), 100);
	}
	if (key == 'd') {
		player.get()->setVelocity(8, player->getVelocity().y);
	}
	if (key == 'a') {
		player.get()->setVelocity(-8, player->getVelocity().y);
	}
	
	//change dimension
	if (key == '/' && !shifted) {
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
		if ((e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_polygon) || 
			(e.b->GetType() == b2Shape::e_circle && e.a->GetType() == b2Shape::e_polygon)) {
			grounded = true;
			shifted = false;
			projected = false;
		}
		//teleport to next level
		if (e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle &&
			worldIndex + 1 < kworldCount / 2) {
			worldIndex += 1;
			hint = false;
			update();
			player->setup((box2dArr[worldIndex * 2 + (blue ? 0 : 1)]).getWorld(), 
				player->getPosition().x, player->getPosition().y + 50, 25);
		}
	}
}

void ofApp::contactEnd(ofxBox2dContactArgs& e) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
