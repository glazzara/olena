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

#endif
