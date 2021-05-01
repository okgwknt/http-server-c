#include "header.h"

void generate_response(char *http_request, char *path_name) {
  char line[64];
  sscanf(http_request, "%[^\n]", line);
  char *http_status[3];
  divide_space(http_status, line);
  strncat(path_name, http_status[1], 128);

  int path_last = strlen(path_name);
  if (path_name[path_last - 1] == '/') {
    path_name[path_last - 1] = '\0';
  }

  int check = path_status(path_name);

  if (check == 1) {
    char index_html[16] = "/index.html";
    strncat(path_name, index_html, 16);
  }
}

void divide_space(char *http_status[], char *buf) {
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
