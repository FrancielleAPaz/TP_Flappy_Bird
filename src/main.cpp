#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "Game.hpp"

int main() {

    std::string apelido;
    std::cout << "Digite seu apelido: ";
    std::getline(std::cin, apelido);
    
    Game jogo;
    jogo.run(apelido);  // apelido fictício ou qualquer string
    

    return 0;
}
