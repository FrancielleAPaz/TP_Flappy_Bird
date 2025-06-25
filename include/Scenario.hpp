#pragma once

#include <memory>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

//#include "Game.hpp"
#include "Bird.hpp"
#include "Pipe.hpp"
//#include "PlayerManager.hpp"

class Game; 
enum class GameState;


class Scenario {
private:
    std::shared_ptr<Bird> bird;
    std::vector<std::shared_ptr<Pipe>> pipes;

    /*Variáveis bases da dinâmica do jogo: pontuação, gravidade, velocidade de movimento dos pipes,
     espaço entre os pipes, contador de pipes por frame e um detector de pontuação bool.
     */
    int score;
    float gravity;
    float pipeSpeed;
    float spaceBetweenPipes;
    int pipeSpawnCounter;
    int lastGapY;

    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* logo;
    ALLEGRO_BITMAP* gameover_image;
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* button_play;
    ALLEGRO_BITMAP* topPipeImage;
    ALLEGRO_BITMAP* bottomPipeImage;
    ALLEGRO_BITMAP* birdImage;

    // Coordenadas e tamanhos do button_play
    float playBtnX = 440;
    float playBtnY = 200;
    float playBtnW = 120;
    float playBtnH = 60;


public:
    Scenario();
    ~Scenario();

    std::shared_ptr<Bird> getBird() const;
    ALLEGRO_FONT* getFont() const;
    ALLEGRO_BITMAP* getBackground() const;
    bool updateAndCheckScore();
    void draw(GameState state);
    void reset();
    void addPipe();
    bool checkCollision();
    void setDifficulty(float gravity, float pipeSpeed);
    int getScore() const;
    bool carregarRecursos(); // Retorna true se tudo carregou corretamente
    bool isPlayButtonClicked(int mouseX, int mouseY) const;

};
