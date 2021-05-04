#include "header.h"

void error_check(int err, char target[]) {
  if (err < 0) {
    perror(target);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    return 1;
  }

  int dsocket;
  dsocket = socket(AF_INET, SOCK_STREAM, 0);
  error_check(dsocket, "socket");

  struct sockaddr_in server_addr;

  int port = (int)strtol(argv[1], NULL, 10);
  server_addr.sin_port = htons(port);

  server_addr.sin_family = AF_INET;

  // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_addr.s_addr = INADDR_ANY;

  struct sockaddr_in client_addr;
  int sock_len = sizeof(client_addr);

  int b_err =
      bind(dsocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  error_check(b_err, "bind");

  int l_err = listen(dsocket, 1);
  error_check(l_err, "listen");

  while (1) {

    // accept http request
    int sock = accept(dsocket, (struct sockaddr *)&client_addr, &sock_len);
    error_check(sock, "accept");

    puts("Connected");

    puts("-----------------------------------------------------");

    char http_request[50000];

    receive_request(http_request, sock);

    // レスポンスの動作
    char path_name[256] = PATH_HTDOCS;

    generate_response(http_request, path_name);

    printf("request pathname: %s\n", path_name);

    send_response(path_name, sock);

    // finish connection
    close(sock);
  }
  /* listen するsocketの終了 */
  int c_err = close(dsocket);
  error_check(c_err, "close");
  return 1;
}
