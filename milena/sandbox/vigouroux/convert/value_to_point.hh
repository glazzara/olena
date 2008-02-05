// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_TO_POINT
# define OLENA_VALUE_TO_POINT
# include <oln/core/point1d.hh>
# include <oln/core/point3d.hh>
# include <ntg/int.hh>
# include <ntg/color/color.hh>
# include <oln/convert/conversion.hh>
namespace oln {

  namespace convert {
    /*! Convert a value of pixel to a point.
    **
    ** For example, transform an RGB color to a 3D point
    ** (ntg::rgb_8 => oln::point3d).
    ** This function is useful to build the histogram. \n
    ** Example:
    ** \verbatim
    ** f(oln::convert::value_to_point<ntg::rgb_8>()(ntg::rgb_8(1,6,64)));
    ** // is equivalent to:
    ** f(oln::point3d(1, 6, 64));
    ** \endverbatim
    */
    template <typename Argument_type,
	      class Exact = mlc::final>
    struct value_to_point:
      public abstract::conversion_from_type_to_type
        <Argument_type,
	 point1d,
	 typename mlc::exact_vt<value_to_point<Argument_type,
					       Exact>,
				Exact>::ret>
    {
    private:
      /// By default a scalar is expected. If the type is a vector, a
      /// specialization should be written.
      typedef typename ntg_is_a(Argument_type, ntg::non_vectorial)::ensure_type ensure_type;
    public:
      /// By default return a point1d.
      typedef point1d result_type;
      typedef Argument_type argument_type;

      /// Convert a binary to a point.
      template <typename O, typename I>
      struct doit_binary
      {
	static
	O
	doit(const I &input)
	{
	  return input ? O(1) : O(0);
	}
      };
      /// Convert a non vectorial to a point.
      template <typename O, typename I>
      struct doit_not_binary
      {
	static
	O
	doit(const I &input)
	{
	  return O(input - ntg_min_val(I));
	}
      };

      result_type
      doit(const argument_type &input) const
      {
	typedef typename mlc::if_<ntg_is_a(argument_type, ntg::binary)::ret,
	                          doit_binary<result_type, argument_type>,
	  doit_not_binary<result_type, argument_type> >::ret doit_type;
	  return doit_type::doit(input);
      }
    };

    /*! Specialization for color of three dimension.
    **
    ** \todo Could be generalized to n dimensions if there were a trait that
    ** give a pointkd for a given dimension k.
    */
    template <unsigned Qbits, template <unsigned> class S, class Exact>
    struct value_to_point<ntg::color<3, Qbits, S>, Exact>:
      public abstract::conversion_from_type_to_type
        <typename ntg::color<3, Qbits, S>,
	 point3d,
	 typename mlc::exact_vt<value_to_point<typename ntg::color<3, Qbits, S>,
					       Exact>,
				Exact>::ret>
    {
    public:
      typedef point3d result_type;
      typedef typename ntg::color<3, Qbits, S> argument_type;

      result_type
      doit(const argument_type &input) const
      {
	result_type r;
	for (unsigned i = 0; i < 3; ++i)
	  r.nth(i) = oln::coord(input[i]);
	return r;
      }
    };
  }
}
#endif
