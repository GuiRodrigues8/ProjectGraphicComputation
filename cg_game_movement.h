#pragma once
#include "ofMain.h"

struct Block {
    float x, y, z;
    float width, height;

};


struct Particle {
    ofVec3f pos;
    ofVec3f vel;
    ofColor color;
    float life;
};


class Movement {
public:

    Movement();
    ~Movement();
    void setup();
    void configBall();
    void resetPhysics();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y);
    void checkCollision();
 

    float paddleX, paddleY, paddleWidth, paddleHeight;
    float paddleSpeed;
    bool moveLeft, moveRight;

    ofPoint ball;
    ofVec3f ballDir;
    float ballSpeed;
    float ballRadius;

    std::vector<Block> blocks;
    std::vector<ofVec3f> BlockColors;

    int blockRows, blockCols;
    float blockWidth, blockHeight;

    bool isFlashing;
    float flashTime;
    float flashDuration;
    bool flashColor;
    float flashInterval;
    float flashTimerInterval;

    vector<Particle> particles;
    float particleLife;

    ofVec3f camPos;
    ofVec3f targetPos;

    int perspectiveEnabled;

    int currentScore;
    int highScore;
    int lastScore;
    bool gameWin;
	bool ballMoving;
    bool ambientOn;
    bool dirOn;
    bool pointOn;
	bool spotOn;

    bool boolDirAmb;
	bool boolDirDif;
	bool boolDirSpec;

	bool boolPointAmb;
	bool boolPointDif;
	bool boolPointSpec;

	bool boolSpotAmb;
	bool boolSpotDif;
	bool boolSpotSpec;

	GLfloat ambientLight[4];
	GLfloat zeroLight[4];

    GLfloat dirVec[4];
    GLfloat dirAmb[4];
    GLfloat dirDif[4];
    GLfloat dirSpec[4];
    ofVec3f dirVec3f;
    GLfloat dirVecTheta;

    GLfloat pointPos[4];
    GLfloat pointAmb[4];
    GLfloat pointDif[4];
    GLfloat pointSpec[4];
    GLfloat pointZtheta;
    GLfloat pointAtC, pointAtL, pointAtQ;

    GLfloat spotPos[4];
    GLfloat spotDir[3];
    ofVec3f spotDirVec;
    ofVec3f spotDirAux;
    ofVec3f spotPosVec;
    GLfloat spotAmb[4];
    GLfloat spotDif[4];
    GLfloat spotSpecular[4];
    GLfloat spotExponent;
    GLfloat spotCutoff;
    GLfloat spotTheta;
    GLfloat spotAtC, spotAtL, spotAtQ;
    float intensityFactor;
};