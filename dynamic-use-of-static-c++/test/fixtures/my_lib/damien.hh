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

#ifndef MY_LIB_DAMIEN_HH
# define MY_LIB_DAMIEN_HH

# include <iostream>

struct up
{
  up(int i) : i_(i) {}
  virtual void print(std::ostream&) const = 0;
  virtual void print_noarg() const = 0;
  virtual up* fake_method(const up&) const = 0;
  virtual ~up() {};
  virtual int get_i() const { return i_; }
  int i_;
};


template <typename T>
struct down : public up
{
  down(const T& t) : up(0), t_(t) {}

  virtual void print(std::ostream& ostr) const
  {
    ostr << "down< " << t_ << " >";
  }

  virtual void print_noarg() const
  {
    std::cout << t_ << std::endl;
  }

  virtual up* fake_method(const up& rhs) const
  {
    return new down<int>(i_ + rhs.get_i());
  }

  const T* get_t() const;
  const down<T>* clone();

  const T& t_;

  virtual ~down() {}
};


inline
std::ostream& operator<<(std::ostream& ostr, const up& obj)
{
  obj.print(ostr);
  return ostr;
}

template <typename T>
void print(const T&);

template <typename T>
T sqr(const T&);

#endif // ! MY_LIB_DAMIEN_HH
