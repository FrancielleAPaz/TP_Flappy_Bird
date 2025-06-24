#include "doctest.h"
#include "Pipe.hpp"

extern ALLEGRO_BITMAP* dummyBitmap;
TEST_CASE("Pipe - Movimento")
{
    Pipe p(100.0f, 500.0f, false, dummyBitmap);
    p.setSpeed(-5.0f);
    float xAntes = p.getX();
    p.update();
    CHECK(p.getX() < xAntes);
}

TEST_CASE("Pipe - Is Off Screen")
{
    Pipe p(-200.0f, 0.0f, false, dummyBitmap);
    CHECK(p.isOffScreen()==true);
}

TEST_CASE("Pipe - Is Top()")
{
    Pipe pTopo(0.0f, 600.0f, true, dummyBitmap);
    Pipe pBase(0.0f, 600.0f, false, dummyBitmap);

    CHECK(pTopo.isTop() == true);
    CHECK(pBase.isTop() == false);
}
