#include "graphics.h"
#include "core_utils.h"
#include "text_ui.h"

// Funzione per colorare un pixel tenendo conto dello stride
void set_pixel(volatile uint8_t *vga_buffer, int x, int y, uint8_t color) {
    // Calcola l'offset usando lo stride di 1024
    int offset = (y << 10) + x;  // equivalente a y * 1024 + x
    
    // Verifica che le coordinate siano valide
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        // Scrivi il colore nel buffer
        vga_buffer[offset] = color;
    }
}

// Funzione per svuotare il buffer VGA (schermo nero)
void clear_screen(volatile uint8_t *vga_buffer) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            set_pixel(vga_buffer, x, y, 0x00);
        }
    }
}

// Funzione per disegnare lo sfondo
void draw_background(volatile uint8_t *vga_buffer) {
    // Disegna il cielo (parte superiore)
    for (int y = 0; y < GROUND_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            set_pixel(vga_buffer, x, y, SKY_COLOR);
        }
    }
    
    // Disegna l'erba (parte centrale)
    for (int y = GROUND_HEIGHT; y < GROUND_HEIGHT + GRASS_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            set_pixel(vga_buffer, x, y, GRASS_COLOR);
        }
    }
    
    // Disegna la terra (parte inferiore)
    for (int y = GROUND_HEIGHT + GRASS_HEIGHT; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            set_pixel(vga_buffer, x, y, EARTH_COLOR);
        }
    }

    // Aggiungi le nuvole
    draw_clouds(vga_buffer, CLOUD_COLOR);
}

// Funzione per disegnare un rettangolo
void draw_rectangle(volatile uint8_t *vga_buffer, int x, int y, int width, int height, uint8_t color) {
    int start_x = x;
    int start_y = y;
    
    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            if (start_x + dx >= 0 && start_x + dx < VGA_WIDTH && 
                start_y + dy >= 0 && start_y + dy < VGA_HEIGHT) {
                set_pixel(vga_buffer, start_x + dx, start_y + dy, color);
            }
        }
    }
}