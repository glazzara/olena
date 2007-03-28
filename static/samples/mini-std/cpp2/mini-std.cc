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


#define mstd_internal_select_class(T, CLASS) \
mlc::bexpr_ < sizeof(internal::CLASS##_select(internal::makePtr< T >())) == sizeof(internal::yes_) >

#define mstd_internal_select_mechanism(CLASS) \
template <class E>                                                            \
yes_ CLASS##_select(const CLASS<E>* arg);                                     \
template <class T>                                                            \
no_ CLASS##_select(const T* arg);                                             \
no_ CLASS##_select(...)

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
} // End of namespace mstd.

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
                    virtual public automatic::get_impl<Iterator, Exact>
  {
    stc_typename(value_t);

    value_t& operator*()
    {
      exact(this)->impl_op_star();
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
  struct Container : virtual Any<Exact>
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
  struct ForwardContainer : public virtual Container<Exact>
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
  struct BiDirContainer : public virtual ForwardContainer<Exact>
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

  // Bridge equipment
  namespace internal
  {
    mstd_internal_select_mechanism(forwardIterator);
    mstd_internal_select_mechanism(backwardIterator);
    mstd_internal_select_mechanism(biDirIterator);
    mstd_internal_select_mechanism(randomAccessibleIterator);
    mstd_internal_select_mechanism(associativeIterator);
  } // End of namespace interal

  // Tag.
  struct switch_is_sorted;

  template <typename Exact>
  struct default_case_<switch_is_sorted, Exact>
  {
    typedef Container<Exact> ret;
  };


  // Tag.
  struct switch_is_unique;

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
    public mlc:: where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), forwardIterator) >
  {
        typedef FrontInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_frontinsertion, Exact, 2> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), backwardIterator) >
  {
    typedef FrontInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_frontinsertion, Exact, 3> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), biDirIterator) >
  {
    typedef FrontInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_frontinsertion, Exact, 4> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), randomAccessibleIterator) >
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
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), backwardIterator) >
  {
    typedef BackInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_backinsertion, Exact, 2> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), biDirIterator) >
  {
    typedef BackInsertionContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_is_backinsertion, Exact, 3> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), randomAccessibleIterator) >
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
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), forwardIterator) >
  {
    typedef ForwardContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 2> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), backwardIterator) >
  {
    typedef BiDirContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 3> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), biDirIterator) >
  {
    typedef BiDirContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 4> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), randomAccessibleIterator) >
  {
    typedef RandomAccessibleContainer<Exact> ret;
  };

  template <typename Exact>
  struct case_<switch_iterator_kind, Exact, 5> :
    public mlc::where_ < mstd_internal_select_class(stc_find_type(Exact, iter_t), associativeIterator) >
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
  struct container_base : public virtual switch_<switch_is_sorted, Exact>::ret,
                          public virtual switch_<switch_is_frontinsertion, Exact>::ret,
                          public virtual switch_<switch_is_backinsertion, Exact>::ret,
                          public virtual switch_<switch_iterator_kind, Exact>::ret
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
  };

  template <typename T>
  struct forwardIterator : public Iterator< forwardIterator<T> >
  {
    typedef forwardIterator<T> current;
    typedef Iterator< current> super;


    stc_deduce_typename(current, container_t);
    stc_deduce_typename(current, iter_t);
    stc_deduce_typename(current, value_t);

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
      return *this;
    }

    current operator++(int)
    {
      current ret = *this;
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
    typedef top< biDirIterator<T> > ret;
  };

  template <typename T>
  struct vtypes< biDirIterator<T> >
  {
    typedef T container_t;

    typedef stc_type(container_t, value_t) value_t;
    typedef stc_type(container_t, stdcontainer_t)::iterator iter_t;
  };

  template <typename T>
  struct biDirIterator : public Iterator< biDirIterator<T> >
  {
    typedef biDirIterator<T> current;
    stc_deduce_typename(current, container_t);
    stc_deduce_typename(current, iter_t);
    stc_deduce_typename(current, value_t);

    biDirIterator(container_t& c) :
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
      return *this;
    }

    current operator++(int)
    {
      current ret = *this;
      ++iter_;
      return ret;
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

    void to_begin()
    {
      iter_ = c_.get_stdcontainer().begin();
    }

    void to_end()
    {
      iter_ = c_.get_stdcontainer().end();
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

    value_t& impl_op_star()
    {
      return *iter_;
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

  private:
    stdcontainer_t list_;
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

int main()
{
  mstd::list<int> l;
  int i = 5;

  TEST(l.size());
  TEST(l.empty());
  TESTV(l.impl_push_front(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.impl_pop_front());
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.impl_push_front(i));
  TESTV(l.impl_push_front(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.clear());
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.impl_push_back(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.impl_pop_back());
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.impl_push_back(i));
  TESTV(l.impl_push_back(i));
  TEST(l.size());
  TEST(l.empty());
  TESTV(l.clear());
  TEST(l.size());
  TEST(l.empty());

  TESTV(l.impl_push_back(i));
  TESTV(l.impl_push_back(++i));
  TESTV(l.impl_push_back(++i));
  mstd::forwardIterator< mstd::list <int> > iter(l);
  TEST(*iter);
  ++iter;
  TEST(*iter);
  iter++;
  TEST(*iter);

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

  TEST(iter == iter);
  TEST(iter != iter);

  mstd::list<int>::iter_t it = l.begin();
}
