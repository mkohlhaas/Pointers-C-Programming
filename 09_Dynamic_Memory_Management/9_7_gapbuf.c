#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#define MIN_BUF_SIZE 1024

//  ←   capacity    →
//  foo  'gap'    bar
//    ↑          ↑
//  cursor     gap_end

//  "foo" = front text.
//  "bar" = back  text.
//  `capacity`, `cursor`, `gap_end` are sizes (not pointers).
typedef struct gap_buf {
  size_t size;
  size_t cursor;
  size_t gap_end;
  char *buffer;
} GapBuf;

// `init_size` is size of char buffer.
GapBuf *new_buffer(size_t init_size) {
  GapBuf *buf = malloc(sizeof *buf);
  if (!buf) {
    return NULL;
  }
  init_size = MAX(init_size, MIN_BUF_SIZE);
  buf->buffer = malloc(init_size);
  if (!buf->buffer) {
    free(buf);
    return NULL;
  }
  buf->size = init_size;
  buf->cursor = 0;
  buf->gap_end = init_size;
  return buf;
}

void free_buffer(GapBuf *buf) {
  if (!buf) {
    return;
  }
  free(buf->buffer);
  free(buf);
}

// gap buffer sizes
#define gb_front(buf) ((buf)->cursor)               // size of text before cursor      (e.g. 'foo' → 3)
#define gb_back(buf) ((buf)->size - (buf)->gap_end) // size of text after cursor       (e.g. 'bar' → 3)
#define gb_used(buf) (gb_front(buf) + gb_back(buf)) // total number of used characters (e.g. 6)

// Move backtext of `buf` to back of `new_buf`.
void move_backtext(GapBuf *buf, char *new_buf, size_t new_size) {
  memmove(new_buf + new_size - gb_back(buf), // back of new_buf
          buf->buffer + buf->gap_end,        // back of buf
          gb_back(buf));                     // size of backtext
}

// Shrink buf to new_size - never fails.
// No return value; i.e. no bool as in grow_buffer().
// → does not matter if we cannot shrink buffer
void shrink_buffer(GapBuf *buf, size_t new_size) {
  // we have to move first the backtext forward and then shrink
  new_size = MAX(new_size, MIN_BUF_SIZE);
  if (new_size < gb_used(buf)) {
    return; // we do not resize if we'd lose data!
  }
  move_backtext(buf, buf->buffer, new_size); // move backtext forward
  buf->gap_end = new_size - gb_back(buf);    // set gap_end before updating the size or gb_back(buf) will be wrong
  buf->size = new_size;

  // Shrinks only the gap. The sizes remain the same.
  char *new_buf = realloc(buf->buffer, new_size); // allocate a smaller buffer
  if (new_buf) {
    buf->buffer = new_buf;
  }
}

// grow buf to new_size
bool grow_buffer(GapBuf *buf, size_t new_size) {
  // we have to grow first and then move the backtext
  new_size = MAX(new_size, MIN_BUF_SIZE);
  if (buf->size >= new_size) {
    return false;
  }
  char *new_buf = realloc(buf->buffer, new_size); // allocate a larger buffer
  if (!new_buf) {
    return false;
  }
  move_backtext(buf, new_buf, new_size); // move backtext to end of new buffer
  buf->buffer = new_buf;
  buf->gap_end = new_size - gb_back(buf); // set gap_end before updating the size or gb_back(buf) will be wrong
  buf->size = new_size;

  return true;
}

#define capped_dbl_size(s) ((s) < SIZE_MAX / 2) ? (2 * (s)) : SIZE_MAX

// insert 'b'
// before:
// foo               bar
//   ↑              ↑
//  cursor       gap_end
//
// after:
// foob              bar
//    ↑             ↑
//  cursor       gap_end
bool insert_character(GapBuf *buf, char c) {
  if (buf->cursor == buf->gap_end) { // grow buffer if there is no more space
    size_t new_size = capped_dbl_size(buf->size);
    if (!grow_buffer(buf, new_size)) {
      return false;
    }
  }
  buf->buffer[buf->cursor++] = c;
  return true;
}

// before:
// foo               bar
//   ↑              ↑
// cursor        gap_end
//
// after:
// fo               obar
//  ↑              ↑
// cursor       gap_end
void cursor_left(GapBuf *buf) {
  if (buf->cursor > 0) {
    buf->buffer[--buf->gap_end] = buf->buffer[--buf->cursor];
  }
}

// before:
// foo               bar
//  ↑   .           ↑
// cursor        gap_end
//
// after:
// foob               ar
//    ↑              ↑
//  cursor        gap_end
void cursor_right(GapBuf *buf) {
  if (buf->gap_end < buf->size) {
    buf->buffer[buf->cursor++] = buf->buffer[buf->gap_end++];
  }
}

// before:
// foo               bar
//   ↑              ↑
//  cursor       gap_end
//
// after:
// fo                bar
//  ↑               ↑
// cursor        gap_end
void backspace(GapBuf *buf) {
  // the gap is never printed → we just move the cursor left
  if (buf->cursor > 0) {
    buf->cursor--;
  }
  // shrink if necessary
  if (gb_used(buf) < buf->size / 4) {
    shrink_buffer(buf, buf->size / 2);
  }
}

// before:
// foo               bar
//   ↑              ↑
//  cursor       gap_end
//
// after:
// foo                ar
//   ↑               ↑
//  cursor        gap_end
void delete(GapBuf *buf) {
  // the gap is never printed → we just move the cursor right
  if (buf->gap_end < buf->size) {
    buf->gap_end++;
  }
  // shrink if necessary
  if (gb_used(buf) < buf->size / 4) {
    shrink_buffer(buf, buf->size / 2);
  }
}

char *extract_text(GapBuf *buf) {
  // It is insanely unlikely to happen, but if it does then we do not have space for the zero terminal.
  if (SIZE_MAX == gb_used(buf)) {
    return NULL;
  }

  char *text = malloc(gb_used(buf) + 1);
  if (!text) {
    return NULL;
  }

  strncpy(text, buf->buffer, buf->cursor);
  strncpy(text + buf->cursor, buf->buffer + buf->gap_end, gb_back(buf));
  text[gb_used(buf)] = '\0';
  return text;
}

void print_buffer(GapBuf *buf) {
  char *text = extract_text(buf);
  printf("%s\n", text);
  free(text);
}

int main() {
  GapBuf *buf = new_buffer(2);
  if (!buf) {
    perror("Couldn't allocate buffer");
    exit(EXIT_FAILURE);
  }

  {
    printf("start:   %zu\n", buf->cursor);
    insert_character(buf, 'f');
    printf("insert:  %zu\n", buf->cursor);
    insert_character(buf, 'o');
    printf("insert:  %zu\n", buf->cursor);
    insert_character(buf, 'o');
    printf("insert:  %zu\n", buf->cursor);
    print_buffer(buf); // foo_
  }

  {
    cursor_left(buf);
    printf("left:    %zu\n", buf->cursor);
    insert_character(buf, 'x');
    printf("insert:  %zu\n", buf->cursor);
    print_buffer(buf); // fox_o
  }

  {
    cursor_left(buf);
    printf("left:    %zu\n", buf->cursor);
    cursor_left(buf);
    printf("left:    %zu\n", buf->cursor);
    insert_character(buf, 'y');
    printf("insert:  %zu\n", buf->cursor);
    print_buffer(buf); // fy_oxo
  }

  {
    cursor_right(buf);
    printf("right:   %zu\n", buf->cursor);
    cursor_right(buf);
    printf("right:   %zu\n", buf->cursor);
    cursor_right(buf);
    printf("right:   %zu\n", buf->cursor);
    cursor_right(buf);
    printf("right:   %zu\n", buf->cursor);
    insert_character(buf, 'z');
    printf("insert:  %zu\n", buf->cursor);
    print_buffer(buf); // fyoxoz_
  }

  {
    backspace(buf);
    printf("backspc: %zu\n", buf->cursor);
    print_buffer(buf); // fyoxo_
  }

  {
    cursor_left(buf);
    printf("left:    %zu\n", buf->cursor);
    cursor_left(buf);
    printf("left:    %zu\n", buf->cursor);
    delete (buf);
    printf("del:     %zu\n", buf->cursor);
    print_buffer(buf); // fyo_o
  }

  {
    cursor_left(buf);
    printf("left:    %zu\n", buf->cursor);
    insert_character(buf, 'x');
    printf("insert:  %zu\n", buf->cursor);
    print_buffer(buf); // fyy_oo
  }

  free_buffer(buf);
}
