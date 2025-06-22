#include "GameObject.hpp"
#include "Bird.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Pipe : public GameObject {
private:
    bool top;                   // Indica se é o cano superior
    float speed;                // Velocidade horizontal
    ALLEGRO_BITMAP* image;      // Imagem do cano (específica para topo ou base)
    static const float WIDTH;   // Largura padrão do cano

public:
    Pipe(float x, float y, bool isTop, ALLEGRO_BITMAP* pipeImage);
    
    // Métodos obrigatórios
    void update() override;
    void draw() const override;
    
    // Verifica colisão APENAS com Bird
    bool checkCollision(const Bird& bird) const;
    
    // Métodos específicos
    bool isOffScreen() const;
    bool isTop() const;
    
    // Getters
    float getWidth() const;
    float getHeight() const;
    
    // Setter para velocidade
    void setSpeed(float s);
};