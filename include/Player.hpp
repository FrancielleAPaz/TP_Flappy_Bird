#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <stdexcept>

using namespace std;

class Player { 

    private:
        string name;
        string nickname;
        int numPartidas;
        int pontuacaoTotal;
        int pontuacaoMax; // maior pontuação do jogador

    public:
        Player() = default; // Construtor padrão necessário para o std::map
        Player(const string& nome, const string& apelido); //construtor que cria um jogador novo com nome e apelido informados

        string getNome() const;
        string getApelido() const;
        int getNumPartidas() const;
        int getPontuacaoTotal() const;
        int getPontuacaoMax() const;

        void adicionarPontuacao(int score); //Método que atualiza as estatísticas do jogador após uma partida

        string toString() const; // cria uma string formatada para salvar em arquivo
        static Player fromString(const string& data); //lê essa string e cria um objeto Player, para carregar os dados de volta
};

#endif
