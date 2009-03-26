// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_CONCEPT_FUNCTION_HH
# define MLN_CORE_CONCEPT_FUNCTION_HH

/// \file mln/core/concept/function.hh
///
/// Definition of several concepts of functions.

# include <mln/core/concept/object.hh>


namespace mln
{

  // Fwd decls.
  template <typename E> struct Function;
  template <typename E> struct Function_v2v;
  template <typename E> struct Function_v2w2v;
  template <typename E> struct Function_v2w_w2v;
  template <typename E> struct Function_i2b;
  template <typename E> struct Function_i2v;
  template <typename E> struct Function_p2v;
  template <typename E> struct Function_v2b;
  template <typename E> struct Function_p2b;
  template <typename E> struct Function_p2p;
  template <typename E> struct Function_x2x;
  template <typename E> struct Function_l2b;
  template <typename E> struct Function_l2l;

  template <typename E> struct Function_vv2v;
  template <typename E> struct Function_vv2b;

  /// Function category flag type.
  template <>
  struct Function<void>
  {
    typedef Object<void> super;
  };


  /// Base class for implementation of function-objects.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function : public Object<E>
  {
    typedef Function<void> category;

  /*
  ** An operator() has to be provided.  Its signature depends
  ** on the particular function-object one considers.
  */

  protected:
    Function();
    Function(const Function&);
  };


  /*-----------------.
  | Value -> Value.  |
  `-----------------*/

  template <>
  struct Function_v2v<void> { typedef Function<void> super; };

  /// Base class for implementation of function-objects from
  /// value to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_v2v : public Function<E>
  {
    typedef Function_v2v<void> category;
    typedef void mutable_result; // Meaning: no mutable result by default.
  protected:
    Function_v2v();
    Function_v2v(const Function_v2v&);
  };


  /*----------------------------.
  | Value <-> Value (Bijective) |
  `----------------------------*/

  // FIXME : should it also be a v2v function?

  template <>
  struct Function_v2w2v<void> { typedef Function<void> super; };

  /*!
   * \brief Base class for implementation of function-objects from
   * value V to value W and vice versa.
   *
   * The parameter \a E is the exact type.
   */

  template <typename E>
  struct Function_v2w2v : public Function<E>
  {
    typedef Function_v2w2v<void> category;

    /*
      result operator() (value);
      value  f_1        (result);
    */

  protected:
    Function_v2w2v();
    Function_v2w2v(const Function_v2w2v&);
  };


  /*---------------------------.
  | Value <-> Value (Two ways) |
  `---------------------------*/

  // FIXME : should it also be a v2v function?

  template <>
  struct Function_v2w_w2v<void> { typedef Function<void> super; };

  /*!
   * \brief Base class for implementation of function-objects from
   * value V to value W and from W to V thanks to the previous
   * value V.
   *
   * The parameter \a E is the exact type.
   *
   * eg: f  :  x     -> norm(x)
   *     f_1: (x, n) -> x' := x / norm(x) * n
   */

  template <typename E>
  struct Function_v2w_w2v : public Function<E>
  {
    typedef Function_v2w_w2v<void> category;

    /*
      result operator() (value);
      value  f_1        (result, value);
    */

  protected:
    Function_v2w_w2v();
    Function_v2w_w2v(const Function_v2w_w2v&);
  };


  /*-----------------.
  | Index -> bool.  |
  `-----------------*/

  template <>
  struct Function_i2b<void> { typedef Function_v2b<void> super; };

  /// Base class for implementation of function-objects from
  /// index to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_i2b : public Function_v2b<E>
  {
    typedef Function_i2b<void> category;
    typedef bool result;
  protected:
    Function_i2b();
    Function_i2b(const Function_i2b&);
  };


  /*-----------------.
  | Index -> Value.  |
  `-----------------*/

  template <>
  struct Function_i2v<void> { typedef Function_v2v<void> super; };

  /// Base class for implementation of function-objects from
  /// index to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_i2v : public Function_v2v<E>
  {
    typedef Function_i2v<void> category;
  protected:
    Function_i2v();
    Function_i2v(const Function_i2v&);
  };


  /*-----------------.
  | Point -> Value.  |
  `-----------------*/

  template <>
  struct Function_p2v<void> { typedef Function_v2v<void> super; };

  /// Base class for implementation of function-objects from point to
  /// value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_p2v : public virtual Function_v2v<E>
  {
    typedef Function_p2v<void> category;
  protected:
    Function_p2v();
    Function_p2v(const Function_p2v&);
  };


  /*----------------.
  | Value -> bool.  |
  `----------------*/

  template <>
  struct Function_v2b<void> { typedef Function_v2v<void> super; };

  /// Base class for implementation of function-objects from value to
  /// bool.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_v2b : public virtual Function_v2v<E>
  {
    typedef Function_v2b<void> category;
    typedef bool result;
  protected:
    Function_v2b();
    Function_v2b(const Function_v2b&);
  };


  /*----------------.
  | Point -> bool.  |
  `----------------*/

  template <>
  struct Function_p2b<void> { typedef Function_p2v<void> super; }; // FIXME

  /// Base class for implementation of function-objects from point to
  /// bool.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_p2b : public Function_p2v<E>,
			public Function_v2b<E>
  {
    typedef Function_p2b<void> category;
    typedef bool result;
  protected:
    Function_p2b();
    Function_p2b(const Function_p2b&);
  };


  /*-----------------.
  | Point -> Point.  |
  `-----------------*/

  template <>
  struct Function_p2p<void> { typedef Function_p2v<void> super; }; // FIXME

  /// Base class for implementation of function-objects from point to
  /// point.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_p2p : public Function_p2v<E>
  {
    typedef Function_p2p<void> category;
  protected:
    Function_p2p();
    Function_p2p(const Function_p2p&);
  };


  /*-------------------.
  | Vector -> Vector.  |
  `-------------------*/

  template <>
  struct Function_x2x<void> { typedef Function_v2v<void> super; }; // FIXME

  /// Base class for implementation of function-objects from vector to
  /// Vector.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_x2x : public Function_v2v<E>
  {
    typedef Function_x2x<void> category;
  protected:
    Function_x2x();
    Function_x2x(const Function_x2x&);
  };


  /*--------------------.
  | Vector <-> Vector.  |
  `--------------------*/

  /// Base class for implementation of bijective function-objects from
  ///  vector to vector.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Bijection_x2x : public Function_x2x< E >
  {
    /*
      typedef invert;
      invert inv() const;
    */
  protected:
    Bijection_x2x();
  };


  /*-----------------.
  | Label -> Value.  |
  `-----------------*/

  template <>
  struct Function_l2b<void> { typedef Function_v2b<void> super; };

  /// Base class for implementation of function-objects from
  /// label to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_l2b : public Function_v2b<E>
  {
    typedef Function_l2b<void> category;
  protected:
    Function_l2b();
    Function_l2b(const Function_l2b&);
  };


  /*-----------------.
  | Label -> Label.  |
  `-----------------*/

  template <>
  struct Function_l2l<void> { typedef Function_v2v<void> super; };

  /// Base class for implementation of function-objects from
  /// label to value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_l2l : public Function_v2v<E>
  {
    typedef Function_l2l<void> category;
  protected:
    Function_l2l();
    Function_l2l(const Function_l2l&);
  };


  /*------------------------.
  | Value, Value -> Value.  |
  `------------------------*/

  template <>
  struct Function_vv2v<void> { typedef Function<void> super; };

  /// Base class for implementation of function-objects from
  /// a couple of values to a value.
  ///
  /// The parameter \a E is the exact type.
  ///
  template <typename E>
  struct Function_vv2v : public Function<E>
  {
    typedef Function_vv2v<void> category;
  protected:
    Function_vv2v();
    Function_vv2v(const Function_vv2v&);
  };

  /*------------------------.
  | Value, Value -> Boolean.|
  `------------------------*/

  template <>
  struct Function_vv2b<void> { typedef Function<void> super; };

  /// Base class for implementation of function-objects from
  /// a couple of values to a boolean.
  ///
  /// The parameter \a E is the exact type.
  ///
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
  Function_v2v<E>::Function_v2v()
  {
  }

  template <typename E>
  inline
  Function_v2v<E>::Function_v2v(const Function_v2v<E>& rhs)
    : Function<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function_v2w2v<E>::Function_v2w2v()
  {
  }

  template <typename E>
  inline
  Function_v2w2v<E>::Function_v2w2v(const Function_v2w2v<E>& rhs)
    : Function<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function_v2w_w2v<E>::Function_v2w_w2v()
  {
  }

  template <typename E>
  inline
  Function_v2w_w2v<E>::Function_v2w_w2v(const Function_v2w_w2v<E>& rhs)
    : Function<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function_i2b<E>::Function_i2b()
  {
  }

  template <typename E>
  inline
  Function_i2b<E>::Function_i2b(const Function_i2b<E>& rhs)
    : Function_v2v<E>(rhs),
      Function_v2b<E>(rhs)
  {
  }


  template <typename E>
  inline
  Function_i2v<E>::Function_i2v()
  {
  }

  template <typename E>
  inline
  Function_i2v<E>::Function_i2v(const Function_i2v<E>& rhs)
    : Function_v2v<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function_p2v<E>::Function_p2v()
  {
  }

  template <typename E>
  inline
  Function_p2v<E>::Function_p2v(const Function_p2v<E>& rhs)
    : Function_v2v<E>(rhs)
  {
  }

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

  template <typename E>
  inline
  Function_p2b<E>::Function_p2b()
  {
  }

  template <typename E>
  inline
  Function_p2b<E>::Function_p2b(const Function_p2b<E>& rhs)
    : Function_v2v<E>(rhs),
      Function_p2v<E>(rhs),
      Function_v2b<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function_p2p<E>::Function_p2p()
  {
  }

  template <typename E>
  inline
  Function_p2p<E>::Function_p2p(const Function_p2p<E>& rhs)
    : Function_v2v<E>(rhs),
      Function_p2v<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function_x2x<E>::Function_x2x()
  {
  }

  template <typename E>
  inline
  Function_x2x<E>::Function_x2x(const Function_x2x<E>& rhs)
    : Function_v2v<E>(rhs)
  {
  }

  template <typename E>
  inline
  Bijection_x2x<E>::Bijection_x2x()
  {
    typedef typename E::invert invert;
    invert (E::*m)() const = & E::inv;
    m = 0;
  }

  template <typename E>
  inline
  Function_l2b<E>::Function_l2b()
  {
  }

  template <typename E>
  inline
  Function_l2b<E>::Function_l2b(const Function_l2b<E>& rhs)
    : Function_v2v<E>(rhs),
      Function_v2b<E>(rhs)
  {
  }

  template <typename E>
  inline
  Function_l2l<E>::Function_l2l()
  {
  }

  template <typename E>
  inline
  Function_l2l<E>::Function_l2l(const Function_l2l<E>& rhs)
    : Function_v2v<E>(rhs)
  {
  }

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
