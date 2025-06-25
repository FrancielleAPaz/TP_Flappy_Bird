#include "doctest.h"
#include "PlayerManager.hpp"

TEST_CASE("Player - Atributos e Pontuação") {
    Player p("Fernanda", "Nanda");
    CHECK(p.getNome() == "Fernanda");
    CHECK(p.getApelido() == "Nanda");
    CHECK(p.getNumPartidas() == 0);
    CHECK(p.getPontuacaoTotal() == 0);
    CHECK(p.getPontuacaoMax() == 0);

    p.adicionarPontuacao(50);
    CHECK(p.getNumPartidas() == 1);
    CHECK(p.getPontuacaoTotal() == 50);
    CHECK(p.getPontuacaoMax() == 50);

    p.adicionarPontuacao(30);
    CHECK(p.getNumPartidas() == 2);
    CHECK(p.getPontuacaoTotal() == 80);
    CHECK(p.getPontuacaoMax() == 50);
}

TEST_CASE("PlayerManager - Registrar, Buscar e Remover Player") {
    PlayerManager pm;

    CHECK(pm.registrarPlayer("Fernanda", "Nanda"));
    CHECK_FALSE(pm.registrarPlayer("Outro", "Nanda")); // apelido repetido

    Player* p = pm.buscarPlayer("Nanda");
    REQUIRE(p != nullptr);
    CHECK(p->getNome() == "Fernanda");

    pm.atualizarPontuacao("Nanda", 100);
    CHECK(p->getPontuacaoTotal() == 100);

    pm.atualizarPontuacao("Nanda", 50);
    CHECK(p->getPontuacaoTotal() == 150);

    CHECK(pm.removerPlayer("Nanda"));
    CHECK(pm.buscarPlayer("Nanda") == nullptr);
}

TEST_CASE("PlayerManager - Salvar e Carregar Arquivo") {
    PlayerManager pm;
    pm.registrarPlayer("Fernanda", "Nanda");
    pm.atualizarPontuacao("Nanda", 100);
    pm.salvarEmArquivo("test_players.txt");

    PlayerManager pm2;
    pm2.carregarDeArquivo("test_players.txt");

    Player* p = pm2.buscarPlayer("Nanda");
    REQUIRE(p != nullptr);
    CHECK(p->getPontuacaoTotal() == 100);

    remove("test_players.txt");
}

TEST_CASE("PlayerManager - getTopPlayer") {
    PlayerManager pm;
    pm.registrarPlayer("Fernanda", "Nanda");
    pm.registrarPlayer("Carlos", "Carl");

    pm.atualizarPontuacao("Nanda", 50);
    pm.atualizarPontuacao("Carl", 120);
    pm.atualizarPontuacao("Nanda", 100);

    Player* top = pm.getTopPlayer();
    REQUIRE(top != nullptr);
    CHECK(top->getApelido() == "Carl");
    CHECK(top->getPontuacaoMax() == 120);
}
