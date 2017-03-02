// Cliente socket

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> // define a estrutura hostent, que será usada abaixo.

void error(const char *msg){
  perror(msg);
  exit(0);
}
int main(int argc, char const **argv){

  int servidor, porta, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];
  if(argc < 3){
    printf("Entre com o servidor e a porta: ex: 127.0.0.1 3333 \n");
    exit(0);
  }
  porta = atoi(argv[2]);
  servidor = socket(AF_INET, SOCK_STREAM, 0);
  if(servidor < 0)
    error("Erro no servidor socker");
  server = gethostbyname(argv[1]);
  if(server == NULL)
    fprintf(stderr,"ERROR, no such host\n");
  bzero((char *) &serv_addr,sizeof(serv_addr));

  serv_addr.sin_family  = AF_INET;
  bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_port = htons(porta);

  // Inicio da conexão
  if(connect(servidor,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("Erro ao conectar");

  printf("Welcome to server\n");

  printf("Por favor, entre com a Mensagem: ");

  bzero(buffer, 256);
  fgets(buffer,255,stdin);
  n = write(servidor, buffer, strlen(buffer));
  if(n < 0)
    error("Erro ao escrever no sock");
  bzero(buffer, 256);
  n = read(servidor, buffer, 255);
  if(n < 0)
    error("Erro as ler o sock");
  printf("%s\n",buffer);

  close(servidor);

  return 0;
}
