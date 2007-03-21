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

#ifndef OLN_CORE_VALUE_GRAYLEVEL_HH
# define OLN_CORE_VALUE_GRAYLEVEL_HH

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
    class graylevel;


    /// General gray-level class on n bits.
    template <unsigned nbits>
    class graylevel_ : public oln::abstract::value
    {
      typedef typename internal::encoding<nbits>::ret encoding_t;

    public:
      
      /// Ctor.
      graylevel_();
      
      /// Ctor.
      explicit graylevel_(const typename internal::encoding<nbits>::ret& val);

      /// Access to std type.
      typename internal::encoding<nbits>::ret value() const;

      /// Op encoding_t.
      operator typename internal::encoding<nbits>::ret() const;

      /// Op graylevel.
      operator graylevel() const;

      /// Op<.
      bool operator<(const graylevel_<nbits>& rhs) const;

      /// Op==.
      bool operator==(const graylevel_<nbits>& rhs) const;

    protected:
      encoding_t val_;
    };


    /// Op<<.
    template <unsigned nbits>
    std::ostream& operator<<(std::ostream& ostr, const graylevel_<nbits>& g);


    /// Aliases.
    typedef graylevel_<8>  gl8;
    typedef graylevel_<16> gl16;
    typedef graylevel_<32> gl32;


    template <unsigned nbits, unsigned mbits>
    bool operator==(const graylevel_<nbits>& lhs, const graylevel_<mbits>& rhs);

    template <unsigned nbits, unsigned mbits>
    graylevel operator+(const graylevel_<nbits>& lhs, const graylevel_<mbits>& rhs);

    template <unsigned nbits, unsigned mbits>
    graylevel operator-(const graylevel_<nbits>& lhs, const graylevel_<mbits>& rhs);

    template <unsigned nbits>
    graylevel operator*(int s, const graylevel_<nbits>& rhs);

    template <unsigned nbits>
    graylevel operator*(const graylevel_<nbits>& lhs, int s);

    template <unsigned nbits>
    graylevel operator/(const graylevel_<nbits>& lhs, int s);



    /// General gray-level class where n bits is not know at compile-time.
    /// This class is used for exchange between gray-level types purpose.
    class graylevel : public oln::abstract::value
    {
    public:

      /// Ctor.
      graylevel();

      /// Ctor.
      template <unsigned n>
      graylevel(const graylevel_<n>& val);

      /// Ctor.
      graylevel(unsigned nbits, unsigned long val);

      /// Access to std type.
      unsigned long value() const;

      unsigned nbits() const;

      void set_nbits(unsigned nbits);

      graylevel to_nbits(unsigned nbits) const;

      template <unsigned n>
      operator graylevel_<n>() const;

      bool operator<(const graylevel& rhs) const;

      bool operator==(const graylevel& rhs) const;

    protected:
      unsigned nbits_;
      unsigned long val_;
    };


    std::ostream& operator<<(std::ostream& ostr, const graylevel& g);


    graylevel operator+(const graylevel& lhs, const graylevel& rhs);
    graylevel operator-(const graylevel& lhs, const graylevel& rhs);

    graylevel operator*(int s, const graylevel& rhs);
    graylevel operator*(const graylevel& lhs, int s);

    graylevel operator/(const graylevel& lhs, int s);



# ifndef OLN_INCLUDE_ONLY

    // Graylevel_<nbits>.

    template <unsigned nbits>
    graylevel_<nbits>::graylevel_()
    {
    }

    template <unsigned nbits>
    graylevel_<nbits>::graylevel_(const typename internal::encoding<nbits>::ret& val)
      : val_(val)
    {
    }

    template <unsigned nbits>
    typename internal::encoding<nbits>::ret
    graylevel_<nbits>::value() const
    {
      return val_;
    }

    template <unsigned nbits>
    graylevel_<nbits>::operator graylevel() const
    {
      graylevel tmp(nbits, val_);
      return tmp;
    }

    template <unsigned nbits>
    graylevel_<nbits>::operator typename internal::encoding<nbits>::ret() const
    {
      return val_;
    }

    template <unsigned nbits>
    bool graylevel_<nbits>::operator<(const graylevel_<nbits>& rhs) const
    {
      return val_ < rhs.val_;
    }

    template <unsigned nbits>
    bool graylevel_<nbits>::operator==(const graylevel_<nbits>& rhs) const
    {
      return val_ == rhs.val_;
    }

    template <unsigned nbits>
    std::ostream& operator<<(std::ostream& ostr, const graylevel_<nbits>& g)
    {
      return ostr << g.value();
    }


    template <unsigned nbits, unsigned mbits>
    bool operator==(const graylevel_<nbits>& lhs, const graylevel_<mbits>& rhs)
    {
      return graylevel(lhs) == graylevel(rhs);
    }

    template <unsigned nbits, unsigned mbits>
    graylevel operator+(const graylevel_<nbits>& lhs, const graylevel_<mbits>& rhs)
    {
      return graylevel(lhs) + graylevel(rhs);
    }

    template <unsigned nbits, unsigned mbits>
    graylevel operator-(const graylevel_<nbits>& lhs, const graylevel_<mbits>& rhs)
    {
      return graylevel(lhs) - graylevel(rhs);
    }

    template <unsigned nbits>
    graylevel operator*(int s, const graylevel_<nbits>& rhs)
    {
      precondition(s >= 0);
      graylevel tmp(nbits, s * rhs.value());
      return tmp;
    }

    template <unsigned nbits>
    graylevel operator*(const graylevel_<nbits>& lhs, int s)
    {
      precondition(s >= 0);
      graylevel tmp(nbits, lhs.value() * s);
      return tmp;
    }

    template <unsigned nbits>
    graylevel operator/(const graylevel_<nbits>& lhs, int s)
    {
      precondition(s > 0);
      graylevel tmp(nbits, lhs.value() / s);
      return tmp;
    }



    // Graylevel.

    graylevel::graylevel()
      : nbits_(0)
    {
    }

    template <unsigned n>
    graylevel::graylevel(const graylevel_<n>& g)
      : nbits_(n),
	val_(g.value())
    {
    }

    graylevel::graylevel(unsigned nbits, unsigned long val)
      : nbits_(nbits),
	val_(val)
    {
    }

    unsigned long graylevel::value() const
    {
      invariant(nbits_ != 0);
      return val_;
    }

    unsigned graylevel::nbits() const
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


    void graylevel::set_nbits(unsigned nbits)
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


    graylevel graylevel::to_nbits(unsigned nbits) const
    {
      precondition(nbits != 0);
      invariant(nbits_ != 0);
      graylevel tmp(*this);
      tmp.set_nbits(nbits);
      return tmp;
    }


    template <unsigned n>
    graylevel::operator graylevel_<n>() const
    {
      precondition(nbits_ != 0);
      graylevel_<n> tmp(internal::convert<n>(nbits_, val_));
      assert(tmp.value() < internal::two_pow_(n));
      return tmp;
    }

    bool graylevel::operator<(const graylevel& rhs) const
    {
      precondition(nbits_ != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == nbits_)
	return rhs.value() < val_;
      if (nbits_ > rhs.nbits())
	return val_ < rhs.to_nbits(nbits_).value();
      else
	return this->to_nbits(rhs.nbits()).value() < rhs.value();
    }

    bool graylevel::operator==(const graylevel& rhs) const
    {
      precondition(nbits_ != 0 and rhs.nbits() != 0);
      if (rhs.nbits() == nbits_)
	return rhs.value() == val_;
      if (nbits_ > rhs.nbits())
	return val_ == rhs.to_nbits(nbits_).value();
      else
	return this->to_nbits(rhs.nbits()).value() == rhs.value();
    }

    std::ostream& operator<<(std::ostream& ostr, const graylevel& g)
    {
      return ostr << g.value() << '/' << g.nbits() << "nbits";
    }

    graylevel operator+(const graylevel& lhs, const graylevel& rhs)
    {
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (lhs.nbits() > rhs.nbits())
	{
	  graylevel tmp(lhs.nbits(),
		   lhs.value() + rhs.to_nbits(lhs.nbits()).value());
	  return tmp;
	}
      else
	{
	  graylevel tmp(rhs.nbits(),
		   lhs.to_nbits(rhs.nbits()).value() + rhs.value());
	  return tmp;
	}
    }

    graylevel operator-(const graylevel& lhs, const graylevel& rhs)
    {
      precondition(lhs.nbits() != 0 and rhs.nbits() != 0);
      if (lhs.nbits() > rhs.nbits())
	{
	  unsigned long l = rhs.to_nbits(lhs.nbits()).value();
	  assert(lhs.value() >= l);
	  graylevel tmp(lhs.nbits(),
		   lhs.value() - l);
	  return tmp;
	}
      else
	{
	  unsigned long l = lhs.to_nbits(rhs.nbits()).value();
	  assert(l >= rhs.value());
	  graylevel tmp(rhs.nbits(),
		   l - rhs.value());
	  return tmp;
	}
    }

    graylevel operator*(int s, const graylevel& rhs)
    {
      precondition(s >= 0);
      graylevel tmp(rhs.nbits(), rhs.value() * s);
      return tmp;
    }

    graylevel operator*(const graylevel& lhs, int s)
    {
      precondition(s >= 0);
      graylevel tmp(lhs.nbits(), lhs.value() * s);
      return tmp;
    }

    graylevel operator/(const graylevel& lhs, int s)
    {
      precondition(s > 0);
      graylevel tmp(lhs.nbits(), lhs.value() / s);
      return tmp;
    }

# endif


  } // end of namespace oln::value

} // end of namespace oln



namespace xtd
{

  using oln::value::graylevel;

  template <> struct set_trait_<op_plus,  graylevel, graylevel> { typedef graylevel ret; };
  template <> struct set_trait_<op_minus, graylevel, graylevel> { typedef graylevel ret; };
  template <> struct set_trait_<op_mult,  int,  graylevel> { typedef graylevel ret; };
  template <> struct set_trait_<op_mult,  graylevel, int > { typedef graylevel ret; };
  template <> struct set_trait_<op_div,   graylevel, int > { typedef graylevel ret; };

  using oln::value::graylevel_;

  template <unsigned nbits, unsigned mbits>
  struct set_trait_< op_plus,  graylevel_<nbits>, graylevel_<mbits> > { typedef graylevel ret; };

  template <unsigned nbits, unsigned mbits>
  struct set_trait_< op_minus, graylevel_<nbits>, graylevel_<mbits> > { typedef graylevel ret; };

  template <unsigned nbits>
  struct set_trait_< op_mult, int, graylevel_<nbits> > { typedef graylevel ret; };

  template <unsigned nbits>
  struct set_trait_< op_mult, graylevel_<nbits>, int > { typedef graylevel ret; };

  template <unsigned nbits>
  struct set_trait_< op_div,  graylevel_<nbits>, int > { typedef graylevel ret; };

} // end of namespace xtd


#endif // ! OLN_CORE_VALUE_GRAYLEVEL_HH
