/*!
 * \file   pipo.hh
 * \author etiennefolio <ornthalas@gmail.com>
 */

#ifndef PIPO_HH_
# define PIPO_HH_

# include <mln/value/int_s.hh>
# include <mln/value/int_u.hh>
# include <mln/value/float01.hh>

namespace mln
{

  namespace value { class pipo; }

  namespace trait
  {

    template <>
    struct value_< mln::value::pipo >
    {
      enum {
	dim = 3
      };

      typedef mln::value::int_s<3>	comp_0;
      typedef mln::value::float01_<8>	comp_1;
      typedef mln::value::int_u<7>	comp_2;
    };

  } // end of namespace trait

  namespace value
  {
    class pipo
    {
    public:

      int_s<3>   c0;
      float01_<8> c1;
      int_u<7>   c2;

      pipo();
      pipo(int_s<3> _c0,
	   float01_<8> _c1,
	   int_u<7> _c2);

      pipo& operator=(const pipo& rhs);
    };

# ifndef MLN_INCLUDE_ONLY

    inline
    pipo::pipo()
      : c0(mln_min(int_s<3>)),
	c1(mln_min(float01_<8>)),
	c2(mln_min(int_u<7>))
    {
    }

    inline
    pipo::pipo(int_s<3> _c0,
	       float01_<8> _c1,
	       int_u<7> _c2)
      : c0(_c0),
	c1(_c1),
	c2(_c2)
    {
    }

    inline
    pipo&
    pipo::operator=(const pipo& rhs)
    {
      if (& rhs == this)
	return *this;
      this->c0 = rhs.c0;
      this->c1 = rhs.c1;
      this->c2 = rhs.c2;
      return *this;
    }

    inline
    std::ostream& operator<<(std::ostream& ostr, const pipo& v)
    {
      return ostr << '(' << debug::format(v.c0)
		  << ',' << debug::format(v.c1)
		  << ',' << debug::format(v.c2)
		  << ')';
    }

// dunno why this doesn't compile... *_*"

//     inline
//     std::istream& operator>>(std::istream& istr, pipo& c)
//     {
//       return istr >> c.c0 >> c.c1 >> c.c2;
//     }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace value
}

#endif /* !PIPO_HH_ */
