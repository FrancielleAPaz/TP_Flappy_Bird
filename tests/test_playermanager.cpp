#include <allegro5/allegro.h>
#include "doctest.h"
#include "../include/PlayerManager.hpp"

extern ALLEGRO_BITMAP* dummyBitmap;
TEST_CASE("PlayerManager - Registrar e Buscar")
{
    PlayerManager pm;
    CHECK(pm.registrarPlayer("Gabriela", "Gabi"));
    Player* p = pm.buscarPlayer("Gabi");
    REQUIRE (p != nullptr);
    CHECK (p->getNome() == "Gabriela");
}

TEST_CASE("PlayerManager - Remover jogador")
{
    PlayerManager pm;
    pm.registrarPlayer("Lorena", "Lore");
    CHECK(pm.removerPlayer("Lore"));
    CHECK(pm.buscarPlayer("Lore") == nullptr);

}

TEST_CASE("PlayerManager - Atualizar pontuação")
{
    PlayerManager pm;
    pm.registrarPlayer("Camila", "Mila");
    pm.atualizarPontuacao("Mila",30);
    Player* p = pm.buscarPlayer("Mila");
    REQUIRE (p != nullptr);
    CHECK (p->getPontuacaoMax() == 30);
}

TEST_CASE("PlayerManage - Top Player")
{
    PlayerManager pm;
    pm.registrarPlayer("Gabriel", "Biel");
    pm.registrarPlayer("Fernanda", "Nanda");

    pm.atualizarPontuacao("Biel",15);
    pm.atualizarPontuacao("Nanda",20);
    
    

}