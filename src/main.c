#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

enum ErrCodes{
  ARG_ERROR = 1,
  FILE_OPEN_ERROR,
  GETLINE_ERROR
};

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("USAGE: %s <path-to-input-file>\n", argv[0]);
    return ARG_ERROR;
  }

  size_t buf_size = 0;
  char *line = NULL;

  FILE *input_file = fopen(argv[1], "r");
  if (!input_file) {
    dprintf(STDERR_FILENO, "Couldnt open file %s\n", argv[1]);
    return FILE_OPEN_ERROR;
  }

  while (1) {
    if (getline(&line, &buf_size, input_file) == -1) {
      if (ferror(input_file) != 0) {
        perror("getline");
        free(line);
        return GETLINE_ERROR;
      } else
        break;
    }

    printf("%s", line);
  }

  free(line);
}
