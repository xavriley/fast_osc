#include <ruby.h>
#include <rtosc.h>
#include <rtosc.c>

// Allocate VALUE variables to hold the modules we'll create. Ruby values
// are all of type VALUE. Qnil is the C representation of Ruby's nil.
VALUE FastOsc = Qnil;

// Declare a couple of functions. The first is initialization code that runs
// when this file is loaded, and the second is the actual business logic we're
// implementing.
void Init_fast_osc();
VALUE method_fast_osc_deserialize(VALUE self, VALUE msg);
VALUE method_fast_osc_serialize(VALUE self, VALUE address, VALUE args);

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
  rb_define_singleton_method(FastOsc, "serialize", method_fast_osc_serialize, 2);
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

int buffer_size_for_ruby_string(VALUE rstring) {
  int str_bytesize = FIX2INT(LONG2FIX(RSTRING_LEN(rstring)));
  return (int)((str_bytesize + sizeof(int) - 1) & ~(sizeof(int) - 1));
}

VALUE method_fast_osc_serialize(VALUE self, VALUE address, VALUE args) {
  char* c_address = StringValueCStr(address);

  int no_of_args = NUM2INT(LONG2NUM(RARRAY_LEN(args)));
  int i, arg_byte_size;
  int max_buffer_size = 0;
  VALUE current_arg;

  //output tags and args list
  VALUE tagstring = rb_str_new2(""); //rtosc will handle comma
  rtosc_arg_t output_args[no_of_args];

  for(i = 0; i < no_of_args; i++) {
    current_arg = rb_ary_entry(args, i);

    switch(TYPE(current_arg)) {
      case T_FIXNUM:
        // max bytes for a single numeric representation
        max_buffer_size += 8;

        if(FIX2LONG(current_arg) < ~(1 << 31)) {
          rb_str_concat(tagstring, rb_str_new2("i"));
          output_args[i].i = FIX2INT(current_arg);
        } else {
          rb_str_concat(tagstring, rb_str_new2("h"));
          output_args[i].h = FIX2LONG(current_arg);
        }
        break;
      case T_FLOAT:
        // now align to 4 byte boundary for sizing output buffer
        max_buffer_size += 8;

        rb_str_concat(tagstring, rb_str_new2("f"));
        output_args[i].f = NUM2DBL(current_arg);
        break;
      case T_STRING:
        // now align to 4 byte boundary for sizing output buffer
        max_buffer_size += buffer_size_for_ruby_string(current_arg);

        rb_str_concat(tagstring, rb_str_new2("s"));
        output_args[i].s = StringValueCStr(current_arg);
        break;
    }
  }

  //add space for the address and tag strings to the buffer
  max_buffer_size += buffer_size_for_ruby_string(address);
  max_buffer_size += buffer_size_for_ruby_string(tagstring);

  // Get next largest power of two for buffer
  max_buffer_size--;
  max_buffer_size |= max_buffer_size >> 1;
  max_buffer_size |= max_buffer_size >> 2;
  max_buffer_size |= max_buffer_size >> 4;
  max_buffer_size |= max_buffer_size >> 8;
  max_buffer_size |= max_buffer_size >> 16;
  max_buffer_size++;

  char buffer[max_buffer_size];


  int len = rtosc_amessage(buffer, sizeof(buffer), c_address, StringValueCStr(tagstring), output_args);

  VALUE output = rb_str_new(buffer, len);

  return output;
}
