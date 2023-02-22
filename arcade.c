#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_GAMES 100
#define MAX_PLAYERS 100
#define WAITING_ROOM_SIZE 8
#define PLAYER_MONEY 100
#define GAME_MONEY 1

// mutex per la mutua esclusione sull'accesso alla console di gioco
pthread_mutex_t game_console_mutex;

// semafori per la sincronizzazione tra i thread
sem_t player_sem;
sem_t console_sem;
sem_t money_sem;

// contatori per il denaro totale, il numero di partite giocate e il numero di giocatori che hanno terminato le partite
int total_money;
int games_played;
int players_finished;

// funzione eseguita dai thread giocatore
void* player_function(void* id) {
    int player_id = *(int*) id;
    int remaining_money = PLAYER_MONEY;

    while (1) {
        // il giocatore si ferma per un po' di tempo
        usleep((rand() % 5 + 4) * 1000);

        // il giocatore entra nella sala giochi e cerca un posto in attesa
        printf("[player %d] strolls around the city...\n", player_id);
        sem_wait(&player_sem);
        printf("[player %d] enters the arcade\n", player_id);

        int waiting_room_size;
        sem_getvalue(&player_sem, &waiting_room_size);

        // se c'è un posto libero in attesa, il giocatore si siede e paga per la partita
        if (waiting_room_size <= WAITING_ROOM_SIZE) {
            sem_post(&player_sem);
            printf("[player %d] is in the waiting room\n", player_id);
            sem_wait(&money_sem);

            if (total_money >= GAME_MONEY) {
                total_money -= GAME_MONEY;
                remaining_money -= GAME_MONEY;

                printf("[player %d] pays a coin. Remaining players cash: %d\n", player_id, total_money);
                sem_post(&money_sem);
            } else {
                sem_post(&money_sem);
                printf("[player %d] has no more money and leaves the arcade\n", player_id);
                sem_wait(&player_sem);
                sem_post(&player_sem);
                pthread_exit(NULL);
            }

            // il giocatore aspetta che la console di gioco sia libera e si mette a giocare
            pthread_mutex_lock(&game_console_mutex);
            printf("[player %d] plays until game is over...\n", player_id);
            sem_post(&console_sem);
            pthread_mutex_unlock(&game_console_mutex);

            // il giocatore ha terminato la partita
            games_played++;
            remaining_money += PLAYER_MONEY - GAME_MONEY;

            // se il gioco è finito, il giocatore termina
            if (games_played >= MAX_GAMES) {
                sem
