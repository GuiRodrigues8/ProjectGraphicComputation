#include "cg_game_movement.h"
#include "cg_extras.h"
#include "cg_cam_extras.h"
#include "drawing_extras.h"
#include "materiais.h"

Movement::Movement()
{
    setup();
}

Movement::~Movement()
{
}
//Inicializacao de variaveis e aspetos do jogo
void Movement::setup()
{
    // Configurar a barra
    paddleWidth = 100;
    paddleHeight = 20;
    paddleX = -paddleWidth * 0.5;
    paddleY = -gh() * 0.5 + 40;  
    paddleSpeed = 10;
    moveLeft = moveRight = false;


    // Configurar os blocos
    blockRows = 5;
    blockCols = 8;
    blockWidth = gw() / blockCols;
    blockHeight = 30;


    // Configurar o jogo
    resetPhysics();


    isFlashing = false;
    flashTime = 0;
    flashDuration = 0.5;
    flashColor = false;
    flashInterval = 0.15;
    flashTimerInterval = 0;
    particleLife = 0.5;
    

    highScore = 0;
    lastScore = 0;

    gameWin = false;

    perspectiveEnabled = 0;
	ambientOn = true;
    dirOn = false;
    pointOn = false;
    spotOn = false;
	boolDirAmb = true;
	boolDirDif = true;
	boolDirSpec = true;
	boolPointAmb = true;
	boolPointDif = true;
	boolPointSpec = true;
	boolSpotAmb = true;
	boolSpotDif = true;
	boolSpotSpec = true;

    pointZtheta = 0;
    dirVecTheta = 0.;


    spotExponent = 64;
    spotCutoff = 45;
    spotTheta = 0;
    spotDirVec = ofVec3f(0, 0, 0);
    spotDirAux = ofVec3f(0, 0, 0);
    spotPosVec = ofVec3f(-gw() * 0.25, 0., gh() * 0.25);
    intensityFactor = 0;

  

}

void Movement::resetPhysics() {
    if (currentScore > highScore) {
        highScore = currentScore;
    }

    lastScore = currentScore;
    currentScore = 0;

    configBall();

    ballMoving = false;

    blocks.clear();


    for (int i = 0; i < blockRows; i++) {
        for (int j = 0; j < blockCols; j++) {
            Block block;
            block.x = j * (blockWidth );
            block.y = i * (blockHeight );
			block.z = 0;
            block.width = blockWidth - 0.5;
            block.height = blockHeight -0.5;
            blocks.push_back(block);
        }
    }

}

void Movement::configBall()
{
    ball.set(0, paddleY + paddleHeight + ballRadius + 1);
    float aux = ofRandom(-1, 1);
    while (aux > -0.5 && aux < 0.5) {
        aux = ofRandom(-1, 1);
    }
    ballDir.set(aux, 1, 0);
    ballDir.normalize();
    ballSpeed = 0;
    ballRadius = 10;
}

void Movement::update()
{
    // Movimento da barra
    if (moveLeft && paddleX > -gw() * 0.5) paddleX -= paddleSpeed;
    if (moveRight && paddleX + paddleWidth < gw() * 0.5) paddleX += paddleSpeed;

    // Movimento da bola
    if (ballSpeed == 0) {
        ball.set(paddleX + paddleWidth * 0.5, paddleY + paddleHeight + ballRadius + 1);
    }

    // Movimento da bola
    ball += ballDir * ballSpeed;

    // Colisao com as bordas
    if (ball.x - ballRadius <= -gw() * 0.5 || ball.x + ballRadius >= gw() * 0.5) {
        ballDir.x *= -1; 
    }

    if (ball.y + ballRadius >= gh() * 0.5) {
        ballDir.y *= -1;
    }

    // Colisao com a barra
    if (ball.x + ballRadius > paddleX && ball.x - ballRadius < paddleX + paddleWidth &&
        ball.y + ballRadius > paddleY && ball.y - ballRadius < paddleY + paddleHeight &&
        ball.z + ballRadius > 0 && ball.z - ballRadius < 0) {  // Garantir que a colisão ocorre no plano z = 0
        ballDir.y *= -1;
        ball.y = paddleY + paddleHeight + ballRadius; 
    }

    // Colisao com os blocos
    checkCollision();
    if (currentScore == blockRows * blockCols) {
        resetPhysics();
        gameWin = true;
    }

    // Game Over
    if (ball.y - ballRadius <= -gh() * 0.5) {
        isFlashing = true;
        flashTime = flashDuration;
        flashTimerInterval = 0;
        resetPhysics();
    }

    if (isFlashing) {
        flashTime -= ofGetLastFrameTime();
        flashTimerInterval += ofGetLastFrameTime();
        if (flashTimerInterval >= flashInterval) {
            isFlashing = false;
            flashTimerInterval = 0;
            ofSetBackgroundColor(255, 153, 0);
        }
        else if (flashTime <= 0) {
            isFlashing = false;
            ofSetBackgroundColor(255, 153, 0);
        }
        else {
            flashColor = !flashColor;
            if (flashColor) {
                ofSetBackgroundColor(0, 255, 255);
            }
            else {
                ofSetBackgroundColor(255, 0, 0);
            }
        }
    }
    for (int i = 0; i < particles.size(); i++) {
        particles[i].pos += particles[i].vel;
        particles[i].life -= ofGetLastFrameTime();
        if (particles[i].life <= 0) {
            particles.erase(particles.begin() + i);
        }
    }

   

    //calculo da direcao da luz direcional
    float z = gh() * 0.25 * (cos(dirVecTheta * PI / 180.) * 0.5 + 0.45);
    float y = gh() * 0.5;
    float x = 0;
    dirVec3f = ofVec3f(x, y, z) - ofVec3f(0, 0, 0);
    dirVecTheta += 0.3;

    intensityFactor = (z - (gh() * 0.25 * 0.45)) / (gh() * 0.25 * 0.5);
    intensityFactor = ofClamp(intensityFactor, 0.0, 1.0); // Garantir que fique entre 0 e 1

    //calculo da direcao do spot
    float radius = gh() * 0.5;
    float sx = radius * cos(spotTheta * PI/ 180.0);
    float sy = radius * sin(spotTheta * PI / 180.0);
    //spotDirAux = ofVec3f(sx, sy, -1.0);
    spotDirVec = ofVec3f(sx, sy, -1.0);
    spotDirVec = spotDirVec.normalize();
    spotTheta += 0.75;


}

void Movement::draw() {
	zeroLight[0] = 0.0;
	zeroLight[1] = 0.0;
	zeroLight[2] = 0.0;
	zeroLight[3] = 1.0;

	glViewport(0, 0, gw(), gh());
    perspective(60, 100, 100);
	//glFrustum(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, 100, 1000);
    switch (perspectiveEnabled) {
        case 0:
            lookat(0, 0, 665.108, 0, 0, 0, 0, 1, 0);
            break;
        case 1:
            lookat(0, -gh() * 1.25, gh() * 0.75, 0, 0, 0, 0, 0, 1);
            break;
        case 2:
            lookat(gw() * 0.5, -gh() * 1.25, gh() * 0.75, 0, 0, 0, 0, 0, 1);
            break;
        case 3:
            lookat(-gw() * 0.5, -gh() * 1.25, gh() * 0.75, 0, 0, 0, 0, 0, 1);
            break;
        case 4:
            lookat(0, -gh() * 0.75, gh(), 0, 0, 0, 0, 0, 1);
            break;
    }

    glEnable(GL_LIGHTING);//habilita o uso de ilumina��o
    glEnable(GL_NORMALIZE);//utiliza versores para normais (normais normalizadas)



    if (ambientOn) {
        ambientLight[0] = 1;//R
        ambientLight[1] = 1;//G
        ambientLight[2] = 1;//B
        ambientLight[3] = 1;//useless
        //loadMaterial(9);
        ofSetBackgroundColor(255, 153, 0);
    }
    else {
        ambientLight[0] = 0.;
        ambientLight[1] = 0.;
        ambientLight[2] = 0.;
        ambientLight[3] = 1;
        //loadMaterial(8);
        ofSetBackgroundColor(0, 0, 0);
    }
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);


    //#################################################
    //luz direcional
    dirVec[0] = dirVec3f.x;//x
    dirVec[1] = dirVec3f.y;//y
    dirVec[2] = dirVec3f.z;//z
    dirVec[3] = 0;//vetor - dire��o!

    dirAmb[0] = 0.3 + 0.7 * intensityFactor; // R (de 0.3 a 1.0)
    dirAmb[1] = 0.3 + 0.7 * intensityFactor; // G
    dirAmb[2] = 0.3 + 0.7 * intensityFactor; // B
    dirAmb[3] = 1.;//constante

    dirDif[0] = 0.5 + 0.5 * intensityFactor; // R (de 0.5 a 1.0)
    dirDif[1] = 0.5 + 0.5 * intensityFactor; // G
    dirDif[2] = 0.5 + 0.5 * intensityFactor; // B
    dirDif[3] = 1.0;

    dirSpec[0] = 0.5 + 0.5 * intensityFactor; // R (de 0.5 a 1.0)
    dirSpec[1] = 0.5 + 0.5 * intensityFactor; // G
    dirSpec[2] = 0.5 + 0.5 * intensityFactor; // B
    dirSpec[3] = 1.0;

    glLightfv(GL_LIGHT0, GL_POSITION, dirVec);
    if (boolDirAmb) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, dirAmb);
    }
    else {
		glLightfv(GL_LIGHT0, GL_AMBIENT, zeroLight);
    }
	if (boolDirDif) {
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dirDif);
	}
	else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, zeroLight);
	}
	if (boolDirSpec) {
    glLightfv(GL_LIGHT0, GL_SPECULAR, dirSpec);
	}
    else {
        glLightfv(GL_LIGHT0, GL_SPECULAR, zeroLight);
    }
    if (dirOn) {
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHT0);
    }
    //#################################################

    //#################################################
    //luz pontual
    pointPos[0] = ball.x; // Posição X da bola
    pointPos[1] = ball.y; // Posição Y da bola
    pointPos[2] = ball.z; // Posição Z da bola
    pointPos[3] = 1.0;   // Luz pontual (posição homogênea)

    pointAmb[0] = fabs(0.9 + 0.1 * sin(pointZtheta * PI / 180.0)); // R: varia com seno
    pointAmb[1] = fabs(0.9 + 0.1 * sin(pointZtheta * PI / 180.0)); // G: varia com cosseno
    pointAmb[2] = fabs(0.9 + 0.1 * sin(pointZtheta * PI / 180.0)); // B: defasado no ângulo
    pointAmb[3] = 1.0; // Constante

    pointDif[0] = pointAmb[0]; // Difusa acompanha a cor ambiente
    pointDif[1] = pointAmb[1];
    pointDif[2] = pointAmb[2];
    pointDif[3] = 1.0; // Con

    pointSpec[0] = 1.0; // Luz especular branca
    pointSpec[1] = 1.0;
    pointSpec[2] = 1.0;
    pointSpec[3] = 1.0;

    glLightfv(GL_LIGHT1, GL_POSITION, pointPos);
    if (boolPointAmb) {
        glLightfv(GL_LIGHT1, GL_AMBIENT, pointAmb);
    }
    else {
		glLightfv(GL_LIGHT1, GL_AMBIENT, zeroLight);
    }
    if (boolPointDif) {
        glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDif);
    }
    else {
		glLightfv(GL_LIGHT1, GL_DIFFUSE, zeroLight);
    }
    if (boolPointSpec) {
        glLightfv(GL_LIGHT1, GL_SPECULAR, pointSpec);
    }
    else {
		glLightfv(GL_LIGHT1, GL_SPECULAR, zeroLight);
    }
    //atenuacao
    //reparem que nao e fv (float vector) e so f (float)
    pointAtC = 1;
    pointAtL = 0.0001;
    pointAtQ = 0.00001;
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, pointAtC);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, pointAtL);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, pointAtQ);
    if (pointOn) {
        glEnable(GL_LIGHT1);
    }
    else {
        glDisable(GL_LIGHT1);
    }
    pointZtheta += 0.25;
    //#################################################
    //#################################################
    //luz foco left

    //posicao
    spotPos[0] = 0;
    spotPos[1] = 0;
    spotPos[2] = 20;
    spotPos[3] = 1.;

    //direcao
    spotDir[0] = spotDirVec.x;
    spotDir[1] = spotDirVec.y;
    spotDir[2] = spotDirVec.z;
    //spotDir[3] = 0.;Nao tem a 4 coordenada, e sempre vetor

    //ambiente
    spotAmb[0] = 1.;//R
    spotAmb[1] = 1.;//G
    spotAmb[2] = 1.;//B
    spotAmb[3] = 1.;//constante

    //difusa
    spotDif[0] = 1.;//R
    spotDif[1] = 1.;//G
    spotDif[2] = 1.;//B
    spotDif[3] = 1.;//constante

    //specular
    spotSpecular[0] = 1.;//R
    spotSpecular[1] = 1.;//G
    spotSpecular[2] = 1.;//B
    spotSpecular[3] = 1.;//constante

    //concentracao
    spotExponent = 10;//0 - 128

    //angulo
    spotCutoff = 45;//0 - 90

    if (spotOn) {
        glEnable(GL_LIGHT2);
    }
    else {
        glDisable(GL_LIGHT2);
    }

    glLightfv(GL_LIGHT2, GL_POSITION, spotPos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);

    if (boolSpotAmb) {
        glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmb);
	}
	else {
		glLightfv(GL_LIGHT2, GL_AMBIENT, zeroLight);
	}
    if (boolSpotDif) {
        glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDif);
    }
    else {
		glLightfv(GL_LIGHT2, GL_DIFFUSE, zeroLight);
    }
	if (boolSpotSpec) {
    glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpecular);
    }
    else {
		glLightfv(GL_LIGHT2, GL_SPECULAR, zeroLight);
    }

    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spotExponent);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotCutoff);

    //atenuacao
    spotAtC = 1.;
    spotAtL = 0.;
    spotAtQ = 0.;
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, spotAtC);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, spotAtL);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, spotAtQ);



    // Desenhar a barra
    glPushMatrix();
    //glColor3f(0.3, 0.6, 0.0);
    loadMaterial(7);
    glTranslatef(paddleX + paddleWidth / 2, paddleY + paddleHeight / 2, 0); // Posicionar no centro
    glScalef(paddleWidth, paddleHeight, 20); // Dimensionar
    drawCube(); // Função para desenhar o cubo
    glPopMatrix();

	//Desenhar a bola
   
    //glColor3f(1, 0, 0);
	loadMaterial(0);
    float segments = 90;
    float angleStep = TWO_PI / segments;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(ball.x, ball.y, 0);  // Centro do círculo
    for (float angle = 0; angle <= TWO_PI; angle += angleStep) {
        float x = ball.x + cos(angle) * ballRadius;
        float y = ball.y + sin(angle) * ballRadius;
        glVertex3f(x, y, 0);
    }
    glEnd();

    // Desenhar os blocos
    for (const Block& block : blocks) {
        glPushMatrix();

        int row = static_cast<int>(block.y / blockHeight);

        // Define a cor da linha de blocos
        if (row == 0) loadMaterial(2); //glColor3f(0.39, 0.011, 0.011); // Castanho
        else if (row == 1) loadMaterial(3); //glColor3f(0.8, 0, 0); // Laranja
        else if (row == 2) loadMaterial(4); //glColor3f(0.8, 0, 0.4); // Rosa
        else if (row == 3) loadMaterial(5); //glColor3f(0.011, 0.39, 0.26); // Verde
        else if (row == 4) loadMaterial(6); //glColor3f(0, 0.6, 0.6); // Azul

        glTranslatef(block.x - gw() * 0.5 + block.width / 2, gh() * 0.5 - block.y - block.height / 2, -50);
        glScalef(block.width, block.height, 20);
        drawCube(); 

        glPopMatrix();
    }

    // Partículas
    for (const Particle& p : particles) {
        //glColor3f(p.color.r, p.color.g, p.color.b);
		loadMaterial(0);
        float size = 3.0f; // Tamanho do triângulo
        glBegin(GL_TRIANGLES);
        glVertex3f(p.pos.x, p.pos.y - size, 0);     // Vértice superior
        glVertex3f(p.pos.x - size, p.pos.y + size, 0); // Vértice inferior esquerdo
        glVertex3f(p.pos.x + size, p.pos.y + size, 0); // Vértice inferior direito
        glEnd();
    }

    // Mensagem "Press space to start"
    if (ballSpeed == 0) {
        std::string message = "To start";
        std::string dificultyMessage = "Press SPACE for easy mode, press 'h' for hard mode";
        std::string CurrentScoreMessage = "Last Score: " + std::to_string(lastScore);
        std::string highScoreMessage = "Best score: " + std::to_string(highScore);

        //float x = -message.length() * 4; // Centralizado
        //float y = gh() * 0.25;

        //ofSetColor(204, 0, 51);
        loadMaterial(1);
        ofDrawBitmapString(message, -60, 0);
        ofDrawBitmapString(dificultyMessage, - 200,  - 20);
        ofDrawBitmapString(CurrentScoreMessage, -60,  - 40);
        ofDrawBitmapString(highScoreMessage, -60,  - 60);
    }
    else {
        std::string message = "Score: " + std::to_string(currentScore);
        //ofSetColor(204, 1024, 150);
		loadMaterial(1);
        ofDrawBitmapString(message, -gw() * 0.5 + 10, gh() * 0.5 - 20);
    }

    if (gameWin) {
        std::string message = "You Win!!!";
        float x = -message.length() * 8 / 2; // Centralizado
        float y = gh() * 0.25;              
        //ofSetColor(204, 0, 51);
        loadMaterial(1);
        ofDrawBitmapString(message, -60,  20);
    }

}

void Movement::keyPressed(int key)
{
    if (key == OF_KEY_LEFT) moveLeft = true;
    if (key == OF_KEY_RIGHT) moveRight = true;
    if (!ballMoving) {
        if (key == ' ') { ballSpeed = 8; ballMoving = true; }
        if (key == 'h') { ballSpeed = 12; ballMoving = true; }
    }
    if (key == 'r') resetPhysics();

    if (key == 'p' || key == 'P') {
        perspectiveEnabled++;
        if (perspectiveEnabled > 4) {
            perspectiveEnabled = 0;
        }
    }
	if (key == 'a' || key == 'A') {
		ambientOn = !ambientOn;
	}
	if (key == 'd' || key == 'D') {
		dirOn = !dirOn;
	}
	if (key == 'l' || key == 'L') {
		pointOn = !pointOn;
	}
	if (key == 's' || key == 'S') {
		spotOn = !spotOn;
	}
	if (key == '1') {
		boolDirAmb = !boolDirAmb;
	}
	if (key == '2') {
		boolDirDif = !boolDirDif;
	}
	if (key == '3') {
		boolDirSpec = !boolDirSpec;
	}
	if (key == '4') {
		boolPointAmb = !boolPointAmb;
	}
	if (key == '5') {
		boolPointDif = !boolPointDif;
	}
	if (key == '6') {
		boolPointSpec = !boolPointSpec;
	}
	if (key == '7') {
		boolSpotAmb = !boolSpotAmb;
	}
	if (key == '8') {
		boolSpotDif = !boolSpotDif;
	}
	if (key == '9') {
		boolSpotSpec = !boolSpotSpec;
	}


}

void Movement::keyReleased(int key)
{
    if (key == OF_KEY_LEFT) moveLeft = false;
    if (key == OF_KEY_RIGHT) moveRight = false;
}

void Movement::mouseMoved(int x,int y) {
    paddleX = ofMap(x, 0, ofGetWidth(), -gw() * 0.5, gw() * 0.5 - paddleWidth);

    
    paddleX = ofClamp(paddleX, -gw() * 0.5, gw() * 0.5 - paddleWidth);
}


void Movement::mousePressed(int x, int y) {
    if (ballSpeed == 0) {
        ballSpeed = 8;
    }
}


void Movement::checkCollision() {
    bool directionChanged = false; // Prevenir o caso em que a bola bate em dois blocos ao mesmo tempo

    for (auto it = blocks.begin(); it != blocks.end();) {
        Block& block = *it; // Obter o bloco atual

        // Ajustar as coordenadas do bloco para o sistema do OpenFrameworks
        float blockTop = gh() / 2 - block.y;                 // Topo do bloco
        float blockBottom = gh() / 2 - (block.y + block.height); // Base do bloco
        float blockLeft = block.x - gw() / 2;               // Lado esquerdo do bloco
        float blockRight = block.x + block.width - gw() / 2; // Lado direito do bloco
        float blockNear = block.z - 10;                              // Frente do bloco
		float blockFar = block.z + 30;						  // Trás do bloco

        // Verificar colisão da bola com o bloco
        if (ball.x + ballRadius > blockLeft &&
            ball.x - ballRadius < blockRight &&
            ball.y + ballRadius > blockBottom &&
            ball.y - ballRadius < blockTop &&
            ball.z + ballRadius > blockNear &&
            ball.z - ballRadius < blockFar) {

            if (!directionChanged) {
                // Verificar de qual lado houve a colisão
                if (ball.x - ballRadius < blockLeft || ball.x + ballRadius > blockRight) {
                    ballDir.x *= -1; // Reflexão horizontal
                }
                else if (ball.y - ballRadius < blockBottom || ball.y + ballRadius > blockTop) {
                    ballDir.y *= -1; // Reflexão vertical
                }
                else {
					ballDir.z *= -1; // Reflexão em profundidade
                }
                directionChanged = true;
            }

            currentScore += 1;

            float blockCenterX = block.x - gw() / 2 + block.width * 0.5f; // Centro X do bloco
            float blockCenterY = gh() / 2 - block.y - block.height * 0.5f; // Centro Y do bloco
            float blockCenterZ = block.z; // Centro Z do bloco;

            int numParticles = 20; // Número de partículas
            for (int i = 0; i < numParticles; i++) {
                Particle p;
                p.pos = ofVec3f(blockCenterX, blockCenterY, 0); // Centro do bloco ajustado
                p.vel = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), 0); // Velocidade aleatória
                p.color = { 255, 0, 0 }; // Cor vermelha
                p.life = particleLife; // Tempo de vida das partículas
                particles.push_back(p); // Adicionar partícula à lista
            }

            // Remover o bloco atingido
            it = blocks.erase(it);
        }
        else {
            ++it; // Avançar para o próximo bloco
        }

    }
}


