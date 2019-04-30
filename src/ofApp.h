#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class ofApp : public ofBaseApp{

	public:

		static const int kwidth = 2048;
		static const int kheight = 1200;
		static const int kMapWidth = 20;
		static const int kMapHeight = 88;
		static const int kboxHw = 25;
		static const int kboxHh = 5;
		static const int kboxVw = 15;
		static const int kboxVh = 40;
		static const int kworldCount = 10;

		string s;
		
		vector   <ofxBox2d>                    box2dArr;

		vector   <shared_ptr<ofxBox2dRect> >   boxesB;   // defalut blue rects
		vector   <shared_ptr<ofxBox2dRect> >   boxesR;   // defalut red rects
		vector   <shared_ptr<ofxBox2dCircle> >   portals;

		shared_ptr<ofxBox2dCircle> player;
		shared_ptr<ofxBox2dCircle> portalStart;

		bool grounded = true;
		bool shifted = false;
		bool blue = true;
		int worldIndex = 0;

		void setup();
		void update();
		void draw();

		void playerInit();

		void mapInit(vector<vector<char*>> map);
		void boxBHCreate(int k, int x, int y);
		void boxRHCreate(int k, int x, int y);
		void boxBVCreate(int k, int x, int y);
		void boxRVCreate(int k, int x, int y);
		void portalCreate(int x, int y, int k, int i);
		void playerUpdate();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// this is the function for contacts
		void contactStart(ofxBox2dContactArgs& e);
		void contactEnd(ofxBox2dContactArgs& e);

		const vector<vector<char*>> map = {
		//0
		{	".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".................................................................................=.....",
			".......................................................................................",
			"_______________________________________________________________________________________" },
		//1
		{	".......................................................................................",
			"....................................................................................=..",
			"..................................................................+++++++++............",
			".......................................................................................",
			".........................................................--------......................",
			".......................................................................................",
			"....................................................++++++++...........................",
			".......................................................................................",
			".............................................--------..................................",
			".......................................................................................",
			"......................................++++++++.........................................",
			".......................................................................................",
			"...............................--------................................................",
			".......................................................................................",
			".........................+++++++++.....................................................",
			".......................................................................................",
			"..............----------...............................................................",
			".......................................................................................",
			".......+++++++++++.....................................................................",
			".......................................................................................",
			"_______________________________________________________________________________________" },
		//2
		{	".......................................................................................",
			"..=....................................................................................",
			".........................+......+.................................../..........._______",
			".....--- ...+++....---. .+......+.................................../..................",
			".........................+......+..........................*......../..................",
			".........................+......+..........................*......../..................",
			".........................+......+..........................*......../..................",
			".........................+......+..........................*......../....../...........",
			".........................+......+..........................*.............../...........",
			".........................+......+..........................*.............../...........",
			".........................+......+......................................................",
			".........................+......+......................__..............................",
			".........................+......+...................__.................................",
			".........................+......+................__....................................",
			".........................+......+.............__.......................................",
			".........................+......+...........__.........................................",
			".........................................__............................................",
			"................................------__...............................................",
			".......................................................................................",
			".......................................................................................",
			"_______________________________________________________________________________________" },
		//3
		{	".......................................................................................",
			"...................................................................................=...",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".................................................-----------...........................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			"..............................-----------..............................................",
			".......................................................................................",
			".........................+++++++++.....................................................",
			".......................................................................................",
			".......................................................................................",
			"..............----------...............................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			"_______________________________________________________________________________________" },
		//4
		{	".......................................................................................",
			"......................................................................=................",
			"........=....................+.....+.......++++.......+.....+..........................",
			"..............................+...+.......+....+......+.....+...........=..............",
			"...............................+.+........+....+......+.....+..........................",
			"..................=.............+.........+....+......+.....+..........................",
			"................................+.........+....+......+.....+..................=.......",
			"................................+..........++++.........++++...........................",
			".....................................................................=.................",
			".............=.........................................................................",
			"...................-.......-.......-.........-------.........--....-...................",
			"....................-.....-.-.....-.............-............-.-...-...................",
			".....................-...-...-...-......=.......-............-..-..-.............=.....",
			"........=.............-.-.....-.-...............-............-...-.-...................",
			".......................-.......-.............-------.........-....--...................",
			".......................................................................................",
			".......+.....-.....+.....-.....+.....-.....+.....-.....+.....-.....+.....-.....+.......",
			".........+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+.........",
			".........-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-.........",
			".......-.....+.....-.....+.....-.....+.....-.....+.....-.....+.....-.....+.....-.......",
			"_______________________________________________________________________________________" } };
};