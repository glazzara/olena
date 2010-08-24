// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_SIGN_HH
# define MLN_VALUE_SIGN_HH

/*! \file
 *
 * \brief Definition of the mln::value::sign class.
 */
# include <mln/value/internal/integer.hh>
# include <mln/trait/value_.hh>
# include <mln/literal/zero.hh>
# include <mln/literal/one.hh>
# include <mln/debug/format.hh>

namespace mln
{

  namespace value
  {
    /*!
    ** \brief The sign class represents the value type
    ** composed by the set (-1, 0, 1)
    ** sign value type is a subset of the int value type.
    */
    class sign : public internal::Integer<sign>
    {
    public:
      /// FIXME Are these typedefs correct?

      /// Define the encoding type
      typedef int enc;

      /// Define the equivalent type
      typedef int equiv;


      /// Constructor without argument.
      sign();

      /// Constructor from an integer.
      sign(int i);

      /// \{ Constructors/assignments with literals.
      sign(const mln::literal::zero_t&);
      sign& operator=(const mln::literal::zero_t&);
      sign(const mln::literal::one_t&);
      sign& operator=(const mln::literal::one_t&);
      /// \}

      /// Conversion to an integer.
      operator int() const;

      /// Return the value associated to the sign object
      int val_() const;

      /// Assignment from an integer.
      sign& operator=(int i);


      /// Zero value.
      static const sign zero;

      /// Unit value.
      static const sign one;

    protected:

      /// The value
      int v_;
    };

    /*! \brief Print an signed integer \p i into the output stream \p ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] i An sign value
     *
     * \return The modified output stream \p ostr.
     */
    std::ostream& operator<<(std::ostream& ostr, const sign& i);

    /// Comparaison operator
    bool operator==(const sign& lhs, const sign& rhs);
    bool operator<(const sign& lhs, const sign& rhs);

# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

    const sign sign::zero = 0;
    const sign sign::one = 1;

#  endif // !MLN_WO_GLOBAL_VARS

    inline
    sign::sign()
    {
    }

    inline
    sign::operator int() const
    {
      return this->v_;
    }

    inline
    int
    sign::val_() const
    {
      return this->v_;
    }

    inline
    sign::sign(int i)
    {
      mln_precondition(i >= -1);
      mln_precondition(i <= 1);
      this->v_ = i;
    }

    inline
    sign&
    sign::operator=(int i)
    {
      mln_precondition(i >= -1);
      mln_precondition(i <= 1);
      this->v_ = i;
      return *this;
    }

    inline
    sign::sign(const mln::literal::zero_t&)
    {
      this->v_ = 0;
    }

    inline
    sign&
    sign::operator=(const mln::literal::zero_t&)
    {
      this->v_ = 0;
      return *this;
    }

    inline
    sign::sign(const mln::literal::one_t&)
    {
      this->v_ = 1;
    }

    inline
    sign&
    sign::operator=(const mln::literal::one_t&)
    {
      this->v_ = 1;
      return *this;
    }


    inline
    std::ostream& operator<<(std::ostream& ostr, const sign& i)
    {
      return ostr << debug::format(i.val_());
    }

    inline
    bool operator==(const sign& lhs, const sign& rhs)
    {
      return lhs.val_() == rhs.val_();
    }

    inline
    bool operator<(const sign& lhs, const sign& rhs)
    {
      return lhs.val_() == rhs.val_();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace value

  namespace trait
  {

    template <>
    struct value_<mln::value::sign>
    {

      enum {
	dim = 1,
	nbits = 2,
	card  = 3
      };

      typedef trait::value::nature::integer nature;
      typedef trait::value::kind::gray      kind;
      typedef trait::value::quant::low   quant;

      static mln::value::sign min() { return -1; }
      static mln::value::sign max() { return 1; }
      static mln::value::sign epsilon() { return 0; }

      typedef int comp;

      typedef int sum;
    };

  } // end of namespace trait

} // end of namespace mln


#endif // ! MLN_VALUE_SIGN_HH
