#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b)     (((a) > (b)) ? (a) : (b))
#define MIN_BUF_SIZE  1024

// |<-   size       ->|
// foo<-  gap    ->bar
//    ^           ^
//    |           | 
//    cursor      gap_end
typedef struct gap_buf
{
  size_t  size;
  size_t  cursor;
  size_t  gap_end;
  char*   buffer;
} GapBuf;

GapBuf*
new_buffer(size_t init_size)
{
  GapBuf* buf  = malloc(sizeof *buf); if (!buf) return NULL;
  init_size    = MAX(init_size, MIN_BUF_SIZE);
  buf->buffer  = malloc(init_size); if (!buf->buffer) { free(buf); return NULL; }
  buf->size    = init_size;
  buf->gap_end = init_size;
  buf->cursor  = 0;
  return buf;
}

void
free_buffer(GapBuf* buf)
{
  if (!buf) return;
  free(buf->buffer);
  free(buf);
}

#define gb_front(buf) ((buf)->cursor)                  /* size of text before cursor      */
#define gb_back(buf)  ((buf)->size - (buf)->gap_end)   /* size of text after cursor       */
#define gb_used(buf)  (gb_front(buf) + gb_back(buf))   /* total number of used characters */

// move back of buf to back of new_buf
void
move_back(GapBuf* buf, char* new_buf, size_t new_size)
{
  memmove(new_buf + new_size - gb_back(buf),      // back of new_buf
          buf->buffer + buf->gap_end,             // back of buf
          gb_back(buf));                          // size of back text
}

// shrink buf to new_size
// no return value; no bool as in grow_buffer() -> does not matter if we cannot shrink buffer
// in other words: shrink_buffer() cannot fail!
void
shrink_buffer(GapBuf* buf, size_t new_size)
{
  // we have to move first the back text forward and then shrink
  new_size      = MAX(new_size, MIN_BUF_SIZE);
  if (new_size < gb_used(buf)) return;            // we do not resize if we'd lose data!
  move_back(buf, buf->buffer, new_size);          // move back text forward
  buf->gap_end  = new_size - gb_back(buf);        // set gap_end before updating the size or gb_back(buf) will be wrong
  buf->size     = new_size;
  
  char* new_buf = realloc(buf->buffer, new_size); // allocate a smaller buffer
  if (new_buf) buf->buffer = new_buf;
}

// grow buf to new_size
bool
grow_buffer(GapBuf* buf, size_t new_size)
{
  // we have to grow first and then move the back text
  new_size = MAX(new_size, MIN_BUF_SIZE);
  if (buf->size >= new_size) return false;
  char* new_buf = realloc(buf->buffer, new_size); // allocate a larger buffer
  if (!new_buf) return false;
  move_back(buf, new_buf, new_size);              // move back to end of new buffer
  buf->buffer   = new_buf;
  buf->gap_end  = new_size - gb_back(buf);        // set gap_end before updating the size or gb_back(buf) will be wrong
  buf->size     = new_size;

  return true;
}

#define capped_dbl_size(s) ((s) < SIZE_MAX / 2) ? (2 * (s)) : SIZE_MAX

// insert 'b'
// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// foob              bar
//     ^            ^
//     |            |
//     cursor       gap_end
bool
insert_character(GapBuf* buf, char c)
{
  if (buf->cursor == buf->gap_end) {                   // grow buffer if there is no more space
    size_t new_size = capped_dbl_size(buf->size);
    if (!grow_buffer(buf, new_size)) return false;
  }
  buf->buffer[buf->cursor++] = c;                      // insert character c
  return true;
}

// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// fo               obar
//   ^             ^
//   |             |
//   cursor        gap_end
void
cursor_left(GapBuf* buf)
{
  if (buf->cursor > 0) buf->buffer[--buf->gap_end] = buf->buffer[--buf->cursor];
}

// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// foob               ar
//     ^             ^
//     |             |
//     cursor        gap_end
void
cursor_right(GapBuf* buf)
{
  if (buf->gap_end < buf->size) buf->buffer[buf->cursor++] = buf->buffer[buf->gap_end++];
}

// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// fo                bar
//   ^              ^
//   |              |
//   cursor         gap_end
void
backspace(GapBuf* buf)
{
  // the gap is not printed, just move the cursor
  if (buf->cursor > 0)               buf->cursor--;
  if (gb_used(buf) < buf->size / 4)  shrink_buffer(buf, buf->size / 2);
}

// before:
// foo               bar
//    ^             ^
//    |             |
//    cursor        gap_end
// after:
// foo                ar
//    ^              ^
//    |              |
//    cursor         gap_end
void delete(GapBuf* buf)
{
  if (buf->gap_end < buf->size)      buf->gap_end++;
  if (gb_used(buf) < buf->size / 4)  shrink_buffer(buf, buf->size / 2);
}

char*
extract_text(GapBuf* buf)
{
  // It is insanely unlikely to happen, but if it does then we do not have space for the zero terminal.
  if (SIZE_MAX == gb_used(buf)) return NULL;

  char* text = malloc(gb_used(buf) + 1);
  if (!text) return NULL;

  strncpy(text              , buf->buffer               , buf->cursor);
  strncpy(text + buf->cursor, buf->buffer + buf->gap_end, gb_back(buf));
  text[gb_used(buf)] = '\0';
  return text;
}

void
print_buffer(GapBuf* buf) {
  char* text = extract_text(buf);
  printf("%s\n", text);
  free(text);
}

int
main()
{
  GapBuf* buf = new_buffer(2);
  if (!buf) { perror("Couldn't allocate buffer"); exit(1); }

  {
    insert_character(buf, 'f');
    insert_character(buf, 'o');
    insert_character(buf, 'o');
    print_buffer(buf);                  // foo_
  }

  {
    cursor_left(buf);
    insert_character(buf, 'x');
    print_buffer(buf);                  // fox_o
  }

  {
    cursor_left(buf);
    cursor_left(buf);
    insert_character(buf, 'y');
    print_buffer(buf);                  // fy_oxo
  }

  {
    cursor_right(buf);
    cursor_right(buf);
    cursor_right(buf);
    cursor_right(buf);
    insert_character(buf, 'z');
    print_buffer(buf);                  // fyoxoz_
  }

  {
    backspace(buf);
    print_buffer(buf);                  // fyoxo_
  }

  {
    cursor_left(buf);
    cursor_left(buf);
    delete (buf);
    print_buffer(buf);                  // fyo_o
  }

  {
    cursor_left(buf);
    insert_character(buf, 'y');
    print_buffer(buf);                  // fyy_oo
  }

  free_buffer(buf);
}
