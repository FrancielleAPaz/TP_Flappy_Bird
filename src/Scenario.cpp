#include "Scenario.hpp"
#include "Game.hpp" // para GameState
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>


// Construtor
Scenario::Scenario() {
    std::cout << "[INFO] Inicializando cenário..." << std::endl;

    ALLEGRO_BITMAP* birdImage = al_load_bitmap("../assets/birds.png");
    if (!birdImage) {
        std::cerr << "[ERRO] falha ao carregar birds.png\n";
        exit(1); // Aborta o programa imediatamente
    }
    bird = std::make_shared<Bird>(500, 300, birdImage);

    font = al_load_font("../assets/flappyfont.ttf", 36, 0);
    if (!font) std::cerr << "[ERRO] falha ao carregar flappyfont.ttf\n";

    background = al_load_bitmap("../assets/background.bmp");
    if (!background) std::cerr << "[ERRO] falha ao carregar background.bmp\n";

    logo = al_load_bitmap("../assets/logo.bmp");
    if (!logo) std::cerr << "[ERRO] falha ao carregar logo.bmp\n";

    button_play = al_load_bitmap("../assets/button_play.png");
    if (!button_play) std::cerr << "[ERRO] falha ao carregar button_play.png\n";

    gameover_image = al_load_bitmap("../assets/gameover.png");
    if (!gameover_image) std::cerr << "[ERRO] falha ao carregar gameover.png\n";

    floor_image = al_load_bitmap("../assets/ground.png");
    if (!floor_image) std::cerr << "[ERRO] falha ao carregar ground.png\n";

    topPipeImage = al_load_bitmap("../assets/tunnel_up.png");
    if (!topPipeImage) std::cerr << "[ERRO] falha ao carregar tunnel_up.png\n";

    bottomPipeImage = al_load_bitmap("../assets/tunnel_down.png");
    if (!bottomPipeImage) std::cerr << "[ERRO] falha ao carregar tunnel_down.png\n";

    std::cout << "[INFO] Cenário carregado com sucesso (se nenhum erro acima apareceu).\n";

    score = 0;
    gravity = 0.5f;
    pipeSpeed = 4.0f;
    spaceBetweenPipes = 220.f;
    pipeSpawnCounter = 0;
    addScoreFlag = false;
    cx1 = 0;
    cx2 = 1000;
}


// Destrutor
Scenario::~Scenario() {
    if (font) al_destroy_font(font);
    if (background) al_destroy_bitmap(background);
    if (logo) al_destroy_bitmap(logo);
    if (button_play) al_destroy_bitmap(button_play);
    if (gameover_image) al_destroy_bitmap(gameover_image);
    if (floor_image) al_destroy_bitmap(floor_image);
    if (topPipeImage) al_destroy_bitmap(topPipeImage);
    if (bottomPipeImage) al_destroy_bitmap(bottomPipeImage);

}

// Retorna o ponteiro para o pássaro
std::shared_ptr<Bird> Scenario::getBird() const {
    return bird;
}

// Atualiza todos os objetos e lógica da partida
void Scenario::update() {
    bird->applyGravity(gravity);
    bird->update();

    if (bird->isDead()) {
    return; // não atualiza mais cenário se o pássaro morreu
    }

    // Move canos e verifica passagem para score
    for (size_t i = 0; i < pipes.size(); ++i) {
        pipes[i]->update();

        // Adiciona ponto se pássaro passou pelo cano
        if (!addScoreFlag &&
            pipes[i]->getX() + pipes[i]->getWidth() < bird->getX()) {
            score++;
            addScoreFlag = true;
        }
    }

    // Remove canos que saíram da tela
    pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
        [](std::shared_ptr<Pipe> p) {
            return p->getX() + p->getWidth() < 0;
        }), pipes.end());

    // Gera novos canos
    pipeSpawnCounter++;
    if (pipeSpawnCounter >= 120) {
        addPipe();
        pipeSpawnCounter = 0;
        addScoreFlag = false;
    }

    // Atualiza chão (scroll horizontal)
    cx1 -= pipeSpeed;
    cx2 -= pipeSpeed;
    if (cx1 <= -1000) cx1 = 1000;
    if (cx2 <= -1000) cx2 = 1000;
}

// Desenha os elementos do jogo
void Scenario::draw(GameState state) {
    al_draw_bitmap(background, 0, 0, 0);

    switch (state) {
    case GameState::INICIO:
        al_draw_scaled_bitmap(logo, 0, 0, 95, 28, 400, 100, 180, 58, 0);
        al_draw_scaled_bitmap(button_play, 0, 0, 366, 204, 440, 200, 120, 60, 0);
        break;

    case GameState::JOGANDO:
        for (auto& pipe : pipes)
            pipe->draw();
        bird->draw();
        al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %d", score);
        break;

    case GameState::PERDEU:
        for (auto& pipe : pipes)
            pipe->draw();
        bird->draw();
        al_draw_bitmap(gameover_image, 350, 200, 0);
        al_draw_scaled_bitmap(button_play, 0, 0, 366, 204, 440, 350, 120, 60, 0);
        al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %d", score);
        break;
    }

    // Chão em movimento
    al_draw_bitmap(floor_image, cx1, 500, 0);
    al_draw_bitmap(floor_image, cx2, 500, 0);

    al_flip_display();
}

// Reinicia o cenário para nova partida
void Scenario::reset() {
    pipes.clear();
    score = 0;
    pipeSpawnCounter = 0;
    addScoreFlag = false;
    bird->reset();
    cx1 = 0;
    cx2 = 1000;
}

// Adiciona um novo par de canos na tela
void Scenario::addPipe() {
    int alturaAleatoria = 150 + (std::rand() % 200); // entre 150 e 350

    // Carrega as imagens dos canos (apenas uma vez)
    auto pipeTop = std::make_shared<Pipe>(1000, alturaAleatoria - spaceBetweenPipes, true, topPipeImage);
    auto pipeBottom = std::make_shared<Pipe>(1000, alturaAleatoria, false, bottomPipeImage);

    // Configura a velocidade
    pipeTop->setSpeed(-pipeSpeed);
    pipeBottom->setSpeed(-pipeSpeed);

    pipes.push_back(pipeTop);
    pipes.push_back(pipeBottom);
}

// Verifica colisões entre o pássaro e os canos
bool Scenario::checkCollision() {
    for (const auto& pipe : pipes) {
        if (bird->checkCollision(*pipe)) {
            bird->kill();  // marca o pássaro como morto
            return true;
        }
    }

    if (bird->getY() < 0 || bird->getY() + bird->getHeight() > 600) {
        bird->kill();  // morreu ao tocar no chão ou teto
        return true;
    }

    return false;
}

// Define configurações como gravidade e velocidade dos obstáculos
void Scenario::setDifficulty(float g, float speed) {
    gravity = g;
    pipeSpeed = speed;
}

// Retorna a pontuação atual
int Scenario::getScore() const {
    return score;
}


// Verifica o clique do mouse na tela
bool Scenario::isPlayButtonClicked(int mouseX, int mouseY) const {
    return mouseX >= playBtnX && mouseX <= playBtnX + playBtnW &&
           mouseY >= playBtnY && mouseY <= playBtnY + playBtnH;
}

