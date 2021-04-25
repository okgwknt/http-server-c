#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_HTDOCS "../htdocs"

void path_create(char *buf, char *path_name);
void space_divide(char *http_status[], char *buf);
int path_status(char *path_name);

void thread_func(void);

int sock;
int sock0;

int main(int argc, char const *argv[]) {

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

  int b_err =
      bind(dsocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  error_check(b_err, "bind");

  int l_err = listen(dsocket, 1);
  error_check(l_err, "listen");

  // スレッド作成
  // pthread_t *thread0 = (pthread_t *)malloc(sizeof(pthread_t));
  // pthread_t *thread1 = (pthread_t *)malloc(sizeof(pthread_t));
  // pthread_t *thread2 = (pthread_t *)malloc(sizeof(pthread_t));
  // pthread_t *thread3 = (pthread_t *)malloc(sizeof(pthread_t));
  // not pointa
  pthread_t thread0;
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;
  int ret[4] = {0, 0, 0, 0};
  ret[0] = pthread_create(&thread0, NULL, (void *)thread_func, NULL);
  ret[1] = pthread_create(&thread1, NULL, (void *)thread_func, NULL);
  ret[2] = pthread_create(&thread2, NULL, (void *)thread_func, NULL);
  ret[3] = pthread_create(&thread3, NULL, (void *)thread_func, NULL);

  // エラー処理
  if (ret[0] != 0) {
    perror("thread0");
    exit(EXIT_FAILURE);
  }
  if (ret[1] != 0) {
    perror("thread1");
    exit(EXIT_FAILURE);
  }
  if (ret[2] != 0) {
    perror("thread2");
    exit(EXIT_FAILURE);
  }
  if (ret[3] != 0) {
    perror("thread3");
    exit(EXIT_FAILURE);
  }

  // 変換した後の情報を捨てる
  ret[0] = pthread_detach(thread0);
  if (ret[0] != 0) {
    perror("thread0");
  }
  free(thread0);
  ret[1] = pthread_detach(thread1);
  if (ret[1] != 0) {
    perror("thread1");
  }
  free(thread1);
  ret[2] = pthread_detach(thread2);
  if (ret[2] != 0) {
    perror("thread2");
  }
  free(thread2);
  ret[3] = pthread_detach(thread3);
  if (ret[3] != 0) {
    perror("thread3");
  }
  free(thread3);

  return 1;
}
void thread_func(void) {
  struct sockaddr_in client_addr;
  int sock_len = sizeof(client_addr);

  while (1) {

    // accept http request
    if ((sock = accept(sock0, (struct sockaddr *)&client_addr, &sock_len)) ==
        -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    puts("Connected");

    puts("************************HTTP REREQUEST************************");
    // リクエストのread
    char http_request[2048];
    memset(http_request, 0, sizeof(http_request));
    int read_count = read(sock, http_request, sizeof(http_request));
    if (read_count < 0) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    // レスポンスの動作
    // PATH
    char path_name[256] = PATH_HTDOCS;

    path_create(http_request, path_name);
    // パスの一番後ろは / なしになっているはず
    printf("request pathname: %s\n", path_name);

    int check = path_status(path_name);

    if (check == 1) {
      char index_html[16] = "/index.html";
      strncat(path_name, index_html, 16);
    }

    int file_d = open(path_name, O_RDONLY);
    // FILE *file_p = fopen(path_name, "rb");
    // char test[2048];
    // while (read(file_d, test, 2048) > 0) {
    //   printf("%s", test);
    // }

    char success[] = "HTTP/1.1 200 OK\r\n";
    char fail[] = "HTTP/1.1 404 Not Found\r\n";
    if (file_d >= 0) {
      write(sock, success, strlen(success));
    }
    if (file_d < 0) {
      write(sock, fail, 25);
    }
    char crlf[] = "\r\n";
    write(sock, crlf, strlen(crlf));

    while (1) {
      char http_response[4096];
      // memset(http_response, 0, sizeof(http_response));
      int fd_read = read(file_d, http_response, sizeof(http_response));
      if (fd_read <= 0) {
        break;
      }
      //   int fd_read = fread(http_response, sizeof(char), 32, file_p);
      int w_err = write(sock, http_response, fd_read);
      if (w_err < 1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
    }
    close(file_d);
    // finish connection
    close(sock);

    // finish connection
    close(sock);

    puts("--------------------------------------------------------------");
  }
  /* listen するsocketの終了 */
  if (close(sock0) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}
void path_create(char *buf, char *path_name) {
  // パスを分析
  char line[64];
  sscanf(buf, "%[^\n]", line);
  fpurge(stdin);
  char *http_status[3];
  space_divide(http_status, line);
  strncat(path_name, http_status[1], 128);

  int path_last = strlen(path_name);
  if (path_name[path_last - 1] == '/') {
    path_name[path_last - 1] = '\0';
  }
}

void space_divide(char *http_status[], char *buf) {
  int i;

  for (i = 0; *buf != '\0'; i++) {
    while (*buf == ' ') {
      *buf = '\0';
      buf++;
    }

    if (*buf == '\0') {
      break;
    }
    http_status[i] = buf;
    while (*buf != '\0' && *buf != ' ') {
      buf++;
    }
  }

  http_status[i] = 0;
}
int path_status(char *path_name) {
  // ファイルかディレクトリかを確認する
  struct stat status_path;
  if (stat(path_name, &status_path) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }
  // ディレクトリの時
  if (S_ISDIR(status_path.st_mode)) {
    return 1;
  }
  // ファイルの時
  if (S_ISREG(status_path.st_mode)) {
    return 0;
  }
  return 0;
}