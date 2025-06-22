#include "Pipe.hpp"

// Largura padrão do cano
const float Pipe::WIDTH = 80.0f;

// Construtor
Pipe::Pipe(float x, float y, bool isTop, ALLEGRO_BITMAP* pipeImage) 
    : GameObject(x, y), top(isTop), image(pipeImage), speed(0) {}

// Atualiza a posição do cano
void Pipe::update() {
    x += speed;
}

// Desenha o cano (sem fallback)
void Pipe::draw() const {
    al_draw_bitmap(image, x, y, 0);
}

// Verifica colisão com o pássaro
bool Pipe::checkCollision(const Bird& bird) const {
    // Calcula os limites do pássaro
    float birdLeft = bird.getX();
    float birdRight = bird.getX() + bird.getWidth();
    float birdTop = bird.getY();
    float birdBottom = bird.getY() + bird.getHeight();
    
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

// Retorna a altura do cano
float Pipe::getHeight() const {
    return al_get_bitmap_height(image);
}

// Define a velocidade do cano
void Pipe::setSpeed(float s) {
    speed = s;
}