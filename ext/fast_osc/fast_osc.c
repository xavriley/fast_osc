#include <ruby.h>
#include <rtosc.h>

typedef struct {
    int32_t len;
    uint8_t *data;
} rtosc_blob_t;

typedef union {
    int32_t       i;   //i,c,r
    char          T;   //I,T,F,N
    float         f;   //f
    double        d;   //d
    int64_t       h;   //h
    uint64_t      t;   //t
    uint8_t       m[4];//m
    const char   *s;   //s,S
    rtosc_blob_t  b;   //b
} rtosc_arg_t;

size_t rtosc_amessage(char        *buffer,
                      size_t       len,
                      const char  *address,
                      const char  *arguments,
                      const rtosc_arg_t *args);

typedef struct {
    const char    *type_pos;
    const uint8_t *value_pos;
} rtosc_arg_itr_t;

typedef struct {
    char type;
    rtosc_arg_t val;
} rtosc_arg_val_t;

rtosc_arg_itr_t rtosc_itr_begin(const char *msg);
rtosc_arg_val_t rtosc_itr_next(rtosc_arg_itr_t *itr);
int rtosc_itr_end(rtosc_arg_itr_t itr);

// Allocate VALUE variables to hold the modules we'll create. Ruby values
// are all of type VALUE. Qnil is the C representation of Ruby's nil.
VALUE FastOsc = Qnil;

// Declare a couple of functions. The first is initialization code that runs
// when this file is loaded, and the second is the actual business logic we're
// implementing.
void Init_fast_osc();
VALUE method_fast_osc_deserialize(RString *msg);

// Initial setup function, takes no arguments and returns nothing. Some API
// notes:
//
// * rb_define_module() creates and returns a top-level module by name
//
// * rb_define_module_under() takes a module and a name, and creates a new
//   module within the given one
//
// * rb_define_singleton_method() take a module, the method name, a reference to
//   a C function, and the method's arity, and exposes the C function as a
//   single method on the given module
//
void Init_fast_osc() {
  FastOsc = rb_define_module("FastOsc");
  rb_define_singleton_method(FastOsc, "deserialize", method_fast_osc_deserialize, 1);
}

// The business logic -- this is the function we're exposing to Ruby. It returns
// a Ruby VALUE, and takes three VALUE arguments: the receiver object, and the
// method parameters. Notes on APIs used here:
//
// * RARRAY_LEN(VALUE) returns the length of a Ruby array object
// * rb_ary_new2(int) creates a new Ruby array with the given length
// * rb_ary_entry(VALUE, int) returns the nth element of a Ruby array
// * NUM2INT converts a Ruby Fixnum object to a C int
// * INT2NUM converts a C int to a Ruby Fixnum object
// * rb_ary_store(VALUE, int, VALUE) sets the nth element of a Ruby array
//
VALUE method_fast_osc_deserialize(VALUE *rb_string) {
  rtosc_arg_itr_t itr;
  itr = rtosc_itr_begin(*msg, RSTRING_LEN(*msg), )

  VALUE output = rb_ary_new2(2);

  int arg_count = 0;

  while(!rtosc_itr_end(itr)) {
    rb_ary_store(output, arg_count, VALUE(rtosc_itr_next(itr)));
    arg_count++;
  }

  return output;
}
