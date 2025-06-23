#ifndef PLAYERMANAGER_HPP
#define PLAYERMANAGER_HPP

#include "Player.hpp" //A classe PlayerManager usa objetos da classe Player
#include <map>
#include <string>

using namespace std;

class PlayerManager { //classe que é responsável por todo o sistema de cadastro de jogadores

    private:
        map<string, Player> players; // Armazena todos os jogadores, usando o apelido como chave

    public:
        PlayerManager();

        bool registrarPlayer(const string& nome, const string& apelido); //Adiciona um novo jogador ao cadastro, desde que o apelido ainda não exista
        bool removerPlayer(const string& apelido);
        Player* buscarPlayer(const string& apelido);
        void atualizarPontuacao(const string& apelido, int score);
        Player* getTopPlayer() const; //Retorna o jogador com maior pontuação máxima entre todos cadastrados
        void salvarEmArquivo(const string& filename) const;
        void carregarDeArquivo(const string& filename);
        void listarJogadores() const;
};

#endif
