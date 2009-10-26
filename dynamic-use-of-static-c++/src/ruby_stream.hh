#ifndef RUBY_STREAM_HH
#define RUBY_STREAM_HH

// #define DEBUG_RUBY_STREAM

// Protection against already loaded config.h
# undef PACKAGE_BUGREPORT
# undef PACKAGE_NAME
# undef PACKAGE_STRING
# undef PACKAGE_TARNAME
# undef PACKAGE_VERSION
# undef HAVE_DLFCN_H

# ifdef DEBUG_RUBY_STREAM
#  include <iostream>
# endif
# include <string>
# include <sstream>
# include <ruby.h>

// Remove variables defined by the config.h of ruby
# undef PACKAGE_BUGREPORT
# undef PACKAGE_NAME
# undef PACKAGE_STRING
# undef PACKAGE_TARNAME
# undef PACKAGE_VERSION
# undef HAVE_DLFCN_H

# include "_config/config.h"
# include "config.hh"

namespace ruby
{
  struct environment
  {
    environment()
    {
      ruby_init();
      ruby_script("embeddeed ruby");
      ruby_init_loadpath();
    }

    ~environment()
    {
      ruby_finalize();
    }
  };

  struct eval_type {};
  extern eval_type eval;

  struct stream
  {
    stream() : last_value_(Qnil) {}

    void
    push(const eval_type&)
    {
      eval();
    }

    template <class T>
    void
    push(const T& obj)
    {
      contents_ << obj;
    }

    VALUE
    eval()
    {
      if (contents_.str().size() != 0)
      {
#       ifdef DEBUG_RUBY_STREAM
          std::cerr << "Eval (" << contents_.str() << ")" << std::endl;
#       endif
        last_value_ = rb_eval_string(contents_.str().c_str());
        contents_.str(std::string());
      }
      return last_value_;
    }

    VALUE last_value() { return last_value_; }

    protected:

    std::ostringstream  contents_;
    VALUE               last_value_;
  };

}

template <class T>
ruby::stream& operator<< (ruby::stream& stream, const T& obj)
{
  stream.push(obj);
  return stream;
}

#endif
