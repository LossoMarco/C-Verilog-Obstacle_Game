#include "core_utils.h"

// Coda di eventi
static Event event_queue[MAX_EVENTS];
static int event_count = 0;

// Funzione per aggiungere un evento alla coda
void add_event(int type, int data) {
    if (event_count < MAX_EVENTS) {
        event_queue[event_count].type = type;
        event_queue[event_count].data = data;
        event_count++;
    }
}

// Funzione per ottenere il prossimo evento dalla coda
Event get_next_event() {
    Event e = {EVENT_NONE, 0};
    if (event_count > 0) {
        e = event_queue[0];
        // Sposta tutti gli eventi in avanti
        for (int i = 0; i < event_count - 1; i++) {
            event_queue[i] = event_queue[i + 1];
        }
        event_count--;
    }
    return e;
}

// Funzione di ritardo semplice
void delay(int cycles) {
    volatile int i;
    for (i = 0; i < cycles; i++) {
        // Ciclo vuoto per consumare tempo
    }
}

// Funzione per ottenere un timestamp approssimativo
uint32_t get_time() {
    static uint32_t counter = 0;
    static uint32_t last_update = 0;
    static int update_frequency = 10; // Aggiorna il contatore ogni 10 chiamate
    
    last_update++;
    if (last_update >= update_frequency) {
        counter++;
        last_update = 0;
    }
    
    return counter;
}

// Funzione per ottenere un timestamp approssimativo per il salto
uint32_t get_jump_time() {
    static uint32_t counter = 0;
    counter++;
    return counter;
}

// Funzione per creare un colore RGB in formato 8-bit (3:3:2)
uint8_t rgb8(uint8_t r, uint8_t g, uint8_t b) {
    // Formato 8-bit RGB (3:3:2)
    uint8_t r3 = (r >> 5) & 0x07;  // 3 bit per il rosso
    uint8_t g3 = (g >> 5) & 0x07;  // 3 bit per il verde
    uint8_t b2 = (b >> 6) & 0x03;  // 2 bit per il blu
    
    // Combina i componenti nel formato 8-bit
    return (r3 << 5) | (g3 << 2) | b2;
}

// Funzione per generare un numero casuale
uint32_t random(uint32_t min, uint32_t max) {
    static uint32_t seed = 12345;
    
    // Algoritmo lineare congruenziale semplice
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    
    // Scala il risultato nell'intervallo desiderato
    return min + (seed % (max - min + 1));
}