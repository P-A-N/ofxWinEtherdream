#include "ofApp.h"

void ofApp::setup() 
{
	ofBackground(ofColor::black);
	ofSetFrameRate(30);
	ofSetLogLevel(OF_LOG_VERBOSE);

	dac = make_unique<ofxWinEtherdream>("10.0.0.23", 100000);
	ildaFrame.setup();
	gui.setup(ildaFrame.parameters, "dac.xml");
	gui.loadFromFile("dac.xml");

	b_curve = false;
}
void ofApp::update() 
{
	if (b_curve)
	{
		static int frm = 0;
		ildaFrame.getLastPoly().curveTo(glm::vec3(ofRandom(0.45, 0.55), ofRandom(0.45, 0.55), 0));

		frm++;
		if (frm > 100)
		{
			ildaFrame.clear();
			dac->clear();
			frm = 0;
		}
	}

	ildaFrame.update();
	dac->set_points(ildaFrame.getPoints());
}
void ofApp::draw() 
{
	ofPushStyle();
	ofSetColor(ofColor::white);
	ildaFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
	ofPopStyle();
	gui.draw();
	ofDrawBitmapStringHighlight("fps:" + ofToString(ofGetFrameRate(), 3), 10, ofGetHeight() - 20);
}
void ofApp::keyPressed(int key) 
{
	if (key == ' ')
		ildaFrame.addPoly();
	if (key == 'c')
	{
		ildaFrame.clear();
		dac->clear();
	}
	if (key == '1')
		b_curve = !b_curve;
}
void ofApp::mousePressed(int x, int y, int button) { ildaFrame.getLastPoly().lineTo(x / (float)ofGetWidth(), y / (float)ofGetHeight()); }
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}