// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_GRAY_HH
# define MLN_VALUE_GRAY_HH

# include <iostream>
# include <cmath>

# include <mln/value/graylevel.hh>
# include <mln/value/concept/integer.hh>


namespace mln
{

  /// \{ Fwd decls.
  namespace literal { struct black_t; struct white_t; }
  namespace value {    template <unsigned N> class graylevel; class gray; }
  /// \}


  namespace value
  {

    /// General gray-level class where n bits is not know at compile-time.
    /// This class is used for exchange between gray-level types purpose.

    class gray : public Integer< gray >
    {
    public:

      /// Encoding associated type.
      typedef unsigned long enc;

      /// Equivalent associated type.
      typedef unsigned long equiv;

      /// Constructor without argument.
      gray();

      /// \{ Constructors/assignments with literals.
      gray(const literal::white_t&);
      gray& operator=(const literal::white_t&);
      gray(const literal::black_t&);
      gray& operator=(const literal::black_t&);
      /// \}
      

      /// Ctor.
      template <unsigned N>
      gray(const graylevel<N>& val);

      /// Ctor.
      gray(unsigned nbits, unsigned long val);

      /// Access to std type.
      unsigned long value() const;

      unsigned nbits() const;

      void set_nbits(unsigned nbits);

      gray to_nbits(unsigned nbits) const;

      template <unsigned N>
      operator graylevel<N>() const;

    protected:
      unsigned nbits_;
      unsigned long val_;
    };

    std::ostream& operator<<(std::ostream& ostr, const gray& g);

    bool operator==(const gray& lhs, const gray& rhs);
    bool operator<(const gray& lhs, const gray& rhs);

    gray operator+(const gray& lhs, const gray& rhs);
    gray operator-(const gray& lhs, const gray& rhs);

    gray operator*(int s, const gray& rhs);
    gray operator*(const gray& lhs, int s);

    gray operator/(const gray& lhs, int s);




# ifndef MLN_INCLUDE_ONLY

    template <unsigned N, unsigned M>
    gray operator+(const graylevel<N>& lhs, const graylevel<M>& rhs)
    {
      return gray(lhs) + gray(rhs);
    }

    template <unsigned N, unsigned M>
    gray operator-(const graylevel<N>& lhs, const graylevel<M>& rhs)
    {
      return gray(lhs) - gray(rhs);
    }

    template <unsigned N>
    gray operator*(int s, const graylevel<N>& rhs)
    {
      mln_precondition(s >= 0);
      gray tmp(N, s * rhs.value());
      return tmp;
    }

    template <unsigned N>
    gray operator*(const graylevel<N>& lhs, int s)
    {
      mln_precondition(s >= 0);
      gray tmp(N, lhs.value() * s);
      return tmp;
    }

    template <unsigned N>
    gray operator/(const graylevel<N>& lhs, int s)
    {
      mln_precondition(s > 0);
      gray tmp(N, lhs.value() / s);
      return tmp;
    }

    // Gray.

    gray::gray()
      : nbits_(0)
    {
    }

    template <unsigned n>
    gray::gray(const graylevel<n>& g)
      : nbits_(n),
	val_(g.value())
    {
    }

    gray::gray(const literal::white_t&)
      : nbits_(1),
	val_(1)
    {
    }

    gray& gray::operator=(const literal::white_t&)
    {
      // nbits_ is not modified so that precision does not change.
      val_ = int(std::pow(2.f, float(nbits_))) - 1;
      return *this;
    }

    gray::gray(const literal::black_t&)
      : nbits_(1),
	val_(0)
    {
    }

    gray& gray::operator=(const literal::black_t&)
    {
      // nbits_ is not modified so that precision does not change.
      val_ = 0;
      return *this;
    }

    gray::gray(unsigned nbits, unsigned long val)
      : nbits_(nbits),
	val_(val)
    {
    }

    unsigned long gray::value() const
    {
      mln_invariant(nbits_ != 0);
      return val_;
    }

    unsigned gray::nbits() const
    {
      return nbits_;
    }

    namespace internal
    {

      unsigned long two_pow_(unsigned n)
      {
	if (n == 0)
	  return 1;
	else
	  return 2 * two_pow_(n - 1);
      }

      unsigned long two_pow_n_minus_1(unsigned n)
      {
	  return two_pow_(n) - 1;
      }

      template <unsigned n_dest>
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


    void gray::set_nbits(unsigned nbits)
    {
      mln_precondition(nbits != 0);
      mln_invariant(nbits_ != 0);
      if (nbits == nbits_)
	return;
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
    }


    gray gray::to_nbits(unsigned nbits) const
    {
      mln_precondition(nbits != 0);
      mln_invariant(nbits_ != 0);
      gray tmp(*this);
      tmp.set_nbits(nbits);
      return tmp;
    }


    template <unsigned n>
    gray::operator graylevel<n>() const
    {
      mln_precondition(nbits_ != 0);
      graylevel<n> tmp(internal::convert<n>(nbits_, val_));

      mln_assertion(tmp.value() < internal::two_pow_(n));
      return tmp;
    }

    // operators

    std::ostream& operator<<(std::ostream& ostr, const gray& g)
    {
      return ostr << g.value() << '/' << g.nbits() << "nbits";
    }

    bool operator==(const gray& lhs, const gray& rhs)
    {
      mln_precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == lhs.nbits())
	return lhs.value() == rhs.value();
      if (lhs.nbits() > rhs.nbits())
	return lhs.value() == rhs.to_nbits(lhs.nbits()).value();
      else
	return lhs.to_nbits(rhs.nbits()).value() == rhs.value();
    }

    bool operator<(const gray& lhs, const gray& rhs)
    {
      mln_precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == lhs.nbits())
	return lhs.value() < rhs.value();
      if (lhs.nbits() > rhs.nbits())
	return lhs.value() < rhs.to_nbits(lhs.nbits()).value();
      else
	return lhs.to_nbits(rhs.nbits()).value() < rhs.value();
    }

    gray operator+(const gray& lhs, const gray& rhs)
    {
      mln_precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (lhs.nbits() > rhs.nbits())
	{
	  gray tmp(lhs.nbits(),
		   lhs.value() + rhs.to_nbits(lhs.nbits()).value());
	  return tmp;
	}
      else
	{
	  gray tmp(rhs.nbits(),
		   lhs.to_nbits(rhs.nbits()).value() + rhs.value());
	  return tmp;
	}
    }

    gray operator-(const gray& lhs, const gray& rhs)
    {
      mln_precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (lhs.nbits() > rhs.nbits())
	{
	  unsigned long l = rhs.to_nbits(lhs.nbits()).value();
	  assert(lhs.value() >= l);
	  gray tmp(lhs.nbits(),
		   lhs.value() - l);
	  return tmp;
	}
      else
	{
	  unsigned long l = lhs.to_nbits(rhs.nbits()).value();
	  assert(l >= rhs.value());
	  gray tmp(rhs.nbits(),
		   l - rhs.value());
	  return tmp;
	}
    }

    gray operator*(int s, const gray& rhs)
    {
      mln_precondition(s >= 0);
      gray tmp(rhs.nbits(), rhs.value() * s);
      return tmp;
    }

    gray operator*(const gray& lhs, int s)
    {
      mln_precondition(s >= 0);
      gray tmp(lhs.nbits(), lhs.value() * s);
      return tmp;
    }

    gray operator/(const gray& lhs, int s)
    {
      mln_precondition(s > 0);
      gray tmp(lhs.nbits(), lhs.value() / s);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_GRAY_HH
