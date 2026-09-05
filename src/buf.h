#ifndef BUF_H_
#define BUF_H_

#include <stdint.h>

#define MAX_BUFLEN 65536

/*
 * struct Buffer:
 *  const char *start : start of the buffer
 *  const uint32_t len: length of buffer in bytes (max: 65536)
 *  uint32_t idx      : current index in buffer
 *
 * sizeof(Buffer) = 16
 * */
typedef struct {
  const char *start;
  uint32_t len;
  uint32_t idx;
} Buffer;

/*
 * struct BufManager:
 *  const int32_t fd  : file descriptor of the file open
 *  uint32_t file_off : byte offset of current buffer in file
 *  int32_t part_match: index of pattern partially matched from previous buffer(s)
 *                        (default = 0)
 *  Buffer curr_buf   : current in-memory buffer being processed
 *
 * sizeof(BufManager) = 32
 *
 * @notes:
 *  always: file_off + rem_size = file_size
 * */

typedef struct {
  int32_t fd;
  uint32_t file_off;
  uint32_t rem_size;
  int32_t part_match;
  Buffer curr_buf;
} BufManager;

void create_buf_manager(const char *path, BufManager *manager);
void free_buf_manager(BufManager *manager);

#endif
