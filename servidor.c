// Criando um Socket para a conexão
// Sockets Servidor - Gerencias as requisições solicitadas pelo o cliente
// Desenvolvedor: Walderlan Sena - Estudo simples na criação de socket Linux

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>   //
#include <sys/types.h>   //  Sockets libraries
#include <netinet/in.h> // Estruturas necessárias para endereços de domínio da Internet.

#define BUFFER_SIZE 400

void response(int);

// function retorna os possivel erros do sockets
void error(const char *msg){
    perror(msg);
    exit(1);
}
// Inicio do main
int main(int argc, char **argv) {

  // Variaveis de manupulação e definição dos dados  e descritor socket
  int socket_servidor, socket_cliente, porta, pid;
  socklen_t clilen;
  // Estruturas default do socket
  struct sockaddr_in servidor, cliente;

  if(argc < 2){
    printf("Entre com o valor da porta para inicializar o servidor !\n");
    exit(1);
  }

  // Inicialização do Socket || Descritor de arquivo
  socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
  // Verifica se foi possivel criar o socket
  if(socket_servidor < 0)
    error("ERROR opening socket\n\n");

  // Define todos os valores em um buffer para zero
  bzero((char *) &servidor, sizeof(servidor));

  porta = atoi(argv[1]); // O número da porta em que o servidor irá ouvir as conexões

  // Passando or Intanciando as estruturas
  servidor.sin_family      = AF_INET;        // código para a família de endereços
  servidor.sin_addr.s_addr = INADDR_ANY;    // Este campo contém o endereço IP do host
  servidor.sin_port        = htons(porta); // converte um número de porta na ordem de byte do host
                                          // para um número de porta na ordem de bytes da rede.
  // Vinculando um nome a um socket
  if(bind(socket_servidor, (struct sockaddr *) &servidor, sizeof(servidor)) < 0)
    printf("ERROR ni binding \n\n");

  printf("Bem Vindo !\n");
  listen(socket_servidor, 2); // Permite que o processo escute no soquete para conexões.
  clilen = sizeof(cliente);

  while(1){
    // faz com que o processo para bloquear até que um cliente se conecta ao servidor
    socket_cliente = accept(socket_servidor, (struct sockaddr *) &servidor,&clilen);
    if(socket_cliente < 0)
      error("Erro accept");
    pid = fork();
    if(pid < 0)
      error("Erro pid");
    if(pid == 0){
      close(socket_servidor);
      response(socket_cliente);
      exit(0);
    }else{
      close(socket_cliente);
    }
  } // end while
  close(socket_servidor);

  return 0;
}
/*
  Existe uma instância separada desta função para cada ligação.
  Ele lida com todas as comunicações uma vez estabelecida a conexão.
*/
void response(int sock){

  int n;
  char buffer[BUFFER_SIZE];

  bzero(buffer,256);
  n = read(sock,buffer,255);

  if(n < 0)
    error("Erro ao ler sock");
  printf("Voce disse: %s\n",buffer);
  n = write(sock, "Mensagem recebida !",18);
  if(n < 0)
    error("Erro ao escrever sock");
}
