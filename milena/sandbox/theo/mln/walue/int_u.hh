// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WALUE_INT_U_HH
# define MLN_WALUE_INT_U_HH

/// \file
///
/// Define a generic class for unsigned integers.

# include <mln/core/concept/proxy.hh>
# include <mln/walue/set_encoding.hh>



namespace mln
{

  // Forward declarations.
  namespace walue {
    template <unsigned n> struct int_u;
  }
  namespace literal {
    struct zero_t;
    struct one_t;
  }


  namespace walue
  {

    /// Type aliases.
    typedef int_u<8>  int_u8;
    typedef int_u<16> int_u16;


    namespace internal
    {
      template <unsigned n> struct encoding_type_;

      // FIXME: Revamp...

      template <> struct encoding_type_<2>  { typedef unsigned char  ret; };
      template <> struct encoding_type_<3>  { typedef unsigned char  ret; };
      template <> struct encoding_type_<4>  { typedef unsigned char  ret; };
      template <> struct encoding_type_<5>  { typedef unsigned char  ret; };
      template <> struct encoding_type_<6>  { typedef unsigned char  ret; };
      template <> struct encoding_type_<7>  { typedef unsigned char  ret; };
      template <> struct encoding_type_<8>  { typedef unsigned char  ret; };

      template <> struct encoding_type_<16> { typedef unsigned short ret; };

      template <> struct encoding_type_<31> { typedef unsigned int ret; };
      template <> struct encoding_type_<32> { typedef unsigned int ret; };

    } // internal


    /// Unsigned integer value class.
    ///
    /// The parameter is \c n the number of encoding bits.
    //
    template <unsigned n>
    class int_u
      : public Proxy< int_u<n> >,
	public mln::internal::proxy_impl< unsigned, int_u<n> >
    {
    public:

      /// Encoding associated type.
      typedef typename internal::encoding_type_<n>::ret encoding_t;

      /// Constructor without argument.
      int_u()
      {
      }

      /// Constructor from an integer.
      int_u(int i)
      {
	mln_precondition(i >= 0);
	// FIXME: mln_precondition(unsigned(i) <= mln_max(enc_));
	v_ = static_cast<encoding_t>(i);
	mln_postcondition(static_cast<int>(v_) == i);
      }

      /// Assignment from an integer.
      int_u<n>& operator=(int i)
      {
	mln_precondition(i >= 0);
	// FIXME: mln_precondition(unsigned(i) <= mln_max(enc_));
	v_ = static_cast<encoding_t>(i);
	mln_postcondition(static_cast<int>(v_) == i);
	return *this;
      }

      /// Subject as a the proxy.
      unsigned subj_()
      {
	return v_;
      }


      int_u(const mln::literal::one_t&) { v_ = 1; }
      int_u& operator=(const mln::literal::one_t&) { v_ = 1; return *this; }

      int_u(const mln::literal::zero_t&) { v_ = 0; }
      int_u& operator=(const mln::literal::zero_t&) { v_ = 0; return *this; }


      template <typename T>
      int_u(const internal::encoding<T>& e)
      {
	mlc_converts_to(T, encoding_t)::check();
	v_ = e.value();
	mln_postcondition(static_cast<T>(v_) == e.value());
      }

      encoding_t encoding() const
      {
	return v_;
      }

      int_u& set_encoding(encoding_t v)
      {
	v_ = v;
	return *this;
      }

    protected:

      encoding_t v_;
    };



# ifndef MLN_INCLUDE_ONLY

    // FIXME

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::walue

} // end of namespace mln


#endif // ! MLN_WALUE_INT_U_HH
