// Copyright (C) 2006, 2007, 2008, 2009, 2011 EPITA Research and
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

#ifndef MLN_VALUE_FLOAT01_HH
# define MLN_VALUE_FLOAT01_HH

/// \file
///
/// Define a generic class for float.

# include <iostream>
# include <utility>

# include <mln/core/concept/value.hh>
# include <mln/value/concept/floating.hh>
# include <mln/trait/value_.hh>
# include <mln/trait/all.hh> // FIXME!



namespace mln
{

  namespace value
  {

    // Fwd decl.
    template <unsigned n> struct float01_;
    class float01;


    /// Class for floating values restricted to the interval [0..1]
    /// and discretized with n bits.
    class float01 : public Floating<float01>
    {
    public:

      /// Encoding associated type.
      typedef std::pair<unsigned, unsigned long> enc;

      /// Equivalent associated type.
      typedef float equiv;

      /// Ctor.
      float01();

      /// Ctor.
      template <unsigned n>
      float01(const float01_<n>& val);

      /// Ctor.
      float01(unsigned nbits, float val);

      /// Access to std type.
      float value() const;

      /// Access to the position in the quantized interval.
      unsigned long value_ind() const;

      /// Access to the encoding size.
      unsigned nbits() const;


      /// Set the encoding size to nbits.
      float01&     set_nbits(unsigned nbits);

      /// Return an equivalent gray encoded on \p nbits bits.
      const float01 to_nbits(unsigned nbits) const;

      /// Conversion to float.
      operator float() const;

//       template <unsigned n>
//       operator float01_<n>() const;

    protected:
      /// Encoding size.
      unsigned nbits_;

      /// Value.
      unsigned long val_;
    };

    std::ostream& operator<<(std::ostream& ostr, const float01& g);

    bool operator==(const float01& lhs, const float01& rhs);
    bool operator<(const float01& lhs, const float01& rhs);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      inline
      unsigned long two_pow_(unsigned n)
      {
	if (n == 0)
	  return 1;
	else
	  return 2 * two_pow_(n - 1);
      }

      inline
      unsigned long two_pow_n_minus_1(unsigned n)
      {
	  return two_pow_(n) - 1;
      }

      template <unsigned n_dest>
      inline
      unsigned long convert(unsigned n_src, unsigned long val)
      {
	if (n_dest == n_src)
	  return val;
	else
	  if (n_dest > n_src)
	    return val * two_pow_n_minus_1(n_dest) / two_pow_n_minus_1(n_src);
	  else
	    return val / two_pow_(n_src - n_dest);
      }

    } // end of mln::value::internal


    // Float01.
    inline
    float01::float01()
      : nbits_(0) // FIXME: Cost at run-time...
    {
    }

    template <unsigned n>
    inline
    float01::float01(const float01_<n>& g)
      : nbits_(n),
	val_(g.to_enc())
    {
    }

    inline
    float01::float01(unsigned nbits, float val)
      : nbits_(nbits)
    {
      val_ = static_cast<unsigned long>(val * float(internal::two_pow_n_minus_1(nbits)));
    }

    inline
    float float01::value() const
    {
      mln_invariant(nbits_ != 0);
      return float(val_) / float(internal::two_pow_n_minus_1(nbits_));
    }

    inline
    unsigned long float01::value_ind() const
    {
      mln_invariant(nbits_ != 0);
      return val_;
    }

    inline
    unsigned float01::nbits() const
    {
      return nbits_;
    }

    inline
    float01&
    float01::set_nbits(unsigned nbits)
    {
      mln_precondition(nbits != 0);
      mln_invariant(nbits_ != 0);
      if (nbits == nbits_)
	return *this;
      if (nbits > nbits_)
	{
	  val_ *= internal::two_pow_n_minus_1(nbits);
	  val_ /= internal::two_pow_n_minus_1(nbits_);
	}
      else // nbits < nbits_
	{
	  val_ /= internal::two_pow_(nbits_ - nbits);
	}
      nbits_ = nbits;
      return *this;
    }

    inline
    const float01
    float01::to_nbits(unsigned nbits) const
    {
      mln_precondition(nbits != 0);
      mln_invariant(nbits_ != 0);
      float01 tmp(*this);
      tmp.set_nbits(nbits);
      return tmp;
    }

    inline
    float01::operator float() const
    {
      mln_precondition(nbits_ != 0);
      float tmp = float(val_) / float(internal::two_pow_n_minus_1(nbits_));
      return tmp;
    }

//     template <unsigned n>
//     float01::operator float01_<n>() const
//     {
//       mln_precondition(nbits_ != 0);
//       float01_<n> tmp;
//       tmp.set_ind(internal::convert<n>(nbits_, val_));
//       mln_assertion(tmp.value() < internal::two_pow_(n));
//       return tmp;
//     }


    // Operators.

    inline
    std::ostream& operator<<(std::ostream& ostr, const float01& g)
    {
      return ostr << g.value() << '/' << g.nbits() << "nbits";
    }

    inline
    bool operator==(const float01& lhs, const float01& rhs)
    {
      mln_precondition(lhs.nbits() != 0 && rhs.nbits() != 0);

      if (rhs.nbits() == lhs.nbits())
	return lhs.value_ind() == rhs.value_ind();

      if (lhs.nbits() < rhs.nbits())
	return lhs.value_ind() == rhs.to_nbits(lhs.nbits()).value_ind();
      else
      {
	return lhs.to_nbits(rhs.nbits()).value_ind() == rhs.value_ind();
      }
    }

    inline
    bool operator<(const float01& lhs, const float01& rhs)
    {
      mln_precondition(lhs.nbits() != 0 && rhs.nbits() != 0);
      if (rhs.nbits() == lhs.nbits())
	return lhs.value() < rhs.value();
      if (lhs.nbits() > rhs.nbits())
	return lhs.value() < rhs.to_nbits(lhs.nbits()).value();
      else
	return lhs.to_nbits(rhs.nbits()).value() < rhs.value();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

# include <mln/value/float01_.hh>

#endif // ! MLN_VALUE_FLOAT01_HH
