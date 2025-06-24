#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Bird.hpp"

ALLEGRO_BITMAP* dummyBitmap = nullptr;
TEST_CASE("Bird - Jump e Reset")
{
    Bird b(100.0f, 300.0f, dummyBitmap);
    float yAntes = b.getY();
    b.jump();
    CHECK(b.getY() < yAntes);
    b.reset();
    CHECK(b.getY() == 300);
}