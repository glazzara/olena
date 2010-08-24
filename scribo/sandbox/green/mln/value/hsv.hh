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

#ifndef MLN_VALUE_HSV_HH
# define MLN_VALUE_HSV_HH

#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
// #include <mln/algebra/vec.hh>

// Used in from_to
// #include <mln/fun/v2v/rgb_to_hsv.hh>


namespace mln
{

  // Forward declarations.
  namespace value
  {

    template <typename H, typename S, typename V>
    class hsv_;

  }


/*
  namespace convert
  {

    namespace over_load
    {

      // rgb to hsv_
      void
      from_to_(const value::rgb<16>& from, value::hsv_<float,float,float>& to);

      // rgb to hsv_
      void
      from_to_(const value::rgb<8>& from, value::hsv_<float,float,float>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert
*/


  namespace trait
  {

    template <typename H, typename S, typename V>
    struct set_precise_binary_< op::plus, mln::value::hsv_<H,S,V>,
				mln::value::hsv_<H,S,V> >
    {
      typedef mln::value::hsv_<H,S,V> ret;
    };

    template <typename H, typename S, typename V>
    struct set_precise_binary_< op::minus, mln::value::hsv_<H,S,V>,
				mln::value::hsv_<H,S,V> >
    {
      typedef mln::value::hsv_<H,S,V> ret;
    };

    template <typename H, typename S, typename V, typename S2>
    struct set_precise_binary_< op::times, mln::value::hsv_<H,S,V>,
				mln::value::scalar_<S2> >
    {
      typedef mln::value::hsv_<H,S,V> ret;
    };

    template <typename H, typename S, typename V, typename S2>
    struct set_precise_binary_< op::div, mln::value::hsv_<H,S,V>,
				mln::value::scalar_<S2> >
    {
      typedef mln::value::hsv_<H,S,V> ret;
    };


    // FIXME : Is there any way more generic? a way to factor
    //  set_precise_binary_< op::div, mln::value::hsv_<H,S,V>, mln::value::scalar_<S> >
    //  and
    //  set_precise_binary_< op::div, mln::value::hsv_<H,S,V>, mln::value::int_u<m> >
    //  as for op::times.

    template <typename H, typename S, typename V, unsigned m>
    struct set_precise_binary_< op::times, mln::value::hsv_<H,S,V>,
				mln::value::int_u<m> >
    {
      typedef mln::value::hsv_<H,S,V> ret;
    };

    template <typename H, typename S, typename V, unsigned m>
    struct set_precise_binary_< op::div, mln::value::hsv_<H,S,V>,
				mln::value::int_u<m> >
    {
      typedef mln::value::hsv_<H,S,V> ret;
    };

    template <typename H, typename S, typename V>
    struct value_< mln::value::hsv_<H,S,V> >
    {
      enum {
	dim = 3,
	nbits = (sizeof (H) + sizeof (S) + sizeof (V)) * 8,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::color       kind;
      typedef mln_value_quant_from_(card)     quant;

      typedef void comp;
      typedef H comp_0;
      typedef S comp_1;
      typedef V comp_2;

      template <typename VAL>
      static comp_0 get_comp_0(const VAL& v) { return v.hue(); }

      template <typename VAL>
      static comp_1 get_comp_1(const VAL& v) { return v.sat(); }

      template <typename VAL>
      static comp_2 get_comp_2(const VAL& v) { return v.val(); }

      typedef mln::value::hsv_<H,S,V> sum;
    };

  } // end of namespace trait


  namespace value
  {

    template <typename E>
    struct HSV : Object<E>
    {
    };

    template <typename H, typename S, typename V>
    class hsv_ : public HSV< hsv_<H,S,V> >
    {
    public:

      typedef H h_type;
      typedef S s_type;
      typedef V v_type;

      /// Constructor without argument.
      hsv_()
      {
      }

      hsv_(const literal::zero_t&)
	: hue_(0),
	  sat_(0),
	  val_(0)
      {
      }

      /// Constructor from component values.
      hsv_(const H& hue, const S& sat, const V& val)
	: hue_(hue),
	  sat_(sat),
	  val_(val)
      {
      }

      /// Read-only access to the hue component.
      const H& hue() const;
      const S& sat() const;
      const V& val() const;

      /// Read-write access to the hue component.
      H& hue();
      S& sat();
      V& val();

    private:
      //FIXME: Don't we want to store these values in a vector?
      H hue_;
      S sat_;
      V val_;
    };


    typedef hsv_<float, float, float> hsv_f;
    typedef hsv_<double, double, double> hsv_d;


    /// Print an hsv \p c into the output stream \p ostr.
    ///
    /// \param[in,out] ostr An output stream.
    /// \param[in] c An rgb.
    ///
    /// \return The modified output stream \p ostr.
    template <typename H, typename S, typename V>
    std::ostream& operator<<(std::ostream& ostr, const hsv_<H,S,V>& c);


    /// Addition.
    /// {
    template <typename H, typename S, typename V>
    hsv_<H,S,V>
    operator+(const hsv_<H,S,V>& lhs, const hsv_<H,S,V>& rhs);
    /// \}

    /// Subtraction.
    /// \{
    template <typename H, typename S, typename V>
    hsv_<H,S,V>
    operator-(const hsv_<H,S,V>& lhs, const hsv_<H,S,V>& rhs);
    /// \}

    /// Product.
    /// \{
    template <typename H, typename S, typename V, typename S2>
    hsv_<H,S,V>
    operator*(const hsv_<H,S,V>& lhs, const mln::value::scalar_<S2>& s);
    /// \}

    /// Division.
    /// \{
    template <typename H, typename S, typename V, typename S2>
    hsv_<H,S,V>
    operator/(const hsv_<H,S,V>& lhs, const mln::value::scalar_<S2>& s);
    /// \}

    /// Comparison.
    /// \{
    template <typename H, typename S, typename V>
    bool
    operator==(const hsv_<H,S,V>& lhs, const hsv_<H,S,V>& rhs);
    /// \}

  } // end of namespace mln::value



  // More forward declarations
  namespace fun
  {
    namespace v2v
    {

      template <typename T_hsv>
      struct f_rgb_to_hsv_;

      typedef f_rgb_to_hsv_<value::hsv_f> f_rgb_to_hsv_f_t;

      extern f_rgb_to_hsv_f_t f_rgb_to_hsv_f;

    }

  }

# ifndef MLN_INCLUDE_ONLY


  namespace value
  {

    template <typename H, typename S, typename V>
    const H&
    hsv_<H,S,V>::hue() const
    {
      return this->hue_;
    }

    template <typename H, typename S, typename V>
    const S&
    hsv_<H,S,V>::sat() const
    {
      return this->sat_;
    }

    template <typename H, typename S, typename V>
    const V&
    hsv_<H,S,V>::val() const
    {
      return this->val_;
    }

    template <typename H, typename S, typename V>
    H&
    hsv_<H,S,V>::hue()
    {
      return this->hue_;
    }

    template <typename H, typename S, typename V>
    S&
    hsv_<H,S,V>::sat()
    {
      return this->sat_;
    }

    template <typename H, typename S, typename V>
    V&
    hsv_<H,S,V>::val()
    {
      return this->val_;
    }


    template <typename H, typename S, typename V>
    inline
    std::ostream& operator<<(std::ostream& ostr, const hsv_<H,S,V>& v)
    {
      return ostr << '(' << debug::format(v.hue())
		  << ',' << debug::format(v.sat())
		  << ',' << debug::format(v.val())
		  << ')';
    }


    template <typename H, typename S, typename V>
    hsv_<H,S,V>
    operator+(const hsv_<H,S,V>& lhs, const hsv_<H,S,V>& rhs)
    {
      return hsv_<H,S,V>(lhs.hue() + rhs.hue(),
			 lhs.sat() + rhs.sat(),
			 lhs.val() + rhs.val());
    }


    template <typename H, typename S, typename V>
    hsv_<H,S,V>
    operator-(const hsv_<H,S,V>& lhs, const hsv_<H,S,V>& rhs)
    {
      return hsv_<H,S,V>(lhs.hue() - rhs.hue(),
			 lhs.sat() - rhs.sat(),
			 lhs.val() - rhs.val());
    }


    template <typename H, typename S, typename V, typename S2>
    hsv_<H,S,V>
    operator*(const hsv_<H,S,V>& lhs, const mln::value::scalar_<S2>& s)
    {
      return hsv_<H,S,V>(lhs.hue() * s,
			 lhs.sat() * s,
			 lhs.val() * s);
    }


    template <typename H, typename S, typename V, typename S2>
    hsv_<H,S,V>
    operator/(const hsv_<H,S,V>& lhs, const mln::value::scalar_<S2>& s)
    {
      return hsv_<H,S,V>(lhs.hue() / s,
			 lhs.sat() / s,
			 lhs.val() / s);
    }

    template <typename H, typename S, typename V>
    bool
    operator==(const hsv_<H,S,V>& lhs, const hsv_<H,S,V>& rhs)
    {
      return lhs.hue() == rhs.hue()
	  && lhs.sat() == rhs.sat()
	  && lhs.val() == rhs.val();
    }

  } // end of namespace mln::value

/*
  namespace convert
  {

    namespace over_load
    {

      inline
      void
      from_to_(const value::rgb<16>& from, value::hsv_<float,float,float>& to)
      {
	to = fun::v2v::f_rgb_to_hsv_f(from);
      }

      inline
      void
      from_to_(const value::rgb<8>& from, value::hsv_<float,float,float>& to)
      {
	to = fun::v2v::f_rgb_to_hsv_f(from);
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert
*/

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_VALUE_HSV_HH
