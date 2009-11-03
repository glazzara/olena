/* Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE)

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

#ifndef DYN_NAME_OF_HH
# define DYN_NAME_OF_HH

# include <typeinfo>

# include <string>
# include <sstream>

# ifndef NO_COMPILER_DEMANGLING

std::string demangle(const char* name);

template <class T>
std::string mlc_name_of(const T&)
{
  return demangle(typeid(T).name());
}

template <typename T>
struct mlc_name
{
  static std::string of()
  {
    return demangle(typeid(T).name());
  }
};

# else

# define mlc_set_name(NAME) \
template <> struct mlc_name <NAME> { static std::string of() { return #NAME; } }

# define mlc_set_name_TC(CLASS) \
template <class C> struct mlc_name < CLASS<C> > \
{ static std::string of() { return std::string(#CLASS) + "< " + mlc_name<C>::of() + " >"; } }

# define mlc_set_name_TCC(CLASS) \
template <class C1, class C2> struct mlc_name < CLASS<C1,C2> > \
{ static std::string of() { return std::string(#CLASS) + "< " + mlc_name<C1>::of() + ", " +  mlc_name<C2>::of() + " >"; } }

# define mlc_set_name_TCc(CLASS) \
template <class C1, class C2> struct mlc_name < CLASS<C1,C2> > \
{ static std::string of() { return std::string(#CLASS) + "< " + mlc_name<C1>::of() + " >"; } }

# define mlc_set_name_TCcc(CLASS) \
template <class C1, class C2, class C3> struct mlc_name < CLASS<C1,C2,C3> > \
{ static std::string of() { return std::string(#CLASS) + "< " + mlc_name<C1>::of() + " >"; } }

template <typename message>
struct mlc_error_message
{
  private:
    mlc_error_message() {}
};

template <class T>
struct mlc_name
{
  static std::string of()
  {
    struct mlc_name_of_on_an_unkown_type_please_use_mlc_set_name_on_this_T {};
    mlc_error_message<mlc_name_of_on_an_unkown_type_please_use_mlc_set_name_on_this_T> e;
    return "UNKNOWN!";
  }
};

template <class T>
struct mlc_name2
{
  static std::string of(const T&)
  {
    return mlc_name<T>::of();
  }
};

template <class T>
std::string mlc_name_of(const T& t)
{
  return mlc_name2<T>::of(t);
}

// ptr
template <typename T>
struct mlc_name <T*>
{
  static std::string of() { return mlc_name<T>::of() + "*"; }
};

template <typename T>
struct mlc_name2 <T*>
{
  static std::string of(const T* const& t) { return mlc_name2<T>::of(*t) + "*"; }
};


// const
template <typename T>
struct mlc_name <const T>
{
  static std::string of() { return std::string("const ") + mlc_name<T>::of(); }
};

template <typename T>
struct mlc_name2 <const T>
{
  static std::string of(const T& t) { return std::string("const ") + mlc_name2<T>::of(t); }
};

// ref
template <typename T>
struct mlc_name <T&>
{
  static std::string of() { return mlc_name<T>::of() + "&"; }
};

template <typename T>
struct mlc_name2 <T&>
{
  static std::string of(const T& t) { return mlc_name2<T>::of(t) + "&"; }
};

// T []
template <typename T>
struct mlc_name <T[]>
{
  static std::string of() { return mlc_name<T>::of() + " []"; }
};

template <typename T>
struct mlc_name2 <T[]>
{
  static std::string of(const T []) { return mlc_name<T>::of() + " []"; }
};

// T [N]
template <typename T, unsigned int N>
struct mlc_name <T[N]>
{
  static std::string of()
  {
    std::ostringstream ostr;
    ostr << mlc_name<T>::of() << " [" << N << "]";
    return ostr.str();
  }
};

// built-in

mlc_set_name(void);
mlc_set_name(char);
mlc_set_name(signed char);
mlc_set_name(unsigned char);
mlc_set_name(short);
mlc_set_name(unsigned short);
mlc_set_name(int);
mlc_set_name(unsigned);
mlc_set_name(long);
mlc_set_name(unsigned long);
mlc_set_name(float);
mlc_set_name(double);
mlc_set_name(bool);

// some std

#include <iosfwd>

namespace std {
  template <class _Tp, class _Alloc>                   class vector;
  template <class _Key, class _Compare, class _Alloc>  class set;
  template <class _Tp, class _Alloc>                   class list;
}
mlc_set_name_TCc(std::vector);
mlc_set_name_TCcc(std::set);
mlc_set_name_TCc(std::list);
mlc_set_name(std::string);
mlc_set_name(std::istream);
mlc_set_name(std::ostream);
mlc_set_name_TCc(std::basic_istream);
mlc_set_name_TCc(std::basic_ostream);
mlc_set_name(std::istringstream);
mlc_set_name(std::ostringstream);

# endif

#endif // ! DYN_NAME_OF_HH
