#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Pipe.hpp"

ALLEGRO_BITMAP* dummyBitmap = nullptr;
TEST_CASE("Pipe - Movimento")
{
    Pipe p(0.0f, 500.0f, false, dummyBitmap);
    float xAntes = p.getX();
    p.update();
    CHECK(p.getX() < xAntes);
}

TEST_CASE("Pipe - Is Off Screen")
{
    Pipe p(0.0f, -100.0f, false, dummyBitmap);
    CHECK(p.isOffScreen());
}

TEST_CASE("Pipe - Is Top()")
{
    Pipe pTopo(0.0f, 600.0f, true, dummyBitmap);
    Pipe pBase(0.0f, 600.0f, false, dummyBitmap);

    CHECK(pTopo.isTop() == true);
    CHECK(pBase.isTop() == false);
}
