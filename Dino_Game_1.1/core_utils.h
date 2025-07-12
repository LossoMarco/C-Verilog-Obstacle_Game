#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include <stdint.h>

// Definizione della risoluzione VGA
#define VGA_WIDTH 640
#define VGA_HEIGHT 480

// Stride di 1024 byte per riga
#define VGA_STRIDE 1024  

// Indirizzi base - Questi sono gli indirizzi corretti dal file v0.5
#define VGA_BUFFER_BASE 0xFF280000
#define PS2_BASE 0xFF200140
#define PS2_DATA_REG 0
#define PS2_CONTROL_REG 4

// Nelle definizioni dei colori
#define SKY_COLOR rgb8(135, 206, 235)  // Azzurro cielo
#define GRASS_COLOR rgb8(34, 139, 34)  // Verde scuro per l'erba
#define EARTH_COLOR rgb8(139, 69, 19)  // Marrone per la terra
#define GROUND_HEIGHT (VGA_HEIGHT / 2)  // Altezza del terreno (metà schermo)
#define GRASS_HEIGHT 10                 // Altezza dell'erba in pixel

// Definizioni per la coda di eventi
#define MAX_EVENTS 10
#define EVENT_NONE 0
#define EVENT_JUMP 1
#define EVENT_COLLISION 2

// Definizione per gli ostacoli
#define MAX_OBSTACLES 3  // Numero massimo di ostacoli contemporanei
#define OBSTACLE_SPAWN_DISTANCE 2000  // Distanza minima tra gli ostacoli

// Struttura per un evento
typedef struct {
    int type;
    int data;
} Event;

// Struttura per rappresentare un ostacolo
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int active;  // 1 se l'ostacolo è attivo, 0 altrimenti
} Obstacle;

// Funzioni per la gestione degli eventi
void add_event(int type, int data);
Event get_next_event();

// Funzioni di utilità generale
void delay(int cycles);
uint32_t get_time();
uint32_t get_jump_time();
uint32_t random(uint32_t min, uint32_t max);

// Funzione per creare un colore RGB in formato 8-bit (3:3:2)
uint8_t rgb8(uint8_t r, uint8_t g, uint8_t b);

#endif // CORE_UTILS_H