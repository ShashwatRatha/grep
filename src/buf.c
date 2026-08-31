#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "buf.h"

#define min(a, b) (((a) > (b)) ? (b) : (a))

static Buffer create_buffer(const char *start, uint32_t len) {
  return (Buffer) {
    .start = start,
    .len = len,
    .idx = 0
  };
}

void create_buf_manager(const char *path, BufManager *manager) {
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    perror("open");
    manager = NULL;
    return;
  }

  struct stat st;
  if (fstat(fd, &st) < 0) {
    close(fd);
    perror("stat");
    manager = NULL;
    return;
  }
  uint32_t size = st.st_size;
  uint32_t map_size = min(size, MAX_BUFLEN);

  void *data = mmap(
      NULL, map_size, PROT_READ, MAP_PRIVATE, fd, 0
      );
  if (data == (void *)(MAP_FAILED)) {
    perror("mmap");
    close(fd); manager = NULL;
    return;
  }

  *manager = (BufManager) {
    .fd = fd,
    .file_off = 0,
    .part_match = 0,
    .rem_size = size - map_size,
    .curr_buf = create_buffer((const char *)data, map_size)
  };
}
