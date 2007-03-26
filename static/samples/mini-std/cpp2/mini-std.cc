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

#include <mlc/case.hh>
#include <stc/any.hh>
#include <stc/scoop.hh>

#include <list>

#include <iostream>

/*-------------.
  | Equipement.  |
  `-------------*/

// Helper macros.
#define mstd_type_of_(FromType, Alias)					\
  find_vtype<FromType, mstd::typedef_:: Alias##_type>::ret

#define mstd_type_of(FromType, Alias)                                    \
  typename mstd_type_of_(FromType, Alias)


// Add equipment
stc_scoop_equipment_for_namespace(mstd);
mlc_case_equipment_for_namespace(mstd);


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
//   mlc_decl_typedef(is_forward);
//   mlc_decl_typedef(is_bidirectional);
//   mlc_decl_typedef(is_randomaccessible);

//   mlc_decl_typedef(is_isfrontinsertion);
//   mlc_decl_typedef(is_isbackinsertion);

  mlc_decl_typedef(is_unique);
  mlc_decl_typedef(is_sorted);
  mlc_decl_typedef(is_mutable);


  mlc_decl_typedef(size_type);

  mlc_decl_typedef(iter_type);

  mlc_decl_typedef(value_type);
  mlc_decl_typedef(key_type);

  mlc_decl_typedef(container_type);
  mlc_decl_typedef(stdcontainer_type);
} // End of namespace mstd.

/*---------------.
| Abstractions.  |
`---------------*/

namespace mstd
{
  // ---------- //
  // Iterator.  //
  // ---------- //

  template <typename E>
  struct Iterator : public stc::any<E>
  {
    typedef mstd_type_of(E, value) value_t;

    value_t& operator*()
    {
      this->exact().impl_op_star();
    }
  };

  // ----------- //
  // Container.  //
  // ----------- //

  template <typename E>
  struct Container : public stc::any<E>
  {
    typedef mstd_type_of(E, size) size_t;
    typedef mstd_type_of(E, stdcontainer) stdcontainer_t;

    size_t size() const
    {
      return this->exact().impl_size();
    }

    bool empty() const
    {
      return this->exact().impl_empty();
    }

    void clear()
    {
      this->exact().impl_clear();
    }

    stdcontainer_t& get_stdcontainer()
    {
      return this->exact().impl_get_stdcontainer();
    }
  };

  // ----------------- //
  // SortedContainer.  //
  // ----------------- //

  template <typename E>
  struct SortedContainer : public virtual Container<E>
  {
  };

  // ----------------- //
  // UniqueContainer.  //
  // ----------------- //

  template <typename E>
  struct UniqueContainer : public virtual Container<E>
  {
  };

  // ------------------ //
  // MutableContainer.  //
  // ------------------ //

  template <typename E>
  struct MutableContainer : public virtual Container<E>
  {
  };

  // ------------------ //
  // ForwardContainer.  //
  // ------------------ //

  template <typename E>
  struct ForwardContainer : public virtual Container<E>
  {
    typedef mstd_type_of(E, iter) iter_t;

    iter_t begin()
    {
      return this->exact().impl_begin();
    }
  };

  // ---------------- //
  // BiDirContainer.  //
  // ---------------- //

  template <typename E>
  struct BiDirContainer : public virtual ForwardContainer<E>
  {
    typedef mstd_type_of(E, iter) iter_t;

    iter_t end()
    {
      return this->exactl().impl_end();
    }
  };

  // --------------------------- //
  // RandomAccessibleContainer.  //
  // --------------------------- //

  template <typename E>
  struct RandomAccessibleContainer : public virtual BiDirContainer<E>
  {
    typedef mstd_type_of(E, value) value_t;

    value_t& operator[] (unsigned n)
    {
      return this->exact().impl_op_get(n);
    }
  };

  // ---------------------- //
  // AssociativeContainer.  //
  // ---------------------- //

  template <typename E>
  struct AssociativeContainer : public virtual BiDirContainer<E>
  {
    typedef mstd_type_of(E, value) value_t;
    typedef mstd_type_of(E, key) key_t;

    value_t& operator[] (key_t& k)
    {
      //      return this->exact().impl_op_get(k);
    }
  };

  // ------------------------- //
  // FrontInsertionContainer.  //
  // ------------------------- //

  template <typename E>
  struct FrontInsertionContainer : public virtual Container<E>
  {
    typedef mstd_type_of(E, value) value_t;

    void push_front(value_t& v)
    {
      this->exact().impl_push_front(v);
    }

    void pop_front()
    {
      this->exact().impl_pop_front();
    }

    value_t& front()
    {
      return this->exact().impl_front();
    }
  };

  // ------------------------- //
  // BackInsertionContainer.  //
  // ------------------------- //

  template <typename E>
  struct BackInsertionContainer : public virtual Container<E>
  {
    typedef mstd_type_of(E, value) value_t;

    void push_back(value_t& v)
    {
      this->exact().impl_push_back(v);
    }

    void pop_back()
    {
      this->exact().impl_pop_back();
    }

    value_t& back()
    {
      return this->exact().impl_back();
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

  template <typename E>
  struct default_case_<switch_is_sorted, E>
  {
    typedef Container<E> ret;
  };


  // Tag.
  struct switch_is_unique;

  template <typename E>
  struct default_case_<switch_is_unique, E>
  {
    typedef Container<E> ret;
  };


  // Tag.
  struct switch_is_mutable;

  template <typename E>
  struct default_case_<switch_is_mutable, E>
  {
    typedef Container<E> ret;
  };


  // Tag.
  struct switch_is_frontinsertion;

  template <typename E>
  struct case_<switch_is_frontinsertion, E, 1> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), forwardIterator) >
  {
        typedef FrontInsertionContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_is_frontinsertion, E, 2> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), backwardIterator) >
  {
    typedef FrontInsertionContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_is_frontinsertion, E, 3> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), biDirIterator) >
  {
    typedef FrontInsertionContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_is_frontinsertion, E, 4> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), randomAccessibleIterator) >
  {
    typedef FrontInsertionContainer<E> ret;
  };

  template <typename E>
  struct default_case_<switch_is_frontinsertion, E>
  {
    typedef Container<E> ret;
  };

  // Tag.
  struct switch_is_backinsertion;

  template <typename E>
  struct case_<switch_is_backinsertion, E, 1> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), backwardIterator) >
  {
    typedef BackInsertionContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_is_backinsertion, E, 2> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), biDirIterator) >
  {
    typedef BackInsertionContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_is_backinsertion, E, 3> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), randomAccessibleIterator) >
  {
    typedef BackInsertionContainer<E> ret;
  };

  template <typename E>
  struct default_case_<switch_is_backinsertion, E>
  {
    typedef Container<E> ret;
  };

  // Tag.
  struct switch_iterator_kind;

  template <typename E>
  struct case_<switch_iterator_kind, E, 1> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), forwardIterator) >
  {
    typedef ForwardContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_iterator_kind, E, 2> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), backwardIterator) >
  {
    typedef BiDirContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_iterator_kind, E, 3> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), biDirIterator) >
  {
    typedef BiDirContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_iterator_kind, E, 4> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), randomAccessibleIterator) >
  {
    typedef RandomAccessibleContainer<E> ret;
  };

  template <typename E>
  struct case_<switch_iterator_kind, E, 5> :
    public mlc::where_ < mstd_internal_select_class(mstd_type_of(E, iter), associativeIterator) >
  {
    typedef AssociativeContainer<E> ret;
  };

  template <typename E>
  struct default_case_<switch_iterator_kind, E>
  {
    typedef Container<E> ret;
  };

  // ---------------- //
  // Container base.  //
  // ---------------- //

  // Forward declaration.
  template <typename E> struct container_base;

  template<typename E>
  struct set_super_type< container_base<E> >
  {
    typedef mlc::none ret;
  };

  template <typename E>
  struct vtypes< container_base<E> >
  {
    // Properties
//     typedef mlc::false_ is_forward;
//     typedef mlc::false_ is_bidirectional;
//     typedef mlc::false_ is_randomaccessible;

//     typedef mlc::false_ is_isfrontinsertion;
//     typedef mlc::false_ is_isbackinsertion;

    typedef mlc::false_ is_unique;
    typedef mlc::false_ is_sorted;
    typedef mlc::false_ is_mutable;

    // Types
    typedef stc::abstract size_type;
    typedef stc::abstract value_type;
    typedef stc::abstract key_type;

    typedef stc::abstract iter_type;

    typedef stc::abstract stdcontainer_type;
  };

  template <typename E>
  struct container_base : public virtual switch_<switch_is_sorted, E>::ret,
                          public virtual switch_<switch_is_frontinsertion, E>::ret,
                          public virtual switch_<switch_is_backinsertion, E>::ret
  {
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
  struct set_super_type< forwardIterator<T> >
  {
    typedef mlc::none ret;
  };

  template <typename T>
  struct vtypes< forwardIterator<T> >
  {
    typedef T container_type;
    typedef typename container_type::value_t value_type;
    typedef typename container_type::stdcontainer_t::iterator iter_type;
  };

  template <typename T>
  struct forwardIterator : public Iterator< forwardIterator<T> >
  {
    typedef forwardIterator<T> self_t;
    typedef mstd_type_of(self_t, container) container_t;
    typedef mstd_type_of(self_t, iter) iter_t;
    typedef mstd_type_of(self_t, value) value_t;

    forwardIterator(container_t& c) :
      iter_ (c.get_stdcontainer().begin())
    {
    }

    value_t& impl_op_star()
    {
      return *iter_;
    }

    self_t& operator++()
    {
      iter_++;
      return *this;
    }

    self_t operator++(int)
    {
      self_t ret = *this;
      ++iter_;
      return ret;
    }

  protected:
    iter_t iter_;
  };


  // --------------- //
  // BiDirIterator.  //
  // --------------- //

  // Forward declarations.
  template <typename T>
  struct biDirIterator;

  template<typename T>
  struct set_super_type< biDirIterator<T> >
  {
    typedef mlc::none ret;
  };

  template <typename T>
  struct vtypes< biDirIterator<T> >
  {
    typedef T container_type;
    //typedef typename container_type::value_t value_t;
    //typedef typename container_type::stdcontainer_t::iterator iter_t;
  };

  template <typename T>
  struct biDirIterator : public Iterator< biDirIterator<T> >
  {
    typedef biDirIterator<T> self_t;
    typedef mstd_type_of(self_t, container) container_t;
    //    typedef mstd_type_of(self_t, iter) iter_t;
    //    typedef mstd_type_of(self_t, value) value_t;

     biDirIterator(container_t& c)
//       iter_ (c.get_stdcontainer().begin())
    {
    }

//     value_t& impl_op_star()
//     {
//       return *iter_;
//     }

  protected:
    // iter_t iter_;
  };

  // -------------------------- //
  // RandomAccessibleIterator.  //
  // -------------------------- //

  // Forward declarations.
  template <typename T>
  struct randomAccessibleIterator;

  template<typename T>
  struct set_super_type< randomAccessibleIterator<T> >
  {
    typedef mlc::none ret;
  };

  template <typename T>
  struct vtypes< randomAccessibleIterator<T> >
  {
    typedef T container_type;
    //    typedef typename container_t::value_t value_t;
    //     typedef typename container_t::stdcontainer_t::iterator iter_t;
  };

  template <typename T>
  struct randomAccessibleIterator : public Iterator< randomAccessibleIterator<T> >
  {
//     typedef randomAccessibleIterator<T> self_t;
//     typedef mstd_type_of(self_t, container) container_t;
//     typedef mstd_type_of(self_t, iter) iter_t;
//     typedef mstd_type_of(self_t, value) value_t;

//     randomAccessibleIterator(container_t& c) :
//       iter_ (c.get_stdcontainer())
//     {
//     }

//     value_t& impl_op_star()
//     {
//       return *iter_;
//     }

//   protected:
//     iter_t& iter_;
  };




  // ------ //
  // List.  //
  // ------ //

  // Forward declarations.
  template <typename T>
  struct list;

  template<typename T>
  struct set_super_type< list<T> >
  {
    typedef container_base< list<T> > ret;
  };

  template <typename T>
  struct vtypes< list<T> >
  {
    //    typedef mlc::true_ is_forward;

    //     typedef mlc::true_ is_isfrontinsertion;
    //     typedef mlc::true_ is_isbackinsertion;

    typedef mlc::true_ is_mutable;


    typedef unsigned size_type;
    typedef T value_type;

    typedef biDirIterator< list< T > > iter_type;

    typedef std::list<T> stdcontainer_type;
  };

  template <typename T>
  struct list : public container_base< list<T> >
  {
    typedef list<T> self_t;
    typedef mstd_type_of(self_t, size) size_t;
    typedef mstd_type_of(self_t, value) value_t;
    typedef mstd_type_of(self_t, iter) iter_t;
    typedef mstd_type_of(self_t, stdcontainer) stdcontainer_t;

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
      return iter_t(*this);
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
  ++iter;
  TEST(*iter);

  mstd::list<int>::iter_t iter2(l);

  mstd::list<int>::iter_t::container_t::value_t fff;
  fff = 5;
}


