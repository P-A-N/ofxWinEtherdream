#include "ofApp.h"

void ofApp::setup() 
{
	ofBackground(ofColor::black);
	dac = make_unique<ofxWinEtherdream>(0, 70000);
	ildaFrame.setup();
	gui.setup(ildaFrame.parameters, "dac.xml");
	gui.loadFromFile("dac.xml");
}
void ofApp::update() 
{
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
}
void ofApp::keyPressed(int key) 
{
	if (key == ' ')
		ildaFrame.addPoly();
	if (key == 'c')
		ildaFrame.clear();
}
void ofApp::mouseDragged(int x, int y, int button) { }
void ofApp::mousePressed(int x, int y, int button) { ildaFrame.getLastPoly().lineTo(x / (float)ofGetWidth(), y / (float)ofGetHeight()); }
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}