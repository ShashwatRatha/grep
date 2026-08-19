#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t buf_size = 0;
  char *line = NULL;

  errno = 0;
  if (getline(&line, &buf_size, stdin) == -1 && errno != 0) {
    perror("getline");
    free(line);
    return 1;
  }

  printf("%s", line);
  free(line);
}
