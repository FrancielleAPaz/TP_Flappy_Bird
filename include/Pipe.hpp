#pragma once

#include "GameObject.hpp"
#include "Bird.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Pipe : public GameObject {
private:
    bool top;                   // Indica se é o cano superior
    ALLEGRO_BITMAP* image;      // Imagem do cano (específica para topo ou base)    
    float speed;                // Velocidade horizontal
    static const float WIDTH;   // Largura padrão do cano
    bool scored; // NOVO: Flag para rastrear se este cano já gerou um ponto


public:
    Pipe(float x, float y, bool isTop, ALLEGRO_BITMAP* pipeImage);
    
    // Métodos obrigatórios
    void update() override;
    void draw() const override;
    
    // Verifica colisão APENAS com Bird
    bool checkCollision(const GameObject& other) const override;    
    // Métodos específicos
    bool isOffScreen() const;
    bool isTop() const;
    
    // Getters
    float getWidth() const;
    float getHeight() const;
    
    // Setter para velocidade
    void setSpeed(float s);

    // NOVO: Getters e Setters para a flag 'scored'
    bool getScored() const { return scored; }
    void setScored(bool s) { scored = s; }
};