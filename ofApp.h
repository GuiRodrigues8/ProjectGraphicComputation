#pragma once

#include "ofMain.h"
#include "cg_game_movement.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
	void mouseMoved(int key, int y);
    void mousePressed(int x, int y);
    Movement* movement;
};

