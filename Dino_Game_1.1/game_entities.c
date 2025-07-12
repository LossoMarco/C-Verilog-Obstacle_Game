#include "game_entities.h"
#include "graphics.h"
#include "core_utils.h"
#include "text_ui.h"

// Funzione per disegnare un dinosauro semplificato (solo un rettangolo)
void draw_dino(volatile uint8_t *vga_buffer, uint8_t color, int x, int y, int size) {
    // Torniamo alla versione con rettangolo semplice invece di usare draw_better_dino
    int dino_width = size / 2;
    int dino_height = size;
    
    // Disegna un rettangolo per rappresentare il dinosauro
    for (int dy = 0; dy < dino_height; dy++) {
        for (int dx = 0; dx < dino_width; dx++) {
            set_pixel(vga_buffer, x + dx, y - dino_height + dy, color);
        }
    }
}

// Funzione per disegnare un ostacolo
void draw_obstacle(volatile uint8_t *vga_buffer, uint8_t color, int x, int y, int width, int height) {
    // Torniamo alla versione con rettangolo semplice invece di usare draw_better_cactus
    int start_x = x;
    int start_y = y - height;
    
    for (int dy = 0; dy < height; dy++) {
        int offset_base = ((start_y + dy) << 10) + start_x;
        for (int dx = 0; dx < width; dx++) {
            if (start_x + dx >= 0 && start_x + dx < VGA_WIDTH && 
                start_y + dy >= 0 && start_y + dy < VGA_HEIGHT) {
                vga_buffer[offset_base + dx] = color;
            }
        }
    }
}

// Funzione per inizializzare gli ostacoli
void init_obstacles(Obstacle obstacles[], int count) {
    for (int i = 0; i < count; i++) {
        obstacles[i].active = 0;  // Inizialmente nessun ostacolo è attivo
    }
}

// Funzione per generare un nuovo ostacolo
void spawn_obstacle(Obstacle obstacles[], int count) {
    for (int i = 0; i < count; i++) {
        if (!obstacles[i].active) {
            // Dimensioni più appropriate per un cactus
            obstacles[i].width = random(20, 65);
            obstacles[i].height = random(30, 40);
            
            // Posizione iniziale sempre fuori dallo schermo a destra
            obstacles[i].x = VGA_WIDTH + random(0, 50);
            
            // Posiziona l'ostacolo esattamente sul terreno
            obstacles[i].y = GROUND_HEIGHT;
            obstacles[i].active = 1;
            return;
        }
    }
}

// Funzione per aggiornare la posizione degli ostacoli
void update_obstacles(volatile uint8_t *vga_buffer, Obstacle obstacles[], int count, 
                     uint8_t color, int speed, int *score) {
    
    // Versione semplificata dalla v0.5
    for (int i = 0; i < count; i++) {
        if (obstacles[i].active) {
            // Cancella l'ostacolo dalla posizione precedente
            draw_obstacle(vga_buffer, SKY_COLOR, obstacles[i].x, obstacles[i].y, 
                         obstacles[i].width, obstacles[i].height);
            
            // Aggiorna la posizione (muove di 1 pixel alla volta)
            obstacles[i].x -= speed;
            
            // Se l'ostacolo è uscito dallo schermo
            if (obstacles[i].x + obstacles[i].width < 0) {
                obstacles[i].active = 0;
                (*score)++;
            } else {
                // Ridisegna nella nuova posizione
                draw_obstacle(vga_buffer, color, obstacles[i].x, obstacles[i].y, 
                             obstacles[i].width, obstacles[i].height);
            }
        }
    }
    
    // Genera un nuovo ostacolo se non ce ne sono attivi
    int active_obstacles = 0;
    for (int i = 0; i < count; i++) {
        if (obstacles[i].active) {
            active_obstacles++;
        }
    }
    
    if (active_obstacles == 0) {
        spawn_obstacle(obstacles, count);
    }
    // Genera un secondo ostacolo con probabilità casuale se c'è solo uno attivo
    else if (active_obstacles < count) {
        // Trova l'ostacolo attivo più a destra
        int rightmost_x = 0;
        for (int i = 0; i < count; i++) {
            if (obstacles[i].active && obstacles[i].x > rightmost_x) {
                rightmost_x = obstacles[i].x;
            }
        }
        
        // Genera un nuovo ostacolo solo se l'ostacolo più a destra è abbastanza lontano
        // e con una probabilità casuale
        if (rightmost_x < VGA_WIDTH - OBSTACLE_SPAWN_DISTANCE && random(0, 100) < 30) { // 30% di probabilità
            spawn_obstacle(obstacles, count);
        }
    }
}

// Funzione per aggiornare la posizione del dinosauro durante il salto
void update_jump(volatile uint8_t *vga_buffer, uint8_t color, int x, int *y, int size, 
                int *is_jumping, int *jump_phase, int *jump_start_time, int original_y, int obstacle_speed) {
    
    if (!(*is_jumping)) {
        return;
    }
    
    // Versione semplificata dalla v0.5
    int jump_height = 75;     // Altezza massima del salto
    int jump_duration = 70;   // Durata totale del salto in frame

    if (obstacle_speed == 1) {
        jump_duration = 150;   // Durata aumentata per l'autoplay
    }
    
    // Calcola la fase del salto (0-100%)
    (*jump_phase)++;
    
    // Calcola la nuova posizione y usando una parabola
    float progress = (float)(*jump_phase) / jump_duration;
    if (progress > 1.0f) progress = 1.0f;
    
    // Cancella il dinosauro precedente
    draw_dino(vga_buffer, SKY_COLOR, x, *y, size);
    
    // Calcola la nuova posizione usando una parabola
    // y = h * (1 - 4 * (x - 0.5)^2) dove x va da 0 a 1
    float x_param = progress - 0.5f;
    float y_offset = jump_height * (1.0f - 4.0f * x_param * x_param);
    
    // Aggiorna la posizione y
    *y = original_y - (int)y_offset;
    
    // Disegna il dinosauro nella nuova posizione
    draw_dino(vga_buffer, color, x, *y, size);
    
    // Se il salto è completato, resetta le variabili
    if (*jump_phase >= jump_duration) {
        *is_jumping = 0;
        *jump_phase = 0;
        *y = original_y;
    }
}

// Funzione per iniziare il salto del dinosauro
void start_jump(int *is_jumping, int *jump_phase, int *jump_start_time) {
    if (!(*is_jumping)) {
        *is_jumping = 1;
        *jump_phase = 0;
        *jump_start_time = 0; // Non usato nella versione semplificata
    }
}

// Funzione per verificare la collisione tra dinosauro e ostacoli
int check_collision(int dino_x, int dino_y, int dino_size, Obstacle obstacles[], int count) {
    int dino_width = dino_size / 2;
    int dino_height = dino_size;
    int dino_top = dino_y - dino_height;
    
    for (int i = 0; i < count; i++) {
        if (obstacles[i].active) {
            int obstacle_top = obstacles[i].y - obstacles[i].height;
            
            // Verifica sovrapposizione
            if (dino_x + dino_width > obstacles[i].x && 
                dino_x < obstacles[i].x + obstacles[i].width &&
                dino_top < obstacles[i].y && 
                dino_y > obstacle_top) {
                return 1;
            }
        }
    }
    return 0;
}