// Copyright (C) 2007 EPITA Research and Development Laboratory.
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


/* \file doc/tiny/a_la_scoop_2/main.cc

   \brief Tiny sample use of SCOOP 2.  */


#include "../local/scoop.hh"


stc_equip_namespace(abc);



namespace abc
{

  // List of associated types.
  stc_decl_associated_type(value);


  // Iterator

  template <typename Exact>
  struct Iterator : public Concept<Exact>,
		    public automatic::impl<Iterator, Exact>
  {
    stc_typename(value);
    void next()               { this->exact().impl_next();  }
    bool is_valid() const     { return this->exact().impl_is_valid(); }
    void set(const value& v)  { this->exact().impl_set(v);  }
  };



  // array_iterator

# define current    array_iterator<T>
  //                -----------------

# define templ      template <typename T>
# define classname  array_iterator

# define super      top< current >


  stc_Header;

  typedef stc::is<Iterator> category;
  typedef T value;

  stc_End;


  template <typename T>
  class array_iterator : public super
  {
  public:
    
    stc_using(value);
    void impl_next()               { i_ = i_ + 1; }
    bool impl_is_valid() const     { return i_ >= 0 and i_ < n_; }
    void impl_set(const value& v)  { v_ = v; }

    array_iterator(int n)          : i_(0), n_(n) {}
  protected:
    int   i_, n_;
    value v_;
  };

# include "../local/undefs.hh"



  // algo

  template <typename I>
  void algo(Iterator<I>& iter, typename I::value val)
  {
    if (iter.is_valid())
      {
	iter.set(val);
	iter.next();
      }
  }


} // abc


int main()
{
  abc::array_iterator<int> i(7);
  int val = 51;

  abc::algo(i, val);
}
