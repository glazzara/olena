// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_INT_U_HH
# define MLN_VALUE_INT_U_HH

/*! \file mln/value/int_u.hh
 *
 * \brief Define a generic class for unsigned integers.
 */

# include <mln/core/concept/value.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/props.hh>


namespace mln
{

  namespace value
  {


    namespace internal
    {
      template <unsigned n> struct encoding_;
      template <> struct encoding_<8> { typedef unsigned char ret; };
    }


    /*! \brief Unsigned integer value class.
     *
     * The parameter is \c n the number of encoding bits.
     */
    template <unsigned n>
    struct int_u_
      : public internal::value_like_< typename internal::encoding_<n>::ret,
				      int_u_<n> >
    {
    protected:
      typedef internal::value_like_< typename internal::encoding_<n>::ret,
				     int_u_<n> > super;

    public:

      /// Encoding associated type.
      typedef typename super::enc enc;

      /// Constructor without argument.
      int_u_();

      /// Constructor from an integer.
      int_u_(int i);
    };


    

    template <unsigned n>
    struct props< int_u_<n> > : public props< typename internal::encoding_<n>::ret >
    {
    };


    // Fwd decl.
    template <typename T> struct vset_;


    /// Alias for unsigned 8bit integers.
    typedef int_u_<8> int_u8;

    /// Alias for the set of unsigned 8bit integers.
    typedef vset_<int_u8> int_u8_set;




    /*! \brief Print an int_u8 \p i into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] i An int_u8.
     *
     * \return The modified output stream \p ostr.
     */
    std::ostream& operator<<(std::ostream& ostr, const int_u8& i);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n>
    int_u_<n>::int_u_()
    {
    }

    template <unsigned n>
    int_u_<n>::int_u_(int i)
    {
      mln_precondition(i >= 0);
      mln_precondition(i <= mln_max(enc));
      this->v_ = enc(i);
    }

    std::ostream& operator<<(std::ostream& ostr, const int_u8& i)
    {
      return ostr << unsigned(i.to_equiv());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INT_U_HH
