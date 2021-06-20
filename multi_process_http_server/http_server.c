#include "header.h"

void error_check(int err, char target[]) {
  if (err < 0) {
    perror(target);
    exit(EXIT_FAILURE);
  }
}
void signal_sigchld(int sig) {
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (WIFEXITED(status) != 0) {
      puts("------------------------------------------------");
      printf("finish PID: %d\n", pid);
      return;
    }
  }
}

int main(int argc, char const *argv[]) {

  if (signal(SIGCHLD, signal_sigchld) == SIG_ERR) {
    exit(EXIT_FAILURE);
  }

  if (argc != 2) {
    return 1;
  }
  int dsocket;
  dsocket = socket(AF_INET, SOCK_STREAM, 0);
  error_check(dsocket, "socket");

  struct sockaddr_in server_addr;
  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  int port = (int)strtol(argv[1], NULL, 10);
  server_addr.sin_port = htons(port);
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

    puts("------------------------------------------------");

    // accept http request
    int sock = accept(dsocket, (struct sockaddr *)&client_addr, &sock_len);
    error_check(sock, "accept");
    puts("Connected");

    pid_t pid;

    pid = fork();
    if (pid == -1) {
      exit(EXIT_FAILURE);
    }

    // child
    if (pid == 0) {

      // リクエストのread
      char http_request[2048];
      request_handler(http_request, sock);

      // レスポンスの動作
      // PATH
      char path_name[256] = PATH_HTDOCS;

      generate_response(http_request, path_name);

      printf("request pathname: %s\n", path_name);

      send_response(path_name, sock);

      // finish connection
      close(sock);
      close(dsocket);

      exit(EXIT_SUCCESS);
    }

    // parent
    if (pid > 0) {
      printf("PID: %d\n", pid);
      close(sock);
    }
  }
  /* listen するsocketの終了 */
  if (close(dsocket) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  return 1;
}