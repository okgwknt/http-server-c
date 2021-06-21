#include "header.h"

void error_check(int err, char target[]) {
  if (err < 0) {
    perror(target);
    exit(EXIT_FAILURE);
  }
}

int dsocket;
struct sockaddr_in client_addr;
int sock_len;

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    return 1;
  }

  // int dsocket;
  dsocket = socket(AF_INET, SOCK_STREAM, 0);
  error_check(dsocket, "socket");

  struct sockaddr_in server_addr;

  int port = (int)strtol(argv[1], NULL, 10);
  server_addr.sin_port = htons(port);

  server_addr.sin_family = AF_INET;

  // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_addr.s_addr = INADDR_ANY;

  sock_len = sizeof(client_addr);

  int b_err =
      bind(dsocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  error_check(b_err, "bind");

  int l_err = listen(dsocket, 1);
  error_check(l_err, "listen");

  pthread_t thread[4];
  int ret[4];

  for (int i = 0; i < 4; i++) {
    char number[] = "";
    ret[i] = pthread_create(&thread[i], NULL, (void *)thread_func, NULL);
    if (ret[i] != 0) {
      perror("thread");
      exit(EXIT_FAILURE);
    }
  }

  // 変換した後の情報を捨てる
  // ret[0] = pthread_detach(thread[0]);
  // if (ret[0] != 0) {
  //   perror("thread0");
  // }
  // // free(thread[0]);
  // ret[1] = pthread_detach(thread[1]);
  // if (ret[1] != 0) {
  //   perror("thread1");
  // }
  // // free(thread[1]);
  // ret[2] = pthread_detach(thread[2]);
  // if (ret[2] != 0) {
  //   perror("thread2");
  // }
  // // free(thread[2]);
  // ret[3] = pthread_detach(thread[3]);
  // if (ret[3] != 0) {
  //   perror("thread3");
  // }
  // free(thread[3]);

  puts("Thread");
  pthread_exit(NULL);
  return 1;
}

void thread_func(void) {

  int error = pthread_detach(pthread_self());
  if (error < 0) {
    exit(EXIT_FAILURE);
  }

  printf("Thread ID: %d\n", pthread_self());

  while (1) {

    // accept http request
    int sock = accept(dsocket, (struct sockaddr *)&client_addr, &sock_len);
    error_check(sock, "accept");

    puts("-----------------------------------------------------");

    printf("Connected Thread ID: %d\n", pthread_self());

    char http_request[50000];

    request_handler(http_request, sock);

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

  return;
}
