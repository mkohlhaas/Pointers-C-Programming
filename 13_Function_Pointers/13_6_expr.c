#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ==================== 1. Basic Stuff ====================================================================== //

// The C standard guarantees that the first object you put in a struct goes at the first
// memory address of instances of that struct. If you have a pointer to an instance
// of the struct, then you can safely cast it to a pointer to the first element.

// memory layouts:
// A → char a
// B → char a
//     char b
// C → char a
//     char b
//     char c

typedef struct A
{
  char a;
} A;

typedef struct B
{
  A    a;
  char b;
} B;

typedef struct C
{
  B    b;
  char c;
} C;

// ============ 2. Single Inheritance Objects and Classes =================================================== //

// From p. 429 "Single Inheritance Objects and Classes":

// We can use this to create classes and objects (or instances) in an object-oriented
// programming sense. It is close to how C++ was initially implemented as a preprocessor
// to C. Use nested structs for objects, so derived objects contain the data their base
// cases have. For classes, have a struct for virtual functions (or functions with dynamic
// dispatch, or whatever you want to call them), and use nested structs for derived classes.
// Since we need to be able to both extend instances, so objects of derived classes can
// carry more data than the base classes, and extend classes, so derived classes can have
// more virtual functions than base classes, we need two parallel hierarchies of nested
// structs. Obviously, we cannot put both of these at the top of a struct, so the casting
// trick cannot work that way. Instead, we make objects and classes separate structs. Each
// object will need to know its class, so objects will have a pointer to their class struct.

// Summary
// -------
// Two hierarchies:
//   - class  hierarchy for virtual functions
//   - object hierarchy for member variables

// Class hierarchy:
// obj → base_expr(*) → value_expr
//                    → bin_expr(*) → add_expr
//                                  → sub_expr
//                    → var_expr
//
// (*) abstract class

// ============ Class/Object: Definitions & Allocations ===================================================== //

typedef void *cls;

typedef struct obj
{
  cls cls;
} obj;

// Cast `x` to `base`. `base` can be a class or an object instance.
#define basetype(x, base)   ((base *)x)
#define vtbl(inst, baseCls) basetype (((obj *)inst)->cls, baseCls)

void *
alloc_cls (size_t cls_size)
{
  cls cls = malloc (cls_size);
  if (!cls)
    {
      abort ();
    }
  return cls;
}

#define INIT_CLS(p, init)                                                                                              \
  do                                                                                                                   \
    {                                                                                                                  \
      if (!p)                                                                                                          \
        {                                                                                                              \
          p = alloc_cls (sizeof *p);                                                                                   \
          init (p);                                                                                                    \
        }                                                                                                              \
    }                                                                                                                  \
  while (0)

void *
alloc_obj (size_t obj_size, cls cls)
{
  obj *obj = malloc (obj_size);
  if (!obj)
    {
      abort ();
    }
  obj->cls = cls;
  return obj;
}

#define NEW_OBJ(p, cls) alloc_obj (sizeof *p, cls)

// Look for these sections in the source code:
// == 1. define object hierarchy and variables ==
// == 2. define object init function ==
// == 3. define class hierarchy and virtual functions ==
// == 4. implement global and class virtual functions ==
// == 5. define class init function ==
// == 6. global variable for class ==
// == 7. object constructor ==

// clang-format off
// Explanations:
// 1. define object hierarchy and variables        → a struct that references parent object and member variables
// 2. define object init function                  → calls init function of parent object and initializes member variables
// 3. define class hierarchy and virtual functions → a struct that references parent class and virtual functions
// 4. implement global and class virtual functions → global functions call `vtbl`
// 5. define class init function                   → calls init function of parent object and sets virtual functions to function pointers in the classes
// 6. global variable for class (*)
// 7. object constructor (*)                       → a. creates and inits class (`INIT_CLS` uses class init function from 5. and sets global variable for class from 6.)
//                                                   b. creates object          (`NEW_OBJ`)
//                                                   c. inits object            (object init function from 2.)
// (*) only for concrete classes;
// clang-format on

// Naming Conventions:
//   object (1.): e.g.              `value_expr`
//   typedef value_expr *           `value_expr_t`;
//   object init function (2.):     `init_value_expr`
//   class (3.)                     `value_expr_cls`
//   typedef value_expr_cls *       `value_expr_cls_t`;
//   class init function (5.):      `init_value_expr_cls`
//   global variable for class (6.):`VALUE_EXPR_CLS`
//   object constructor (7.):       `value`
//   class virtual functions:       `value_expr_name_of_fun` (e.g.  `value_expr_print`, `value_expr_eval`, …)

// ============== Base Expression Class ===================================================================== //

// == 1. define object hierarchy and variables ==

typedef struct base_expr
{
  obj obj;
} base_expr;

typedef base_expr *base_expr_t;

// == 2. define object init function ==

void
init_base_expr (base_expr_t inst)
{
}

// == 3. define class hierarchy and virtual functions ==

typedef struct base_expr_cls
{
  void (*print) (base_expr_t);
  double (*eval) (base_expr_t);
} base_expr_cls;

typedef base_expr_cls *base_expr_cls_t;

// == 4. implement global and class virtual functions ==

void
print (base_expr_t expr)
{
  vtbl (expr, base_expr_cls)->print (expr);
}

double
eval (base_expr_t expr)
{
  return vtbl (expr, base_expr_cls)->eval (expr);
}

// == 5. define class init function

void
init_base_expr_cls (base_expr_cls_t cls)
{
}

// ==================== Value Expression Class ============================================================== //

// == 1. define object hierarchy and variables ==

typedef struct value_expr
{
  base_expr base_expr;
  double    value;
} value_expr;

typedef value_expr *value_expr_t;

// == 2. define object init function ==

void
init_value_expr (value_expr_t val, double value)
{
  init_base_expr (basetype (val, base_expr));
  val->value = value;
}

// == 3. define class hierarchy and virtual functions ==

typedef struct value_expr_cls
{
  base_expr_cls base_expr_cls;
} value_expr_cls;

typedef value_expr_cls *value_expr_cls_t;

// == 4. implement global and class virtual functions ==

void
value_expr_print (base_expr_t val)
{
  printf ("%.3f", ((value_expr_t)val)->value);
}

double
value_expr_eval (base_expr_t val)
{
  return ((value_expr_t)val)->value;
}

// == 5. define class init function ==

void
init_value_expr_cls (value_expr_cls_t cls)
{
  base_expr_cls_t base_expr = basetype (cls, base_expr_cls);
  init_base_expr_cls (base_expr);
  base_expr->print = value_expr_print;
  base_expr->eval  = value_expr_eval;
}

// == 6. global variable for class ==

value_expr_cls *VALUE_EXPR_CLS;

// == 7. object constructor ==

base_expr_t
value (double value)
{
  INIT_CLS (VALUE_EXPR_CLS, init_value_expr_cls);   // create and init class
  value_expr_t val = NEW_OBJ (val, VALUE_EXPR_CLS); // create object
  init_value_expr (val, value);                     // init object
  return (base_expr_t)val;
}

// ==================== Binary Expression Class ============================================================= //

// == 1. define object hierarchy and variables ==

typedef struct bin_expr
{
  base_expr   base_expr;
  char        symb;
  base_expr_t left;
  base_expr_t right;
} bin_expr;

typedef bin_expr *bin_expr_t;

// == 2. define object init function ==

void
init_bin_expr (bin_expr_t binop, char symb, base_expr_t left, base_expr_t right)
{
  init_base_expr (basetype (binop, base_expr));
  binop->symb  = symb;
  binop->left  = left;
  binop->right = right;
}

// == 3. define class hierarchy and virtual functions ==

typedef struct bin_expr_cls
{
  base_expr_cls base_expr_cls;
} bin_expr_cls;

typedef bin_expr_cls *bin_expr_cls_t;

// == 4. implement global and class virtual functions ==

void
print_binexpr (base_expr_t expr)
{
  bin_expr_t binop = (bin_expr_t)expr;
  putchar ('(');
  print (binop->left);
  putchar (')');
  putchar (binop->symb);
  putchar ('(');
  print (binop->right);
  putchar (')');
}

// == 5. define class init function ==

void
init_bin_expr_cls (bin_expr_cls_t cls)
{
  init_base_expr_cls (basetype (cls, base_expr_cls));
  base_expr_cls_t base_expr = basetype (cls, base_expr_cls);
  base_expr->print          = print_binexpr;
}

// ==================== Addition Expression Class =========================================================== //

// == 1. define object hierarchy and variables ==

typedef struct add_expr
{
  bin_expr binexpr;
} add_expr;

typedef add_expr *add_expr_t;

// == 2. define object init function ==

void
init_add_expr (add_expr_t expr, base_expr_t left, base_expr_t right)
{
  init_bin_expr (basetype (expr, bin_expr), '+', left, right);
}

// == 3. define class hierarchy and virtual functions ==

typedef struct add_expr_cls
{
  bin_expr_cls binexpr_cls;
} add_expr_cls;

typedef add_expr_cls *add_expr_cls_t;

// == 4. implement global and class virtual functions ==

double
eval_add_expr (base_expr_t expr)
{
  bin_expr_t base = basetype (expr, bin_expr);
  return eval (base->left) + eval (base->right);
}

// == 5. define class init function ==

void
init_add_expr_cls (add_expr_cls_t cls)
{
  init_bin_expr_cls (basetype (cls, bin_expr_cls));
  base_expr_cls_t base_expr = basetype (cls, base_expr_cls);
  base_expr->eval           = eval_add_expr;
}

// == 6. global variable for class ==

add_expr_cls *ADD_EXPR_CLS;

// == 7. object constructor ==

base_expr_t
add (base_expr_t left, base_expr_t right)
{
  INIT_CLS (ADD_EXPR_CLS, init_add_expr_cls);
  add_expr_t expr = NEW_OBJ (expr, ADD_EXPR_CLS);
  init_add_expr (expr, left, right);
  return (base_expr_t)expr;
}

// ==================== Subtraction Expression Class ======================================================== //

// == 1. define object hierarchy and variables ==

typedef struct sub_expr
{
  bin_expr binexpr;
} sub_expr;

typedef sub_expr *sub_expr_t;

// == 2. define object init function ==

void
init_sub_expr (sub_expr_t expr, base_expr_t left, base_expr_t right)
{
  init_bin_expr (basetype (expr, bin_expr), '-', left, right);
}

// == 3. define class hierarchy and virtual functions ==

typedef struct sub_expr_cls
{
  bin_expr_cls binexpr_cls;
} sub_expr_cls;

typedef sub_expr_cls *sub_expr_cls_t;

// == 4. implement global and class virtual functions ==

double
eval_sub_expr (base_expr_t expr)
{
  bin_expr_t base = basetype (expr, bin_expr);
  return eval (base->left) - eval (base->right);
}

// == 5. define class init function ==

void
init_sub_expr_cls (sub_expr_cls_t cls)
{
  init_bin_expr_cls (basetype (cls, bin_expr_cls));
  base_expr_cls_t base_expr = basetype (cls, base_expr_cls);
  base_expr->eval           = eval_sub_expr;
}

// == 6. global variable for class ==

sub_expr_cls *SUB_EXPR_CLS;

// == 7. object constructor ==

base_expr_t
sub (base_expr_t left, base_expr_t right)
{
  INIT_CLS (SUB_EXPR_CLS, init_sub_expr_cls);
  sub_expr_t expr = NEW_OBJ (expr, SUB_EXPR_CLS);
  init_sub_expr (expr, left, right);
  return (base_expr_t)expr;
}

// ==================== Variable Expression Class =========================================================== //

// == 1. define object hierarchy and variables ==

typedef struct var_expr
{
  base_expr   base_expr;
  char const *name;
  double      value;
} var_expr;

typedef var_expr *var_expr_t;

// == 2. define object init function ==

void
init_var_expr (var_expr_t var, char const *name)
{
  init_base_expr (basetype (var, base_expr));
  var->name  = name;
  var->value = NAN;
}

// == 3. define class hierarchy and virtual functions ==

typedef struct var_expr_cls
{
  base_expr_cls base_expr_cls;
  void (*bind) (var_expr_t var, base_expr_t val);
  void (*unbind) (var_expr_t var);
} var_expr_cls;

typedef var_expr_cls *var_expr_cls_t;

// == 4. implement global and class virtual functions ==

void
bind (var_expr_t var, base_expr_t e)
{
  vtbl (var, var_expr_cls)->bind (var, e);
}

void
unbind (var_expr_t var)
{
  vtbl (var, var_expr_cls)->unbind (var);
}

void
var_expr_bind (var_expr_t var, base_expr_t e)
{
  var->value = eval (e);
}

void
var_expr_unbind (var_expr_t var)
{
  var->value = NAN;
}

void
var_expr_print (base_expr_t expr)
{
  var_expr_t var = (var_expr_t)expr;
  if (isnan (var->value))
    {
      printf ("%s", var->name);
    }
  else
    {
      printf ("%f", var->value);
    }
}

double
var_expr_eval (base_expr_t expr)
{
  var_expr_t var = (var_expr_t)expr;
  return var->value;
}

// == 5. define class init function ==

void
init_var_expr_cls (var_expr_cls_t cls)
{
  init_base_expr_cls (basetype (cls, base_expr_cls));
  base_expr_cls_t base_expr = basetype (cls, base_expr_cls);
  base_expr->print          = var_expr_print;
  base_expr->eval           = var_expr_eval;
  cls->bind                 = var_expr_bind;
  cls->unbind               = var_expr_unbind;
}

// == 6. global variable for class ==

var_expr_cls *VAR_EXPR_CLS;

// == 7. object constructor ==

var_expr_t
var (char const *name)
{
  INIT_CLS (VAR_EXPR_CLS, init_var_expr_cls);
  var_expr_t var = NEW_OBJ (var, VAR_EXPR_CLS);
  init_var_expr (var, name);
  return var;
}

// ==================== Main ================================================================================ //

int
main ()
{
  // 1. Basic Stuff
  C *cp     = malloc (sizeof *cp);
  cp->c     = 'c';
  cp->b.b   = 'b';
  cp->b.a.a = 'a';
  assert (cp->b.b == ((B *)cp)->b);
  assert (cp->b.a.a == ((B *)cp)->a.a);
  assert (cp->b.a.a == ((A *)cp)->a);
  free (cp);

  // 2. Single Inheritance Objects and Classes
  var_expr_t  x    = var ("x");
  base_expr_t expr = add (value (1.0), sub ((base_expr_t)x, value (2.0)));

  print (expr);                              // prints 'x' for x and evaluates to nan
  printf ("\nevaluates to %f\n", eval (expr));

  bind (x, add (value (40.0), value (2.0))); // set x to 42
  print (expr);                              // now prints '42' for x and evaluates to 41
  printf ("\nevaluates to %f\n", eval (expr));
}
