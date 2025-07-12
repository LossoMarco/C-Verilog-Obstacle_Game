#include <stdio.h>
#include <stdint.h>

// Inclusione delle librerie create
#include "core_utils.h"
#include "graphics.h"
#include "text_ui.h"
#include "game_entities.h"
#include "game_logic.h"

int main() {
    // Puntatori ai registri hardware - utilizziamo uint8_t per il buffer VGA
    volatile uint8_t *vga_buffer = (volatile uint8_t *)VGA_BUFFER_BASE;
    volatile uint32_t *ps2_base = (volatile uint32_t *)PS2_BASE;
    
    // Flag per il loop principale del programma
    int quit_game = 0;

    // Colori disponibili in formato 8-bit
    uint8_t colors[] = {
        rgb8(255, 0, 0),      // Rosso (0xE0)
        rgb8(0, 255, 0),      // Verde (0x1C)
        rgb8(0, 0, 255),      // Blu   (0x03)
        rgb8(255, 255, 0),    // Giallo (0xFC)
        rgb8(255, 255, 255),  // Bianco (0xFF)
        rgb8(128, 0, 128),    // Viola (0x80)
        rgb8(0, 0, 0)         // Nero (0x00)
    };

    // Inizializza lo schermo con un colore di test per verificare che funzioni
    clear_screen(vga_buffer);
    
    // Mostra la schermata iniziale e attendi l'input dell'utente
    show_start_screen(vga_buffer, ps2_base, colors);
    
    // Loop principale del programma (permette di riavviare il gioco)
    while (!quit_game) {
        // Svuota il buffer VGA (schermo nero)
        clear_screen(vga_buffer);

        // Disegna lo sfondo
        draw_background(vga_buffer);
        
        // Esegui il loop principale del gioco
        game_loop(vga_buffer, ps2_base, colors);
        
        // Gestisci il riavvio o l'uscita dal gioco
        int score = 0; // Il punteggio dovrebbe essere passato dal game_loop
        restart_game(vga_buffer, ps2_base, &quit_game, score);
    }
    
    return 0;
}