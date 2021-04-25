#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_HTDOCS "/Users/kanta/Desktop/RDocument/AdvancedProgramming/htdocs"

void path_create(char *buf, char *path_name);
void space_divide(char *http_status[], char *buf);
int path_status(char *path_name);

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
    puts("--------------------------------------------------------------");

    // accept http request
    int sock = accept(dsocket, (struct sockaddr *)&client_addr, &sock_len);
    error_check(sock, "accept");
    puts("Connected");

    // puts("************************HTTP REREQUEST************************");
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
      int fd_read = read(file_d, http_response, sizeof(http_response));
      if (fd_read <= 0) {
        break;
      }
      int w_err = write(sock, http_response, fd_read);
      if (w_err < 1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
    }
    close(file_d);

    // finish connection
    close(sock);
  }
  /* listen するsocketの終了 */
  int c_err = close(dsocket);
  error_check(c_err, "close");
  return 1;
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
