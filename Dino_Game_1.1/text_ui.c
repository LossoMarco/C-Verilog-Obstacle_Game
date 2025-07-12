#include "text_ui.h"
#include "graphics.h"
#include "core_utils.h"
#include "game_logic.h"

// Funzione per disegnare un carattere sullo schermo
void draw_char(volatile uint8_t *vga_buffer, char c, int x, int y, uint8_t color) {
    // Definizione semplificata dei caratteri (5x7 pixel)
    // Ogni carattere è rappresentato da un array di 7 byte, dove ogni bit rappresenta un pixel
    static const uint8_t chars[][7] = {
        // 'G'
        {
            0b01110,
            0b10001,
            0b10000,
            0b10111,
            0b10001,
            0b10001,
            0b01110
        },
        // 'A'
        {
            0b01110,
            0b10001,
            0b10001,
            0b11111,
            0b10001,
            0b10001,
            0b10001
        },
        // 'M'
        {
            0b10001,
            0b11011,
            0b10101,
            0b10001,
            0b10001,
            0b10001,
            0b10001
        },
        // 'E'
        {
            0b11111,
            0b10000,
            0b10000,
            0b11110,
            0b10000,
            0b10000,
            0b11111
        },
        // 'O'
        {
            0b01110,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b01110
        },
        // 'V'
        {
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b10001,
            0b01010,
            0b00100
        },
        // 'R'
        {
            0b11110,
            0b10001,
            0b10001,
            0b11110,
            0b10100,
            0b10010,
            0b10001
        },
        // '0'
        {
            0b01110,
            0b10001,
            0b10011,
            0b10101,
            0b11001,
            0b10001,
            0b01110
        },
        // '1'
        {
            0b00100,
            0b01100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b01110
        },
        // '2'
        {
            0b01110,
            0b10001,
            0b00001,
            0b00010,
            0b00100,
            0b01000,
            0b11111
        },
        // '3'
        {
            0b01110,
            0b10001,
            0b00001,
            0b00110,
            0b00001,
            0b10001,
            0b01110
        },
        // '4'
        {
            0b00010,
            0b00110,
            0b01010,
            0b10010,
            0b11111,
            0b00010,
            0b00010
        },
        // '5'
        {
            0b11111,
            0b10000,
            0b10000,
            0b11110,
            0b00001,
            0b10001,
            0b01110
        },
        // '6'
        {
            0b00110,
            0b01000,
            0b10000,
            0b11110,
            0b10001,
            0b10001,
            0b01110
        },
        // '7'
        {
            0b11111,
            0b00001,
            0b00010,
            0b00100,
            0b01000,
            0b01000,
            0b01000
        },
        // '8'
        {
            0b01110,
            0b10001,
            0b10001,
            0b01110,
            0b10001,
            0b10001,
            0b01110
        },
        // '9'
        {
            0b01110,
            0b10001,
            0b10001,
            0b01111,
            0b00001,
            0b00010,
            0b01100
        },
        // 'S'
        {
            0b01111,
            0b10000,
            0b10000,
            0b01110,
            0b00001,
            0b00001,
            0b11110
        },
        // 'C'
        {
            0b01110,
            0b10001,
            0b10000,
            0b10000,
            0b10000,
            0b10001,
            0b01110
        },
        // 'P'
        {
            0b11110,
            0b10001,
            0b10001,
            0b11110,
            0b10000,
            0b10000,
            0b10000
        },
        // 'Z'
        {
            0b11111,
            0b00001,
            0b00010,
            0b00100,
            0b01000,
            0b10000,
            0b11111
        },
        // 'I'
        {
            0b01110,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b01110
        },
        // 'N'
        {
            0b10001,
            0b11001,
            0b10101,
            0b10101,
            0b10011,
            0b10001,
            0b10001
        },
        // ':'
        {
            0b00000,
            0b00100,
            0b00100,
            0b00000,
            0b00100,
            0b00100,
            0b00000
        },
        // 'H' (Heart symbol)
        {
            0b01010,
            0b11111,
            0b11111,
            0b11111,
            0b01110,
            0b00100,
            0b00000
        }
    };
    
    // Mappa dei caratteri supportati
    int char_index = -1;
    switch(c) {
        case 'G': char_index = 0; break;
        case 'A': char_index = 1; break;
        case 'M': char_index = 2; break;
        case 'E': char_index = 3; break;
        case 'O': char_index = 4; break;
        case 'V': char_index = 5; break;
        case 'R': char_index = 6; break;
        case '0': char_index = 7; break;
        case '1': char_index = 8; break;
        case '2': char_index = 9; break;
        case '3': char_index = 10; break;
        case '4': char_index = 11; break;
        case '5': char_index = 12; break;
        case '6': char_index = 13; break;
        case '7': char_index = 14; break;
        case '8': char_index = 15; break;
        case '9': char_index = 16; break;
        case 'S': char_index = 17; break;
        case 'C': char_index = 18; break;
        case 'P': char_index = 19; break;
        case 'Z': char_index = 20; break;
        case 'I': char_index = 21; break;
        case 'N': char_index = 22; break;
        case ':': char_index = 23; break;
        case 'H': char_index = 24; break; // Heart symbol
        default: return; // Carattere non supportato
    }
    
    if (char_index >= 0) {
        // Disegna il carattere pixel per pixel
        for (int dy = 0; dy < 7; dy++) {
            for (int dx = 0; dx < 5; dx++) {
                if ((chars[char_index][dy] >> (4 - dx)) & 1) {
                    // Scala il carattere per renderlo più grande (3x)
                    for (int sy = 0; sy < 3; sy++) {
                        for (int sx = 0; sx < 3; sx++) {
                            set_pixel(vga_buffer, x + dx * 3 + sx, y + dy * 3 + sy, color);
                        }
                    }
                }
            }
        }
    }
}

// Funzione per disegnare una stringa sullo schermo
void draw_string(volatile uint8_t *vga_buffer, const char *str, int x, int y, uint8_t color) {
    int char_width = 5 * 3; // Larghezza del carattere (5 pixel * scala 3)
    int char_spacing = 2 * 3; // Spazio tra i caratteri (2 pixel * scala 3)
    
    int current_x = x;
    while (*str) {
        draw_char(vga_buffer, *str, current_x, y, color);
        current_x += char_width + char_spacing;
        str++;
    }
}

// Funzione per disegnare un numero sullo schermo
void draw_number(volatile uint8_t *vga_buffer, int number, int x, int y, uint8_t color) {
    char buffer[16];
    int digits = 0;
    
    // Gestisci il caso speciale dello zero
    if (number == 0) {
        buffer[0] = '0';
        digits = 1;
    } else {
        // Converti il numero in una stringa di cifre
        int temp = number;
        while (temp > 0) {
            buffer[digits++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    // Disegna le cifre da destra a sinistra
    int char_width = 5 * 3; // Larghezza del carattere (5 pixel * scala 3)
    int char_spacing = 2 * 3; // Spazio tra i caratteri (2 pixel * scala 3)
    
    // Calcola la larghezza totale del numero
    int total_width = digits * (char_width + char_spacing) - char_spacing;
    
    // Centra il numero rispetto alla posizione x
    int start_x = x - total_width / 2;
    
    // Rimosso il blocco che pulisce l'area prima di disegnare il numero
    
    // Ora disegna i numeri
    for (int i = digits - 1; i >= 0; i--) {
        draw_char(vga_buffer, buffer[i], start_x + (digits - 1 - i) * (char_width + char_spacing), y, color);
    }
}

// Funzione per disegnare il messaggio di Game Over
void draw_game_over(volatile uint8_t *vga_buffer, uint8_t color) {
    // Calcola la posizione centrale per il testo
    int text_width = (5 * 3) * 8 + (2 * 3) * 7; // 8 caratteri, 7 spazi
    int text_x = (VGA_WIDTH - text_width) / 2 - 15;
    int text_y = VGA_HEIGHT / 4;
    
    // Disegna la stringa "GAME OVER"
    draw_string(vga_buffer, "GAME OVER", text_x, text_y, color);
}

// Funzione per disegnare il punteggio
void draw_score(volatile uint8_t *vga_buffer, int score, uint8_t color) {
    // Posizione in alto al centro
    int y = 20;
    
    // Disegna la stringa "SCORE: "
    draw_string(vga_buffer, "SCORE:", VGA_WIDTH / 2 - 70, y, color);
    
    // Disegna il numero del punteggio
    draw_number(vga_buffer, score, VGA_WIDTH / 2 + 80, y, color);
}

// Funzione per disegnare i cuori (vite)
void draw_hearts(volatile uint8_t *vga_buffer, int lives, uint8_t color) {
    // Posizione in alto a destra
    int x = VGA_WIDTH - 150;
    int y = 20;
    
    // Dimensioni del cuore
    int heart_width = 5 * 3; // Larghezza del carattere (5 pixel * scala 3)
    int heart_spacing = 4 * 3; // Spazio tra i cuori (4 pixel * scala 3)
    
    // Prima cancella l'area dei cuori
    for (int dy = 0; dy < 7 * 3; dy++) {
        for (int dx = 0; dx < (heart_width + heart_spacing) * 3; dx++) {
            set_pixel(vga_buffer, x + dx, y + dy, SKY_COLOR);
        }
    }
    
    // Disegna i cuori in base al numero di vite rimaste
    for (int i = 0; i < lives; i++) {
        draw_char(vga_buffer, 'H', x + i * (heart_width + heart_spacing), y, color);
    }
}

// Funzione per mostrare la schermata iniziale e attendere l'input dell'utente
void show_start_screen(volatile uint8_t *vga_buffer, volatile uint32_t *ps2_base, uint8_t *colors) {

    // Schermata iniziale - sfondo nero
    clear_screen(vga_buffer);
    
    // Disegna lo sfondo
    draw_background(vga_buffer);
    
    // Disegna la stringa "PREMERE SPAZIO PER INIZIARE"
    int text_width = (5 * 3) * 24 + (2 * 3) * 23; // 24 caratteri, 23 spazi
    int text_x = (VGA_WIDTH - text_width) / 2 - 30;
    int text_y = VGA_HEIGHT / 6;
    draw_string(vga_buffer, "PREMERE SPAZIO PER INIZIARE", text_x, text_y, colors[6]); // Bianco
    
    // Avvia l'autoplay in background
    autoplay_loop(vga_buffer, ps2_base, colors);
    
    // Svuota il buffer PS/2 prima di iniziare
    while (*ps2_base & 0x8000) {
        volatile uint32_t dummy = *ps2_base;
        (void)dummy;
    }
}

// Funzione per disegnare una nuvola
void draw_cloud(volatile uint8_t *vga_buffer, int x, int y, int size, uint8_t color) {
    // Disegna la forma base della nuvola (un insieme di cerchi sovrapposti)
    int cloud_parts[][2] = {
        {0, 0},       // Centro
        {size/2, 0},  // Destra
        {-size/2, 0}, // Sinistra
        {0, -size/3}, // Sopra
        {size/3, -size/4}, // Sopra-destra
        {-size/3, -size/4} // Sopra-sinistra
    };
    
    // Disegna ogni parte della nuvola come un cerchio
    for (int i = 0; i < 6; i++) {
        int cx = x + cloud_parts[i][0];
        int cy = y + cloud_parts[i][1];
        int radius = (i == 0) ? size/2 : size/3; // Il centro è più grande
        
        // Disegna un cerchio pieno
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                // Verifica se il punto è all'interno del cerchio
                if (dx*dx + dy*dy <= radius*radius) {
                    set_pixel(vga_buffer, cx + dx, cy + dy, color);
                }
            }
        }
    }
}

// Funzione per disegnare le nuvole nel background
void draw_clouds(volatile uint8_t *vga_buffer, uint8_t color) {
    // Posizioni predefinite per le nuvole (x, y, dimensione)
    int clouds[][3] = {
        {VGA_WIDTH / 5, VGA_HEIGHT / 6, 15},
        {VGA_WIDTH / 2, VGA_HEIGHT / 8, 20},
        {VGA_WIDTH * 3 / 4, VGA_HEIGHT / 5, 18},
        {VGA_WIDTH / 8, VGA_HEIGHT / 4, 12},
        {VGA_WIDTH * 7 / 8, VGA_HEIGHT / 7, 26}
    };
    
    // Disegna ogni nuvola
    for (int i = 0; i < 5; i++) {
        draw_cloud(vga_buffer, clouds[i][0], clouds[i][1], clouds[i][2], color);
    }
}