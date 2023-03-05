// @author: Soufiane EL KHARMOUDI
// @brief: Travail Pratique sur les sémaphores/mutex (tp3)
// @date: 05/03/2023

/*
But du projet:
    Ce code simule une arcade où les joueurs entrent, attendent leur tour dans une salle d'attente, ils paient pour jouer à un jeu, puis quittent l'arcade.
    La capacité de la salle d'attente est limitée à MAX_WAITING_SEATS.
    Il y a MAX_PLAYERS joueurs entrant dans l'arcade. Le montant total d'argent dans la machine est TOTAL_MONEY.
    Le temps de marche des joueurs dans la ville varie de MIN_STROLL_TIME à MAX_STROLL_TIME secondes,
    tandis que le temps de jeu varie de MIN_GAME_DURATION à MAX_GAME_DURATION secondes.

    Le code utilise des sémaphores et des mutex pour gérer l'accès simultané aux ressources partagées telles que le lobby,l'argent et le nombre de parties jouées.
    Le sémaphore sem_waiting_room gère l'accès à la salle d'attente,tandis que le sémaphore sem_console gère l'accès à la console.
    Les mutex mutex_money, mutex_waiting_seats et mutex_games_played gèrent l'accès aux ressources partagées money, wait_seats et games_played,respectivement.

    La fonction player_play représente le comportement d'un seul joueur.
    Le joueur marche pendant un laps de temps aléatoire, puis entre dans l'arcade.
    S'il y a des places disponibles dans la salle d'attente, le joueur s'y assoit et attend son tour.
    Quand c'est son tour, il paie une pièce et joue jusqu'à ce qu'il ait fini.
    S'il n'y a plus de places disponibles dans le hall ou s'il n'y a plus d'argent dans la machine, le joueur quitte l'arcade.

    La fonction console_play représente le comportement de la console.
    La console attend un joueur puis lance une partie.
    Le nombre total de parties jouées ne doit pas dépasser 100.

    La fonction principale crée des threads de lecteur et de console.
    Attendez les threads du lecteur et de la console avant de terminer le programme.

    Enfin, détruisez les sémaphores et les mutex.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PLAYERS 20
#define MAX_WAITING_SEATS 8
#define MIN_STROLL_TIME 4
#define MAX_STROLL_TIME 8
#define MIN_GAME_DURATION 2
#define MAX_GAME_DURATION 4
#define TOTAL_MONEY 100

int waiting_seats = MAX_WAITING_SEATS;
int money = TOTAL_MONEY;
int games_played = 0;

pthread_mutex_t mutex_money;
pthread_mutex_t mutex_waiting_seats;
pthread_mutex_t mutex_games_played;
sem_t sem_waiting_room;
sem_t sem_console;

/// @brief This function represents a player in an arcade game
/// @param arg pointer to the player number
/// @return NULL
void *player_play(void *arg)
{
    int player_number = *((int *)arg); // player number is extracted from the argument
    int games_played_by_player = 0;    // number of games played by this player
    while (1)
    {
        usleep((MIN_STROLL_TIME + (rand() % (MAX_STROLL_TIME - MIN_STROLL_TIME + 1))) * 1000); // wait for a random amount of time
        printf("[Player %d] strolls around the city ...\n", player_number);                    // player is strolling around

        printf("[Player %d] enters the arcade\n", player_number); // player enters the arcade

        pthread_mutex_lock(&mutex_waiting_seats); // lock the mutex for waiting seats
        if (waiting_seats > 0)
        {
            waiting_seats--; // decrement the number of waiting seats
            printf("[player %d] is in waiting room\n", player_number);
            pthread_mutex_unlock(&mutex_waiting_seats);
            sem_wait(&sem_waiting_room); // wait for a seat in the waiting room

            pthread_mutex_lock(&mutex_money); // lock the mutex for money
            if (money > 0)
            {
                money--; // decrement the money
                printf("[Player %d] pays a coin. Remaining players cash: %d\n", player_number, money);
                pthread_mutex_unlock(&mutex_money);

                sem_wait(&sem_console); // wait for the console to be free
                printf("[Player %d] plays until game is over ...\n", player_number);
                usleep((MIN_GAME_DURATION + (rand() % (MAX_GAME_DURATION - MIN_GAME_DURATION + 1))) * 1000);
                sem_post(&sem_console); // release the console

                games_played_by_player++;
            }
            else
            {
                pthread_mutex_unlock(&mutex_money);
                pthread_mutex_lock(&mutex_waiting_seats);
                waiting_seats++;
                pthread_mutex_unlock(&mutex_waiting_seats);
                sem_post(&sem_waiting_room);
                printf("[Player %d] leaves the shop (nb_games: %d)\n", player_number, games_played_by_player);
                pthread_exit(NULL); // exit the player thread
            }
        }
        else
        {
            pthread_mutex_unlock(&mutex_waiting_seats);
            printf("[player %d] leaves the arcade\n", player_number); // player leaves the arcade
        }
    }
    return NULL;
}

/// @brief This function represents the console in the arcade
/// @return NULL
void *console_play()
{
    while (1)
    {
        printf("[GAME] waiting player to start...\n");
        sem_post(&sem_console);                  // signal that the console is ready for a new game
        pthread_mutex_lock(&mutex_games_played); // lock the mutex for games played
        if (games_played == 100)
        {
            pthread_mutex_unlock(&mutex_games_played);
            pthread_exit(NULL); // exit the console thread if 100 games have been played
        }
        games_played++;
        pthread_mutex_unlock(&mutex_games_played);
        printf("[GAME] start...\n");
        usleep((MIN_GAME_DURATION + (rand() % (MAX_GAME_DURATION - MIN_GAME_DURATION + 1))) * 1000);
        printf("[GAME] finished (game over)! Machine cash: %d\n", games_played);
        pthread_mutex_lock(&mutex_waiting_seats);
        waiting_seats++; // increment the number of waiting seats
        pthread_mutex_unlock(&mutex_waiting_seats);
        sem_post(&sem_waiting_room); // signal that a seat is available in the waiting room
    }
    return NULL;
}

/// @brief The main function that sets up and runs the arcade game simulation
/// @param argc number of command line arguments
/// @param argv array of command line arguments
/// @return 0 if successful, 1 otherwise
int main(int argc, char *argv[])
{
    int i, num_players;
    pthread_t players[MAX_PLAYERS]; // array of player threads
    pthread_t console;              // console thread
    int player_number[MAX_PLAYERS]; // array to store player numbers
    if (argc != 2)
    {
        printf("Usage: \n%s <num_players>\n", argv[0]);
        return 1; // return error if number of players is not specified
    }

    num_players = atoi(argv[1]); // convert string to integer
    if (num_players > MAX_PLAYERS)
    {
        printf("Error: max number of players is %d\n", MAX_PLAYERS);
        return 1; // return error if number of players exceeds the maximum number
    }

    srand(time(NULL));
    pthread_mutex_init(&mutex_money, NULL);            // initialize the mutex for money
    pthread_mutex_init(&mutex_waiting_seats, NULL);    // initialize the mutex for waiting seats
    pthread_mutex_init(&mutex_games_played, NULL);     // initialize the mutex for games played
    sem_init(&sem_waiting_room, 0, MAX_WAITING_SEATS); // initialize the semaphore for waiting room with maximum number of seats
    sem_init(&sem_console, 0, 0);                      // initialize the semaphore for console

    for (i = 0; i < num_players; i++)
    {
        player_number[i] = i + 1;
        pthread_create(&players[i], NULL, player_play, (void *)&player_number[i]); // create player threads
    }

    pthread_create(&console, NULL, console_play, NULL); // create the console thread
    for (i = 0; i < num_players; i++)
    {
        pthread_join(players[i], NULL); // wait for player threads to finish
    }

    pthread_join(console, NULL); // wait for the console thread to finish

    pthread_mutex_destroy(&mutex_money);         // destroy the mutex for money
    pthread_mutex_destroy(&mutex_waiting_seats); // destroy the mutex for waiting seats
    pthread_mutex_destroy(&mutex_games_played);  // destroy the mutex for games played
    sem_destroy(&sem_waiting_room);              // destroy the semaphore for waiting room
    sem_destroy(&sem_console);                   // destroy the semaphore for console

    printf("Numero di partite giocate: %d\n", games_played);
    return 0; // return success
}