#include "game_logic.h"
#include "core_utils.h"
#include "graphics.h"
#include "text_ui.h"
#include "game_entities.h"
#include <stdio.h>

// Funzione per controllare gli input senza bloccare
void check_input(volatile uint32_t *ps2_base) {
    // Controlla se ci sono dati disponibili dal PS2
    if (*ps2_base & 0x8000) {  // Bit 15 indica dati disponibili
        uint8_t scan_code = *ps2_base & 0xFF;  // Prendi i primi 8 bit
        
        // Consuma il dato
        volatile uint32_t dummy = *ps2_base;
        (void)dummy; // Avoid unused variable warning
        
        // Svuota completamente il buffer PS/2 prima di procedere
        while (*ps2_base & 0x8000) {
            dummy = *ps2_base;
        }
        
        // Gestione semplificata
        if (scan_code == 0x29) {  // PS2_SPACE
            // Aggiungi un evento di salto alla coda
            add_event(EVENT_JUMP, 0);
        }
    }
}

// Funzione per il loop principale del gioco
void game_loop(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, uint8_t *colors) {
    // Dimensione del dinosauro
    int dino_size = 50;
    int dino_x = VGA_WIDTH / 4;  // Posizione x del dinosauro (1/4 dello schermo)
    int dino_y = VGA_HEIGHT / 2;  // Posizione y del dinosauro (metà schermo)
    int original_y = dino_y;  // Salva la posizione y originale
    
    // Variabili per il salto non bloccante
    int is_jumping = 0;
    int jump_phase = 0;
    int jump_start_time = 0;
    
    // Inizializzazione degli ostacoli
    Obstacle obstacles[MAX_OBSTACLES];
    init_obstacles(obstacles, MAX_OBSTACLES);
    int obstacle_speed = 3;  // Velocità degli ostacoli (pixel per frame)
    int score = 0;  // Punteggio
    int lives = 3;  // Numero di vite iniziali
    int game_over = 0;  // Flag per il game over
    
    // Disegna il dinosauro inizialmente
    draw_dino(vga_buffer, colors[3], dino_x, dino_y, dino_size);
    
    // Disegna le vite iniziali
    draw_hearts(vga_buffer, lives, colors[0]);  // Usa il colore rosso per i cuori
    
    // Genera il primo ostacolo
    spawn_obstacle(obstacles, MAX_OBSTACLES);

    // Svuota la coda degli eventi prima di iniziare
    Event e;
    while ((e = get_next_event()).type != EVENT_NONE) {
        // Svuota la coda
    }

    // Loop principale del gioco
    while (!game_over) {
        // FASE 1: Raccolta input (non bloccante)
        check_input(ps2_base);
        
        // FASE 2: Elaborazione eventi
        Event current_event = get_next_event();
        if (current_event.type == EVENT_JUMP && !is_jumping) {
            // Inizia il salto (non bloccante)
            start_jump(&is_jumping, &jump_phase, &jump_start_time);
        }
        
        // FASE 3: Aggiornamento stato di gioco
        
        // Aggiorna la posizione degli ostacoli
        // Aumenta la velocità in base al punteggio
        int current_speed = obstacle_speed;
        if (score > 10) current_speed = 4;
        if (score > 20) current_speed = 5;
        if (score > 30) current_speed = 6;
        
        update_obstacles(vga_buffer, obstacles, MAX_OBSTACLES, colors[5], current_speed, &score);
        
        // Aggiorna il punteggio visualizzato
        // Prima cancella l'area del punteggio disegnando un rettangolo del colore del cielo
        for (int dy = 15; dy < 40; dy++) {
            for (int dx = VGA_WIDTH / 2 - 120; dx < VGA_WIDTH / 2 + 120; dx++) {
                set_pixel(vga_buffer, dx, dy, SKY_COLOR);
            }
        }
        // Poi ridisegna il punteggio aggiornato
        draw_score(vga_buffer, score, colors[4]); // Usa il colore bianco

        // Aggiorna la posizione del dinosauro se sta saltando
        update_jump(vga_buffer, colors[3], dino_x, &dino_y, dino_size, 
                   &is_jumping, &jump_phase, &jump_start_time, original_y, obstacle_speed);

        // Verifica collisioni
        if (check_collision(dino_x, dino_y, dino_size, obstacles, MAX_OBSTACLES)) {
            lives--;  // Riduci le vite
            
            // Aggiorna la visualizzazione delle vite
            draw_hearts(vga_buffer, lives, colors[0]);
            
            if (lives <= 0) {
                // Game over se non ci sono più vite
                add_event(EVENT_COLLISION, score);
                game_over = 1;
            } else {
                // Altrimenti, rimuovi l'ostacolo che ha causato la collisione
                for (int i = 0; i < MAX_OBSTACLES; i++) {
                    if (obstacles[i].active) {
                        // Calcola se questo ostacolo è in collisione con il dinosauro
                        int dino_width = dino_size / 2;
                        int dino_height = dino_size;
                        int dino_top = dino_y - dino_height;
                        int obstacle_top = obstacles[i].y - obstacles[i].height;
                        
                        if (dino_x + dino_width > obstacles[i].x && 
                            dino_x < obstacles[i].x + obstacles[i].width &&
                            dino_top < obstacles[i].y && 
                            dino_y > obstacle_top) {
                            
                            // Cancella l'ostacolo
                            draw_obstacle(vga_buffer, SKY_COLOR, obstacles[i].x, obstacles[i].y, 
                                         obstacles[i].width, obstacles[i].height);
                            obstacles[i].active = 0;
                            break;
                        }
                    }
                }
            }
        }
        
        // Piccolo ritardo per controllare la velocità del gioco
        delay(10);
    }

    // Piccola pausa per assicurarsi che il buffer VGA sia aggiornato
    delay(1000);

    // Disegna "GAME OVER" sullo schermo
    draw_game_over(vga_buffer, colors[0]); // Usa il colore rosso
    
    // Attendi alcuni secondi prima di tornare alla schermata iniziale
    delay(10000000);
    
    // Pulisci lo schermo e torna alla schermata iniziale
    clear_screen(vga_buffer);
    draw_background(vga_buffer);
    
    // Disegna la stringa "PREMERE SPAZIO PER INIZIARE"
    int text_width = (5 * 3) * 24 + (2 * 3) * 23; // 24 caratteri, 23 spazi
    int text_x = (VGA_WIDTH - text_width) / 2 - 30;
    int text_y = VGA_HEIGHT / 6;
    draw_string(vga_buffer, "PREMERE SPAZIO PER INIZIARE", text_x, text_y, colors[6]); // Nero
    
    // Avvia l'autoplay
    autoplay_loop(vga_buffer, ps2_base, colors);
}

// Funzione per gestire il riavvio o l'uscita dal gioco
void restart_game(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, int *quit_game, int score) {
    
    // Attendi un input per ricominciare o uscire
    int restart = 0;
    while (!restart) {
        // Controlla se ci sono dati disponibili dal PS2
        if (*ps2_base & 0x8000) {
            uint8_t scan_code = *ps2_base & 0xFF;  // Prendi i primi 8 bit
            
            // Consuma il dato
            volatile uint32_t dummy = *ps2_base;
            (void)dummy; // Avoid unused variable warning
            
            // Svuota completamente il buffer PS/2 prima di procedere
            while (*ps2_base & 0x8000) {
                dummy = *ps2_base;
            }
            
            // Gestione input
            if (scan_code == 0x29) {  // PS2_SPACE - Riavvia il gioco
                restart = 1;
            } else if (scan_code == 0x76) {  // PS2_ESC - Esci dal gioco
                restart = 1;
                *quit_game = 1;
            }
        }
        delay(10);
    }
}

// Funzione per il salto automatico (AI)
void auto_jump(Obstacle obstacles[], int max_obstacles, int dino_x, int dino_size, int *is_jumping, int *jump_phase, int *jump_start_time) {
    // Se il dinosauro sta già saltando, non fare nulla
    if (*is_jumping) return;
    
    // Cerca l'ostacolo più vicino davanti al dinosauro
    for (int i = 0; i < max_obstacles; i++) {
        if (obstacles[i].active) {
            // Calcola la distanza tra il dinosauro e l'ostacolo
            int distance = obstacles[i].x - (dino_x + dino_size/2);
            
            // Se l'ostacolo è abbastanza vicino, inizia un salto
            if (distance > 0 && distance < 30) {
                start_jump(is_jumping, jump_phase, jump_start_time);
                break;
            }
        }
    }
}

// Funzione per il loop di autoplay (dinosauro che gioca automaticamente)
void autoplay_loop(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, uint8_t *colors) {
    // Dimensione del dinosauro
    int dino_size = 50;
    int dino_x = VGA_WIDTH / 4;  // Posizione x del dinosauro (1/4 dello schermo)
    int dino_y = VGA_HEIGHT / 2;  // Posizione y del dinosauro (metà schermo)
    int original_y = dino_y;  // Salva la posizione y originale
    
    // Variabili per il salto non bloccante
    int is_jumping = 0;
    int jump_phase = 0;
    int jump_start_time = 0;
    
    // Inizializzazione degli ostacoli
    Obstacle obstacles[MAX_OBSTACLES];
    init_obstacles(obstacles, MAX_OBSTACLES);
    int obstacle_speed = 1;  // Velocità degli ostacoli più lenta per l'autoplay
    int score = 0;  // Punteggio (non verrà mostrato)
    int game_over = 0;  // Flag per il game over
    
    // Disegna il dinosauro inizialmente
    draw_dino(vga_buffer, colors[3], dino_x, dino_y, dino_size);
    
    // Genera il primo ostacolo
    spawn_obstacle(obstacles, MAX_OBSTACLES);

    // Svuota la coda degli eventi prima di iniziare
    Event e;
    while ((e = get_next_event()).type != EVENT_NONE) {
        // Svuota la coda
    }

    // Loop principale dell'autoplay
    while (!game_over) {
        // Controlla se l'utente ha premuto spazio per iniziare il gioco vero
        if (*ps2_base & 0x8000) {
            uint8_t scan_code = *ps2_base & 0xFF;
            if (scan_code == 0x29) {  // PS2_SPACE
                return;  // Esci dall'autoplay e inizia il gioco vero
            }
            
            // Consuma il dato
            volatile uint32_t dummy = *ps2_base;
            (void)dummy;
            
            // Svuota completamente il buffer PS/2
            while (*ps2_base & 0x8000) {
                dummy = *ps2_base;
                (void)dummy;
            }
        }
        
        // FASE 1: Logica di autoplay - Usa la funzione auto_jump
        auto_jump(obstacles, MAX_OBSTACLES, dino_x, dino_size, &is_jumping, &jump_phase, &jump_start_time);
        
        // FASE 2: Aggiornamento stato di gioco
        
        // Aggiorna la posizione degli ostacoli
        update_obstacles(vga_buffer, obstacles, MAX_OBSTACLES, colors[5], obstacle_speed, &score);

        // Aggiorna la posizione del dinosauro se sta saltando
        update_jump(vga_buffer, colors[3], dino_x, &dino_y, dino_size, 
                   &is_jumping, &jump_phase, &jump_start_time, original_y, obstacle_speed);

        // Verifica collisioni - se c'è una collisione, ricomincia l'autoplay
        if (check_collision(dino_x, dino_y, dino_size, obstacles, MAX_OBSTACLES)) {
            // Cancella tutti gli ostacoli
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                if (obstacles[i].active) {
                    draw_obstacle(vga_buffer, SKY_COLOR, obstacles[i].x, obstacles[i].y, 
                                 obstacles[i].width, obstacles[i].height);
                    obstacles[i].active = 0;
                }
            }
            
            // Resetta il dinosauro alla posizione originale
            if (is_jumping) {
                draw_dino(vga_buffer, SKY_COLOR, dino_x, dino_y, dino_size);
                dino_y = original_y;
                is_jumping = 0;
                jump_phase = 0;
                draw_dino(vga_buffer, colors[3], dino_x, dino_y, dino_size);
            }
            
            // Genera un nuovo ostacolo
            spawn_obstacle(obstacles, MAX_OBSTACLES);
            
            // Resetta il punteggio
            score = 0;
        }
        
        // Piccolo ritardo per controllare la velocità dell'autoplay
        delay(4000);
    }
}