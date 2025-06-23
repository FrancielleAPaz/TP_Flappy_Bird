#include "Player.hpp"
#include <sstream>

Player::Player(const string& nome, const string& apelido) 
    : name(nome), nickname(apelido), numPartidas(0), pontuacaoTotal(0), pontuacaoMax(0) {} //inicializa todos os atributos com valores vazios ou zero

string Player::getNome() const {
    return name;
} 

string Player::getApelido() const {
    return nickname;
} 

int Player::getNumPartidas() const {
    return numPartidas;
}

int Player::getPontuacaoTotal() const {
    return pontuacaoTotal;
}

int Player::getPontuacaoMax() const {
    return pontuacaoMax;
}

void Player::adicionarPontuacao(int score) {

    if(score < 0) throw invalid_argument("Pontuação não pode ser negativa");

    numPartidas++;
    pontuacaoTotal += score;

    if(score > pontuacaoMax) pontuacaoMax = score;

} //Atualiza todas as estatísticas

string Player::toString() const {
    // Formato: nome|apelido|numPartidas|pontuacaoTotal|pontuacaoMax

    ostringstream oss;
    oss << name << "|" << nickname << "|" << numPartidas << "|" << pontuacaoTotal << "|" << pontuacaoMax;
    return oss.str();

} //Gera uma linha de texto única com os dados do jogador

Player Player::fromString(const string& data) {

    istringstream iss(data);
    string token;
    string nome, apelido;
    int partidas = 0, total = 0, max = 0;

    if(!getline(iss, nome, '|')) throw runtime_error("Erro leitura nome");
    if(!getline(iss, apelido, '|')) throw runtime_error("Erro leitura apelido");
    if(!getline(iss, token, '|')) throw runtime_error("Erro leitura partidas");
    partidas = stoi(token);
    if(!getline(iss, token, '|')) throw runtime_error("Erro leitura pontuacao total");
    total = stoi(token);
    if(!getline(iss, token, '|')) throw runtime_error("Erro leitura pontuacao max");
    max = stoi(token);

    Player p(nome, apelido);
    p.numPartidas = partidas;
    p.pontuacaoTotal = total;
    p.pontuacaoMax = max;
    return p;

} //Recebe uma linha de texto salva no arquivo e reconstrói um objeto Player
