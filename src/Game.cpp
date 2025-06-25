#include "Game.hpp"
#include <iostream>
#include <allegro5/allegro_native_dialog.h>

// Construtor: inicializa Allegro, cria display, timer, fila de eventos e define estado inicial
Game::Game() {
    // Inicialização da Allegro e addons
    if (!al_init()) {
        al_show_native_message_box(nullptr, "Erro", "Allegro", "Falha ao inicializar Allegro", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        exit(1);
    }

    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();

    if (!al_reserve_samples(16)) { 
        std::cerr << "[ERRO] Falha ao reservar samples de áudio.\n";
    }

    al_install_mouse();

    // Criar display (al_create_display)
    display = al_create_display(960, 600);
    if (!display) {
        al_show_native_message_box(nullptr, "Erro", "Display", "Falha ao criar display", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        exit(1);
    }

    if (!scenario.carregarRecursos()) {
    al_show_native_message_box(display, "Erro", "Recursos", "Falha ao carregar recursos gráficos", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    exit(1);
    }


    // Criar fila de eventos e timer (al_create_event_queue, al_create_timer)
    timer = al_create_timer(1.0 / 30.0); // 30 FPS recomendado pelo professor Hector
    event_queue = al_create_event_queue();

    // Registrar fontes de evento (teclado, display, timer)
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));  
    al_register_event_source(event_queue, al_get_mouse_event_source());
  

    // Definir estado inicial como GameState::INICIO
    currentState = GameState::INICIO;

    running = true;

    som_pulo = al_load_sample("./assets/sound/jump.ogg");
    som_morte = al_load_sample("./assets/sound/dead.ogg");
    trilha = al_load_audio_stream("./assets/sound/trilha.wav", 4, 2048);
    som_score = al_load_sample("./assets/sound/score.wav");

    if (!som_pulo) std::cerr << "[ERRO] Não foi possível carregar jump.ogg\n";
    if (!som_morte) std::cerr << "[ERRO] Não foi possível carregar dead.ogg\n";
    if (!trilha) std::cerr << "[ERRO] Não foi possível carregar trilha.wav\n";
    if (!som_score) std::cerr << "[ERRO] Não foi possível carregar score.wav\n";



    if (trilha) {
        al_attach_audio_stream_to_mixer(trilha, al_get_default_mixer());
        al_set_audio_stream_playmode(trilha, ALLEGRO_PLAYMODE_LOOP);
        al_set_audio_stream_playing(trilha, true); // Começa a tocar a trilha imediatamente
    }

}

// Destrutor: libera memória e destrói objetos Allegro
Game::~Game() {
    // Destruir timer, event_queue e display
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    if (som_pulo) al_destroy_sample(som_pulo); 
    if (som_morte) al_destroy_sample(som_morte); 
    if (trilha) al_destroy_audio_stream(trilha);
    if (som_score) al_destroy_sample(som_score); 

    // Shutdown de addons 
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_uninstall_keyboard();
    al_uninstall_mouse();
}

void Game::telaInserirJogador() {
    std::string nome = "";
    std::string apelido = "";
    bool nomeConfirmado = false;
    bool apelidoConfirmado = false;
    bool showCursor = true;
    double cursorTimer = 0.0;

    while (running && !apelidoConfirmado) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            cursorTimer += al_get_timer_speed(timer);
            if (cursorTimer >= CURSOR_BLINK_RATE) {
                showCursor = !showCursor;
                cursorTimer = 0.0;
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            int unicode = event.keyboard.unichar;

            // Decide se está digitando o nome ou o apelido
            std::string& campoAtual = nomeConfirmado ? apelido : nome;
            bool& confirmadoAtual = nomeConfirmado ? apelidoConfirmado : nomeConfirmado;

            if (unicode >= 32 && unicode <= 126 && campoAtual.size() < 20) {
                campoAtual += static_cast<char>(unicode);
            } else if (unicode == 8 && !campoAtual.empty()) {
                campoAtual.pop_back();  // backspace
            } else if (unicode == 13) {  // Enter
                if (!campoAtual.empty()) {
                    confirmadoAtual = true;
                }
            }
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        // Renderização da tela
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(scenario.getBackground(), 0, 0, 0);
        al_draw_bitmap(scenario.getBackground(), 480, 0, 0);

        al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 150, ALLEGRO_ALIGN_CENTER, "BEM-VINDO AO FLAPPY BIRD!");

        if (!nomeConfirmado) {
            al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 220, ALLEGRO_ALIGN_CENTER, "Por favor, digite seu NOME:");
        } else {
            al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 220, ALLEGRO_ALIGN_CENTER, "Por favor, digite seu APELIDO:");
        }

        // Caixa de entrada
        al_draw_filled_rectangle(300, 290, 700, 350, al_map_rgb(30, 30, 30));
        al_draw_rectangle(300, 290, 700, 350, al_map_rgb(100, 100, 100), 2);

        std::string textoExibido = nomeConfirmado ? apelido : nome;
        float text_width = al_get_text_width(scenario.getFont(), textoExibido.c_str());

        al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 0), 500 - text_width / 2, 300, ALLEGRO_ALIGN_LEFT, textoExibido.c_str());

        if (showCursor) {
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 0), 500 - text_width / 2 + text_width, 300, ALLEGRO_ALIGN_LEFT, "_");
        }

        al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 400, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para confirmar.");

        al_flip_display();

        // Quando nome e apelido confirmados, registra o jogador
        if (nomeConfirmado && apelidoConfirmado) {
            jogadorAtual = apelido;  // Usa o apelido como identificador do jogador

            playerManager.carregarDeArquivo("data/players.txt");
            if (!playerManager.buscarPlayer(jogadorAtual)) {
                std::cout << "[INFO] Jogador não encontrado. Criando novo jogador.\n";
                playerManager.registrarPlayer(nome, jogadorAtual);
            }

            currentState = GameState::INICIO;  // volta para o menu ou próximo estado
            break;  // sai do loop da tela de inserção
        }
    }
}

void Game::telaRemoverJogador() {
    apelidoParaRemover = "";
    remocaoConfirmada = false;
    cursorTimer = 0;
    showCursor = true;

    while (!remocaoConfirmada && running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // Cursor piscante
        if (event.type == ALLEGRO_EVENT_TIMER) {
            cursorTimer += al_get_timer_speed(timer);
            if (cursorTimer >= CURSOR_BLINK_RATE) {
                showCursor = !showCursor;
                cursorTimer = 0.0;
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            int unicode = event.keyboard.unichar;

            if (unicode >= 32 && unicode <= 126 && apelidoParaRemover.size() < 20) {
                apelidoParaRemover += static_cast<char>(unicode);
            } else if (unicode == 8 && !apelidoParaRemover.empty()) {
                apelidoParaRemover.pop_back();  // backspace
            } else if (unicode == 13) {  // Enter
                if (!apelidoParaRemover.empty()) {
                    remocaoConfirmada = true;

                    playerManager.carregarDeArquivo("data/players.txt");
                    if (playerManager.removerPlayer(apelidoParaRemover)) {
                        std::cout << "[INFO] Jogador '" << apelidoParaRemover << "' removido com sucesso.\n";
                    } else {
                        std::cout << "[INFO] Jogador '" << apelidoParaRemover << "' não encontrado.\n";
                    }

                    playerManager.salvarEmArquivo("data/players.txt");

                    //al_rest(2.0);  // Espera 2 segundos para a pessoa ver a mensagem
                    return;        // Sai da função (pode redirecionar de volta para o menu se quiser)
                }
            }
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        // Renderização da tela de remoção
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(scenario.getBackground(), 0, 0, 0);
        al_draw_bitmap(scenario.getBackground(), 480, 0, 0);

        al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 150, ALLEGRO_ALIGN_CENTER, "REMOÇÃO DE JOGADOR");
        al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 220, ALLEGRO_ALIGN_CENTER, "Digite o apelido a ser removido:");

        al_draw_filled_rectangle(300, 290, 700, 350, al_map_rgb(30, 30, 30));
        al_draw_rectangle(300, 290, 700, 350, al_map_rgb(100, 100, 100), 2);

        float text_width = al_get_text_width(scenario.getFont(), apelidoParaRemover.c_str());
        al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 0), 500 - text_width / 2, 300, ALLEGRO_ALIGN_LEFT, apelidoParaRemover.c_str());

        if (showCursor) {
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 0), 500 - text_width / 2 + text_width, 300, ALLEGRO_ALIGN_LEFT, "_");
        }

        al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 400, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para confirmar.");

        al_flip_display();
    }
}

#include <vector>
#include <algorithm>

void Game::telaMostrarPlacar() {
    // Carrega jogadores do arquivo
    playerManager.carregarDeArquivo("data/players.txt");

    bool mostrarPlacar = true;

    while (mostrarPlacar && running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (event.keyboard.unichar == 13) { // Enter
                mostrarPlacar = false; // Sai da tela de placar e volta para o menu
            }
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;  // Fecha o jogo se fechar janela
        }

        // Copiar jogadores para vetor para ordenar
        std::vector<Player> jogadores;
        for (const auto& par : playerManager.getPlayersMap()) {
            jogadores.push_back(par.second);
        }

        // Ordenar por pontuação máxima decrescente
        std::sort(jogadores.begin(), jogadores.end(), [](const Player& a, const Player& b) {
            return a.getPontuacaoMax() > b.getPontuacaoMax();
        });

        // Renderização da tela de placar
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(scenario.getBackground(), 0, 0, 0);
        al_draw_bitmap(scenario.getBackground(), 480, 0, 0);

        al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 100, ALLEGRO_ALIGN_CENTER, "PLACAR DE JOGADORES");
        al_draw_text(scenario.getFont(), al_map_rgb(0, 0, 0), 500, 140, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para voltar");

        // Lista jogadores e pontuações ordenados
        int y = 180;
        for (const auto& jogador : jogadores) {
            std::string linha = jogador.getNome() + " (" + jogador.getApelido() + ") - Máx: " + std::to_string(jogador.getPontuacaoMax()) + ", Total: " + std::to_string(jogador.getPontuacaoTotal());
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 0), 100, y, ALLEGRO_ALIGN_LEFT, linha.c_str());
            y += 40;

            if (y > 580) break; // Limita para caber na tela
        }

        al_flip_display();
    }
}



// Loop principal do jogo
void Game::run() {
    al_start_timer(timer);
    ALLEGRO_EVENT event;

    nomeDigitado = "";
    nomeConfirmado = false;

    double cursorTimer = 0.0;
    bool showCursor = true;

    bool gameOverAtivo = false;  // flag para indicar que estamos na tela de game over aguardando input

    while (running) {
        al_wait_for_event(event_queue, &event);

        // MENU: roda enquanto o jogador não confirmou a opção
        if (!opcaoConfirmada && !gameOverAtivo) {
            if (event.type == ALLEGRO_EVENT_TIMER) {
                cursorTimer += al_get_timer_speed(timer);
                if (cursorTimer >= CURSOR_BLINK_RATE) {
                    showCursor = !showCursor;
                    cursorTimer = 0.0;
                }
            }

            if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
                int unicode = event.keyboard.unichar;

                if (unicode >= '1' && unicode <= '5' && opcaoDigitada.empty()) {
                    opcaoDigitada = static_cast<char>(unicode);
                } else if (unicode == 8 && !opcaoDigitada.empty()) {
                    opcaoDigitada.pop_back();  // backspace
                } else if (unicode == 13 && !opcaoDigitada.empty()) {  // Enter
                    opcaoEscolhida = opcaoDigitada[0];
                    opcaoConfirmada = true;

                    switch (opcaoEscolhida) {
                        case '1':
                            std::cout << "[INFO] Opção 1: Escolher jogador.\n";
                            telaInserirJogador();
                            break;

                        case '2':
                            std::cout << "[INFO] Opção 2: Jogar sem login.\n";
                            currentState = GameState::JOGANDO; // Começa o jogo
                            break;

                        case '3':
                            std::cout << "[INFO] Opção 3: Remover jogador.\n";
                            telaRemoverJogador();
                            opcaoDigitada.clear();
                            opcaoConfirmada = false;
                            break;

                        case '4':
                            std::cout << "[INFO] Opção 4: Mostrar pontuação.\n";
                            telaMostrarPlacar();
                            opcaoDigitada.clear();
                            opcaoConfirmada = false;
                            break;

                        case '5':
                            std::cout << "[INFO] Opção 5: Sair.\n";
                            running = false;
                            break;
                    }
                }
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }

            // Renderização do menu
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(scenario.getBackground(), 0, 0, 0);
            al_draw_bitmap(scenario.getBackground(), 480, 0, 0);

            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 255), 500, 100, ALLEGRO_ALIGN_CENTER, "MENU PRINCIPAL");
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 255), 500, 150, ALLEGRO_ALIGN_CENTER, "1 - Escolher Jogador");
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 255), 500, 190, ALLEGRO_ALIGN_CENTER, "2 - Jogar sem login");
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 255), 500, 230, ALLEGRO_ALIGN_CENTER, "3 - Remover jogador");
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 255), 500, 270, ALLEGRO_ALIGN_CENTER, "4 - Mostrar pontuação");
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 255), 500, 310, ALLEGRO_ALIGN_CENTER, "5 - Sair");

            al_draw_filled_rectangle(400, 340, 600, 390, al_map_rgb(30, 30, 30));
            al_draw_rectangle(400, 340, 600, 390, al_map_rgb(100, 100, 100), 2);

            float text_width = al_get_text_width(scenario.getFont(), opcaoDigitada.c_str());
            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 0), 500 - text_width / 2, 350, ALLEGRO_ALIGN_LEFT, opcaoDigitada.c_str());

            if (showCursor) {
                al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 0), 500 - text_width / 2 + text_width, 350, ALLEGRO_ALIGN_LEFT, "_");
            }

            al_draw_text(scenario.getFont(), al_map_rgb(255, 255, 255), 500, 410, ALLEGRO_ALIGN_CENTER, "Digite uma opção de 1 a 5 e pressione ENTER.");

            if (!mensagemMenu.empty()) {
                al_draw_text(scenario.getFont(), al_map_rgb(0, 255, 0), 500, 460, ALLEGRO_ALIGN_CENTER, mensagemMenu.c_str());
            }

            al_flip_display();
            continue;
        }

        // LOOP DE JOGO E GAME OVER
        switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            if (currentState == GameState::JOGANDO && !gameOverAtivo) {
                // Atualização do cenário com tratamento de erros
                try {
                    bool scoredThisFrame = scenario.updateAndCheckScore();
                    if (scoredThisFrame && som_score) {
                        al_play_sample(som_score, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                    }

                    if (scenario.checkCollision()) {
                        showGameOver();

                        // Atualização de pontuação com tratamento de erros
                        try {
                            playerManager.atualizarPontuacao(jogadorAtual, scenario.getScore());
                            playerManager.salvarEmArquivo("data/players.txt");
                        } catch (const std::exception& e) {
                            std::cerr << "[ERRO] Falha ao atualizar pontuação: " << e.what() << "\n";
                        }

                        gameOverAtivo = true;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "[ERRO] Falha na atualização do jogo: " << e.what() << "\n";
                }
            }

            // Renderização com tratamento de erros
            try {
                scenario.draw(currentState);
                al_flip_display();
            } catch (const std::exception& e) {
                std::cerr << "[ERRO] Falha na renderização: " << e.what() << "\n";
            }
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            if (gameOverAtivo) {
                // Teclas de reinício com verificação de estado
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || 
                    event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    
                    try {
                        scenario.reset();
                        currentState = GameState::JOGANDO;
                        gameOverAtivo = false;
                    } catch (const std::exception& e) {
                        std::cerr << "[ERRO] Falha ao reiniciar cenario: " << e.what() << "\n";
                    }
                } else {
                    gameOverAtivo = false;
                    currentState = GameState::INICIO;
                    opcaoConfirmada = false;
                    opcaoDigitada.clear();
                }
            } else {
                handleInput(event.keyboard.keycode);
            }
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;
        }
    }
}


// Trata entradas do teclado de acordo com o estado atual
void Game::handleInput(int keycode) {
    // Verificação defensiva: ação de pulo
    if (keycode == ALLEGRO_KEY_SPACE) {
        if (som_pulo) {
            try {
                al_play_sample(som_pulo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
            } catch (...) {
                std::cerr << "[AVISO] Falha ao reproduzir som de pulo\n";
            }
        }
        
        // Verificação de objeto bird válido
        if (scenario.getBird()) {
            try {
                scenario.getBird()->jump();
            } catch (const std::exception& e) {
                std::cerr << "[ERRO] Falha no pulo: " << e.what() << "\n";
            }
        }
    }

    // if estado == INICIO → se apertar SPACE, muda para JOGANDO
    if (currentState == GameState::INICIO) {
        if (keycode == ALLEGRO_KEY_SPACE) {
            currentState = GameState::JOGANDO;
        }
    // if estado == JOGANDO → se apertar espaço, chama bird->jump()
    } else if (currentState == GameState::JOGANDO) {
        if (keycode == ALLEGRO_KEY_SPACE) {
            scenario.getBird()->jump(); 
        }
    // if estado == PERDEU → se apertar espaço ou ENTER, chama startNewRound()
    } else if (currentState == GameState::PERDEU) {
        if (keycode == ALLEGRO_KEY_SPACE || keycode == ALLEGRO_KEY_ENTER) {
            startNewRound();
        }
    }
}

// Mostra tela de game over, muda estado para PERDEU
void Game::showGameOver() {
    // Reprodução de som com verificação defensiva
    if (som_morte) {
        try {
            al_play_sample(som_morte, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } catch (...) {
            std::cerr << "[AVISO] Falha ao reproduzir som de morte\n";
        }
    }
    currentState = GameState::PERDEU;
}

// Reinicia a partida mantendo o jogador atual
void Game::startNewRound() {
    // Reinicialização com tratamento de erros
    try {
        scenario.reset();
        currentState = GameState::JOGANDO;
    } catch (const std::exception& e) {
        std::cerr << "[ERRO] Falha ao reiniciar partida: " << e.what() << "\n";
        // Tentativa de recuperação
        currentState = GameState::INICIO;
    }
}