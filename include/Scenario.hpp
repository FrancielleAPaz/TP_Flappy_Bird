#pragma once

#include <memory>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "Bird.hpp"
#include "Pipe.hpp"
#include "PlayerManager.hpp"

class Scenario {
private:
    std::shared_ptr<Bird> bird;
    std::vector<std::shared_ptr<Pipe>> pipes;

    int score;
    float gravity;
    float pipeSpeed;
    float spaceBetweenPipes;
    int pipeSpawnCounter;
    bool addScoreFlag;

    ALLEGRO_FONT* font;

public:
    Scenario();
    ~Scenario();

    void update();
    void draw();
    void reset();
    void addPipe();
    bool checkCollision();
    void setDifficulty(float gravity, float pipeSpeed);
    int getScore() const;
};
