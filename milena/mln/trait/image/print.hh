// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_TRAIT_IMAGE_PRINT_HH
# define MLN_TRAIT_IMAGE_PRINT_HH

/*! \file mln/trait/image/print.hh
 *
 * \brief Print the collection of traits for an image type.
 */

# include <iostream>
# include <mln/trait/images.hh>
# include <mln/metal/is_a.hh>



namespace mln
{

  // Fwd decl.
  template <typename E> struct Image;


  namespace trait
  {

    namespace image
    {

      template <typename I>
      void print(std::ostream& ostr);

      template <typename I>
      void print(const Image<I>& ima, std::ostream& ostr);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void print(std::ostream& ostr)
      {
	mlc_is_a(I, Image)::check();
	typedef mln::trait::image_<I> the;
	ostr << "{ "
	     << typename the::category().name() << ", "
	     << typename the::speed().name() << ", "
	     << typename the::size().name() << ", "
	     << typename the::value_access().name() << ", "
	     << typename the::value_storage().name() << ", "
	     << typename the::value_browsing().name() << ", "
	     << typename the::value_io().name() << ", "
	     << typename the::localization().name() << ", "
	     << typename the::dimension().name() << ", "
	     << typename the::ext_domain().name() << ", "
	     << typename the::ext_value().name() << ", "
	     << typename the::ext_io().name() << ", "
	     << typename the::kind().name() << ", "
	     << typename the::nature().name() << ", "
	     << typename the::quant().name() << " }" << std::endl;
      }

      template <typename I>
      inline
      void print(const Image<I>&, std::ostream& ostr)
      {
	print<I>(ostr);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::trait::image

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_IMAGE_PRINT_HH
