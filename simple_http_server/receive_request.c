#include "header.h"

void receive_request(char *http_request, int sock) {
  int read_count = read(sock, http_request, sizeof(char) * 50000);

  printf("%s", http_request);
  if (read_count < 0) {
    perror("read");
    exit(EXIT_FAILURE);
  }
}