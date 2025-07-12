#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

#include <stdint.h>
#include "core_utils.h"  

// Funzioni per disegnare le entità di gioco
void draw_dino(volatile uint8_t *vga_buffer, uint8_t color, int x, int y, int size);
void draw_obstacle(volatile uint8_t *vga_buffer, uint8_t color, int x, int y, int width, int height);

// Funzioni per la gestione degli ostacoli
void init_obstacles(Obstacle obstacles[], int count);
void spawn_obstacle(Obstacle obstacles[], int count);
void update_obstacles(volatile uint8_t *vga_buffer, Obstacle obstacles[], int count, uint8_t color, int speed, int *score);
int check_collision(int dino_x, int dino_y, int dino_size, Obstacle obstacles[], int count);

// Funzioni per la gestione del salto
void start_jump(int *is_jumping, int *jump_phase, int *jump_start_time);
void update_jump(volatile uint8_t *vga_buffer, uint8_t color, int dino_x, int *dino_y, int dino_size, 
                int *is_jumping, int *jump_phase, int *jump_start_time, int original_y, int obstacle_speed);

#endif // GAME_ENTITIES_H