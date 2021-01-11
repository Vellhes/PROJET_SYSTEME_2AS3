#ifndef HEADER_H
# define HEADER_H
# define NB_DOSS 100 //Maximum de réservations possibles
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <stdbool.h>
# include <time.h>
# include <unistd.h>

# include <netinet/in.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/socket.h>

struct doss{ 
    int dispo;
    char *surname;
    char *name;
    char *num_doss;
};
typedef struct doss Doss;

struct cl{
    Doss *ensmbl_doss;
    int CLsock;
};
typedef struct cl CL;
#endif