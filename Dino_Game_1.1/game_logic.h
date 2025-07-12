#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include "core_utils.h"  

// Funzioni per la gestione degli input
void check_input(volatile uint32_t *ps2_base);

// Funzioni per la gestione del gioco
void game_loop(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, uint8_t *colors);

// Funzione per il loop di autoplay (dinosauro che gioca automaticamente)
void autoplay_loop(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, uint8_t *colors);

// Funzione per gestire il riavvio o l'uscita dal gioco
void restart_game(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, int *quit_game, int score);

#endif // GAME_LOGIC_H