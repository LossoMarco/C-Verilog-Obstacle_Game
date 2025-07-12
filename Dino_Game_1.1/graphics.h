#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

// Definizione della risoluzione VGA
#define VGA_WIDTH 640
#define VGA_HEIGHT 480

// Stride di 1024 byte per riga
#define VGA_STRIDE 1024

// Definizioni dei colori
#define SKY_COLOR rgb8(135, 206, 235)  // Azzurro cielo
#define GRASS_COLOR rgb8(34, 139, 34)  // Verde scuro per l'erba
#define EARTH_COLOR rgb8(139, 69, 19)  // Marrone per la terra
#define GROUND_HEIGHT (VGA_HEIGHT / 2)  // Altezza del terreno (metà schermo)
#define GRASS_HEIGHT 10                 // Altezza dell'erba in pixel
#define CLOUD_COLOR 0xFF  // Bianco per le nuvole

// Funzioni per la gestione grafica di base
void set_pixel(volatile uint8_t *vga_buffer, int x, int y, uint8_t color);
void clear_screen(volatile uint8_t *vga_buffer);
void draw_background(volatile uint8_t *vga_buffer);
void draw_rectangle(volatile uint8_t *vga_buffer, int x, int y, int width, int height, uint8_t color);

#endif // GRAPHICS_H