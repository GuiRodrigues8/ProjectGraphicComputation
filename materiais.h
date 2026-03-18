#ifndef MATERIALS_H
#define MATERIALS_H

#include "ofMain.h"

// Array de nomes de materiais para fácil referência
inline extern const char* Materiais[] = {
    "red", "text", "castanho", "laranja", "rosa", "verde", "azul", "barra"
};

// Função para carregar materiais com base em um índice
inline void loadMaterial(int material) {
    switch (material) {
    case 0: // Red
    {
        GLfloat ambient[] = { 0.7, 0.0, 0.0, 1.0 };
        GLfloat diffuse[] = { 0.6, 0.0, 0.0, 1.0 };
        GLfloat specular[] = { 0.8, 0.6, 0.6, 1.0 };
        GLfloat shininess = 32.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    case 1: // Text
    {
        GLfloat ambient[] = { 0.7, 0.0f, 0.3, 1.0f }; // Cor convertida para intervalo [0,1]
        GLfloat diffuse[] = { 0.7, 0.0f, 0.3, 1.0f };
        GLfloat specular[] = { 0.8f, 0.6f, 0.6f, 1.0f };  // Reflexão especular padrão
        GLfloat shininess = 32.0f; // Brilho padrão
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    case 2: // Castanho
    {
        GLfloat ambient[] = { 0.39, 0.011, 0.011, 1.0 };
        GLfloat diffuse[] = { 0.78, 0.022, 0.022, 1.0 };
        GLfloat specular[] = { 0.8, 0.6, 0.6, 1.0 };
        GLfloat shininess = 32.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    case 3: // Laranja
    {
        GLfloat ambient[] = { 0.8, 0.0, 0.0, 1.0 };
        GLfloat diffuse[] = { 0.8, 0.0, 0.0, 1.0 };
        GLfloat specular[] = { 0.8, 0.6, 0.6, 1.0 };
        GLfloat shininess = 32.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    case 4: // Rosa
    {
        GLfloat ambient[] = { 0.8, 0.0, 0.4, 1.0 };
        GLfloat diffuse[] = { 0.8, 0.0, 0.4, 1.0 };
        GLfloat specular[] = { 0.8, 0.6, 0.6, 1.0 };
        GLfloat shininess = 32.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    case 5: // Verde
    {
        GLfloat ambient[] = { 0.011, 0.39, 0.26, 1.0 };
        GLfloat diffuse[] = { 0.022, 0.78, 0.52, 1.0 };
        GLfloat specular[] = { 0.8, 0.6, 0.6, 1.0 };
        GLfloat shininess = 32.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    case 6: // Azul
    {
        GLfloat ambient[] = { 0.0, 0.6, 0.6, 1.0 };
        GLfloat diffuse[] = { 0.0, 0.6, 0.6, 1.0 };
        GLfloat specular[] = { 0.8, 0.6, 0.6, 1.0 };
        GLfloat shininess = 32.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    case 7: // Barra
    {
        GLfloat ambient[] = { 0.3, 0.6, 0.0, 1.0 };  // Cor verde
        GLfloat diffuse[] = { 0.0, 0.0, 0.0, 1.0 };  // Sem cor difusa
        GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 }; // Sem reflexão especular
        GLfloat shininess = 0.0;                       // Sem brilho
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        break;
    }
    default:
        break;
    }
}

#endif
