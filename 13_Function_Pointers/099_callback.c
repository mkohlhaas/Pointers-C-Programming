#include <stdio.h>
#include <stdlib.h>

typedef enum
{
  MOUSEDOWN,
  MOUSEUP,
  CLICKED,
  DBLCLICKED,
  // more...
} ButtonEvent;

typedef struct button Button;

typedef void (*ButtonCB)(Button*, ButtonEvent);

typedef struct button
{
  char*    text;
  // lots of gui stuff here ...
  ButtonCB cb_func;
} Button;

Button*
new_button(char* text)
{
  Button* b = malloc(sizeof *b);
  if (b) {
    b->text    = text;
    b->cb_func = NULL;
    // ...
  }
  return b;
}

void
install_button_callback(Button* b, ButtonCB cb_func)
{
  b->cb_func = cb_func;
}

// Events
void
click_button(Button* b)
{
  // do whatever GUI thing you want...
  if (b->cb_func) b->cb_func(b, CLICKED);
}

void
double_click_button(Button* b)
{
  if (b->cb_func) b->cb_func(b, DBLCLICKED);
}

// Callback
void
button_callback(Button* but, ButtonEvent event)
{
  if      (event == CLICKED)    printf("Button %s was clicked.\n",        but->text);
  else if (event == DBLCLICKED) printf("Button %s was double-clicked.\n", but->text);
}

int
main()
{
  Button* button = new_button("'My Button'");
  install_button_callback(button, button_callback);
  click_button(button);
  double_click_button(button);

  free(button);
}
