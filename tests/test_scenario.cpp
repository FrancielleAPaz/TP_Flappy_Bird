#include "doctest.h"
#include "Scenario.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>

struct GlobalAllegroSetup {
    GlobalAllegroSetup() {
        if (!al_init()) std::cerr << "[ERRO] Falha ao inicializar Allegro!" << std::endl;
        al_init_image_addon();
        al_init_font_addon();
        al_init_ttf_addon();
        al_init_primitives_addon();
        al_install_keyboard();
        al_install_audio();
        al_init_acodec_addon();
    }
};

static GlobalAllegroSetup globalAllegro;

class SafeScenario : public Scenario {
public:
    SafeScenario() {}
    void manualInit() { carregarRecursos(); }
};

TEST_CASE("Scenario - carregarRecursos e getScore") {
    SafeScenario s;
    s.manualInit();
    CHECK(s.getScore() == 0);
}

TEST_CASE("Scenario - setDifficulty sem crash") {
    SafeScenario s;
    s.manualInit();
    s.setDifficulty(2.0f, 3.0f);
    CHECK(true);
}

TEST_CASE("Scenario - addPipe e reset") {
    SafeScenario s;
    s.manualInit();
    s.addPipe();
    s.reset();
    CHECK(s.getScore() == 0);
}
