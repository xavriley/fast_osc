#include <ruby.h>
#include <rtosc.h>

// Allocate VALUE variables to hold the modules we'll create. Ruby values
// are all of type VALUE. Qnil is the C representation of Ruby's nil.
VALUE FastOsc = Qnil;

// Declare a couple of functions. The first is initialization code that runs
// when this file is loaded, and the second is the actual business logic we're
// implementing.
void Init_fast_osc();
VALUE method_fast_osc_deserialize(VALUE self, VALUE msg);

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

VALUE method_fast_osc_deserialize(VALUE self, VALUE msg) {
  rtosc_arg_itr_t itr;
  char* data = StringValuePtr(msg);
  itr = rtosc_itr_begin(data);
  VALUE output = rb_ary_new();

  rtosc_arg_val_t next_val;

  while(!rtosc_itr_end(itr)) {

    next_val = rtosc_itr_next(&itr);

    switch(next_val.type) {
      case 'i' :
        // INT2FIX() for integers within 31bits.
        rb_ary_push(output, INT2FIX(next_val.val.i));
        break;
      case 'f' :
        rb_ary_push(output, rb_float_new(next_val.val.f));
        break;
      case 's' :
        rb_ary_push(output, rb_str_new2(next_val.val.s));
        break;
      case 'b' :
        rb_ary_push(output, rb_str_new((const char*)next_val.val.b.data, next_val.val.b.len));
        break;
      case 'h' :
        // INT2NUM() for arbitrary sized integers
        rb_ary_push(output, INT2NUM(next_val.val.h));
        break;
      case 't' :
        // OSC time tag
        // not implemented
        break;
      case 'd' :
        rb_ary_push(output, rb_float_new(next_val.val.d));
        break;
      case 'S' :
        rb_ary_push(output, ID2SYM(rb_intern(next_val.val.s)));
        break;
      case 'c' :
        rb_ary_push(output, rb_str_concat(rb_str_new2(""), INT2FIX(next_val.val.i)));
        break;
    }

  }

  return output;
}
