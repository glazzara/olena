// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_CONCEPT_FUNCTION_HH
# define MLN_CORE_CONCEPT_FUNCTION_HH

/// \file
///
/// \brief Definition of several concepts of functions.

# include <mln/core/concept/object.hh>


namespace mln
{

  // Forward declarations.
  template <typename E> struct Function;
  template <typename E> struct Function_n2v;
  template <typename E> struct Function_v2v;
  template <typename E> struct Function_v2b;
  template <typename E> struct Function_vv2v;
  template <typename E> struct Function_vv2b;


  /// Function category flag type.
  template <>
  struct Function<void>
  {
    typedef Object<void> super;
  };


  /// \brief Base class for implementation of function-objects.
  ///
  /// The parameter \a E is the exact type.
  ///
  /// \ingroup modfun
  //
  template <typename E>
  struct Function : public Object<E>
  {
    typedef Function<void> category;

    /// An operator() has to be provided.  Its signature depends
    /// on the particular function-object one considers.

  protected:
    Function();
    Function(const Function&);

    /* Workaround for Apple's llvm-gcc 4.2.1 (Mac OS X Lion 10.7.1)

       Apple's llvm-gcc has a bug causing memmove() errors if the copy
       constructor is not declared along with operator=().  */
    Function<E>& operator=(const Function<E>&);
  };


  /*---------------.
  | Nil -> Value.  |
  `---------------*/

  template <>
  struct Function_n2v<void> { typedef Function<void> super; };


  /// \brief Base class for implementation of function-objects from
  /// Nil to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  /// \ingroup modfun
  //
  template <typename E>
  struct Function_n2v : public Function<E>
  {
    typedef Function_n2v<void> category;
  protected:
    Function_n2v();
    Function_n2v(const Function_n2v&);
  };


  /*-----------------.
  | Value -> Value.  |
  `-----------------*/

  template <>
  struct Function_v2v<void> { typedef Function<void> super; };


  /// \brief Base class for implementation of function-objects from
  /// value to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  /// \ingroup modfun
  //
  template <typename E>
  struct Function_v2v : public Function<E>
  {
    typedef Function_v2v<void> category;
    typedef void mutable_result; // Meaning: no mutable result by default.
  protected:
    Function_v2v();
    Function_v2v(const Function_v2v&);
  };


  /*----------------.
  | Value -> bool.  |
  `----------------*/

  template <>
  struct Function_v2b<void> { typedef Function_v2v<void> super; };


  /// \brief Base class for implementation of function-objects from a
  /// value to a Boolean.
  ///
  /// The parameter \a E is the exact type.
  ///
  /// \ingroup modfun
  //
  template <typename E>
  struct Function_v2b : public virtual Function_v2v<E>
  {
    typedef Function_v2b<void> category;
    typedef bool result;
  protected:
    Function_v2b();
    Function_v2b(const Function_v2b&);
  };



  /*--------------------------.
  | (Value, Value) -> Value.  |
  `--------------------------*/

  template <>
  struct Function_vv2v<void> { typedef Function<void> super; };


  /// \brief Base class for implementation of function-objects from a
  /// couple of values to a value.
  ///
  /// The parameter \a E is the exact type.
  ///
  /// \ingroup modfun
  //
  template <typename E>
  struct Function_vv2v : public Function<E>
  {
    typedef Function_vv2v<void> category;
  protected:
    Function_vv2v();
    Function_vv2v(const Function_vv2v&);
  };


  /*--------------------------.
  | (Value, Value) -> Boolean.|
  `--------------------------*/

  template <>
  struct Function_vv2b<void> { typedef Function<void> super; };


  /// \brief Base class for implementation of function-objects from a
  /// couple of values to a Boolean.
  ///
  /// The parameter \a E is the exact type.
  ///
  /// \ingroup modfun
  //
  template <typename E>
  struct Function_vv2b : public Function<E>
  {
    typedef bool result;
    typedef Function_vv2b<void> category;
  protected:
    Function_vv2b();
    Function_vv2b(const Function_vv2b&);
  };



# ifndef MLN_INCLUDE_ONLY

  // Function.

  template <typename E>
  inline
  Function<E>::Function()
  {
    typedef mln_result(E) result;
  }

  template <typename E>
  inline
  Function<E>::Function(const Function<E>& rhs)
    : Object<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function<E>&
  Function<E>::operator=(const Function<E>&)
  {
    return *this;
  }

  // Function_n2v.

  template <typename E>
  inline
  Function_n2v<E>::Function_n2v()
  {
  }

  template <typename E>
  inline
  Function_n2v<E>::Function_n2v(const Function_n2v<E>& rhs)
    : Function<E>(rhs)
  {
  }


  // Function_v2v.

  template <typename E>
  inline
  Function_v2v<E>::Function_v2v()
  {
  }

  template <typename E>
  inline
  Function_v2v<E>::Function_v2v(const Function_v2v<E>& rhs)
    : Function<E>(rhs)
  {
  }

  // Function_v2b.

  template <typename E>
  inline
  Function_v2b<E>::Function_v2b()
  {
  }

  template <typename E>
  inline
  Function_v2b<E>::Function_v2b(const Function_v2b<E>& rhs)
    : Function_v2v<E>(rhs)
  {
  }

  // Function_vv2v.

  template <typename E>
  inline
  Function_vv2v<E>::Function_vv2v()
  {
  }

  template <typename E>
  inline
  Function_vv2v<E>::Function_vv2v(const Function_vv2v<E>& rhs)
    : Function<E>(rhs)
  {
  }

  // Function_vv2b.

  template <typename E>
  inline
  Function_vv2b<E>::Function_vv2b()
  {
  }

  template <typename E>
  inline
  Function_vv2b<E>::Function_vv2b(const Function_vv2b<E>& rhs)
    : Function<E>(rhs)
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_FUNCTION_HH
