#include "GameObject.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Bird : public GameObject {
private:
    float velocityY;          // Velocidade vertical
    bool isAlive;             // Estado do pássaro
    ALLEGRO_BITMAP* sprite;   // Sprite do pássaro (com 3 frames)
    float frameWidth;         // Largura de cada frame
    float height;             // Altura do pássaro
    int currentFrame;         // Frame atual da animação (0, 1, 2)
    double lastFrameTime;     // Tempo do último frame
    float animationSpeed;     // Velocidade da animação

public:
    Bird(float startX, float startY, ALLEGRO_BITMAP* sprite);
    
    // Métodos necessários
    void applyGravity(float gravity);
    void update() override;
    void draw() const override;
    bool checkCollision(const GameObject& other) const override;
    
    // Controle do pássaro
    void jump();
    void reset();
    
    // Getters
    bool isDead() const;
    float getVelocityY() const;
    float getWidth() const;
    float getHeight() const;
    
    // Configuração da animação
    void setAnimationSpeed(float speed);
};
