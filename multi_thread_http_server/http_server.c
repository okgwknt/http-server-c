#include "header.h"

void error_check(int err, char target[]) {
  if (err < 0) {
    perror(target);
    exit(EXIT_FAILURE);
  }
}

// int sock;
// int sock0;

int main(int argc, char const *argv[]) {

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

  /*


    スレッド処理をやります


  */

  // pthread_t *thread0 = (pthread_t *)malloc(sizeof(pthread_t));
  // pthread_t *thread1 = (pthread_t *)malloc(sizeof(pthread_t));
  // pthread_t *thread2 = (pthread_t *)malloc(sizeof(pthread_t));
  // pthread_t *thread3 = (pthread_t *)malloc(sizeof(pthread_t));
  // not pointa
  // pthread_t thread0;
  // pthread_t thread1;
  // pthread_t thread2;
  // pthread_t thread3;
  // int ret[4] = {0, 0, 0, 0};
  // ret[0] = pthread_create(&thread0, NULL, (void *)thread_func, NULL);
  // ret[1] = pthread_create(&thread1, NULL, (void *)thread_func, NULL);
  // ret[2] = pthread_create(&thread2, NULL, (void *)thread_func, NULL);
  // ret[3] = pthread_create(&thread3, NULL, (void *)thread_func, NULL);

  // エラー処理
  // if (ret[0] != 0) {
  //   perror("thread0");
  //   exit(EXIT_FAILURE);
  // }
  // if (ret[1] != 0) {
  //   perror("thread1");
  //   exit(EXIT_FAILURE);
  // }
  // if (ret[2] != 0) {
  //   perror("thread2");
  //   exit(EXIT_FAILURE);
  // }
  // if (ret[3] != 0) {
  //   perror("thread3");
  //   exit(EXIT_FAILURE);
  // }

  // // 変換した後の情報を捨てる
  // ret[0] = pthread_detach(thread0);
  // if (ret[0] != 0) {
  //   perror("thread0");
  // }
  // free(thread0);
  // ret[1] = pthread_detach(thread1);
  // if (ret[1] != 0) {
  //   perror("thread1");
  // }
  // free(thread1);
  // ret[2] = pthread_detach(thread2);
  // if (ret[2] != 0) {
  //   perror("thread2");
  // }
  // free(thread2);
  // ret[3] = pthread_detach(thread3);
  // if (ret[3] != 0) {
  //   perror("thread3");
  // }
  // free(thread3);

  return 1;
}
void thread_func(void) {

  while (1) {

    // accept http request
    int sock = accept(dsocket, (struct sockaddr *)&client_addr, &sock_len);
    error_check(sock, "accept");

    puts("Connected");

    puts("-----------------------------------------------------");

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
  if (close(sock0) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}
