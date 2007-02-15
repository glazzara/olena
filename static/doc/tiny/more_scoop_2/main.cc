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
#include "concepts.hh"



namespace abc
{

  stc_equip_namespace;

  
  // List of associated types.
  stc_decl_associated_type(value);
  stc_decl_associated_type(forward);
  stc_decl_associated_type(backward);
  stc_decl_associated_type(read);
  stc_decl_associated_type(write);



  // - - - - - - - - - - - - - - - - - - - -   concepts


  // Iterator

  template <typename Exact>
  struct Iterator : public virtual any<Exact>,
		    public automatic::get_impl<Iterator, Exact>
  {
    stc_typename(value);

    void init()               { return this->exact().impl_init(); }
    bool is_valid() const     { return this->exact().impl_is_valid(); }
  };



  // Forward_Iterator

  template <typename Exact>
  struct Forward_Iterator : public virtual Iterator<Exact>,
			    public automatic::get_impl<Forward_Iterator, Exact>
  {
    void next()  { this->exact().impl_next();  }
  };


  // Backward_Iterator

  template <typename Exact>
  struct Backward_Iterator : public virtual Iterator<Exact>,
			     public automatic::get_impl<Backward_Iterator, Exact>
  {
    void prev()  { this->exact().impl_prev();  }
  };


  // Bidirectional_Iterator

  template <typename Exact>
  struct Bidirectional_Iterator : public Forward_Iterator<Exact>,
				  public Backward_Iterator<Exact>,
				  public automatic::get_impl<Bidirectional_Iterator, Exact>
  {
  };



  // Read_Iterator

  template <typename Exact>
  struct Read_Iterator : public virtual Iterator<Exact>,
			 public automatic::get_impl<Read_Iterator, Exact>
  {
    typedef Iterator<Exact> super;
    stc_using(value);
    // FIXME: read below
    const value& get_value() const   { return this->exact().impl_get_value(); }
  };


  // Read_Only_Iterator

  template <typename Exact>
  struct Read_Only_Iterator : public Read_Iterator<Exact>,
			      public automatic::get_impl<Read_Only_Iterator, Exact>
  {
  };


  // Write_Iterator

  template <typename Exact>
  struct Write_Iterator : public virtual Iterator<Exact>,
			  public automatic::get_impl<Write_Iterator, Exact>
  {
    typedef Iterator<Exact> super;
    stc_using(value);
    void set_value(const value& v)   { return this->exact().impl_set_value(v); }
  };


  // Write_Only_Iterator

  template <typename Exact>
  struct Write_Only_Iterator : public Write_Iterator<Exact>,
			       public automatic::get_impl<Write_Only_Iterator, Exact>
  {
  };


  // Read_Write_Iterator

  template <typename Exact>
  struct Read_Write_Iterator : public Read_Iterator<Exact>,
			       public Write_Iterator<Exact>,
			       public automatic::get_impl<Read_Write_Iterator, Exact>
  {
  };


  


  // FIXME: pick a solution:
  //
  //     typedef Iterator<Exact> super;
  //     stc_using(value);
  //
  //     stc_super_is(Iterator);
  //     stc_using(value);
  //
  //     stc_super_using(Iterator, value);
  //
  //     stc_using_from(value, Iterator);



  // - - - - - - - - - - - - - - - - - - - -   FIXME: plug


  namespace internal
  {

    // selector 1 is about Iterator browsing mode

    typedef  selector<Iterator, 1>  Iterator_browsing;
  
    template <typename E>
    struct case_< Iterator_browsing, E,  1 > : where_< mlc::and_< stc_is(forward),
								  stc_is(backward) > >
    {
      typedef Bidirectional_Iterator<E> ret;
    };
  
    template <typename E>
    struct case_< Iterator_browsing, E,  2 > : where_< stc_is(forward) >
    {
      typedef Forward_Iterator<E> ret;
    };
  
    template <typename E>
    struct case_< Iterator_browsing, E,  3 > : where_< stc_is(backward) >
    {
      typedef Backward_Iterator<E> ret;
    };


    // selector 2 is about Iterator accessibility (readability / writability)

    typedef  selector<Iterator, 2>  Iterator_accessibility;
  
    template <typename E>
    struct case_< Iterator_accessibility, E,  1 > : where_< mlc::and_< stc_is(read),
								       stc_is(write) > >
    {
      typedef Read_Write_Iterator<E> ret;
    };
  
    template <typename E>
    struct case_< Iterator_accessibility, E,  2 > : where_< mlc::and_< stc_is(read),
								       stc_is_not(write) > >
    {
      typedef Read_Only_Iterator<E> ret;
    };
  
    template <typename E>
    struct case_< Iterator_accessibility, E,  3 > : where_< mlc::and_< stc_is(write),
								       stc_is_not(read) > >
    {
      typedef Write_Only_Iterator<E> ret;
    };

  
  } // end of internal


} // end of namespace abc






# include "identity.hh"






namespace abc
{

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
    typedef stc::final< stc::is<Iterator> > category;
    typedef stc::abstract value;
  stc_End;


  template <typename E>
  class iterator_base : public super
  {
  public:
    stc_using(value);
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
    typedef stc::abstract delegatee;
  stc_End;


  template <typename E>
  class lightweight_iterator : public super
  {
  public:
    stc_lookup(delegatee);
    delegatee& delegatee_;
  protected:
    lightweight_iterator(delegatee& d) : delegatee_(d) {}
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
    typedef stc::true_ forward;
    typedef stc::true_ write;
  stc_End;


  template <typename T>
  class array_iterator : public super
  {
  public:

    stc_using(value);

    void impl_init()               { i_ = 0; }
    void impl_next()               { i_ = i_ + 1; }
    bool impl_is_valid() const     { return i_ >= 0 and i_ < n_; }
    void impl_set_value(const value& v)  { this->v_ = v; }

    array_iterator(int n)          : n_(n) {}
    int& index()                   { return i_; }
    int  index() const             { return i_; }
  protected:
    int i_, n_;
  };

# include "../local/undefs.hh"





  // hello_iterator

# define current    hello_iterator<I>
  //               -------------------
# define super      lightweight_iterator< current >
# define templ      template <typename I>
# define classname  hello_iterator


  stc_Header;
    typedef I delegatee;
    typedef behavior::identity behavior;
  stc_End;


  template <typename I>
  class hello_iterator : public super
  {
  public:

    void impl_init()
    {
      std::cout << "hello!" << std::endl;
      super::impl_init();
    }

    hello_iterator(Iterator<I>& it)
      : super(it.exact())
    {
    }
  };


# include "../local/undefs.hh"




  // Behavior tag for reverse<T>.

  namespace behavior { struct reverse; }




# define current  reverse<T>
  //             ------------
# define templ      template <typename T>
# define classname  reverse
# define super      top< current >

  stc_Header;
  typedef T delegatee;
  typedef behavior::reverse behavior;
  stc_End;

  template <typename T>
  class reverse : public super
  {
  public:
    T& delegatee_;
    reverse(T& d) : delegatee_(d) {}  // ctor
  };


# include "../local/undefs.hh"

  

} // abc





// when reverse applies to an iterator:

# include "reverse_iterator.hh"




// test routines


template <typename I>
void init(abc::Iterator<I>& iter)
{
  iter.init();
}

template <typename I>
void echo(const abc::Forward_Iterator<I>& iter)
{
  std::cout << "Forward Iterator" << std::endl;
}

template <typename I>
void echo(const abc::Backward_Iterator<I>& iter)
{
  std::cout << "Backward Iterator" << std::endl;
}



int main()
{
  typedef abc::array_iterator<int> iterator_t;

  {
    // hello

    typedef abc::hello_iterator<iterator_t> iterator2_t;

    iterator_t i(7);
    i.index() = 51;

    iterator2_t j(i);
    init(j);
    std::cout << i.index() << std::endl;
  }

  {
    // reverse

    typedef abc::reverse<iterator_t> iterator2_t;

    iterator_t i(7);
    echo(i);
    iterator2_t j = abc::make_reverse(i);
    echo(j);
    
    i.init();
    std::cout << i.index() << std::endl;
    
    j.prev();
    std::cout << i.index() << std::endl;
  }

}
