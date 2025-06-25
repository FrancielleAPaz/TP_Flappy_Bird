#include "Bird.hpp"
#include "Pipe.hpp"
#include <allegro5/allegro.h>
#include <iostream>
#include <stdexcept>  // Para std::invalid_argument e std::runtime_error
#include <cmath>      // Para std::isnan

// Construtor - sprite obrigatório com 3 frames
Bird::Bird(float startX, float startY, ALLEGRO_BITMAP* sprite) 
    : GameObject(startX, startY), velocityY(0), isAlive(true), sprite(sprite),
      currentFrame(0), lastFrameTime(0), animationSpeed(0.15f) {
    
    // Verificação defensiva: sprite deve ser válido
    if (sprite == nullptr) {
        throw std::invalid_argument("Sprite do passaro não pode ser nulo");
    }
    
    // Calcula dimensões do sprite (3 frames horizontais)
    int fullWidth = al_get_bitmap_width(sprite);
    frameWidth = fullWidth / 3.0f;
    height = al_get_bitmap_height(sprite);
    
    // Verificação defensiva: dimensões válidas para a animação
    if (fullWidth < 3) {
        throw std::invalid_argument("A largura do sprite do passaro tem que ser > que 3 pixels");
    }
    if (height <= 0) {
        throw std::invalid_argument("A altura do sprite tem que ser positiva");
    }
}

void Bird::applyGravity(float gravity) {
    // Verificação de parâmetro inválido
    if (std::isnan(gravity)) {
        throw std::invalid_argument("O valor da gravidade é nulo");
    }
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
        
        // Verificação defensiva: corrige tempo inválido
        if (lastFrameTime < 0) {
            lastFrameTime = currentTime;  // Reseta tempo inválido
        }
        
        if (currentTime - lastFrameTime > animationSpeed) {
            currentFrame = (currentFrame + 1) % 3; // Cicla entre 0, 1, 2
            lastFrameTime = currentTime;
        }
    }
}

void Bird::draw() const {
    // Verificação crítica antes de desenhar
    if (sprite == nullptr) {
        throw std::runtime_error("Não é possivel desenhar passaro: sprite nulo");
    }
    
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

void Bird::kill() {
    isAlive = false;
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
    } else {
        // Prevenção de estado inválido
        throw std::runtime_error("Não pode pular quando morto");
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
    // Validação de parâmetro
    if (speed <= 0.0f) {
        throw std::invalid_argument("A velocidade de animação tem que ser positva");
    }
    animationSpeed = speed;
}