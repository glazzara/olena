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

#ifndef OLN_VALUE_GRAY_HH
# define OLN_VALUE_GRAY_HH

# include <iostream>
# include <oln/value/gray_level.hh>



namespace oln
{

  // Fwd decls.
  namespace value
  {
    template <unsigned N> class gray_level;
    class gray;
  }

  // Super type.

  template <>
  struct super_trait_< value::gray >
  {
    typedef oln::internal::graylevel_value_< value::gray > ret;
  };

  // Virtual types.

  template <>
  struct vtypes< value::gray >
  {
    typedef unsigned long encoding; // FIXME: std::pair?
    typedef mlc::uint_<0> nbits; // FIXME: means unknown?
  };


  namespace value
  {


    /// General gray-level class where n bits is not know at compile-time.
    /// This class is used for exchange between gray-level types purpose.

    class gray : public oln::internal::graylevel_value_< gray >
    {
    public:

      /// Ctor.
      gray();

      /// Ctor.
      template <unsigned N>
      gray(const gray_level<N>& val);

      /// Ctor.
      gray(unsigned nbits, unsigned long val);

      /// Access to std type.
      unsigned long value() const;

      unsigned nbits() const;

      void set_nbits(unsigned nbits);

      gray to_nbits(unsigned nbits) const;

      template <unsigned N>
      operator gray_level<N>() const;

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




# ifndef OLN_INCLUDE_ONLY

    template <unsigned N, unsigned M>
    gray operator+(const gray_level<N>& lhs, const gray_level<M>& rhs)
    {
      return gray(lhs) + gray(rhs);
    }

    template <unsigned N, unsigned M>
    gray operator-(const gray_level<N>& lhs, const gray_level<M>& rhs)
    {
      return gray(lhs) - gray(rhs);
    }

    template <unsigned N>
    gray operator*(int s, const gray_level<N>& rhs)
    {
      precondition(s >= 0);
      gray tmp(N, s * rhs.value());
      return tmp;
    }

    template <unsigned N>
    gray operator*(const gray_level<N>& lhs, int s)
    {
      precondition(s >= 0);
      gray tmp(N, lhs.value() * s);
      return tmp;
    }

    template <unsigned N>
    gray operator/(const gray_level<N>& lhs, int s)
    {
      precondition(s > 0);
      gray tmp(N, lhs.value() / s);
      return tmp;
    }



    // Gray.

    gray::gray()
      : nbits_(0)
    {
    }

    template <unsigned n>
    gray::gray(const gray_level<n>& g)
      : nbits_(n),
	val_(g.value())
    {
    }

    gray::gray(unsigned nbits, unsigned long val)
      : nbits_(nbits),
	val_(val)
    {
    }

    unsigned long gray::value() const
    {
      invariant(nbits_ != 0);
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

    } // end of oln::value::internal


    void gray::set_nbits(unsigned nbits)
    {
      precondition(nbits != 0);
      invariant(nbits_ != 0);
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
      precondition(nbits != 0);
      invariant(nbits_ != 0);
      gray tmp(*this);
      tmp.set_nbits(nbits);
      return tmp;
    }


    template <unsigned n>
    gray::operator gray_level<n>() const
    {
      precondition(nbits_ != 0);
      gray_level<n> tmp(internal::convert<n>(nbits_, val_));
      assert(tmp.value() < internal::two_pow_(n));
      return tmp;
    }

    // operators

    std::ostream& operator<<(std::ostream& ostr, const gray& g)
    {
      return ostr << g.value() << '/' << g.nbits() << "nbits";
    }

    bool operator==(const gray& lhs, const gray& rhs)
    {
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == lhs.nbits())
	return lhs.value() == rhs.value();
      if (lhs.nbits() > rhs.nbits())
	return lhs.value() == rhs.to_nbits(lhs.nbits()).value();
      else
	return lhs.to_nbits(rhs.nbits()).value() == rhs.value();
    }

    bool operator<(const gray& lhs, const gray& rhs)
    {
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == lhs.nbits())
	return lhs.value() < rhs.value();
      if (lhs.nbits() > rhs.nbits())
	return lhs.value() < rhs.to_nbits(lhs.nbits()).value();
      else
	return lhs.to_nbits(rhs.nbits()).value() < rhs.value();
    }

    gray operator+(const gray& lhs, const gray& rhs)
    {
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
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
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
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
      precondition(s >= 0);
      gray tmp(rhs.nbits(), rhs.value() * s);
      return tmp;
    }

    gray operator*(const gray& lhs, int s)
    {
      precondition(s >= 0);
      gray tmp(lhs.nbits(), lhs.value() * s);
      return tmp;
    }

    gray operator/(const gray& lhs, int s)
    {
      precondition(s > 0);
      gray tmp(lhs.nbits(), lhs.value() / s);
      return tmp;
    }

    template <unsigned N>
    gray_level<N>&
    gray_level<N>::operator=(const gray& g)
    {
      precondition(g.nbits() == N);
      assert(g.value() < internal::two_pow_(N));
      this->val = g.value(); // FIXME: Add static_cast.
    }

# endif // ! OLN_INCLUDE_ONLY

    /*

    template <unsigned N, unsigned M>
    bool operator==(const gray_level<N>& lhs, const gray_level<M>& rhs);

    template <unsigned N, unsigned M>
    gray operator+(const gray_level<N>& lhs, const gray_level<M>& rhs);

    template <unsigned N, unsigned M>
    gray operator-(const gray_level<N>& lhs, const gray_level<M>& rhs);

    template <unsigned N>
    gray operator*(int s, const gray_level<N>& rhs);

    template <unsigned N>
    gray operator*(const gray_level<N>& lhs, int s);

    template <unsigned N>
    gray operator/(const gray_level<N>& lhs, int s);

    */

  } // end of namespace oln::value

} // end of namespace oln


/*

namespace xtd
{

  using oln::value::gray;

  template <> struct set_trait_<op_plus,  gray, gray> { typedef gray ret; };
  template <> struct set_trait_<op_minus, gray, gray> { typedef gray ret; };
  template <> struct set_trait_<op_mult,  int,  gray> { typedef gray ret; };
  template <> struct set_trait_<op_mult,  gray, int > { typedef gray ret; };
  template <> struct set_trait_<op_div,   gray, int > { typedef gray ret; };

  using oln::value::gray_level;

  template <unsigned nbits, unsigned M>
  struct set_trait_< op_plus,  gray_level<nbits>, gray_level<M> > { typedef gray ret; };

  template <unsigned nbits, unsigned M>
  struct set_trait_< op_minus, gray_level<nbits>, gray_level<M> > { typedef gray ret; };

  template <unsigned nbits>
  struct set_trait_< op_mult, int, gray_level<nbits> > { typedef gray ret; };

  template <unsigned nbits>
  struct set_trait_< op_mult, gray_level<nbits>, int > { typedef gray ret; };

  template <unsigned nbits>
  struct set_trait_< op_div,  gray_level<nbits>, int > { typedef gray ret; };

} // end of namespace xtd

*/


#endif // ! OLN_VALUE_GRAY_HH
