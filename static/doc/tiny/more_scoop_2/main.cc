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


/* \file doc/tiny/more_scoop_2/main.cc

   \brief Extension of the tiny sample use of SCOOP 2.  */

#include <iostream>
#include "../local/scoop.hh"


stc_equip_namespace(abc);



namespace abc
{

  // List of associated types.
  stc_decl_associated_type(value);
  stc_decl_associated_type(reference);
  stc_decl_associated_type(forward);
  stc_decl_associated_type(backward);



  // - - - - - - - - - - - - - - - - - - - -   concepts


  // Iterator

  template <typename Exact>
  struct Iterator : public concept_<Exact>,
		    public automatic::impl<Iterator, Exact>
  {
    stc_typename(value);

    bool is_valid() const     { return this->exact().impl_is_valid(); }
    void set(const value& v)  { this->exact().impl_set(v);  }
  };


  // Forward_Iterator

  template <typename Exact>
  struct Forward_Iterator : public Iterator<Exact>,
			    public automatic::impl<Forward_Iterator, Exact>
  {
    void next()  { this->exact().impl_next();  }
  };


  // Bidirectional_Iterator

  template <typename Exact>
  struct Bidirectional_Iterator : public Forward_Iterator<Exact>,
				  public automatic::impl<Bidirectional_Iterator, Exact>
  {
    void prev()  { this->exact().impl_prev();  }
  };




  // - - - - - - - - - - - - - - - - - - - -   FIXME: plug


  namespace internal
  {

    // selector 1 is about Iterator browsing mode

    typedef  selector<Iterator, 1>  Iterator_browsing;
  
    template <typename E>
    struct case_< Iterator_browsing, E,  1 > : where_< mlc::and_< stc_prop(forward), stc_prop(backward) > >
    {
      typedef Bidirectional_Iterator<E> ret;
    };
  
    template <typename E>
    struct case_< Iterator_browsing, E,  2 > : where_< stc_prop(forward) >
    {
      typedef Forward_Iterator<E> ret;
    };
  
  } // end of internal



  // - - - - - - - - - - - - - - - - - - - -   classes



  // iterator_base


# define super      top<E>
  //               --------
  //                  ^
  //                  |
# define current    iterator_base<E>
  //               ------------------

# define templ      template <typename E>
# define classname  iterator_base


  stc_Header;

  typedef stc::is<Iterator> category;
  typedef stc::abstract value;
  typedef stc::final< stc_deferred(value) & > reference;

  stc_End;


  template <typename E>
  class iterator_base : public super
  {
  public:

    stc_lookup(value);

    // a base class can factor some code!
    // for instance:
    const value& get() const { return v_; }

  protected:
    iterator_base() {}
    value v_;
  };

# include "../local/undefs.hh"




  // lightweight_iterator


# define super      iterator_base<E>
  //               ------------------
  //                  ^
  //                  |
# define current    lightweight_iterator<E>
  //               -------------------------

# define templ      template <typename E>
# define classname  lightweight_iterator


  stc_Header;

  typedef stc::abstract delegatee_type;

  stc_End;


  template <typename E>
  class lightweight_iterator : public super
  {
  public:

    stc_lookup(delegatee_type);

    const delegatee_type& delegatee() const  { return delegatee_; }
          delegatee_type& delegatee()        { return delegatee_; }

  protected:
    lightweight_iterator(delegatee_type& d) : delegatee_(d) {}
    delegatee_type& delegatee_;
  };


# include "../local/undefs.hh"
  




  // array_iterator<T>


  //                iterator_base
  //               ---------------
  //                     ^
  //                     |
# define current    array_iterator<T>
  //               ------------------

# define templ      template <typename T>
# define classname  array_iterator

# define super      iterator_base< current >


  stc_Header;

  typedef T value;
  typedef stc::true_  forward;
  typedef stc::true_ backward;

  stc_End;


  template <typename T>
  class array_iterator : public super
  {
  public:

    stc_using(value);
    void impl_next()               { i_ = i_ + 1; }
    bool impl_is_valid() const     { return i_ >= 0 and i_ < n_; }
    void impl_set(const value& v)  { this->v_ = v; }
    void impl_prev()               { i_ = i_ - 1; }

    array_iterator(int n)          : i_(0), n_(n) {}
  protected:
    int i_, n_;
  };

# include "../local/undefs.hh"







  // algo

  template <typename I>
  void algo(Iterator<I>& iter, typename I::value val)
  {
    std::cout << "Iterator" << std::endl;
    if (iter.is_valid())
      {
	iter.set(val);
      }
  }

  template <typename I>
  void algo(Forward_Iterator<I>& iter, typename I::value val)
  {
    std::cout << "Forward_Iterator" << std::endl;
    while (iter.is_valid())
      {
	std::cout << '.';
	iter.set(val);
	iter.next();
      }
    std::cout << std::endl;
  }

  template <typename I>
  void algo(Bidirectional_Iterator<I>& iter, typename I::value val)
  {
    std::cout << "Bidirectional_Iterator" << std::endl;
    while (iter.is_valid())
      {
	std::cout << '.';
	iter.set(val);
	iter.next();
      }
    std::cout << std::endl;
  }


} // abc





namespace abc
{

  namespace tag
  {
    struct identity;

  } // abc::tag

} // abc

#include "identity.hh"



namespace abc
{



  // skippy


# define current    skippy<I>
  //               -------------------------

# define templ      template <typename I>
# define classname  skippy

# define super      lightweight_iterator< current >


  stc_Header;

  typedef I delegatee_type;
  typedef tag::identity tag;

  stc_End;


  template <typename I>
  class skippy : public super
  {
  public:

    skippy(Iterator<I>& it, unsigned skip)
      :
      super(it.exact()),
      skip_(skip)
    {
    }

    void impl_next()
    {
      for (unsigned i = 0; i <= skip_; ++i)
	this->delegatee().next();
    }

    void impl_prev()
    {
      for (unsigned i = 0; i <= skip_; ++i)
	this->delegatee().prev();
    }

  protected:
    unsigned skip_;
  };


# include "../local/undefs.hh"


  template <typename I>
  skippy<I> skip(Iterator<I>& it, unsigned s)
  {
    skippy<I> tmp(it, s);
    return tmp;
  }



} // abc




int main()
{
  typedef abc::array_iterator<int> iterator_t;
  int val = 51;

  {
    iterator_t i(8);
    abc::algo(i, val);
  }
  {
    iterator_t i(8);
    abc::skippy<iterator_t> ii = skip(i, 1u);
    abc::algo(ii, val);
  }
}
