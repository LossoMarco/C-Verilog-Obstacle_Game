#ifndef TEXT_UI_H
#define TEXT_UI_H

#include <stdint.h>

// Funzioni per il rendering del testo
void draw_char(volatile uint8_t *vga_buffer, char c, int x, int y, uint8_t color);
void draw_string(volatile uint8_t *vga_buffer, const char *str, int x, int y, uint8_t color);
void draw_number(volatile uint8_t *vga_buffer, int number, int x, int y, uint8_t color);

// Funzioni per le schermate di gioco
void draw_game_over(volatile uint8_t *vga_buffer, uint8_t color);
void draw_score(volatile uint8_t *vga_buffer, int score, uint8_t color);
void show_start_screen(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, uint8_t *colors);

// Funzione per disegnare i cuori (vite)
void draw_hearts(volatile uint8_t *vga_buffer, int lives, uint8_t color);

// Funzioni per il rendering grafico
void draw_cloud(volatile uint8_t *vga_buffer, int x, int y, int size, uint8_t color);
void draw_clouds(volatile uint8_t *vga_buffer, uint8_t color);

#endif // TEXT_UI_H