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