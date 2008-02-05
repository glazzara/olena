// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef NTG_COLOR_COLOR_HH
# define NTG_COLOR_COLOR_HH

/*
  Header for generic color type, from which real color types are defined.
*/

# include <ntg/basics.hh>
# include <ntg/int.hh>
# include <ntg/vect/vec.hh>
# include <ntg/core/pred_succ.hh>

# include <mlc/cmp.hh>

# include <iostream>
# include <sstream>
# include <string>

namespace ntg {

  namespace internal {

    /*------------------.
    | typetraits<color> |
    `------------------*/

    template <unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct typetraits<color<ncomps, qbits, color_system> >
    {
      enum { nb_comp = ncomps };

      typedef color<ncomps, qbits, color_system>	self;
      typedef self					ntg_type;
      typedef vectorial					abstract_type;
      typedef int_u<qbits>				comp_type;
      typedef self					base_type;
      typedef vec<ncomps, int_u<qbits> >		storage_type;
    };

    /*-------------------------.
    | Helper structs for float |
    `-------------------------*/

    /*!
      Helper struct to convert vec<N,T> to vec<N,float>,
      taking color_system into account.
    */
    template <unsigned n,
	      unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct _to_float
    {
      typedef int_u<qbits>		T;
      typedef vec<ncomps, T>		in_type;
      typedef vec<ncomps, float>	out_type;

      static void
      doit (const in_type& in, out_type& out)
      {
	float in_range = float(ntg_max_val(T)) - float(ntg_min_val(T));
	float out_range = float(color_system<n>::upper_bound())
	  - float(color_system<n>::lower_bound());
	out[n] = ((float(in[n]) - float(ntg_min_val(T)))
		  * out_range / in_range
		  + float(color_system<n>::lower_bound()));

	// process next componant recursively:
	_to_float<n + 1, ncomps, qbits, color_system>::doit(in, out);
      }
    };

    // Stop recursion when n == ncomps.
    template <unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct _to_float<ncomps, ncomps, qbits, color_system>
    {
      typedef vec<ncomps, int_u<qbits> >	in_type;
      typedef vec<ncomps, float>		out_type;

      static void
      doit (const in_type&, out_type&)
      {}
    };

    /*!
      Helper struct to convert vec<N,float> to vec<N,T>,
      taking color_system into account.
    */
    template <unsigned n,
	      unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct _from_float
    {
      typedef int_u<qbits>		T;
      typedef vec<ncomps, float>	in_type;
      typedef vec<ncomps, T>		out_type;

      static void
      doit (const in_type& in, out_type& out)
      {
	float out_range = float(optraits<T>::max())
	  - float(optraits<T>::min());
	float in_range = float(color_system<n>::upper_bound())
	  - float(color_system<n>::lower_bound());

	out[n] = cast::rbound<int_u<qbits> >
	  ((in[n] - float(color_system<n>::lower_bound()))
	   * out_range / in_range
	   + float(color_system<n>::lower_bound()));

	// process next componant recursively:
	_from_float<n + 1, ncomps, qbits, color_system>::doit(in, out);
      }
    };

    // stop recursion when n == ncomps.
    template <unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct _from_float<ncomps, ncomps, qbits, color_system>
    {
      typedef vec<ncomps, float>		in_type;
      typedef vec<ncomps, int_u<qbits> >	out_type;

      static void
      doit (const in_type&, out_type&)
      {}
    };

  } // end of internal.

  /*-----------------------------------.
  | color<ncomps, qbits, color_system> |
  `-----------------------------------*/

  //! Generic type for color.
  /*!
    Specific color types (such as rgb, xyz, etc.) are defined by
    specifying ncomps, qbits and a color_system trait.

    ncomps: number of components.
    qbits: number of bits of each unsigned integer component.
    color_system: traits defining the intervals of each component.

    Colors are implemented and seen as a vector of components.
  */
  template <unsigned ncomps,
	    unsigned qbits,
	    template <unsigned> class color_system>
  struct color : public vect_value<color<ncomps, qbits, color_system> >
  {
    typedef int_u<qbits>		comp_type;
    typedef vec<ncomps, comp_type>	vec_type;
    typedef vec<ncomps, float>		float_vec_type;

    color() {};
    color(const vec_type& vec) { this->val_ = vec; };
    color(const float_vec_type& vec)
    {
      internal::_from_float<0,ncomps,qbits,color_system>::doit(vec,this->val_);
    }

    color(const comp_type& c1, const comp_type& c2, const comp_type& c3)
    {
      mlc::is_true<ncomps == 3>::ensure();
      this->val_[0] = c1;
      this->val_[1] = c2;
      this->val_[2] = c3;
    }

    vec_type&		as_vec()       { return this->val_; }
    const vec_type&	as_vec() const { return this->val_; }

    float_vec_type
    to_float() const
    {
      float_vec_type tmp;
      internal::_to_float<0,ncomps,qbits,color_system>::doit(this->val_, tmp);
      return tmp;
    }

    bool
    operator==(const color& r) const
    { return this->val_ == r.val_; }
  };

  /*!
    Helper function to complete color_system (by inheritance).
  */
  template<int lval, int uval>
  struct interval
  {
    static int lower_bound() { return lval; }
    static int upper_bound() { return uval; }
  };

  template <unsigned ncomps,
	    unsigned qbits,
	    template <unsigned> class color_system>
  inline std::ostream&
  operator<<(std::ostream& o,
	     const color<ncomps, qbits, color_system>& r)
  {
    o << r.as_vec();
    return o;
  }

  namespace internal
  {

    /*----------------.
    | optraits<color> |
    `----------------*/

    template <unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct optraits<color<ncomps, qbits, color_system> >
    {
    private:
      typedef color<ncomps, qbits, color_system> self;
      typedef typename typetraits<self>::storage_type storage_type;

    public:
      static unsigned max_print_width ()
      {
	return ntg_max_print_width(storage_type);
      }

      static std::string
      name()
      {
	std::ostringstream out;
	// FIXME: Output color_system somehow.
	out << "color<" << ncomps << "," << qbits <<  ",...>" << std::ends;
	return out.str();
      }
    };


    template <typename T> struct default_less;

    /*! The specialized version of default_less for colors.
    **
    ** \warning This class is only provided to build classes
    ** that need a less class, it does not correspond to
    ** the reality. \n
    ** Example of a std::set of RGB colors:
    ** \verbatim
    ** std::set<ntg:rgb_8,
    **          ntg::internal::default_less<ntg::rgb8> > s;
    ** s.insert(ntg::rgb_8(10, 16, 64));
    ** \endverbatim
    */
    template <unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct default_less< ntg::color<ncomps, qbits, color_system> >
    {
      typedef ntg::color<ncomps, qbits, color_system> arg_type;
      bool operator()(const arg_type& l,
		      const arg_type& r) const
      {
	for (unsigned i = 0; i < ntg_nb_comp(arg_type); ++i)
	  if (l[i] < r[i])
	    return true;
	  else if (l[i] > r[i])
	    return false;
	return false;
      }
    };
  } // end of internal.

} // end of ntg.

#endif // !NTG_COLOR_COLOR_HH
