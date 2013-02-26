// Copyright (C) 2006, 2007, 2008, 2009, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_INTERNAL_GRAY_F_HH
# define MLN_VALUE_INTERNAL_GRAY_F_HH

/// \file
///
/// Declarations of the mln::value::internal::gray_f class.

# include <iostream>

# include <mln/value/ops.hh>

# include <mln/core/contract.hh>
# include <mln/metal/math/pow.hh>
# include <mln/metal/bexpr.hh>
# include <mln/literal/ops.hh>

# include <mln/value/float01_f.hh>
# include <mln/trait/value_.hh>

/* Because of mutual dependencies between the implementations of
   mln::value::graylevel_f and mln::value::internal::gray_f, we have
   to ensure that only the interfaces of the required classes are
   included here.  Implementations are included later, at the bottom of
   this file.  */

# ifdef MLN_INCLUDE_ONLY
#  include <mln/value/graylevel_f.hh>
# else
#  define MLN_INCLUDE_ONLY
#  include <mln/value/graylevel_f.hh>
#  undef MLN_INCLUDE_ONLY
# endif


namespace mln
{

  namespace literal
  {
    /// Forward declarations.
    /// \{
    struct black_t;
    struct medium_gray_t;
    struct white_t;
    /// \}
  }
  namespace value
  {
    /// Forward declarations.
    /// \{
    namespace internal
    {
      template <unsigned n> class gray_;
      struct gray_f;
    }

    template <unsigned n> struct graylevel;
    struct graylevel_f;
    struct float01_f;
    /// \}
  }



  namespace trait
  {


    template < template <class, class> class Name>
    struct set_precise_binary_< Name, mln::value::internal::gray_f, mln::value::internal::gray_f >
    {
      typedef mln::value::internal::gray_f ret;
    };

    template <>
    struct set_precise_binary_< op::greater, mln::value::internal::gray_f, mln::value::internal::gray_f >
    {
      typedef bool ret;
    };

    template <>
    struct set_precise_binary_< op::eq, mln::value::internal::gray_f, mln::value::internal::gray_f >
    {
      typedef bool ret;
    };

    /* FIXME: Dead code?  */
    // Necessary??
//     template <typename F>
//     struct set_binary_< op::eq,
// 			mln::value::Floating,  mln::value::internal::gray_f,
// 			mln::value::Floating, F >
//     {
//       typedef bool ret;
//     };

    /// Forward declaration.
    template <typename T> struct value_;

    // 'gray_f' as a value.
    template <>
    struct value_<mln::value::internal::gray_f>
    {
    private:
      typedef mln::value::internal::gray_f self_;
      typedef float equiv_;

    public:

      enum {
	nbits = mln_nbits(equiv_),
	card  = 0
      };

      typedef trait::value::nature::floating nature;
      typedef trait::value::kind::gray       kind;
      typedef mln_trait_value_quant_(equiv_)   quant;

      static equiv_ min() { return 0; }
      static equiv_ max() { return 1; }
      static equiv_ epsilon() { return mln_epsilon(equiv_); }

      typedef float sum;
    };

  } // end of namespace mln::trait



  namespace value
  {

    namespace internal
    {

      /// General gray-level class on n bits.
      struct gray_f
	:
	public Floating< gray_f >,

	public internal::value_like_< float,     // Equivalent.
				      float,     // Encoding.
				      gray_f,    // Interoperation.
				      gray_f >   // Exact.
      {
	/// Constructor without argument.
	gray_f();

	/// \{ Constructors/assigments with gray_f.
	gray_f(const gray_f& rhs);
	gray_f& operator=(const gray_f& rhs);
	/// \}

	/// \{ Constructors/assigments with gray_<n>.
	template <unsigned n>
	gray_f(const gray_<n>& rhs);
	template <unsigned n>
	gray_f& operator=(const gray_<n>& rhs);
	/// \}

	/// \{ Constructors/assigments with float.
	gray_f(float val);
	gray_f& operator=(float val);
	/// \}

	/// \{ Constructors/assigments with graylevel_f.
	gray_f(const graylevel_f& val);
	gray_f& operator=(const graylevel_f& val);
	/// \}

	/// Conversion to graylevel<n>
	template <unsigned m>
	operator graylevel<m>() const;

	/// Conversion to gray_<n>
	template <unsigned m>
	operator gray_<m>() const;

	/// Conversion to graylevel_f
	operator graylevel_f() const;

	/// Access to std type.
	float value() const;
      };

      // Operators.

      /// Op<<.
      std::ostream& operator<<(std::ostream& ostr, const gray_f& g);

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


# ifndef MLN_INCLUDE_ONLY
#  include <mln/value/internal/gray_f.hxx>
#  include <mln/value/graylevel_f.hxx>
# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_VALUE_INTERNAL_GRAY_F_HH
