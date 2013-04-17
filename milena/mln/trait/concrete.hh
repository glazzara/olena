// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_TRAIT_CONCRETE_HH
# define MLN_TRAIT_CONCRETE_HH

/// \file
///
/// \brief Definition of the concrete image trait.

# include <mln/trait/ch_value.hh>

/*!  Macros provided for convenience. Should be used in a templated
  function only.
  \relates mln::trait::concrete
*/
# define mln_concrete(I)  typename mln::trait::concrete< I >::ret

/*!  Macros provided for convenience. Should be used in a
  non-templated function only.
  \relates mln::trait::concrete
*/
# define mln_concrete_(I)          mln::trait::concrete< I >::ret


namespace mln
{

  namespace trait
  {

    /*!
      \brief Compute the concrete type of an image type.

      \tparam I An image type.

      This macro is used to compute the most appropriate concrete image
      type to be used to store a copy of an image of type I.

      A concrete image type is an image type storing all its site
      values in a single memory buffer. For instance, mln::image1d,
      mln::image2d, mln::image3d are considered as concrete types.
      Image morphers are not concrete types since they can alter, on
      the fly, the domain and the values.

      This macro is usually used while writing generic algorithms to
      handle all possible cases while image morphers and concrete images
      are interacting.

      Few examples:

      \code
      mln_concrete(mln::image2d<int>)  => mln::image2d<int>
      mln_concrete(mln::extended<mln::image2d<int> >) => mln::image2d<int>
      \endcode

      Therefore, you can safely write the following code:
      \code
      template<typename I>
      mln_concrete(I) my_function(const Image<I>& ima)
      {
        mln_ch_value(I,bool) output;
        ...
	return output;
      }

      int main()
      {
        image2d<int> ima;
	image2d<int> result = my_function(ima);
      }
      \endcode

      Computing a type with trait::concrete can be done using macros
      mln_concrete() and mln_concrete_().

      Note that, due to C++ syntax ambiguities, mln_concrete() should
      be used in a templated function whereas mln_concrete_() shoud be
      used in a non-templated function.

      If you are not sure of the concrete type that should be returned
      by the macro, take a look at mln_var() which can guess it for you.
    */
    template <typename I>
    struct concrete
    {
      typedef typename I::value value;
      typedef typename ch_value<I, value>::ret ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_CONCRETE_HH
