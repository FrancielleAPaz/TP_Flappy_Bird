#include "Pipe.hpp"
#include <stdexcept>  // Para std::invalid_argument, std::runtime_error
#include <cmath>      // Para std::isnan
#include <iostream>   // Para std::cerr


// Largura padrão do cano
const float Pipe::WIDTH = 80.0f;

// Construtor
Pipe::Pipe(float x, float y, bool isTop, ALLEGRO_BITMAP* pipeImage) 
    : GameObject(x, y), top(isTop), image(pipeImage), speed(0), scored(false) {}

// Atualiza a posição do cano
void Pipe::update() {
    x += speed;
}

// Desenha o cano (sem fallback)
void Pipe::draw() const {
    al_draw_bitmap(image, x, y, 0);
}

// Verifica colisão com o pássaro
bool Pipe::checkCollision(const GameObject& other) const {
    // Tenta converter o outro objeto para um Bird
    const Bird* bird = dynamic_cast<const Bird*>(&other);
    if (!bird) return false;  // Se não for um pássaro, sem colisão
    // Calcula os limites do pássaro
    float birdLeft = bird->getX();
    float birdRight = bird->getX() + bird->getWidth();
    float birdTop = bird->getY();
    float birdBottom = bird->getY() + bird->getHeight();
    
    // Calcula os limites do cano
    float pipeLeft = x;
    float pipeRight = x + WIDTH;
    float pipeTop = y;
    float pipeBottom = y + al_get_bitmap_height(image);
    
    // Verifica sobreposição
    return (birdRight > pipeLeft && 
            birdLeft < pipeRight && 
            birdBottom > pipeTop && 
            birdTop < pipeBottom);
}

// Verifica se o cano saiu da tela
bool Pipe::isOffScreen() const {
    return (x + WIDTH < 0);
}

// Indica se é o cano superior
bool Pipe::isTop() const {
    return top;
}

// Retorna a largura do cano
float Pipe::getWidth() const {
    return WIDTH;
}

// Retorna a altura do cano com verificação de recurso
float Pipe::getHeight() const {
    // Verificação crítica antes de acessar recurso
    if (image == nullptr) {
        throw std::runtime_error("Não foi possivel pegar a altura do cano: imagem nula");
    }
    return static_cast<float>(al_get_bitmap_height(image));
}

// Define a velocidade do cano com validação
void Pipe::setSpeed(float s) {
    // Validação de parâmetro
    if (std::isnan(s)) {
        throw std::invalid_argument("Velcoidade do cano não pode ser nan");
    }
    speed = s;
}