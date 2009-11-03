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

#ifndef DYN_DATA_CC
# define DYN_DATA_CC

# include "dyn.hh"
# include "data.hh"

# ifdef DYNDEBUG
#  include <iostream>
# else
#  include <fstream>
# endif

namespace dyn {

# ifdef DYNDEBUG
  std::ostream& logger(std::cerr);
# else
#  ifdef NDEBUG
  std::ofstream dev_null("/dev/null");
  std::ostream& logger(dev_null);
#  else
  std::ofstream dyn_log("dyn.log");
  std::ostream& logger(dyn_log);
#  endif
# endif

  const NilClass nil_object(0);
  data_nil* nil_proxy = new data_nil(nil_object);
  const data nil;

  namespace internal {
    op operator_push("<<");
    op operator_pop(">>");
    op operator_incr("++");
    op operator_decr("--");
    op operator_plus("+");
    op operator_star("*");
    op operator_equal("==");
    op operator_not_equal("!=");
    meth operator_square_brackets("operator[]");
  }


  meth data::method(const std::string& method_name)
  {
    meth m(method_name, "*", this);
    return m;
  }

  const data& data::operator*() const
  {
    return internal::operator_star(*this).self();
  }

  data data::operator*()
  {
    return internal::operator_star(*this);
  }

  data data::operator[](const data& at)
  {
    return internal::operator_square_brackets(*this, at);
  }

  data::data(const language::var& rhs) : proxy_(0)
  {
    logger << "data(const language::var& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs);
  }

  data::data(const language::val& rhs) : proxy_(0)
  {
    logger << "data(const language::val& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs);
  }

  data::data(language::var& rhs) : proxy_(0)
  {
    logger << "data(language::var& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs);
  }

  data::data(language::val& rhs) : proxy_(0)
  {
    logger << "data(language::val& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs); // FIXME should copy it's contents not just the proxy
  }
}

std::ostream& operator<<(std::ostream& ostr, const dyn::data& d)
{
  return dyn::internal::operator_push(ostr, d).get_ref_on<std::ostream>();
}

std::istream& operator>>(std::istream& istr, dyn::data& d)
{
  return dyn::internal::operator_pop(istr, d).get_ref_on<std::istream>();
}

dyn::data& operator++(dyn::data& d)
{
  dyn::internal::operator_incr(d);
  return d;
}

dyn::data& operator--(dyn::data& d)
{
  dyn::internal::operator_decr(d);
  return d;
}

bool operator!=(const dyn::data& lhs, const dyn::data& rhs)
{
  return dyn::internal::operator_not_equal(lhs, rhs);
}

bool operator==(const dyn::data& lhs, const dyn::data& rhs)
{
  return dyn::internal::operator_equal(lhs, rhs);
}


dyn::data operator+(const dyn::data& lhs, const dyn::data& rhs)
{
  return dyn::internal::operator_plus(lhs, rhs);
}

#endif // ! DYN_DATA_CC
