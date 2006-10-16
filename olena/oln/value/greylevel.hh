// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_VALUE_GREYLEVEL_HH
# define OLN_CORE_VALUE_GREYLEVEL_HH

# include <iostream>
# include <mlc/contract.hh>
# include <mlc/bexpr.hh>
# include <xtd/optraits.hh>
# include <oln/core/abstract/value.hh>



namespace oln
{

  namespace value
  {


    namespace internal
    {

      template <unsigned nbits>
      struct encoding;

      template <> struct encoding <1>  { typedef bool           ret; };
      template <> struct encoding <8>  { typedef unsigned char  ret; };
      template <> struct encoding <16> { typedef unsigned short ret; };
      template <> struct encoding <32> { typedef unsigned long  ret; };

    } // end of oln::value::internal


    /// Fwd decl.
    class greylevel;


    /// General grey-level class on n bits.
    template <unsigned nbits>
    class greylevel_ : public oln::abstract::value
    {
      typedef typename internal::encoding<nbits>::ret encoding_t;

    public:
      
      /// Ctor.
      greylevel_();
      
      /// Ctor.
      explicit greylevel_(const typename internal::encoding<nbits>::ret& val);

      /// Access to std type.
      typename internal::encoding<nbits>::ret value() const;

      /// Op encoding_t.
      operator typename internal::encoding<nbits>::ret() const;

      /// Op greylevel.
      operator greylevel() const;

      /// Op<.
      bool operator<(const greylevel_<nbits>& rhs) const;

      /// Op==.
      bool operator==(const greylevel_<nbits>& rhs) const;

    protected:
      encoding_t val_;
    };


    /// Op<<.
    template <unsigned nbits>
    std::ostream& operator<<(std::ostream& ostr, const greylevel_<nbits>& g);


    /// Aliases.
    typedef greylevel_<8>  gl8;
    typedef greylevel_<16> gl16;
    typedef greylevel_<32> gl32;


    template <unsigned nbits, unsigned mbits>
    bool operator==(const greylevel_<nbits>& lhs, const greylevel_<mbits>& rhs);

    template <unsigned nbits, unsigned mbits>
    greylevel operator+(const greylevel_<nbits>& lhs, const greylevel_<mbits>& rhs);

    template <unsigned nbits, unsigned mbits>
    greylevel operator-(const greylevel_<nbits>& lhs, const greylevel_<mbits>& rhs);

    template <unsigned nbits>
    greylevel operator*(int s, const greylevel_<nbits>& rhs);

    template <unsigned nbits>
    greylevel operator*(const greylevel_<nbits>& lhs, int s);

    template <unsigned nbits>
    greylevel operator/(const greylevel_<nbits>& lhs, int s);



    /// General grey-level class where n bits is not know at compile-time.
    /// This class is used for exchange between grey-level types purpose.
    class greylevel : public oln::abstract::value
    {
    public:

      /// Ctor.
      greylevel();

      /// Ctor.
      template <unsigned n>
      greylevel(const greylevel_<n>& val);

      /// Ctor.
      greylevel(unsigned nbits, unsigned long val);

      /// Access to std type.
      unsigned long value() const;

      unsigned nbits() const;

      void set_nbits(unsigned nbits);

      greylevel to_nbits(unsigned nbits) const;

      template <unsigned n>
      operator greylevel_<n>() const;

      bool operator<(const greylevel& rhs) const;

      bool operator==(const greylevel& rhs) const;

    protected:
      unsigned nbits_;
      unsigned long val_;
    };


    std::ostream& operator<<(std::ostream& ostr, const greylevel& g);


    greylevel operator+(const greylevel& lhs, const greylevel& rhs);
    greylevel operator-(const greylevel& lhs, const greylevel& rhs);

    greylevel operator*(int s, const greylevel& rhs);
    greylevel operator*(const greylevel& lhs, int s);

    greylevel operator/(const greylevel& lhs, int s);



# ifndef OLN_INCLUDE_ONLY

    // Greylevel_<nbits>.

    template <unsigned nbits>
    greylevel_<nbits>::greylevel_()
    {
    }

    template <unsigned nbits>
    greylevel_<nbits>::greylevel_(const typename internal::encoding<nbits>::ret& val)
      : val_(val)
    {
    }

    template <unsigned nbits>
    typename internal::encoding<nbits>::ret
    greylevel_<nbits>::value() const
    {
      return val_;
    }

    template <unsigned nbits>
    greylevel_<nbits>::operator greylevel() const
    {
      greylevel tmp(nbits, val_);
      return tmp;
    }

    template <unsigned nbits>
    greylevel_<nbits>::operator typename internal::encoding<nbits>::ret() const
    {
      return val_;
    }

    template <unsigned nbits>
    bool greylevel_<nbits>::operator<(const greylevel_<nbits>& rhs) const
    {
      return val_ < rhs.val_;
    }

    template <unsigned nbits>
    bool greylevel_<nbits>::operator==(const greylevel_<nbits>& rhs) const
    {
      return val_ == rhs.val_;
    }

    template <unsigned nbits>
    std::ostream& operator<<(std::ostream& ostr, const greylevel_<nbits>& g)
    {
      return ostr << g.value();
    }


    template <unsigned nbits, unsigned mbits>
    bool operator==(const greylevel_<nbits>& lhs, const greylevel_<mbits>& rhs)
    {
      return greylevel(lhs) == greylevel(rhs);
    }

    template <unsigned nbits, unsigned mbits>
    greylevel operator+(const greylevel_<nbits>& lhs, const greylevel_<mbits>& rhs)
    {
      return greylevel(lhs) + greylevel(rhs);
    }

    template <unsigned nbits, unsigned mbits>
    greylevel operator-(const greylevel_<nbits>& lhs, const greylevel_<mbits>& rhs)
    {
      return greylevel(lhs) - greylevel(rhs);
    }

    template <unsigned nbits>
    greylevel operator*(int s, const greylevel_<nbits>& rhs)
    {
      precondition(s >= 0);
      greylevel tmp(nbits, s * rhs.value());
      return tmp;
    }

    template <unsigned nbits>
    greylevel operator*(const greylevel_<nbits>& lhs, int s)
    {
      precondition(s >= 0);
      greylevel tmp(nbits, lhs.value() * s);
      return tmp;
    }

    template <unsigned nbits>
    greylevel operator/(const greylevel_<nbits>& lhs, int s)
    {
      precondition(s > 0);
      greylevel tmp(nbits, lhs.value() / s);
      return tmp;
    }



    // Greylevel.

    greylevel::greylevel()
      : nbits_(0)
    {
    }

    template <unsigned n>
    greylevel::greylevel(const greylevel_<n>& g)
      : nbits_(n),
	val_(g.value())
    {
    }

    greylevel::greylevel(unsigned nbits, unsigned long val)
      : nbits_(nbits),
	val_(val)
    {
    }

    unsigned long greylevel::value() const
    {
      invariant(nbits_ != 0);
      return val_;
    }

    unsigned greylevel::nbits() const
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


    void greylevel::set_nbits(unsigned nbits)
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


    greylevel greylevel::to_nbits(unsigned nbits) const
    {
      precondition(nbits != 0);
      invariant(nbits_ != 0);
      greylevel tmp(*this);
      tmp.set_nbits(nbits);
      return tmp;
    }


    template <unsigned n>
    greylevel::operator greylevel_<n>() const
    {
      precondition(nbits_ != 0);
      greylevel_<n> tmp(internal::convert<n>(nbits_, val_));
      assert(tmp.value() < internal::two_pow_(n));
      return tmp;
    }

    bool greylevel::operator<(const greylevel& rhs) const
    {
      precondition(nbits_ != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == nbits_)
	return rhs.value() < val_;
      if (nbits_ > rhs.nbits())
	return val_ < rhs.to_nbits(nbits_).value();
      else
	return this->to_nbits(rhs.nbits()).value() < rhs.value();
    }

    bool greylevel::operator==(const greylevel& rhs) const
    {
      precondition(nbits_ != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == nbits_)
	return rhs.value() == val_;
      if (nbits_ > rhs.nbits())
	return val_ == rhs.to_nbits(nbits_).value();
      else
	return this->to_nbits(rhs.nbits()).value() == rhs.value();
    }

    std::ostream& operator<<(std::ostream& ostr, const greylevel& g)
    {
      return ostr << g.value() << '/' << g.nbits() << "nbits";
    }

    greylevel operator+(const greylevel& lhs, const greylevel& rhs)
    {
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (lhs.nbits() > rhs.nbits())
	{
	  greylevel tmp(lhs.nbits(),
		   lhs.value() + rhs.to_nbits(lhs.nbits()).value());
	  return tmp;
	}
      else
	{
	  greylevel tmp(rhs.nbits(),
		   lhs.to_nbits(rhs.nbits()).value() + rhs.value());
	  return tmp;
	}
    }

    greylevel operator-(const greylevel& lhs, const greylevel& rhs)
    {
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (lhs.nbits() > rhs.nbits())
	{
	  unsigned long l = rhs.to_nbits(lhs.nbits()).value();
	  assert(lhs.value() >= l);
	  greylevel tmp(lhs.nbits(),
		   lhs.value() - l);
	  return tmp;
	}
      else
	{
	  unsigned long l = lhs.to_nbits(rhs.nbits()).value();
	  assert(l >= rhs.value());
	  greylevel tmp(rhs.nbits(),
		   l - rhs.value());
	  return tmp;
	}
    }

    greylevel operator*(int s, const greylevel& rhs)
    {
      precondition(s >= 0);
      greylevel tmp(rhs.nbits(), rhs.value() * s);
      return tmp;
    }

    greylevel operator*(const greylevel& lhs, int s)
    {
      precondition(s >= 0);
      greylevel tmp(lhs.nbits(), lhs.value() * s);
      return tmp;
    }

    greylevel operator/(const greylevel& lhs, int s)
    {
      precondition(s > 0);
      greylevel tmp(lhs.nbits(), lhs.value() / s);
      return tmp;
    }

# endif


  } // end of namespace oln::value

} // end of namespace oln



namespace xtd
{

  using oln::value::greylevel;

  template <> struct set_trait_<op_plus,  greylevel, greylevel> { typedef greylevel ret; };
  template <> struct set_trait_<op_minus, greylevel, greylevel> { typedef greylevel ret; };
  template <> struct set_trait_<op_mult,  int,  greylevel> { typedef greylevel ret; };
  template <> struct set_trait_<op_mult,  greylevel, int > { typedef greylevel ret; };
  template <> struct set_trait_<op_div,   greylevel, int > { typedef greylevel ret; };

  using oln::value::greylevel_;

  template <unsigned nbits, unsigned mbits>
  struct set_trait_< op_plus,  greylevel_<nbits>, greylevel_<mbits> > { typedef greylevel ret; };

  template <unsigned nbits, unsigned mbits>
  struct set_trait_< op_minus, greylevel_<nbits>, greylevel_<mbits> > { typedef greylevel ret; };

  template <unsigned nbits>
  struct set_trait_< op_mult, int, greylevel_<nbits> > { typedef greylevel ret; };

  template <unsigned nbits>
  struct set_trait_< op_mult, greylevel_<nbits>, int > { typedef greylevel ret; };

  template <unsigned nbits>
  struct set_trait_< op_div,  greylevel_<nbits>, int > { typedef greylevel ret; };

} // end of namespace xtd


#endif // ! OLN_CORE_VALUE_GREYLEVEL_HH
