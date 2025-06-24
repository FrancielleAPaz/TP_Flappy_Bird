#include "PlayerManager.hpp"
#include <fstream>
#include <iostream>

using namespace std;

PlayerManager::PlayerManager() {
     carregarDeArquivo("data/players.txt");
}

bool PlayerManager::registrarPlayer(const string& nome, const string& apelido) {

    if (players.count(apelido) > 0) {
        return false; // apelido já existe
    }
    players[apelido] = Player(nome, apelido);
    return true;

} //Verifica se o apelido já existe. Se não existir, cria um novo Player e o adiciona ao map

bool PlayerManager::removerPlayer(const string& apelido) {

    return players.erase(apelido) > 0;

} //Remove o jogador identificado pelo apelido. Retorna true se conseguiu remover, false se o apelido não existia.

Player* PlayerManager::buscarPlayer(const string& apelido) {

    auto it = players.find(apelido);
    if (it != players.end()) {
        return &it->second;
    }
    return nullptr;

} //Busca o jogador pelo apelido no map. Retorna um ponteiro para o jogador se encontrado, ou nullptr se não existir.

void PlayerManager::atualizarPontuacao(const string& apelido, int score) {

    auto it = players.find(apelido);
    if (it != players.end()) {
        it->second.adicionarPontuacao(score);
    }

} //Encontra o jogador pelo apelido. Se existir, atualiza suas estatísticas com o novo score.

Player* PlayerManager::getTopPlayer() const {

    if (players.empty()) return nullptr;

    auto top = players.begin();
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->second.getPontuacaoMax() > top->second.getPontuacaoMax()) {
            top = it;
        }
    }
    return const_cast<Player*>(&top->second); // para compatibilidade com Player*

} //Percorre todos os jogadores. Compara a pontuacaoMax para achar o melhor jogador.

void PlayerManager::salvarEmArquivo(const string& filename) const {

    ofstream arquivo(filename);
    if (!arquivo.is_open()) return;

    for (const auto& par : players) {
        arquivo << par.second.toString() << '\n';
    }

    arquivo.close();

} //Abre um arquivo texto para escrita. Para cada jogador, chama toString() e salva uma linha no arquivo.

void PlayerManager::carregarDeArquivo(const string& filename) {

    ifstream arquivo(filename);
    if (!arquivo.is_open()) {
    // Se o arquivo não existir, cria vazio
    ofstream criaArquivo(filename);
    criaArquivo.close();
    return;
}

    string linha;
    while (getline(arquivo, linha)) {
        Player p = Player::fromString(linha);
        players[p.getApelido()] = p;
    }

    arquivo.close();

} //Abre o arquivo de texto com os dados. Lê cada linha com getline, transforma em um Player usando fromString(). Armazena no map, indexando pelo apelido.

void PlayerManager::listarJogadores() const {
    for (const auto& par : players) {
        cout << par.second.toString() << endl;
    }
}
