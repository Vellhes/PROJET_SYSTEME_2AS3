#include "header.h"
int sock;
void nClient(Doss *doss){
    FILE *file;
    int i = 0;
    file = fopen("../clients.txt", "w"); //Ouverture du fichier contenant les réservations en écriture
    while(i < NB_DOSS){
        if (!doss[i].dispo)
        {            
            fputs(doss[i].surname, file);
            fputc('\n',file); //On place le nom dans le fichier
            //On répète pour chaque information
            fputs(doss[i].name, file);
            fputc('\n',file);  
            fputs(doss[i].num_doss, file);
            fputc('\n',file);}i++;}
    fclose(file); //fermeture du fichier
}
void *reserv(void * cl) {
    CL *param;
    int j = 0;
    int i = -1;
    param = (CL*)cl;
    char buffer[100];
    char num_doss[5];
    read(param -> CLsock, buffer, sizeof(buffer));
    if ('r' == buffer[0]) { 
    while (NB_DOSS>j) {
        if (param -> ensmbl_doss[j].dispo) {   //On vérifie d'abord que le dossier est disponible
         param -> ensmbl_doss[j].dispo = 0; //Si dispo alors on le réserve
            i = j;
            j = NB_DOSS;}
        j++;}
        if (i == -1)write(param -> CLsock, "Aucune place disponible :(", 128);
        else {
        write(param -> CLsock, "Place disponible :)", 128);
        read(param -> CLsock, buffer, sizeof(buffer)); //On récupère le nom du client
        param -> ensmbl_doss[i].surname = strdup(buffer); //On affecte les données 
        read(param -> CLsock, buffer, sizeof(buffer)); 
        param -> ensmbl_doss[i].name = strdup(buffer); 
        j = 0;
        while (j < 5) {
        num_doss[j] = '0' + (rand() % 10);
        j++;}
        num_doss[5] = '\0';
        param -> ensmbl_doss[i].num_doss = strdup(num_doss); //On affecte le numéro au dossier
        write(param -> CLsock, num_doss, sizeof(num_doss));
        printf("Dossier : %s réservé par %s %s\n", num_doss, param -> ensmbl_doss[i].surname, param -> ensmbl_doss[i].name);
        nClient(param -> ensmbl_doss);}} else { //Annulation d'une réservation
        read(param -> CLsock, buffer, sizeof(buffer));
        read(param -> CLsock, num_doss, sizeof(num_doss)); 
        j = 0;
        i = 0;
        while (j < NB_DOSS) { //On parcours les dossiers pour chercher celui saisit
            if (!param -> ensmbl_doss[j].dispo) {
                if (!strcmp(buffer, param -> ensmbl_doss[j].surname) &&
                    !strcmp(num_doss, param -> ensmbl_doss[j].num_doss)) { //On vérifie qu'il s'agit du bon dossier pour ensuite le supprimer
                param -> ensmbl_doss[j].dispo = 1; //On rend le dossier dispo
                    param -> ensmbl_doss[j].surname = NULL; //On efface les informations
                    param -> ensmbl_doss[j].name = NULL;
                    param -> ensmbl_doss[j].num_doss = NULL;
                j = NB_DOSS;
                    i = 1;
                    write(param -> CLsock, "Réservation annulée", 128);
                    reserv(param -> ensmbl_doss);}}
            j++;}
        if (!i) 
            write(param -> CLsock, "échec de l'annulation", 128); }
    shutdown(param -> CLsock, 2);
    close(param -> CLsock);};

int main() {
struct sockaddr_in IP;    
struct sockaddr_in IP2;    
int IP3;
Doss *doss;
FILE *file;
int CLsock;
int i;
int j;
pthread_t td;
char buffer[100];
char n;
    system("echo 'IP : ' && hostname -I | cut -d' ' -f1"); 
    srand(time(NULL));
    bzero(&IP, sizeof(IP));
    IP.sin_port = htons(10000); IP.sin_family = AF_INET; IP.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    exit(0); }
    bind(sock, (struct sockaddr * ) &IP, sizeof(IP)); 
    listen(sock, 5);
    IP3 = sizeof(IP2);
 doss = (Doss *) malloc(sizeof(Doss) * NB_DOSS);
    file = fopen("../clients.txt", "r");
    i=0;
    fclose(file);
    while (i < NB_DOSS) { 
        doss[i].dispo = 1;
        doss[i].num_doss = NULL;
        doss[i].surname = NULL;
        doss[i].name = NULL;
        i++;}
    while ((CLsock = accept(sock, (struct sockaddr *) &IP2, &IP3)) > 0) { //On attend au moins une connexion
        CL *Connec;
        Connec = (CL *) malloc(sizeof(CL) * 1);
        Connec -> CLsock = CLsock;
        Connec -> ensmbl_doss = doss;
        pthread_create(&td, NULL, reserv, Connec); }
    close(sock);}
