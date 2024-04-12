#include <stdio.h>
#include <stdlib.h>

// -- Typedefs ----------------------------------------------------

typedef enum
{
  MOUSEDOWN,
  MOUSEUP,
  CLICKED,
  DBLCLICKED,
  // …
} button_event;

struct button;

typedef void (*button_cb) (struct button *, button_event);

typedef struct button
{
  char     *text;
  button_cb cb_func;
  // lots of gui stuff here …
} button;

// -- Button ------------------------------------------------------

button *
new_button (char *text, button_cb cb_func)
{
  button *b = malloc (sizeof *b);
  if (b)
    {
      b->text    = text;
      b->cb_func = cb_func;
      // …
    }
  return b;
}

// -- callback
void
button_callback (button *but, button_event event)
{
  switch (event)
    {
    case CLICKED:
      printf ("Button %s was clicked.\n", but->text);
      break;
    case DBLCLICKED:
      printf ("Button %s was double-clicked.\n", but->text);
      break;
    default:
      break;
    }
}

// -- Events Simulation -------------------------------------------

void
click_button (button *b)
{
  b->cb_func (b, CLICKED);
}

void
double_click_button (button *b)
{
  b->cb_func (b, DBLCLICKED);
}

// -- Main --------------------------------------------------------

int
main ()
{
  button *button = new_button ("'My Button'", button_callback);

  // invoke events
  click_button (button);
  double_click_button (button);

  free (button);
}
