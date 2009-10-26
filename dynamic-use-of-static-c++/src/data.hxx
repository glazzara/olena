#ifndef DYN_DATA_HXX
# define DYN_DATA_HXX

# include <string>
# include <cassert>
# include <iostream>

# include "function.hh"
# include "name_of.hh"

namespace dyn {

  template <typename T>
  T data::convert_to() const
  {
    static fun dyn_data_cast(std::string("data_cast< ") +
                             type() + ", " + mlc_name<T>::of() + " >");
    return dyn_data_cast(*this);
  }


  template <typename T>
  T& data::get_ref_on()
  {
    assert(proxy_);
    data_proxy_by_ref<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy_by_ref<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->obj();
  }


  template <typename T>
  const T& data::get_ref_on() const
  {
    assert(proxy_);
    data_proxy<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->const_ref();
  }


  template <typename T>
  data& data::operator=(const T& rhs)
  {
    assert(proxy_);
    static fun dyn_data_assign(std::string("data_assign<") + proxy()->type() + ", " + mlc_name<T>::of() + ">");
    dyn_data_assign(*this, rhs);
    return *this;
  }

}

std::ostream& operator<<(std::ostream& ostr, const dyn::data& d);
std::istream& operator>>(std::istream& istr, dyn::data& d);
dyn::data& operator++(dyn::data& d);
dyn::data& operator--(dyn::data& d);
bool operator!=(const dyn::data& lhs, const dyn::data& rhs);
bool operator==(const dyn::data& lhs, const dyn::data& rhs);
dyn::data operator+(const dyn::data& lhs, const dyn::data& rhs);

#endif
