## Descrição
#TP_Flappy_Bird

Projeto final da discipliana de Programação e Desenvolvimento de Software II - DCC/UFMG, 2025/1

O TP_Flappy_Bird é um projeto realizado por 4 alunos dos cursos do Departamento de Ciência da Computação da UFMG. Esse trabalho teve o intuito de implementar em C++ o clássico jogo Flappy Bird utilizando a biblioteca Allegro para a renderização, entrada de dados e elementos gráficos. O projeto possui uma estrutura em módulos, onde foram contidas classes para o gerenciamento de cenário, pássaro, canos, jogadores e um sistema básico de pontuação.
O código contém testes unitários implementados com a biblioteca doctest para garantir a corretude das funcionalidades principais do jogo.
 
## Funcionalidades
Toda a física do Flappy Bird original(gravidade, pulo, colisão);
Cenário com obstáculos gerados (canos);
Gerenciamento de pontuação, jogadores e armazenamento; Sons também forma integrados para ca uma das açõoes principais (pular, pontuar e morrer); Um menu funcional e interativo para o jogador digitar seu nickname;
Sistema de testes para as principais classes do jogo para garantir o funcionamento eficaz do jogo;
Recursos gráficos carregados via Allegro 5;
Interface gráfica com recursos visuais básicos.


## Pré - Requisitos
Compilador C++ compatível com C++17;
Biblioteca Allegro 5 instalada com seus módulos completos;
make para facilitar a compilação;
doctest para execução dos testes unitários;

## Estrutura do projeto
TP_Flappy_Bird/
├── assets/                
│    ├── background.bmp
│    ├── birds.png
│    ├── button_play.png
│    ├── flappyfont.ttf
│    ├── gameover.png
│    ├── logo.bmp
│    ├── tunnel_down.png
│    └── tunnel_up.png
│
├── bin/                   
│    └── flappybird        
│    └── testes            
│
├── include/               
│    ├── Bird.hpp
│    ├── Game.hpp
│    ├── GameObject.hpp
│    ├── Pipe.hpp
│    ├── Player.hpp
│    ├── PlayerManager.hpp
│    └── Scenario.hpp
│
├── obj/                   
│    └── *.o                
│
├── src/                    
│    ├── Bird.cpp
│    ├── Game.cpp
│    ├── GameObject.cpp
│    ├── Pipe.cpp
│    ├── Player.cpp
│    ├── PlayerManager.cpp
│    ├── Scenario.cpp
│    └── main.cpp
│
├── tests/                  
│    ├── main_tests.cpp    
│    ├── test_bird.cpp
│    ├── test_pipe.cpp
│    ├── test_player.cpp
│    ├── test_playermanager.cpp
│    └── test_scenario.cpp
│
├── Makefile               
├── README.md               
└── test_players.txt        

## Testes
Todos os testes são escritos com a biblioteca **doctest** e cobrem:
Bird: movimentação e colisão;
Pipe: velocidade e pontuação;
Scenario: lógica de jogo, socre e reset;
Player:  pontuação e identificação;
PlayerManage: registro, remoção, persistência em arquivo.
Para compilar e rodar os testes: make run_test

Resultado esperado:
[doctest] test cases: 15 | 15 passed | 0 failed
[doctest] assertions: 43 | 43 passed | 0 failed
[doctest] Status: SUCCESS!

## Como compilar
No terminal, dentro da pasta do projeto(TP_Flappy_Bird), insira: make.
Isso irá complitar o jogo e gerar o executável em bin/flappybird.

## Como rodar o jogo
Após compilar, execute: make run.
Ou diretamente: ./bin/flappybird

## Como limpar os arquivos compilados
Para limpar os arquivos objetos, binários e testes compilados: make clean.
Para limpar somente o executável dos testes: make clean_test.

## Dependências
No Windows, recomenda -se usar MSYS2 com pacotes Allegro instalados.
No Linux, instale as dependências (Debian/Ubuntu): sudo apt-get install liballegro5-dev liballegro-image5 liballegro-font5 liballegro-ttf5 liballegro-primitives5 liballegro-audio5 liballegro-acodec5 liballegro-dialog5.
No macOS, use brew: brew install allegro.


## Contato
Para quaisquer dúvidas ou contribuições envie um pull request.

