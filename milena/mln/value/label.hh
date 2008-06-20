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

#ifndef MLN_VALUE_LABEL_HH
# define MLN_VALUE_LABEL_HH

/*! \file mln/value/label.hh
 *
 * \brief Define a generic class for labels.
 */

# include <mln/metal/math/pow.hh>
# include <mln/value/concept/symbolic.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/internal/convert.hh>
# include <mln/value/internal/encoding.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  // Fwd decl.
  namespace value {
    template <unsigned n> struct label;
  }



  namespace trait
  {

    template <unsigned n>
    struct value_< mln::value::label<n> >
    {
    private:
      typedef mln::value::label<n> self_;
    public:

      enum {
	nbits = n,
	card  = mln_value_card_from_(n)
      };

      typedef trait::value::nature::symbolic nature;
      typedef trait::value::kind::label      kind;
      typedef mln_value_quant_from_(card)    quant;

      static const self_ min() { return 0; }
      static const self_ max() { return card - 1; }
    };

  } // end of namespace trait



  namespace value
  {


    /*! \brief Label value class.
     *
     * The parameter \c n is the number of encoding bits.
     */
    template <unsigned n>
    struct label : public Symbolic< label<n> >
    {
    public:

      /// Encoding associated type.
      typedef typename internal::encoding_unsigned_<n>::ret enc;

      /// Equivalent associated type.
      typedef typename internal::encoding_unsigned_<n>::ret equiv;

      /// Constructor without argument.
      label();

      /// Constructor from an (unsigned) integer.
      label(unsigned i);

      /// Assignment from an (unsigned) integer.
      label<n>& operator=(unsigned i);

      /// Self increment.
      label<n>& operator++();

      /// Self decrement.
      label<n>& operator--();

      /// Conversion to unsigned.
      const enc& to_enc() const;

      /// Unit value.
      static const label<n> one;

    protected:

      enc v_;
    };


    namespace internal
    {

      /// \internal
      template <unsigned n>
      struct convert_< label<n> >
      {
	static label<n> value_at_index(unsigned i)
	{
	  return i;
	}
	static unsigned index_of_value(const label<n>& v)
	{
	  return v.to_enc();
	}
      };

    } // end of mln::value::internal



    // Safety.
    template <> struct label<0>;
    template <> struct label<1>;


    /// Equality comparison.
    template <unsigned n>
    bool operator==(const label<n>& lhs, const label<n>& rhs);

    /// Ordering comparison.
    template <unsigned n>
    bool operator<(const label<n>& lhs, const label<n>& rhs);



    /*! \brief Print a label \p l into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] l A label.
     *
     * \return The modified output stream \p ostr.
     */
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const label<n>& l);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned n>
    inline
    label<n>::label()
    {
    }

    template <unsigned n>
    inline
    label<n>::label(unsigned i)
    {
      mln_precondition(i <= mln_max(enc));
      v_ = enc(i);
    }

    template <unsigned n>
    inline
    label<n>&
    label<n>::operator=(unsigned i)
    {
      mln_precondition(i <= mln_max(enc));
      v_ = enc(i);
      return *this;
    }

    template <unsigned n>
    inline
    label<n>&
    label<n>::operator++()
    {
      mln_precondition(v_ < mln_max(enc));
      ++v_;
      return *this;
    }

    template <unsigned n>
    inline
    label<n>&
    label<n>::operator--()
    {
      mln_precondition(v_ != 0);
      --v_;
      return *this;
    }

    template <unsigned n>
    const label<n> label<n>::one = 1;

    template <unsigned n>
    inline
    const mln_enc(label<n>)&
    label<n>::to_enc() const
    {
      return v_;
    }

    template <unsigned n>
    inline
    bool operator==(const label<n>& lhs, const label<n>& rhs)
    {
      return lhs.to_enc() == rhs.to_enc();
    }

    template <unsigned n>
    inline
    bool operator<(const label<n>& lhs, const label<n>& rhs)
    {
      return lhs.to_enc() < rhs.to_enc();
    }

    template <unsigned n>
    inline
    std::ostream& operator<<(std::ostream& ostr, const label<n>& i)
    {
      return ostr << i.to_enc();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_LABEL_HH
