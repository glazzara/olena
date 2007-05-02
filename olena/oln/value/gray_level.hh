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

#ifndef OLN_VALUE_GRAY_LEVEL_HH
# define OLN_VALUE_GRAY_LEVEL_HH

# include <iostream>
# include <oln/core/concept/operators.hh>
# include <oln/core/internal/value_base.hh>
# include <mlc/int.hh>


// FIXME: Move!
# include <oln/value/mlc_extra.hh>


namespace oln
{

  namespace value
  {

    namespace internal
    {

      template <typename T> T format(T t) { return t; }
      unsigned format(unsigned char t) { return t; }


      template <unsigned nbits> struct encoding_;

      template <> struct encoding_ <1>  { typedef bool           ret; };

      template <> struct encoding_ <2>  { typedef unsigned char  ret; };
      template <> struct encoding_ <3>  { typedef unsigned char  ret; };
      template <> struct encoding_ <4>  { typedef unsigned char  ret; };
      template <> struct encoding_ <5>  { typedef unsigned char  ret; };
      template <> struct encoding_ <6>  { typedef unsigned char  ret; };
      template <> struct encoding_ <7>  { typedef unsigned char  ret; };
      template <> struct encoding_ <8>  { typedef unsigned char  ret; };

      template <> struct encoding_ < 9> { typedef unsigned short ret; };
      template <> struct encoding_ <10> { typedef unsigned short ret; };
      template <> struct encoding_ <11> { typedef unsigned short ret; };
      template <> struct encoding_ <12> { typedef unsigned short ret; };
      template <> struct encoding_ <13> { typedef unsigned short ret; };
      template <> struct encoding_ <14> { typedef unsigned short ret; };
      template <> struct encoding_ <15> { typedef unsigned short ret; };
      template <> struct encoding_ <16> { typedef unsigned short ret; };

      template <> struct encoding_ <32> { typedef unsigned long  ret; };

    } // end of oln::value::internal


    // Fwd decls.
    template <unsigned N> class gray_level;
    class gray;
    

  } // end of namespace oln::value


  /// \{
  /// Aliases.

  typedef value::gray_level<8>  gl8;
  typedef value::gray_level<16> gl16;
  typedef value::gray_level<32> gl32;
  
  /// \}


# define current gray_level<N>
# define super oln::internal::graylevel_value_< oln::value::current >

  // Super type.

  template <unsigned N>
  struct super_trait_< value::current >
  {
    typedef super ret;
  };

  // Virtual types.

  template <unsigned N>
  struct vtypes< value::current >
  {
    typedef typename value::internal::encoding_<N>::ret encoding;
    typedef mlc::uint_<N> nbits;
  };


  namespace value
  {

    /// General gray-level class on n bits.
    template <unsigned N>
    class gray_level : public super
    {
    public:
      stc_using(encoding);

      // Ctors.
      gray_level();

      // In oln/value/gray.hh:
      gray_level(const gray& g);
      gray_level<N>& operator=(const gray& val);
      operator gray() const;

      // Access to encoding value.
      const encoding& enc() const;
      encoding& enc();

      // Conversions.
      operator float() const;
      operator double() const;

    protected:
      encoding val_;
    };


    // Op<<.
    template <unsigned N>
    std::ostream& operator<<(std::ostream& ostr, const gray_level<N>& g);


    template <unsigned N>
    bool operator==(const gray_level<N>& lhs, const gray_level<N>& rhs);

    template <unsigned N, unsigned M>
    bool operator==(const gray_level<N>& lhs, const gray_level<M>& rhs);


    template <unsigned N>
    bool operator<(const gray_level<N>& lhs, const gray_level<N>& rhs);

    template <unsigned N, unsigned M>
    bool operator<(const gray_level<N>& lhs, const gray_level<M>& rhs);


    // -> gray

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




# ifndef OLN_INCLUDE_ONLY

    template <unsigned N>
    current::gray_level()
    {
    }

    template <unsigned N>
    const typename current::encoding&
    current::enc() const
    {
      return this->val_;
    }

    template <unsigned N>
    typename current::encoding&
    current::enc()
    {
      return this->val_;
    }

    template <unsigned N>
    current:: operator float() const
    {
      static const float coef_
	= 1.f / float(mlc::math::two_pow_<N>::value - 1.f);
      return coef_ * this->val_;
    }

    template <unsigned N>
    current::operator double() const
    {
      static const double coef_
	= 1. / double(mlc::math::two_pow_<N>::value - 1.);
      return coef_ * this->val_;
    }

    template <unsigned N>
    std::ostream& operator<<(std::ostream& ostr, const gray_level<N>& g)
    {
      return ostr << internal::format( g.enc() )
		  << "[/" << N << ']';
    }


    template <unsigned N>
    bool operator==(const gray_level<N>& lhs, const gray_level<N>& rhs)
    {
      return lhs.enc() == rhs.enc();
    }

    template <unsigned N, unsigned M>
    bool operator==(const gray_level<N>& lhs, const gray_level<M>& rhs)
    {
      return
	mlc::math::two_pow_<M>::value * lhs.enc()
	== mlc::math::two_pow_<N>::value * rhs.enc();
      // FIXME: return gray(lhs) == gray(rhs);
    }


    template <unsigned N>
    bool operator<(const gray_level<N>& lhs, const gray_level<N>& rhs)
    {
      return lhs.enc() < rhs.enc();
    }

    template <unsigned N, unsigned M>
    bool operator<(const gray_level<N>& lhs, const gray_level<M>& rhs)
    {
      return
	mlc::math::two_pow_<M>::value * lhs.enc()
	< mlc::math::two_pow_<N>::value * rhs.enc();
      // FIXME: return gray(lhs) < gray(rhs);
    }

    /*

    template <unsigned N>
    gray_level<N>::operator gray() const
    {
      gray tmp(N, this->val_);
      return tmp;
    }

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
      gray tmp(N, s * rhs.enc());
      return tmp;
    }

    template <unsigned N>
    gray operator*(const gray_level<N>& lhs, int s)
    {
      precondition(s >= 0);
      gray tmp(N, lhs.enc() * s);
      return tmp;
    }

    template <unsigned N>
    gray operator/(const gray_level<N>& lhs, int s)
    {
      precondition(s > 0);
      gray tmp(N, lhs.enc() / s);
      return tmp;
    }

    */

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::value

# undef super
# undef current

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

  template <unsigned N, unsigned M>
  struct set_trait_< op_plus,  gray_level<N>, gray_level<M> > { typedef gray ret; };

  template <unsigned N, unsigned M>
  struct set_trait_< op_minus, gray_level<N>, gray_level<M> > { typedef gray ret; };

  template <unsigned N>
  struct set_trait_< op_mult, int, gray_level<N> > { typedef gray ret; };

  template <unsigned N>
  struct set_trait_< op_mult, gray_level<N>, int > { typedef gray ret; };

  template <unsigned N>
  struct set_trait_< op_div,  gray_level<N>, int > { typedef gray ret; };

} // end of namespace xtd

*/


# include <oln/value/gray.hh>


#endif // ! OLN_VALUE_GRAY_LEVEL_HH
