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

/* \file samples/mini-std/mini-std.cc
   \brief A proof of concept of Static using a mini-version of STL.  */

/*
 How to compile:
 g++ mini-std.cc -I../../../../olena/oln -I../../../../metalic
 */

#include <list>
#include <iostream>

#include <mlc/contract.hh>
#include <stc/scoop.hh>

#define mstd_typename_shortcut__(Type, Alias) typename Type::Alias



/*-------------.
| Equipement.  |
`-------------*/

// Add equipment
mlc_case_equipment_for_namespace(mstd);

namespace mstd
{
# include <stc/scoop.hxx>
} // End of namespace mstd

// FIXME
struct transform : public std::unary_function<int&, int>
{
  result_type operator() (argument_type i)
  {
    return i + 1;
  }
};


namespace mstd
{
  namespace internal
  {
    typedef char yes_;
    struct no_ { char dummy[2]; };

    template <typename T>
    T* makePtr();

  } // End of namespace internal
} // End of namespace mstd


// Virtual types declaration.
namespace mstd
{
  mlc_decl_typedef(is_isfrontinsertion);
  mlc_decl_typedef(is_isbackinsertion);

  mlc_decl_typedef(is_unique);
  mlc_decl_typedef(is_sorted);
  mlc_decl_typedef(is_mutable);


  mlc_decl_typedef(size_t);

  mlc_decl_typedef(iter_t);

  mlc_decl_typedef(value_t);
  mlc_decl_typedef(key_t);

  mlc_decl_typedef(container_t);
  mlc_decl_typedef(stdcontainer_t);

  mlc_decl_typedef(primitiveIterator_t);
} // End of namespace mstd.

/*----------------------------.
| Automatic implementations.  |
`----------------------------*/

namespace mstd
{
  namespace behavior { struct identity; }

  // Forward declarations.
  template <typename Exact>
  struct Container;

  template <typename Exact>
  struct ForwardContainer;

  template <typename Exact>
  struct BiDirContainer;

  template <typename Exact>
  struct Iterator;

  namespace automatic
  {
    template <typename Exact>
    struct set_impl< Container, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(value_t);

      value_t impl_size() const
      {
        return exact(this)->get_deleg().impl_size();
      }

      bool impl_empty() const
      {
        return exact(this)->get_deleg().impl_empty();
      }

      void impl_clear()
      {
        exact(this)->get_deleg().impl_clear();
      }
    };

    template <typename Exact>
    struct set_impl< ForwardContainer, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(iter_t);
    };

    template <typename Exact>
    struct set_impl< BiDirContainer, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(iter_t);
    };

    template <typename Exact>
    struct set_impl< Iterator, behavior::identity, Exact > : public virtual Any<Exact>
    {
      bool impl_op_nequal(const Exact& it)
      {
        return exact(this)->get_deleg().impl_op_nequal(it.get_deleg());
      }
    };
  }
};

/*---------------.
| Abstractions.  |
`---------------*/

namespace mstd
{
  // ---------- //
  // Iterator.  //
  // ---------- //

  template <typename Exact>
  struct Iterator : virtual public Any<Exact>,
                    virtual public automatic::set_impl< Iterator, behavior::identity, Exact >
  {
    typedef Iterator<Exact> current;

    stc_typename(container_t);
    stc_typename(iter_t);
    stc_typename(value_t);
    stc_typename(primitiveIterator_t);

    value_t operator*()
    {
      return exact(this)->impl_op_star();
    }

    bool operator ==(const Exact& it)
    {
      return exact(this)->impl_op_equal(it);
    }

    bool operator !=(const Exact& it)
    {
      return exact(this)->impl_op_nequal(it);
    }
  };

  // ----------- //
  // Container.  //
  // ----------- //

  template <typename Exact>
  struct Container : virtual public Any<Exact>,
                     virtual public automatic::set_impl< Container, behavior::identity, Exact >
  //automatic::get_impl<Container, Exact>
  {
    stc_typename(value_t);
    stc_typename(size_t);
    stc_typename(stdcontainer_t);

    size_t size() const
    {
      return exact(this)->impl_size();
    }

    bool empty() const
    {
      return exact(this)->impl_empty();
    }

    void clear()
    {
      exact(this)->impl_clear();
    }

    stdcontainer_t& get_stdcontainer()
    {
      return exact(this)->impl_get_stdcontainer();
    }
  };

  // ----------------- //
  // SortedContainer.  //
  // ----------------- //

  template <typename Exact>
  struct SortedContainer : public virtual Container<Exact>
  {
    typedef Container<Exact> super;
  };

  // ----------------- //
  // UniqueContainer.  //
  // ----------------- //

  template <typename Exact>
  struct UniqueContainer : public virtual Container<Exact>
  {
    typedef Container<Exact> super;
  };

  // ------------------ //
  // MutableContainer.  //
  // ------------------ //

  template <typename Exact>
  struct MutableContainer : public virtual Container<Exact>
  {
    typedef Container<Exact> super;
  };

  // ------------------ //
  // ForwardContainer.  //
  // ------------------ //

  template <typename Exact>
  struct ForwardContainer : public virtual Container<Exact>,
                            public virtual automatic::set_impl<ForwardContainer, behavior::identity, Exact>
  {
    typedef Container<Exact> super;
    stc_typename(iter_t);

    iter_t begin()
    {
      return exact(this)->impl_begin();
    }
  };

  // ---------------- //
  // BiDirContainer.  //
  // ---------------- //

  template <typename Exact>
  struct BiDirContainer : public virtual ForwardContainer<Exact>,
                          public virtual automatic::set_impl<BiDirContainer, behavior::identity, Exact>
  {
    typedef ForwardContainer<Exact> super;
    stc_using(iter_t);

    iter_t end()
    {
      return exact(this)->impl_end();
    }
  };

  // --------------------------- //
  // RandomAccessibleContainer.  //
  // --------------------------- //

  template <typename Exact>
  struct RandomAccessibleContainer : public virtual BiDirContainer<Exact>
  {
    typedef BiDirContainer<Exact> super;
    stc_using(value_t);

    value_t& operator[] (unsigned n)
    {
      return exact(this)->impl_op_get(n);
    }
  };

  // ---------------------- //
  // AssociativeContainer.  //
  // ---------------------- //

  template <typename Exact>
  struct AssociativeContainer : public virtual BiDirContainer<Exact>
  {
    typedef BiDirContainer<Exact> super;
    stc_typename(key_t);
    stc_using(value_t);

    value_t& operator[] (key_t& k)
    {
      //      return exact(this)->impl_op_get(k);
    }
  };

  // ------------------------- //
  // FrontInsertionContainer.  //
  // ------------------------- //

  template<typename Exact>
  struct FrontInsertionContainer;

  template <typename Exact>
  struct FrontInsertionContainer : public virtual Container<Exact>
  {
    typedef Container<Exact> super;
    stc_using(value_t);

    void push_front(value_t& v)
    {
      exact(this)->impl_push_front(v);
    }

    void pop_front()
    {
      exact(this)->impl_pop_front();
    }

    value_t& front()
    {
      return exact(this)->impl_front();
    }
  };

  // ------------------------- //
  // BackInsertionContainer.  //
  // ------------------------- //

  template <typename Exact>
  struct BackInsertionContainer : public virtual Container<Exact>
  {
    typedef Container<Exact> super;
    stc_using(value_t);

    void push_back(value_t& v)
    {
      exact(this)->impl_push_back(v);
    }

    void pop_back()
    {
      exact(this)->impl_pop_back();
    }

    value_t& back()
    {
      return exact(this)->impl_back();
    }
  };
} // End of namespace mstd.

/*---------.
| Bridge.  |
`---------*/
namespace mstd
{
  // -------- //
  // Switch.  //
  // -------- //

  // Forward
  template <typename T>
  struct forwardIterator;

  template <typename T>
  struct backwardIterator;

  template <typename T>
  struct biDirIterator;

  template <typename T>
  struct randomAccessibleIterator;

  template <typename T>
  struct associativeIterator;

  // Tag.
  struct switch_is_sorted;

  template <typename Exact>
  struct case_<switch_is_sorted, Exact, 1> :
    public mlc::where_ < mlc::eq_< stc_find_type(Exact, is_sorted), stc::true_ > >
  {
        typedef SortedContainer<Exact> ret;
  };

  template <typename Exact>
  struct default_case_<switch_is_sorted, Exact>
  {
    typedef Container<Exact> ret;
  };


  // Tag.
  struct switch_is_unique;

  template <typename Exact>
  struct case_<switch_is_unique, Exact, 1> :
    public mlc::where_ < mlc::eq_< stc_find_type(Exact, is_unique), stc::true_ > >
  {
        typedef UniqueContainer<Exact> ret;
  };

  template <typename Exact>
  struct default_case_<switch_is_unique, Exact>
  {
    typedef Container<Exact> ret;
  };


  // Tag.
  struct switch_is_mutable;

  template <typename Exact>
  struct default_case_<switch_is_mutable, Exact>
  {
    typedef Container<Exact> ret;
  };


  // Tag.
  struct switch_is_frontinsertion;

  template <typename Exact>
  struct case_<switch_is_frontinsertion, Exact, 1> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t), forwardIterator) >
  {
        typedef FrontInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_frontinsertion, Exact, 2> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t), backwardIterator) >
  {
    typedef FrontInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_frontinsertion, Exact, 3> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t), biDirIterator) >
  {
    typedef FrontInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_frontinsertion, Exact, 4> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t), randomAccessibleIterator) >
  {
    typedef FrontInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct default_case_<switch_is_frontinsertion, Exact>
  {
    typedef Container<Exact> ret;
  };

  // Tag.
  struct switch_is_backinsertion;

  template <typename Exact>
  struct case_<switch_is_backinsertion, Exact, 1> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t), backwardIterator) >
  {
    typedef BackInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_backinsertion, Exact, 2> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t), biDirIterator) >
  {
    typedef BackInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_backinsertion, Exact, 3> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t), randomAccessibleIterator) >
  {
    typedef BackInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct default_case_<switch_is_backinsertion, Exact>
  {
    typedef Container<Exact> ret;
  };

  // Tag.
  struct switch_iterator_kind;

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 1> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t)::primitiveIterator_t, forwardIterator) >
  {
    typedef ForwardContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 2> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t)::primitiveIterator_t, backwardIterator) >
  {
    typedef BiDirContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 3> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t)::primitiveIterator_t, biDirIterator) >
  {
    typedef BiDirContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 4> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t)::primitiveIterator_t, randomAccessibleIterator) >
  {
    typedef RandomAccessibleContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 5> :
    public mlc::where_ < mlc_is_a(stc_find_type(Exact, iter_t)::primitiveIterator_t, associativeIterator) >
  {
    typedef AssociativeContainer<Exact> ret;
  };

  template <typename Exact>
  struct default_case_<switch_iterator_kind, Exact>
  {
    typedef Container<Exact> ret;
  };

  // ---------------- //
  // Container base.  //
  // ---------------- //

  // Sub-types used to avoid "duplicate base type" problems.
  template <typename Exact>
  struct container_base_is_sorted : public virtual switch_<switch_is_sorted, Exact>::ret
  {};

  template <typename Exact>
  struct container_base_is_unique : public virtual switch_<switch_is_unique, Exact>::ret
  {};

  template <typename Exact>
  struct container_base_is_mutable: public virtual switch_<switch_is_mutable, Exact>::ret
  {};

  template <typename Exact>
  struct container_base_is_frontinsertion
    : public virtual switch_<switch_is_frontinsertion, Exact>::ret
  {};

  template <typename Exact>
  struct container_base_is_backinsertion
    : public virtual switch_<switch_is_backinsertion, Exact>::ret
  {};

  template <typename Exact>
  struct container_base_iterator_kind
    : public virtual switch_<switch_iterator_kind, Exact>::ret
  {};

  // Forward declaration.
  template <typename Exact> struct container_base;

  template<typename Exact>
  struct super_trait_< container_base<Exact> >
  {
    typedef top<Exact> ret;
  };

  template <typename Exact>
  struct vtypes< container_base<Exact> >
  {
    // Properties
    typedef mlc::false_ is_isfrontinsertion;
    typedef mlc::false_ is_isbackinsertion;

    typedef mlc::false_ is_unique;
    typedef mlc::false_ is_sorted;
    typedef mlc::false_ is_mutable;

    // Types
    typedef stc::abstract value_t;
    typedef stc::abstract key_t;

    typedef stc::abstract iter_t;

    typedef stc::abstract stdcontainer_t;

    typedef stc_deferred(stdcontainer_t) stdcontainer_t__;
    typedef stc::final< typename stdcontainer_t__::size_type > size_t;
  };

  template <typename Exact>
  struct container_base : public virtual container_base_is_sorted<Exact>,
                          public virtual container_base_is_unique<Exact>,
                          public virtual container_base_is_mutable<Exact>,
                          public virtual container_base_is_frontinsertion<Exact>,
                          public virtual container_base_is_backinsertion<Exact>,
                          public virtual container_base_iterator_kind<Exact>
  {
    typedef top<Exact> super;

    container_base()
    {
    }
  };

} // End of namespace mstd.

/*------------------.
| Implementations.  |
`------------------*/

namespace mstd
{
  // ----------------- //
  // ForwardIterator.  //
  // ----------------- //

  // Forward declarations.
  template <typename T>
  struct forwardIterator;

  template<typename T>
  struct super_trait_< forwardIterator<T> >
  {
    typedef top< forwardIterator<T> > ret;
  };

  template <typename T>
  struct vtypes< forwardIterator<T> >
  {
    typedef T container_t;

    typedef stc_type(container_t, value_t) value_t;
    typedef stc_type(container_t, stdcontainer_t)::iterator iter_t;
    typedef forwardIterator<T> primitiveIterator_t;
  };

  template <typename T>
  struct forwardIterator : public Iterator< forwardIterator<T> >
  {
    typedef forwardIterator<T> current;
    typedef Iterator< current> super;

    stc_using(container_t);
    stc_using(iter_t);
    stc_using(value_t);

    forwardIterator(container_t& c) :
      c_ (c),
      iter_ (c.get_stdcontainer().begin())
    {
    }

    value_t& impl_op_star()
    {
      return *iter_;
    }

    current& operator++()
    {
      iter_++;
      return *exact(this);
    }

    current operator++(int)
    {
      current ret = *exact(this);
      ++iter_;
      return ret;
    }

    void to_begin()
    {
      iter_ = c_.get_stdcontainer().begin();
    }

    void to_end()
    {
      iter_ = c_.get_stdcontainer().end();
    }

    bool impl_op_equal(const current& it)
    {
      return this->iter_ == it.iter_;
    }

    bool impl_op_nequal(const current& it)
    {
      return this->iter_ != it.iter_;
    }

  protected:
    container_t& c_;
    iter_t iter_;
  };

  // --------------- //
  // BiDirIterator.  //
  // --------------- //

  // Forward declarations.
  template <typename T>
  struct biDirIterator;

  template<typename T>
  struct super_trait_< biDirIterator<T> >
  {
    typedef Iterator<biDirIterator<T> > ret;
  };

  template <typename T>
  struct vtypes< biDirIterator<T> >
  {
    typedef T container_t;

    typedef stc_type(container_t, value_t) value_t;
    typedef stc_type(container_t, stdcontainer_t)::iterator iter_t;
    typedef biDirIterator<T> primitiveIterator_t;
  };

  template <typename T>
  struct biDirIterator : public Iterator< biDirIterator<T> >
  {
    typedef biDirIterator<T> current;
    typedef Iterator<current> super;

    stc_using(container_t);
    stc_using(iter_t);
    stc_using(value_t);

    biDirIterator(container_t& c) :
      c_ (c),
      iter_ (c.get_stdcontainer().begin())
    {
    }

    current& operator--()
    {
      iter_--;
      return *this;
    }

    current operator--(int)
    {
      current ret = *this;
      --iter_;
      return ret;
    }

    value_t& impl_op_star()
    {
      return *iter_;
    }

    current& operator++()
    {
      iter_++;
      return *exact(this);
    }

    current operator++(int)
    {
      current ret = *exact(this);
      ++iter_;
      return ret;
    }

    void to_begin()
    {
      iter_ = c_.get_stdcontainer().begin();
    }

    void to_end()
    {
      iter_ = c_.get_stdcontainer().end();
    }

    bool impl_op_equal(const current& it)
    {
      return this->iter_ == it.iter_;
    }

    bool impl_op_nequal(const current& it)
    {
      return this->iter_ != it.iter_;
    }

  protected:
    container_t& c_;
    iter_t iter_;
  };

  // -------------------------- //
  // RandomAccessibleIterator.  //
  // -------------------------- //

  // Forward declarations.
  template <typename T>
  struct randomAccessibleIterator;

  template<typename T>
  struct super_trait_< randomAccessibleIterator<T> >
  {
    typedef mlc::none ret;
  };

  template <typename T>
  struct vtypes< randomAccessibleIterator<T> >
  {
    typedef T container_t;

    typedef stc_type(container_t, value_t) value_t;
    typedef stc_type(container_t, stdcontainer_t)::iterator iter_t;
    typedef randomAccessibleIterator<T> primitiveIterator_t;
  };

  template <typename T>
  struct randomAccessibleIterator : public Iterator< randomAccessibleIterator<T> >
  {
    typedef randomAccessibleIterator<T> current;

    stc_deduce_typename(current, container_t);
    stc_deduce_typename(current, iter_t);
    stc_deduce_typename(current, value_t);

    randomAccessibleIterator(container_t& c) :
      iter_ (c.get_stdcontainer())
    {
    }

  protected:
    iter_t iter_;
  };




  // ------ //
  // List.  //
  // ------ //

  // Forward declarations.
  template <typename T>
  struct list;

  template<typename T>
  struct super_trait_< list<T> >
  {
    typedef container_base< list<T> > ret;
  };

  template <typename T>
  struct vtypes< list<T> >
  {
    typedef mlc::true_ is_isfrontinsertion;
    typedef mlc::true_ is_isbackinsertion;

    typedef mlc::true_ is_mutable;

    typedef T value_t;

    typedef biDirIterator< list< T > > iter_t;

    typedef std::list<T> stdcontainer_t;
  };

  template <typename T>
  struct list : public container_base< list<T> >
  {
    typedef list<T> current;
    typedef container_base<current> super;

    stc_deduce_typename(current, size_t);
    stc_deduce_typename(current, value_t);
    stc_deduce_typename(current, iter_t);
    stc_deduce_typename(current, stdcontainer_t);

    list() :
      list_ ()
    {
    }

    ~list()
    {
    }

    size_t impl_size() const
    {
      return list_.size();
    }

    bool impl_empty() const
    {
      return list_.empty();
    }

    void impl_clear()
    {
      list_.clear();
    }

    stdcontainer_t& get_stdcontainer()
    {
      return list_;
    }

    void impl_push_front(value_t& v)
    {
      list_.push_front(v);
    }

    void impl_pop_front()
    {
      list_.pop_front();
    }

    void impl_push_back(value_t& v)
    {
      list_.push_back(v);
    }

    void impl_pop_back()
    {
      list_.pop_back();
    }

    iter_t impl_begin()
    {
      return iter_t(*this);
    }

    iter_t impl_end()
    {
      iter_t it(*this);
      it.to_end();
      return it;
    }

    value_t& impl_front()
    {
      return list_.front();
    }

    value_t& impl_back()
    {
      return list_.back();
    }

  protected:
    stdcontainer_t list_;
  };

  // ---------- //
  // Morphers.  //
  // ---------- //

  // Iterators

  // iterator_morpher_
  template <typename Exact>
  struct iterator_morpher_;

  template <typename Exact>
  struct super_trait_< iterator_morpher_<Exact> >
  {
    typedef Iterator<Exact> ret;
  };

  template <typename Exact>
  struct vtypes<iterator_morpher_<Exact> >
  {
  };

  template <typename Exact>
  struct iterator_morpher_ : public Iterator<Exact>
  {
    stc_typename(delegatee);

  protected:
    iterator_morpher_()
    {
    }
  };


  template <typename Iterator, typename Functor>
  struct transform_morpher_iterator;

  template <typename Iterator, typename Functor>
  struct super_trait_<transform_morpher_iterator<Iterator, Functor> >
  {
    typedef iterator_morpher_<transform_morpher_iterator<Iterator, Functor> > ret;
  };

  template <typename Iterator, typename Functor>
  struct vtypes<transform_morpher_iterator<Iterator, Functor> >
  {
    typedef Iterator delegatee;
  };

  template <typename Iterator, typename Functor>
  struct transform_morpher_iterator : public iterator_morpher_<transform_morpher_iterator<Iterator, Functor> >
  {
    typedef transform_morpher_iterator<Iterator, Functor> current;
    typedef iterator_morpher_<current> super;
    stc_using(delegatee);
    stc_using(value_t);
    stc_using(primitiveIterator_t);

    transform_morpher_iterator(delegatee deleg, Functor trans) :
      deleg_ (deleg),
      trans_ (trans)
    {
    }

    value_t impl_op_star()
    {
      typedef std::unary_function<value_t&,value_t> transform_t;
      mlc::assert_< mlc_is_a(Functor, transform_t) >::check();
      value_t val = *deleg_;
      return trans_(val);
    }

    delegatee& get_deleg()
    {
      return deleg_;
    }

    delegatee get_deleg() const
    {
      return deleg_;
    }


    current& operator++()
    {
      deleg_++;
      return *exact(this);
    }

    current operator++(int)
    {
      current ret = *exact(this);
      ++deleg_;
      return ret;
    }
  protected:
    delegatee deleg_;
    Functor trans_;
  };


  // Containers

  // container_morpher_
  template <typename Exact>
  struct container_morpher_;

  template <typename Exact>
  struct super_trait_< container_morpher_<Exact> >
  {
    typedef container_base<Exact> ret;
  };

  template <typename Exact>
  struct vtypes<container_morpher_<Exact> >
  {
    typedef behavior::identity behavior;
  };

  template <typename Exact>
  struct container_morpher_ : public container_base<Exact>
  {
    stc_typename(delegatee);

  protected:
    container_morpher_()
    {
    }
  };


  // transform_morpher
  template <typename Container, typename Functor>
  struct transform_morpher;

  template <typename Container, typename Functor>
  struct super_trait_<transform_morpher<Container, Functor> >
  {
    typedef container_morpher_<Container> ret;
  };

  template <typename Container, typename Functor>
  struct vtypes<transform_morpher<Container, Functor> >
  {
    typedef Container delegatee;
    typedef transform_morpher_iterator<typename Container::iter_t, Functor> iter_t;
  };

  template <typename Container, typename Functor>
  struct transform_morpher : public container_morpher_<transform_morpher<Container, Functor> >
  {
    typedef transform_morpher<Container, Functor> current;
    typedef container_morpher_<current> super;

    stc_using(delegatee);
    stc_using(value_t);
    stc_using(iter_t);

    transform_morpher(delegatee& d, Functor trans) :
      container_morpher_<current>(),
      deleg_ (d),
      trans_ (trans)
    {
    }

    delegatee& get_deleg()
    {
      return deleg_;
    }

    delegatee& get_deleg() const
    {
      return deleg_;
    }

    iter_t impl_begin()
    {
      return iter_t(deleg_.begin(), trans_);
    }

    iter_t impl_end()
    {
      iter_t it(deleg_.end(), trans_);
      it.get_deleg().to_end();
      return it;
    }

  protected:
    delegatee& deleg_;
    Functor trans_;
  };

  // logger morpher
#define LOGGER(name) NAME##_logger_morpher
#define LOGGER_STREAM std::cerr
#define LOGGER_BEGIN(NAME)						\
  template <typename Container>						\
  struct NAME##_logger_morpher;						\
									\
  template <typename Container>						\
  struct super_trait_<NAME##_logger_morpher<Container> >		\
  {									\
    typedef container_morpher_<Container> ret;				\
  };									\
									\
  template <typename Container>						\
  struct vtypes<NAME##_logger_morpher<Container> >			\
  {									\
    typedef Container delegatee;					\
  };									\
									\
  template <typename Container>						\
  struct NAME##_logger_morpher :					\
        public container_morpher_<NAME##_logger_morpher<Container> >	\
  {									\
    typedef NAME##_logger_morpher<Container> current;			\
    typedef container_morpher_<current> super;				\
									\
    stc_using(delegatee);						\
    stc_using(iter_t);							\
									\
    NAME##_logger_morpher(delegatee& d) :				\
      container_morpher_<current>(),					\
      deleg_ (d)							\
    {									\
    }									\
									\
    delegatee& get_deleg()						\
    {									\
      return deleg_;							\
    }									\
									\
    delegatee& get_deleg() const					\
    {									\
      return deleg_;							\
    }

#define LOGMETHOD(MESSAGE, RETURNTYPE, NAME)				\
									\
    RETURNTYPE NAME()							\
    {									\
      LOGGER_STREAM << __FILE__ << ":" << __LINE__ << ": "		\
                << MESSAGE << std::endl;				\
      return deleg_.NAME();						\
    }

#define LOGMETHODVOID(MESSAGE, RETURNTYPE, NAME)			\
									\
    RETURNTYPE NAME()							\
    {									\
      LOGGER_STREAM << __FILE__ << ":" << __LINE__ << ": "		\
                << MESSAGE << std::endl;				\
      deleg_.NAME();							\
    }

#define LOGMETHODARG(MESSAGE, RETURNTYPE, NAME, ARGTYPE, ARGNAME)	\
									\
    RETURNTYPE NAME(ARGTYPE)						\
    {									\
      LOGGER_STREAM << __FILE__ << ":" << __LINE__ << ": "		\
                << MESSAGE << std::endl;				\
      return deleg_.NAME(ARGNAME);					\
    }


#define LOGGER_END()				\
						\
  protected:					\
    delegatee& deleg_;				\
  };

} // End of namespace mstd.



/*-------.
| Main.  |
`--------*/

#define TEST(m)                      \
        std::cout << (#m)            \
                  << " : "           \
                  << (m)             \
                  << std::endl
#define TESTV(m)                     \
        std::cout << (#m)            \
                  << std::endl;      \
        (m)


template <typename X>
class Dummy
{
};

template <typename Exact>
void test(mstd::ForwardContainer<Exact>& fc)
{
}

// Create logger class.
namespace mstd
{
  LOGGER_BEGIN(begin)
    LOGMETHOD("Begin has been triggered.", iter_t, begin)
    LOGMETHOD("End has been triggered.", iter_t, end)
  LOGGER_END()
};

int main()
{
  // List
  mstd::list<int> l;
  int i = 5;

  TEST(l.size());
  TEST(l.empty());
  TESTV(l.push_front(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.pop_front());
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.push_front(i));
  TESTV(l.push_front(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.clear());
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.push_back(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.pop_back());
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.push_back(i));
  TESTV(l.push_back(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.clear());
  TEST(l.size());
  TEST(l.empty());

  TESTV(l.push_back(i));
  TESTV(l.push_back(++i));
  TESTV(l.push_back(++i));

  // Iterators
  mstd::forwardIterator< mstd::list <int> > iter(l);
  TEST(*iter);
  ++iter;
  TEST(*iter);
  iter++;
  TEST(*iter);

  TEST(iter == iter);
  TEST(iter != iter);


  mstd::list<int>::iter_t iter2(l);
  TEST(*iter2);
  ++iter2;
  TEST(*iter2);
  iter2++;
  TEST(*iter2);
  --iter2;
  TEST(*iter2);
  iter2--;
  TEST(*iter2);

  for (mstd::list<int>::iter_t it = l.begin(); it != l.end(); ++it)
    TEST(*it);

  // Fonctions
  test(l);

  //   Dummy<int> a;
  //   test(a);

  // *** Morphers ***
  typedef mstd::transform_morpher<mstd::list<int>, transform > morpher_t;
  typedef mstd::begin_logger_morpher<mstd::list<int> > logger_t;

  typedef mstd::transform_morpher<logger_t, transform > translog_t;


  // Transform
  {
    morpher_t tm(l, transform());

    typedef mstd::transform_morpher_iterator<mstd::list<int>::iter_t, transform> itmorpher_t;
    mlc::assert_< mlc_is_a_(itmorpher_t, mstd::Iterator) >::check();

    typedef mstd::transform_morpher_iterator<mstd::list<int>::iter_t, transform> realiter_t;
    mlc::assert_< mlc_eq(morpher_t::iter_t, realiter_t) >::check();

    std::cout << "= Morpher" << std::endl;
    for (morpher_t::iter_t im = tm.begin(); im != tm.end(); ++im)
      TEST(*im);
    std::cout << "= Morpher fin" << std::endl;

    morpher_t::iter_t im = tm.begin();
    TEST(*im);

    {
      mstd::list<int>::iter_t it = l.begin();
      mstd::transform_morpher_iterator<mstd::list<int>::iter_t, transform> im(it, transform());
      TEST(*it);
      TEST(*im);
    }
    TEST(tm.size());
  }

  // Logger
  {
    logger_t lm(l);

    logger_t::iter_t im = lm.begin();
    for (; im != lm.end(); ++im)
      TEST(*im);
    TEST(*im);
  }

  // Transform + Log
  {
    logger_t lm(l);
    translog_t tlm(lm, transform());

    translog_t::iter_t im = tlm.begin();
    TEST(*im);
  }
}
