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

    // Somente valores iniciais
    score = 0;
    gravity = 0.5f;
    pipeSpeed = 4.0f;
    spaceBetweenPipes = 180.f;
    pipeSpawnCounter = 0;
    lastGapY = 250;

}
// carregando todos os recursos em uma unica função
bool Scenario::carregarRecursos() {
    font = al_load_font("./assets/flappyfont.ttf", 36, 0);
    background = al_load_bitmap("./assets/background.png");
    logo = al_load_bitmap("./assets/logo.bmp");
    button_play = al_load_bitmap("./assets/button_play.png");
    gameover_image = al_load_bitmap("./assets/gameover.png");
    topPipeImage = al_load_bitmap("./assets/tunnel_up.png");
    bottomPipeImage = al_load_bitmap("./assets/tunnel_down.png");
    birdImage = al_load_bitmap("./assets/birds.png");

    if (!font || !background || !logo || !button_play || !gameover_image || !topPipeImage || !bottomPipeImage || !birdImage) {
        std::cerr << "[ERRO] Falha ao carregar um ou mais recursos gráficos do jogo.\n";
        if (!birdImage) std::cerr << "[ERRO] Falha ao carregar birds.png\n";
        return false;
    }

    bird = std::make_shared<Bird>(500, 300, birdImage);

    return true;
}

// Destrutor
Scenario::~Scenario() {
    if (font) al_destroy_font(font);
    if (background) al_destroy_bitmap(background);
    if (logo) al_destroy_bitmap(logo);
    if (button_play) al_destroy_bitmap(button_play);
    if (gameover_image) al_destroy_bitmap(gameover_image);
    if (topPipeImage) al_destroy_bitmap(topPipeImage);
    if (bottomPipeImage) al_destroy_bitmap(bottomPipeImage);
    if (birdImage) al_destroy_bitmap(birdImage);
}

// Retorna o ponteiro para o pássaro
std::shared_ptr<Bird> Scenario::getBird() const {
    return bird;
}

ALLEGRO_FONT* Scenario::getFont() const {
    return font;  // Retorna o ponteiro da fonte carregada
}

ALLEGRO_BITMAP* Scenario::getBackground() const {
    return background; // Retorna o ponteiro para o bitmap do background
}

// Atualiza todos os objetos e lógica da partida e retorna se um ponto foi adicionado
bool Scenario::updateAndCheckScore() {
    bool scoreIncreasedThisFrame = false; // Flag para indicar se a pontuação aumentou neste frame

    bird->applyGravity(gravity);
    bird->update();

    if (bird->isDead()) {
        return false; // Não atualiza mais cenário se o pássaro morreu, e não há pontuação
    }

    // Move canos e verifica passagem para score
    for (size_t i = 0; i < pipes.size(); i += 2) {
        pipes[i]->update();
        if (i + 1 < pipes.size()) { // Garante que há um cano inferior
            pipes[i+1]->update();
        }

        // Adiciona ponto se pássaro passou pelo cano E este cano ainda não foi pontuado
        if (!pipes[i]->getScored() && bird->getX() > pipes[i]->getX() + pipes[i]->getWidth()) {
            score++;
            pipes[i]->setScored(true); // Marca o cano como pontuado
            if (i + 1 < pipes.size()) {
                pipes[i+1]->setScored(true); // Marca o cano inferior também para evitar futuras verificações
            }
            scoreIncreasedThisFrame = true; // Sinaliza que a pontuação aumentou neste frame
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
    }

    return scoreIncreasedThisFrame; // Retorna se um ponto foi feito neste frame
}

// Desenha os elementos do jogo
void Scenario::draw(GameState state) {
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_bitmap(background, 480, 0, 0);

    switch (state) {
    case GameState::INICIO:
        al_draw_scaled_bitmap(logo, 0, 0, 95, 28, 340, 160, 270, 87, 0);
        al_draw_scaled_bitmap(button_play, 0, 0, 366, 204, 380, 250, 200, 100, 0);
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
        al_draw_bitmap(gameover_image, 315, 200, 0);
        al_draw_scaled_bitmap(button_play, 0, 0, 366, 204, 419, 290, 120, 60, 0);
        al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %d", score);
        break;
    }

    al_flip_display();
}

// Reinicia o cenário para nova partida
void Scenario::reset() {
    pipes.clear();
    score = 0;
    pipeSpawnCounter = 0;
    //addScoreFlag = false;
    bird->reset();
}

// Adiciona um novo par de canos na tela
void Scenario::addPipe() {
    int delta = (std::rand() % 361) - 180;  // [-180, 180]
    int gapY = lastGapY + delta;

    // Agora gapY representa o centro do buraco
    int gapSize = static_cast<int>(spaceBetweenPipes);

    // Limites para evitar que o buraco fique fora da tela
    int minGapY = gapSize / 2 + 30;
    int maxGapY = 600 - gapSize / 2 - 30;

    if (gapY < minGapY) gapY = minGapY;
    if (gapY > maxGapY) gapY = maxGapY;

    lastGapY = gapY;

    auto pipeTop = std::make_shared<Pipe>(
        960,
        gapY - gapSize / 2 - al_get_bitmap_height(topPipeImage),
        true,
        topPipeImage
    );

    auto pipeBottom = std::make_shared<Pipe>(
        960,
        gapY + gapSize / 2,
        false,
        bottomPipeImage
    );

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
