#include "GameObject.hpp"

// Construtor
GameObject::GameObject(float startX, float startY) : x(startX), y(startY) {}

//Retorna o valor de x
float GameObject::getX() const {
    return x;
}

//Retorna o valor de y
float GameObject::getY() const {
    return y;
}

