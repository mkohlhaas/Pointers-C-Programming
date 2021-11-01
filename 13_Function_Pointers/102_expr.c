// read p. 429 "Single Inheritance Objects and Classes"; at least first two paragraphs

// Class hierarchy: 
// Obj -> Base Expression -> Value Expression 
//                        -> Binary Expression -> Addition Expression 
//                                             -> Subtraction Expression 
//                        -> Variable Expression 

// To create a new (sub-)class: define...
// - hierarchy structure and object variables 
// - init function 
// - virtual functions and initialize them
// - constructor 

// there are two hierarchies: one for the class with its functions and another one
// with its member variables. Both have their own init functions.

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// -------------------------------------------------------
// -------------------- Preliminaries --------------------
// -------------------------------------------------------
typedef struct {
  char a;
} A;

typedef struct {
  A a;
  char b;
} B;

typedef struct {
  B b;
  char c;
} C;

// ------------------------------------------------------
// ------------ Class and object definitions ------------
// ------------------------------------------------------

// this will be the class hierarchy with functions
typedef void* Cls;

// this will be the object hierarchy with variables
// all objects will be derived from 'Obj'
typedef struct
{
  Cls cls;  // pointer to virtual functions; because it's a void pointer it can point to all kinds of classes
} Obj;

// cast x to base
#define basetype(x, base)  ((base*)x)
// cast inst to Obj*, get its class pointer - which is a void pointer - and cast it to base
#define vtbl(inst, base)   basetype(((Obj*)inst)->cls, base)

// allocate space for the class where the functions are
void*
alloc_cls(size_t cls_size)
{
  Cls cls = malloc(cls_size);
  if (!cls) abort(); // error handling
  return cls;
}

// allocate space for the class where the functions are
// and fill the structure with init function
// call init with p - init(p)
#define INIT_CLS(p, init)                                                      \
  do {                                                                         \
    if (!p) {                                                                  \
      p = alloc_cls(sizeof *p);                                                \
      init(p);                                                                 \
    }                                                                          \
  } while(0)

// allocate space for the object where the 'member variables' are
// and set class pointer to its class
void*
alloc_obj(size_t obj_size, Cls cls)
{
  Obj* obj = malloc(obj_size);
  if (!obj) abort(); // error handling
  obj->cls = cls;
  return obj;
}

#define NEW_OBJ(p, cls) alloc_obj(sizeof *p, cls)

// ----------------------------------------------------
// -------------- Base Expression Class ---------------
// ----------------------------------------------------

// Expressions

// ---------- Define object variables -----------------

typedef struct
{
  // derived from obj
  Obj obj;
  // no new variables ...
} BaseExpr;

// ----- Init function -----
void
init_base_expr(BaseExpr* inst)
{
  // Initialize variables
  // does nothing because we have no new variables
}

typedef BaseExpr* Exp;

// ---------- Define virtual functions ----------------

// define class and its virtual functions
typedef struct
{
  void   (*print)(Exp);
  double (*eval)(Exp);
} BaseExprCls;

void
print(Exp expr)
{
  // no 'return' because 'print' does not return a value
  vtbl(expr, BaseExprCls)->print(expr);
}

double
eval(Exp expr)
{
  // 'return' because we 'eval' returns a double
  return vtbl(expr, BaseExprCls)->eval(expr);
}

// Initialize virtual functions
void
init_base_expr_cls(BaseExprCls* cls)
{
  // base_expr_cls is an abstract class
}

// ---------- Constructor -----------------------------

// no constructor because class is abstract

// ----------------------------------------------------------------
// -------------------- Value Expression Class --------------------
// ----------------------------------------------------------------

// ---------- Define object/member variables -----------------

typedef struct
{
  // derived from base_expr
  BaseExpr base_expr;
  // new variable
  double value;
} ValueExpr;

// ----- Init function -----
void
init_value_expr(ValueExpr* val, double value)
{
  // initializer parent variables
  init_base_expr(basetype(val, BaseExpr));
  val->value = value;
}

// ---------- Define virtual functions ----------------

typedef struct
{
  // derived from base_expr_cls
  BaseExprCls base_expr_cls;
  // no new virtual functions
} ValueExprCls;

// Concrete (not abstract) class
ValueExprCls* VALUE_EXPR_CLS;

void
value_expr_print(Exp val)
{
  printf("%.3f", ((ValueExpr*)val)->value);
}

double
value_expr_eval(Exp val)
{
  return ((ValueExpr*)val)->value;
}

// ----- Init function -----
void
init_value_expr_cls(ValueExprCls* cls)
{
  BaseExprCls* base_expr = basetype(cls, BaseExprCls);
  // initialize parent class
  init_base_expr_cls(base_expr);
  // override virtual functions of parent class
  base_expr->print       = value_expr_print;
  base_expr->eval        = value_expr_eval;
}

// ---------- Constructor -----------------------------

Exp
value(double value)
{
  // allocate space for class and set pointer VALUE_EXPR_CLS to it
  // remember it's a macro (text replacement)
  INIT_CLS(VALUE_EXPR_CLS, init_value_expr_cls);
  ValueExpr* val = NEW_OBJ(val, VALUE_EXPR_CLS);
  // cannot put init function for obj because signatures are different
  init_value_expr(val, value);
  return (Exp)val;
}

// -----------------------------------------------------------------
// -------------------- Binary Expression Class --------------------
// -----------------------------------------------------------------

// ---------- Define object variables -----------------

typedef struct
{
  // derived from base_expr
  BaseExpr base_expr;
  // new variables
  char symb;
  Exp  left;
  Exp  right;
} BinExpr;

// ----- Init function -----
void
init_bin_expr(BinExpr* binop, char symb, Exp left, Exp right)
{
  init_base_expr(basetype(binop, BaseExpr));
  binop->symb  = symb;
  binop->left  = left;
  binop->right = right;
}

// ---------- Define virtual functions ----------------

typedef struct
{
  // derived from base_expr_cls
  BaseExprCls base_expr_cls;
  // no new virtual functions
} BinExprCls;

void
print_binexpr(Exp expr)
{
  BinExpr* binop = (BinExpr*)expr;
  putchar('(');
  print(binop->left);
  putchar(')');
  putchar(binop->symb);
  putchar('(');
  print(binop->right);
  putchar(')');
}

// ----- Init function -----
void
init_bin_expr_cls(BinExprCls* cls)
{
  // initialize parent class
  init_base_expr_cls(basetype(cls, BaseExprCls));
  // override virtual functions of parent class
  // virtual function 'eval' is not overridden
  BaseExprCls* base_expr = basetype(cls, BaseExprCls);
  base_expr->print       = print_binexpr;
}

// ---------- Constructor -----------------------------

// no constructor because it's an abstract class

// -------------------------------------------------------------------
// -------------------- Addition Expression Class --------------------
// -------------------------------------------------------------------

// ---------- Define object variables -----------------

typedef struct
{
  // derived from binexpr
  BinExpr binexpr;
  // no new variables ...
} AddExpr;

// ----- Init function -----
void
init_add_expr(AddExpr* expr, Exp left, Exp right)
{
  init_bin_expr(basetype(expr, BinExpr), '+', left, right);
}

// ---------- Define virtual functions ----------------

typedef struct
{
  // derived from binexpr_cls
  BinExprCls binexpr_cls;
  // no new virtual functions
} AddExprCls;

// Concrete (not abstract) class
AddExprCls* ADD_EXPR_CLS;

double
eval_add_expr(Exp expr)
{
  BinExpr* base = basetype(expr, BinExpr);
  return eval(base->left) + eval(base->right);
}

// ----- Init function -----
void
init_add_expr_cls(AddExprCls* cls)
{
  // initialize parent class
  init_bin_expr_cls(basetype(cls, BinExprCls));
  // override virtual functions of parent class
  // virtual function 'print' is not overridden
  BaseExprCls* base_expr = basetype(cls, BaseExprCls);
  base_expr->eval        = eval_add_expr;
}

// ---------- Constructor -----------------------------

Exp
add(Exp left, Exp right)
{
  INIT_CLS(ADD_EXPR_CLS, init_add_expr_cls);
  AddExpr* expr = NEW_OBJ(expr, ADD_EXPR_CLS);
  init_add_expr(expr, left, right);
  return (Exp)expr;
}

// ----------------------------------------------------------------------
// -------------------- Subtraction Expression Class --------------------
// ----------------------------------------------------------------------

// ---------- Define object variables -----------------

typedef struct
{
  // derived from binexpr
  BinExpr binexpr;
  // no new variables ...
} SubExpr;

// ----- Init function -----
void
init_sub_expr(AddExpr* expr, Exp left, Exp right)
{
  init_bin_expr(basetype(expr, BinExpr), '-', left, right);
}

// ---------- Define virtual functions ----------------

typedef struct
{
  // derived from binexpr_cls
  BinExprCls binexpr_cls;
  // no new virtual functions ...
} SubExprCls;

// Concrete class
SubExprCls* SUB_EXPR_CLS;

double
eval_sub_expr(Exp expr)
{
  BinExpr* base = basetype(expr, BinExpr);
  return eval(base->left) - eval(base->right);
}

void
init_sub_expr_cls(SubExprCls* cls)
{
  // initialize parent class
  init_bin_expr_cls(basetype(cls, BinExprCls));
  // override virtual functions of parent class
  // virtual function 'print' is not overridden
  BaseExprCls* base_expr = basetype(cls, BaseExprCls);
  base_expr->eval        = eval_sub_expr;
}

// ---------- Constructor -----------------------------

Exp
sub(Exp left, Exp right)
{
  INIT_CLS(SUB_EXPR_CLS, init_sub_expr_cls);
  AddExpr* expr = NEW_OBJ(expr, SUB_EXPR_CLS);
  init_sub_expr(expr, left, right);
  return (Exp)expr;
}

// -------------------------------------------------------------------
// -------------------- Variable Expression Class --------------------
// -------------------------------------------------------------------

// ---------- Define object variables -----------------

typedef struct
{
  // derived from base_expr
  BaseExpr    base_expr;
  // new object variables
  char const* name;
  double      value;
} VarExpr;

// ----- Init function -----
void
init_var_expr(VarExpr* var, char const* name)
{
  init_base_expr(basetype(var, BaseExpr));
  var->name  = name;
  var->value = NAN;
}

// Variables
typedef VarExpr* Var;

// ---------- Define virtual functions ----------------

typedef struct
{
  // derived from base_expr_cls
  BaseExprCls  base_expr_cls;
  // new virtual functions
  void       (*bind)(Var var, Exp val);
  void       (*unbind)(Var var);
} VarExprCls;

// Concrete (not abstract) class
VarExprCls* VAR_EXPR_CLS;

// definition of new virtual functions
void
bind(Var var, Exp e)
{
  vtbl(var, VarExprCls)->bind(var, e);
}

void
unbind(Var var)
{
  vtbl(var, VarExprCls)->unbind(var);
}

// implementation of new virtual functions
void
var_expr_bind(Var var, Exp e)
{
  var->value = eval(e);
}

void
var_expr_unbind(Var var)
{
  var->value = NAN;
}

// implementation of virtual functions of base class
void
var_expr_print(Exp expr)
{
  Var var = (Var)expr;
  if   (isnan(var->value))  printf("%s", var->name);  // print name
  else                      printf("%f", var->value); // print value
}

double
var_expr_eval(Exp expr)
{
  Var var = (Var)expr;
  return var->value;
}

// ----- Init function -----
void
init_var_expr_cls(VarExprCls* cls)
{
  // initializer parent class
  init_base_expr_cls(basetype(cls, BaseExprCls));
  // override virtual functions in base class
  BaseExprCls* base_expr = basetype(cls, BaseExprCls);
  base_expr->print       = var_expr_print;
  base_expr->eval        = var_expr_eval;
  // new virtual functions
  cls->bind              = var_expr_bind;
  cls->unbind            = var_expr_unbind;
}

// ---------- Constructor -----------------------------

Var
var(char const* name)
{
  INIT_CLS(VAR_EXPR_CLS, init_var_expr_cls);
  Var var = NEW_OBJ(var, VAR_EXPR_CLS);
  init_var_expr(var, name);
  return var;
}

// ----------------------------------------------------
// -------------------- Main --------------------------
// ----------------------------------------------------
int
main()
{
  // Preliminaries
  C* cp     = malloc(sizeof *cp);
  cp->c     = 'c';
  cp->b.b   = 'b';
  cp->b.a.a = 'a';
  assert(((B*)cp)->b   == cp->b.b);
  assert(((B*)cp)->a.a == cp->b.a.a);
  assert(((A*)cp)->a   == cp->b.a.a);
  free(cp);

  // Single Inheritance Objects
  Var x    = var("x");
  Exp expr = add(value(1.0), sub((Exp)x, value(2.0)));
  
  print(expr);                               // prints 'x' for x and evaluates to nan
  printf("\nevaluates to %f\n", eval(expr));

  bind(x, add(value(40.0), value(2.0)));     // set x to 42
  print(expr);                               // now prints '42' for x and evaluates to 41
  printf("\nevaluates to %f\n", eval(expr));
}
