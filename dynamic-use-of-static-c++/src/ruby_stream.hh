/* Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#ifndef DYN_RUBY_STREAM_HH
# define DYN_RUBY_STREAM_HH

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
      ruby_script("Embedded Ruby");
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

#endif // ! DYN_RUBY_STREAM_HH
