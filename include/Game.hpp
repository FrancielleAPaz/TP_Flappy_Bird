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
    ALLEGRO_SAMPLE* som_score;

    bool running;

    GameState currentState;
    Scenario scenario;
    PlayerManager playerManager;
    std::string jogadorAtual;
    std::string nomeDigitado = "";
    bool nomeConfirmado = false;

    std::string opcaoDigitada = "";
    char opcaoEscolhida = '\0';
    bool opcaoConfirmada = false;
    std::string mensagemMenu = ""; // Para mensagens como "Jogador criado" ou "Jogador não encontrado"

    std::string apelidoParaRemover;
    bool remocaoConfirmada = false;

    bool showCursor = true;  
    double cursorTimer = 0.0; 

    const double CURSOR_BLINK_RATE = 0.5;

    void telaInserirJogador();
    void telaRemoverJogador();
    void telaMostrarPlacar();
    void Menu();


public:
    Game();
    ~Game();

    void run();
    void handleInput(int);
    void showGameOver();
    void startNewRound();
};
