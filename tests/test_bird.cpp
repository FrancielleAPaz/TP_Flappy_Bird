#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Bird.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

struct GlobalAllegroSetup {
    GlobalAllegroSetup() {
        al_init();
        al_init_image_addon();
    }
} globalAllegro;

ALLEGRO_BITMAP* dummyBitmap = al_create_bitmap(1,1);

TEST_CASE("Bird - Jump e Reset") {
    REQUIRE(dummyBitmap != nullptr);

    Bird b(100.0f, 300.0f, dummyBitmap);
    float yAntes = b.getY();

    b.jump();
    b.update();
    CHECK(b.getY() < yAntes);

    b.reset();
    CHECK(b.getY() == 300.0f);
}
