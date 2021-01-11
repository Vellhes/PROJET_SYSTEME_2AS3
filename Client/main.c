#include "header.h"
int connex = -1;
void clean() {
    int n;
    while ((n = getchar()) != EOF && n != '\n');
}
int main() {
    struct sockaddr_in serv;struct hostent * host;
    long IPhost;
    char buffer[100]; char buffer2[100]; char n; char IP[20];
    printf("Veuillez saisir l'IP :\n");
    scanf("%s",IP);
    bzero(&serv, sizeof(serv));
    IPhost = inet_addr(IP);
    if (IPhost != (long) - 1)
        bcopy(&IPhost, &serv.sin_addr, sizeof(IPhost));
    else {
        host = gethostbyname(IP);
        if (host == NULL) {
            printf("Problème au niveau de l'hôte\n");
            exit(0);
        }
        bcopy(host -> h_addr, &serv.sin_addr, host -> h_length);
    }
    serv.sin_port = htons(10000);
    serv.sin_family = AF_INET;
    if (((connex = socket(AF_INET, SOCK_STREAM, 0)) < 0) || (connect(connex, (struct sockaddr *) & serv, sizeof(serv)) < 0)) { 
        printf("erreur");
        exit(0);
    }
    printf("Réservation (R) | Annulation (A)\n"); 
    clean();
    scanf("%c", & n);
    if (n == 'R') { 
        write(connex, "r\0", 2); 
        read(connex, buffer, sizeof(buffer));
        printf("%s\n", buffer);
        if (buffer[0] == 'P') { 
            printf("NOM :\n");
            clean();
            scanf("%s", buffer2);
            write(connex, buffer2, sizeof(buffer2)); 
            printf("PRENOM :\n");
            clean();
            scanf("%s", buffer2);
            write(connex, buffer2, sizeof(buffer2)); read(connex, buffer, sizeof(buffer)); 
            printf("N° DE DOSSIER : %s\n", buffer);}}
    if (n == 'A') { 
        write(connex, "a\0", 2); printf("NOM :\n");
        clean();
        scanf("%s", buffer2);
        write(connex, buffer2, sizeof(buffer2)); printf("N° DE DOSSIER :\n");
        clean();
        scanf("%s", buffer2);
        write(connex, buffer2, sizeof(buffer2)); read(connex, buffer, sizeof(buffer)); printf("%s\n", buffer);
    }
    shutdown(connex, 2);
    close(connex);
}