#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Scenario.hpp"
#include "PlayerManager.hpp"
#include <string>

enum class GameState {
    INICIO,
    JOGANDO,
    PERDEU
};

class Game {
private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_SAMPLE* som_pulo;
    ALLEGRO_SAMPLE* som_morte;
    ALLEGRO_AUDIO_STREAM* trilha;

    bool running;

    GameState currentState;
    Scenario scenario;
    PlayerManager playerManager;
    std::string jogadorAtual;

public:
    Game();
    ~Game();

    void run(const std::string& apelido);
    void handleInput(int);
    void showGameOver();
    void startNewRound();
};
