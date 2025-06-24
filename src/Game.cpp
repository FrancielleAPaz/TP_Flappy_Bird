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
    timer = al_create_timer(1.0 / 30.0); // 30 FPS recomendado pelo professor
    event_queue = al_create_event_queue();

    // Registrar fontes de evento (teclado, display, timer)
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));  
    al_register_event_source(event_queue, al_get_mouse_event_source());
  

    // Definir estado inicial como GameState::INICIO
    currentState = GameState::INICIO;

    running = true;

    // som_pulo = al_load_sample(".assets/sound/jump.mp3");
    // som_morte = al_load_sample(".assets/sound/dead.ogg");
    // trilha = al_load_audio_stream(".assets/sound/trilha.wav", 4, 2048);

    // if (!som_pulo) std::cerr << "[ERRO] Não foi possível carregar jump.mp3\n";
    // if (!som_morte) std::cerr << "[ERRO] Não foi possível carregar dead.ogg\n";
    // if (!trilha) std::cerr << "[ERRO] Não foi possível carregar trilha.wav\n";

    // if (!som_pulo || !som_morte || !trilha) {
    //     std::cerr << "[ERRO] Falha ao carregar algum dos arquivos de áudio.\n";
    //     exit(1);
    // }

    // al_attach_audio_stream_to_mixer(trilha, al_get_default_mixer());
    // al_set_audio_stream_playmode(trilha, ALLEGRO_PLAYMODE_LOOP);

    if (!scenario.carregarRecursos()) {
        al_show_native_message_box(nullptr, "Erro", "Recursos", "Falha ao carregar recursos gráficos do cenário.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        exit(1);
    }
}

// Destrutor: libera memória e destrói objetos Allegro
Game::~Game() {
    // Destruir timer, event_queue e display
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    // al_destroy_sample(som_pulo);
    // al_destroy_sample(som_morte);
    // al_destroy_audio_stream(trilha);

}

// Loop principal do jogo
void Game::run(const std::string& apelido) {
    // Armazena apelido do jogador atual
    jogadorAtual = apelido;

    // Carrega os dados dos jogadores já existentes
    playerManager.carregarDeArquivo("data/players.txt");

    // Cadastra ou recupera jogador
    if (!playerManager.buscarPlayer(apelido)) {
        std::cout << "[INFO] Jogador não encontrado. Será criado um novo.\n";
        playerManager.registrarPlayer("Jogador", apelido);
    }
    
    al_start_timer(timer);

    ALLEGRO_EVENT event;
    while (running) {
        al_wait_for_event(event_queue, &event);

        switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            if (currentState == GameState::JOGANDO) {
                scenario.update();
                if (scenario.checkCollision()) {
                    showGameOver();
                }
            }
            scenario.draw(currentState);
            break;
        
        case ALLEGRO_EVENT_KEY_DOWN:
            handleInput(event.keyboard.keycode);
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;
        }
    }

    // Atualizar pontuação no PlayerManager após perder
    playerManager.atualizarPontuacao(jogadorAtual, scenario.getScore());
    // Salvar dados no arquivo
    playerManager.salvarEmArquivo("data/players.txt");
}

// Trata entradas do teclado de acordo com o estado atual
void Game::handleInput(int keycode) {
    // Configurando para tocar o som jump.mp3 ao pressionar space
    if (keycode == ALLEGRO_KEY_SPACE) {
    //al_play_sample(som_pulo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    scenario.getBird()->jump();
    }

    // if estado == INICIO → se pressionar o botão esquerdo do mouse no playbutton o jogo inicia
    if (currentState == GameState::INICIO) {
        ALLEGRO_MOUSE_STATE mouse;
        al_get_mouse_state(&mouse);
        // Se o cursor do mouse passar sobre o buttonplay, ele muda de forma.
        if (scenario.isPlayButtonClicked(mouse.x, mouse.y)) {
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        } else {
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
        }

        if (al_mouse_button_down(&mouse, 1)) {
            if (scenario.isPlayButtonClicked(mouse.x, mouse.y)) {
                currentState = GameState::JOGANDO;
            }
        }
    }



    // if estado == INICIO → se apertar ENTER, muda para JOGANDO
    if (currentState == GameState::INICIO) {
        if (keycode == ALLEGRO_KEY_ENTER) {
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
    // Configurando som de morte no momento em que o jogador perder
   // al_play_sample(som_morte, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    currentState = GameState::PERDEU;
}

// Reinicia a partida mantendo o jogador atual
void Game::startNewRound() {
    scenario.reset();
    currentState = GameState::JOGANDO;
}
