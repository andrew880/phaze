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
		static const int kboxVh = 60;
		static const int kworldCount = 18;
		static const int kshadowFrameCounts = 100;
		static const int kvelScale = 40;

		vector	<ofxBox2d>						box2dArr; //world arr
		ofxBox2d								shadowWorld; //shadow world

		vector	<shared_ptr<ofxBox2dRect> >		boxesB;   // defalut blue rects
		vector	<shared_ptr<ofxBox2dRect> >		boxesR;   // defalut red rects
		vector	<shared_ptr<ofxBox2dCircle> >	portals; //portal circle

		vector	<ofVec2f>					shadowPos;

		shared_ptr<ofxBox2dCircle> player;
		shared_ptr<ofxBox2dCircle> shadow;
		
		bool blue = true;
		bool grounded = true;
		bool shifted = false;
		bool projected = false;
		int worldIndex = 0;
		string s;

		void setup();
		void update();
		void draw();

		void playerInit();
		void shadowInit();

		void mapInit(vector<vector<char*>> map);
		void boxBHCreate(int k, int x, int y);
		void boxRHCreate(int k, int x, int y);
		void boxBVCreate(int k, int x, int y);
		void boxRVCreate(int k, int x, int y);
		void portalCreate(int x, int y, int k, int i);
		void playerUpdate();
		void shadowUpdate();

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
			"...........*******....../....../.........*..........//////////.......**********........",
			"...........*......*...../....../........*.*................./........*.................",
			"...........*......*...../....../........*.*................/.........*.................",
			"...........*******....../....../.......*...*............../..........*.................",
			"...........*............////////.......*...*............./...........**********........",
			"...........*............/....../......*.***.*.........../............*.................",
			"...........*............/....../......*.....*........../.............*.................",
			"...........*............/....../.....*.......*......../..............*.................",
			"...........*............/....../.....*.......*.......//////////......**********........",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".................................................................................=.....",
			"........................---------.................+++++++++............................",
			"_______________________________________________________________________________________" },
		//tutorial
		{	"......................................||................................................",
			"...=.......................___........||...............................................",
			".....___________.......___............||...............................................",
			"---................___................||...........||..................................",
			"...---........___.....................||...........||..................................",
			".......---............................||...........||..................................",
			".......___---.........................||...........||..................................",
			"...___.........---....................||...........||......-----.......................",
			"___................---................||...........||______|..........|--------........",
			"............___........---............||....................|.........|................",
			".........___...............---........||.................... |........|........++++++++",
			"......___......................---..++||..................... |.......|................",
			"...___..........................+++---||...................... |......|--------........",
			"___.........................+++.......||....................... |_____|................",
			"........................+++...........||..............................|........++++++++",
			"....................+++...............||..............................|................",
			"................+++...................||..............................|--------........",
			"............+++.......................||..............................|................",
			"........+++...........................||..............................|........++++++++",
			"......................................||..............................|................",
			"_______________________________________________________________________________________" },
		//1
		{	".......................................................................................",
			"....................................................................................=..",
			"..................................................................+++++++..............",
			".........................................................................++............",
			".........................................................--------................------",
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
			"____.....................*......***............................................________",
			".........................*......*..*..................................---..............",
			"..................----...*......*...*......................*.......---........---......",
			".....----................*......*....*.....................*....---................--..",
			".........................*......*..........................*----.......................",
			".........................*......*..........................*.........................--",
			".........................*......*..........................*...........................",
			".........................*......*..........................*.......................--..",
			".........................*......*......................................................",
			".........................*......*.......................--...........................--",
			".........................*......*....................--................................",
			".........................*......*.................++...............................--..",
			".........................*......*..............++......................................",
			".........................*......*...........++.......................................--",
			"................................--------++++...........................................",
			"...................................................................................--..",
			".......................................................................................",
			".....................................................................................--",
			"_______________________________________________________________________________________" },
		//3
		{	".......................................................................................",
			"...................................................................................=...",
			".......................................................................................",
			".........................................................._________............._______",
			".......................................................................................",
			".......................................................................................",
			".............................................++++++++++..............--.....++.........",
			".......................................................................................",
			".....................................................................................++",
			"..................................-----------..........................................",
			"....................................................+++++++++...............--.........",
			".......................................................................................",
			".........................+++++++++.....................................................",
			"..................................................................++++++++...........--",
			".......................................................................................",
			"..............----------...................................................+++.........",
			".......................................................................................",
			".......................................................................................",
			"...++++++++++..........................................................................",
			".......................................................................................",
			"_______________________________________________________________________________________" }, 
		//4
		{	"...............|............................................................|..........",
			"...............|............................................................|..........",
			"...............|............................................................|..........",
			"...............|............................................................|..........",
			"...............|............................................................|..........",
			"...............|............................................................|..........",
			"...............|............................................................|__________",
			"...............|.......................................................................",
			"...............|.......................................................................",
			"...............|.......................................................................",
			"...............|...........____________................................................",
			"...............|..........|............|...............................................",
			"...............|..........|............|...............................................",
			"...............|..........|............|...............................................",
			"...............|..........|............|...............................................",
			"...............|..........|............|...............................................",
			"...............|..........|............|...............................................",
			"...............|..........|............|...............................................",
			"...=...........|..........|............|...............................................",
			"...............|..........|............|...............................................",
			"_______________________________________________________________________________________" },
		//5
		{	"//////////////////////////.............................................................",
			"/////////////////////////..............................................................",
			"**************/////*****/..........................................................=....",
			"_______________________.................../////........................................",
			"|......................|...............................................________________",
			"|....._________________|...............................................|||.............",
			"|....|..................................................................|||............",
			"|....|...................................................................|||...........",
			"|....|....................................................................|||..........",
			"|....|.....................................................................|||.........",
			"|....|......................................................................|||........",
			"|....|.......................................................................|||.......",
			"|....|........................................................................|||......",
			"|....|.........................................................................|||.....",
			"|....|..........................................................................|||....",
			"|....|...........................................................................|||...",
			"|....|............................................................................|||..",
			"|....|.............................................................................|||.",
			"|....|..............................................................................|||",
			".....................................................................................||",
			"_______________________________________________________________________________________" },
		//6
		{	".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			".......................................................................................",
			"............................................................................___________",
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
		//7
		{	".......................................................................................",
			"......................................................................=................",
			"........=....................+.....+.......+++++.......+.....+.........................",
			"..............................+...+.......+.....+......+.....+..........=..............",
			"...............................+.+........+.....+......+.....+.........................",
			"..................=.............+.........+.....+......+.....+.........................",
			"................................+.........+.....+......+.....+.................=.......",
			"................................+..........+++++.........++++..........................",
			".....................................................................=.................",
			".............=.........................................................................",
			"...................-.......-.......-.........-------.........--....-...................",
			"....................-.....-.-.....-.........-.......-........-.-...-...................",
			".....................-...-...-...-......=...-.......-........-..-..-.............=.....",
			"........=.............-.-.....-.-...........-.......-........-...-.-...................",
			".......................-.......-.............-------.........-....--...................",
			".......................................................................................",
			".......+.....-.....+.....-.....+.....-.....+.....-.....+.....-.....+.....-.....+.......",
			".........+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+.........",
			".........-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-...-.+...+.-.........",
			".......-.....+.....-.....+.....-.....+.....-.....+.....-.....+.....-.....+.....-.......",
			"_______________________________________________________________________________________" } };
};