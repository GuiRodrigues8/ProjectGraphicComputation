#include "ofApp.h"
#include "materiais.h"

void ofApp::setup() {
    ofSetFrameRate(60);
	ofSetBackgroundColor(255, 153, 0);
    movement = new Movement();

}

void ofApp::update() {
	ofSetWindowTitle("CG Breakout Game running @ " + ofToString(int(ofGetFrameRate())));
	movement->update();
}


void ofApp::draw() {
	movement->draw();
}

void ofApp::keyPressed(int key) {
	movement->keyPressed(key);
}

void ofApp::keyReleased(int key) {
	movement->keyReleased(key);
}

void ofApp::mouseMoved(int key, int y) {
	movement->mouseMoved(key, y);
}

void ofApp::mousePressed(int x, int y) {
	movement->mousePressed(x, y);
}

