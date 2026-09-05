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

/*
 * create_buf_manager(const char *path, BufManager *m):
 *  @description:
 *    initialises *m with values derived
 *    from the file at *path
 *  @params:
 *   - path: path to file
 *   - m: BufManager to initialise
 *  @return:
 *    void
 *  @notes:
 *    in case of errors, m is set to NULL
 * */
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
    .rem_size = size,
    .curr_buf = create_buffer((const char *)data, map_size)
  };
}

/*
 * update_buf_position(BufManager *m, int32_t off):
 *  @description:
 *    updates *m's internal buffer and file offset
 *    to match the given file offset
 *  @params:
 *   - m: BufManager to initialise
 *   - off: new file offset in bytes
 *  @return:
 *    void
 *  @notes:
 *    this is the internal helper that will be called by
 *    next_buf() and prev_buf()
 * */
void update_buf_position(BufManager *manager, int32_t offset) {
  if ((offset < 0 && manager->file_off + offset < 0)
      || (offset >= 0 && offset >= manager->rem_size)) {
    dprintf(STDERR_FILENO, "Specified offset %d is out-of-bounds\n", offset);
    return;
  }

  manager->file_off += offset;

  const char *new_buf_ptr = manager->curr_buf.start + offset;
  const uint32_t new_buf_len =
    (manager->rem_size >= MAX_BUFLEN ? MAX_BUFLEN : manager->rem_size);

  manager->curr_buf = create_buffer(new_buf_ptr, new_buf_len);
}
