#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

int main() {
    if (!al_init()) {
        std::cerr << "Erro ao iniciar o Allegro!" << std::endl;
        return -1;
    }

    al_init_primitives_addon(); // Inicializa o addon para desenhar primitivas (linhas, círculos, etc.)

    ALLEGRO_DISPLAY* display = al_create_display(640, 480);
    if (!display) {
        std::cerr << "Erro ao criar a janela do jogo!" << std::endl;
        return -1;
    }

    al_clear_to_color(al_map_rgb(135, 206, 235)); // Cor azul do céu
    al_draw_filled_circle(320, 240, 50, al_map_rgb(255, 255, 0)); // Um "sol"
    al_flip_display();

    // Espera até a janela ser fechada
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));

    bool running = true;
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
    }

    al_destroy_event_queue(queue);

    al_destroy_display(display);
    return 0;
}
