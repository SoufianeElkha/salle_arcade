# But du projet:

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

```c
soufiane@sek:~/Bureau/PCO/pcu_tp3_salle_arcade/src$ make clean
rm -f tp3_salle_arcade *.o
Cleanup complete!
soufiane@sek:~/Bureau/PCO/pcu_tp3_salle_arcade/src$ make
gcc  -Wall -Wextra -std=gnu99 -fsanitize=address -fsanitize=leak -fsanitize=undefined   -c -o main.o main.c
gcc  main.o -Wall -Wextra -std=gnu99 -fsanitize=address -fsanitize=leak -fsanitize=undefined -o tp3_salle_arcade -lpthread
soufiane@sek:~/Bureau/PCO/pcu_tp3_salle_arcade/src$ 
soufiane@sek:~/Bureau/PCO/pcu_tp3_salle_arcade/src$ ./tp3_salle_arcade 20
...
[Player 4] strolls around the city ...
[Player 4] enters the arcade
[player 4] is in waiting room
[Player 4] leaves the shop (nb_games: 8)
[GAME] finished (game over)! Machine cash: 95
[GAME] waiting player to start...
[GAME] start...
[Player 7] strolls around the city ...
[Player 7] enters the arcade
[player 7] is in waiting room
[Player 7] leaves the shop (nb_games: 2)
[Player 10] strolls around the city ...
[Player 10] enters the arcade
[player 10] is in waiting room
[Player 10] leaves the shop (nb_games: 6)
[GAME] finished (game over)! Machine cash: 96
[GAME] waiting player to start...
[GAME] start...
[GAME] finished (game over)! Machine cash: 97
[GAME] waiting player to start...
[GAME] start...
[GAME] finished (game over)! Machine cash: 98
[GAME] waiting player to start...
[GAME] start...
[GAME] finished (game over)! Machine cash: 99
[GAME] waiting player to start...
[GAME] start...
[GAME] finished (game over)! Machine cash: 100
[GAME] waiting player to start...
All games: 100
soufiane@sek:~/Bureau/PCO/pcu_tp3_salle_arcade/src$ 
```