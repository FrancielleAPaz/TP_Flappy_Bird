#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <allegro5/allegro.h>
#include "doctest.h"
#include "Scenario.hpp"

ALLEGRO_BITMAP* dummyBitmap = nullptr;
TEST_CASE("Scenario - Incialização e Score")
{
    Scenario s;
    CHECK(s.getScore() == 0);
}

TEST_CASE("Scenario - Adicionar Pipe e Reset")
{
    Scenario s;
    s.addPipe();
    CHECK(true);
    s.reset();
    CHECK(s.getScore() == 0);
}

TEST_CASE("Scenario - Set Difficulty")
{
    Scenario s;
    s.setDifficulty(2.0f,3.0f);
    CHECK(true);
}