#include "Bird.hpp"
#include "Pipe.hpp"
#include <allegro5/allegro.h>
#include <iostream>

// Construtor - sprite obrigatório com 3 frames
Bird::Bird(float startX, float startY, ALLEGRO_BITMAP* sprite) 
    : GameObject(startX, startY), velocityY(0), isAlive(true), sprite(sprite),
      currentFrame(0), lastFrameTime(0), animationSpeed(0.15f) {
    
    // Calcula dimensões do sprite (3 frames horizontais)
    int fullWidth = al_get_bitmap_width(sprite);
    frameWidth = fullWidth / 3.0f;
    height = al_get_bitmap_height(sprite);
}

void Bird::applyGravity(float gravity) {
    velocityY += gravity;
}

void Bird::update() {
    // Atualiza posição vertical
    y += velocityY;
    
    // Limita ao topo da tela
    if (y < 0) {
        y = 0;
        velocityY = 0;
    }
    
    // Atualiza animação - apenas se estiver vivo
    if (isAlive) {
        double currentTime = al_get_time();
        if (currentTime - lastFrameTime > animationSpeed) {
            currentFrame = (currentFrame + 1) % 3; // Cicla entre 0, 1, 2
            lastFrameTime = currentTime;
        }
    }
}

void Bird::draw() const {
    // Desenha o frame atual da animação
    al_draw_bitmap_region(
        sprite, 
        currentFrame * frameWidth, // região X do frame
        0,                        // região Y
        frameWidth, 
        height, 
        x, 
        y, 
        0
    );
}

bool Bird::checkCollision(const GameObject& other) const {
    // Delega a detecção para o outro objeto
    return other.checkCollision(*this);
}

void Bird::jump() {
    if (isAlive) {
        velocityY = -10.0f; // Força fixa de pulo
        
        // Reinicia animação para dar feedback visual
        currentFrame = 0;
        lastFrameTime = al_get_time();
    }
}

void Bird::reset() {
    y = 300; // Posição Y inicial
    velocityY = 0;
    isAlive = true;
    currentFrame = 0;
    lastFrameTime = al_get_time();
}

bool Bird::isDead() const {
    return !isAlive;
}

float Bird::getVelocityY() const {
    return velocityY;
}

float Bird::getWidth() const {
    return frameWidth;
}

float Bird::getHeight() const {
    return height;
}

void Bird::setAnimationSpeed(float speed) {
    animationSpeed = speed;
}