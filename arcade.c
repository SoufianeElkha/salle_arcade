#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//gcc -pthread arcade.c -o play

#define NB_SEATS 8
#define MAX_PLAYERS 200
#define MAX_CASH 100
#define PLAY_COST 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_consoles;
int cash = MAX_CASH;
int nb_games = 0;

void *player(void *arg)
{
    int id = *(int *)arg;
    int nb_plays = 0;
    int seat = 0;

    while (cash >= PLAY_COST)
    {
        printf("CHF: %d\n", cash);
        printf("[joueur %d] se promène en ville...\n", id);
        usleep(rand() % 5000 + 4000);
        printf("[joueur %d] entre dans l'arcade\n", id);

        pthread_mutex_lock(&mutex);

        if (seat < NB_SEATS)
        {
            printf("CHF: %d\n", cash);
            printf("[joueur %d] est dans la salle d'attente\n", id);
            seat++;
            printf("[joueur %d] paie une pièce. Argent restant des joueurs : %d\n", id, cash - PLAY_COST);
            cash -= PLAY_COST;
            nb_plays++;

            pthread_mutex_unlock(&mutex);
            sem_wait(&sem_consoles);
            printf("[joueur %d] joue jusqu'à la fin du jeu...\n", id);
            usleep(rand() % 2000 + 2000);
            sem_post(&sem_consoles);
            printf("[JEU] terminé (fin de partie) ! Argent de la machine : %d\n", ++nb_games == 100 ? 0 : 1);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            printf("[joueur %d] quitte la boutique (nb_jeux : %d)\n", id, nb_plays);
        }
    }

    printf("[joueur %d] n'a plus d'argent (nb_jeux : %d)\n", id, nb_plays);

    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Utilisation : %s <nb_joueurs>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int nb_players = atoi(argv[1]);

    if (nb_players < 1 || nb_players > MAX_PLAYERS)
    {
        printf("Nombre de joueurs invalide (entre 1 et %d)\n", MAX_PLAYERS);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    //cash = MAX_PLAYERS * PLAY_COST;

    sem_init(&sem_consoles, 0, 1);

    pthread_t tid[nb_players];

    for (int i = 0; i < nb_players; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&tid[i], NULL, player, (void *)id);
    }

    for (int i = 0; i < nb_players; i++)
    {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&sem_consoles);
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}