#include <stdio.h>

typedef struct point
{
  double x, y;
} Point;

typedef struct rectangle
{
  Point upper_left;
  Point lower_right;
} Rectangle;

void
move_point_horizontally(Point* p, double amount)
{
  p->x += amount;
}

void
move_point_vertically(Point* p, double amount)
{
  p->y += amount;
}

void
move_point(Point* p, double delta_x, double delta_y)
{
  move_point_horizontally(p, delta_x);
  move_point_vertically  (p, delta_y);
}

void
print_point(Point* p)
{
  printf("point <%.2f, %.2f>\n", p->x, p->y);
}

void
move_rectangle(Rectangle* rect, double delta_x, double delta_y)
{
  move_point(&rect->upper_left,  delta_x, delta_y);
  move_point(&rect->lower_right, delta_x, delta_y);
}

void
print_rectangle(Rectangle* rect)
{
  printf("rectangle:\n");
  print_point(&rect->upper_left);
  print_point(&rect->lower_right);
}

int
main()
{
  Point p = { .x = 0.0, .y = 0.0 };
  print_point(&p);

  move_point(&p, 10, 10);
  move_point(&p, 10, 10);
  print_point(&p);
  printf("------------------------------\n");
  Rectangle rect = { .upper_left  = { .x =  0.0, .y = 10.0 },
                     .lower_right = { .x = 10.0, .y =  0.0 } };
  print_rectangle(&rect);
  printf("------------------------------\n");
  move_rectangle(&rect, 10, 10);
  move_rectangle(&rect, 10, 10);
  print_rectangle(&rect);
}
