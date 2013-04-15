// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ROUTINE_INITIALIZE_HH
# define MLN_CORE_ROUTINE_INITIALIZE_HH

/*! \file
 *
 * \brief Definition of function that initialize an image from another
 * one.
 */

# include <mln/core/concept/image.hh>

namespace mln
{

  /*! Initialize the image \p target with data extracted from image \p model.
   *
   * \param[in, out] target The image to be initialized.
   * \param[in] model The image to provide data for the initialization.
   *
   * \pre (not target.is_valid) and model.is_valid
   */
  template <typename I, typename J>
  void initialize(Image<I>& target, const Image<J>& model);


# ifndef MLN_INCLUDE_ONLY


  template <typename I, typename J>
  inline
  void initialize(Image<I>& target, const Image<J>& model)
  {
    mln_trace("core::initialize");
    mln_precondition(! exact(target).is_valid());
    mln_precondition(exact(model).is_valid());

    debug::trace::stop();
    init_(tag::image, exact(target), exact(model));
    debug::trace::resume();

    mln_postcondition(exact(target).is_valid());
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_ROUTINE_INITIALIZE_HH
