#include "doctest.h"
#include "Bird.hpp"

ALLEGRO_BITMAP* dummyBitmap = nullptr;
TEST_CASE("Bird - Jump e Reset")
{
    REQUIRE(al_init());
    REQUIRE(al_init_image_addon());

    dummyBitmap = al_create_bitmap(1,1);
    REQUIRE(dummyBitmap != nullptr);


    Bird b(100.0f, 300.0f, dummyBitmap);
    float yAntes = b.getY();
    b.jump();
    b.update();
    CHECK(b.getY() < yAntes);
    b.reset();
    CHECK(b.getY() == 300);

    al_destroy_bitmap(dummyBitmap);
}