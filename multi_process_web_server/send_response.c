#include "header.h"

void send_response(char *path_name, int sock) {
  int file_d = open(path_name, O_RDONLY);

  char success[] = "HTTP/1.1 200 OK\r\n";
  char fail[] = "HTTP/1.1 404 Not Found\r\n";
  if (file_d >= 0) {
    write(sock, success, strlen(success) + 1);
  }
  if (file_d < 0) {
    write(sock, fail, strlen(fail) + 1);
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
}
