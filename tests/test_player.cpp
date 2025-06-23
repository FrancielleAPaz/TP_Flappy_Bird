#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "PlayerManager.hpp"
#include <stdexcept>

using namespace std;

TEST_CASE("Testar criação e atributos do Player") {
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
    CHECK(p.getPontuacaoMax() == 50);  // máximo não mudou
}

TEST_CASE("Testar PlayerManager registrar, remover, buscar e atualizar pontuação") {
    PlayerManager pm;

    bool res = pm.registrarPlayer("Fernanda", "Nanda");
    CHECK(res == true);
    res = pm.registrarPlayer("Outro", "Nanda");  // apelido repetido
    CHECK(res == false);

    Player* p = pm.buscarPlayer("Nanda");
    CHECK(p != nullptr);
    CHECK(p->getNome() == "Fernanda");

    pm.atualizarPontuacao("Nanda", 100);
    CHECK(p->getPontuacaoTotal() == 100);

    pm.atualizarPontuacao("Nanda", 50);
    CHECK(p->getPontuacaoTotal() == 150);

    bool removed = pm.removerPlayer("Nanda");
    CHECK(removed == true);
    CHECK(pm.buscarPlayer("Nanda") == nullptr);
}

TEST_CASE("Testar salvar e carregar arquivo") {
    PlayerManager pm;
    pm.registrarPlayer("Fernanda", "Nanda");
    pm.atualizarPontuacao("Nanda", 100);
    pm.salvarEmArquivo("test_players.txt");

    PlayerManager pm2;
    pm2.carregarDeArquivo("test_players.txt");
    Player* p = pm2.buscarPlayer("Nanda");
    CHECK(p != nullptr);
    CHECK(p->getPontuacaoTotal() == 100);

    // Limpeza do arquivo
    remove("test_players.txt");
}

TEST_CASE("Testar getTopPlayer") {
    PlayerManager pm;
    pm.registrarPlayer("Fernanda", "Nanda");
    pm.registrarPlayer("Carlos", "Carl");
    pm.atualizarPontuacao("Nanda", 50);
    pm.atualizarPontuacao("Carl", 120);
    pm.atualizarPontuacao("Nanda", 100); // Agora Nanda tem 100 como máximo

    Player* top = pm.getTopPlayer();
    CHECK(top != nullptr);
    CHECK(top->getApelido() == "Carl");
    CHECK(top->getPontuacaoMax() == 120);
}

