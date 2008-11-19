// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_ROUTINE_INITIALIZE_HH
# define MLN_CORE_ROUTINE_INITIALIZE_HH

/*! \file mln/core/routine/initialize.hh
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
   * \pre (not target.has_data) and model.has_data
   */
  template <typename I, typename J>
  void initialize(Image<I>& target, const Image<J>& model);


# ifndef MLN_INCLUDE_ONLY


  template <typename I, typename J>
  inline
  void initialize(Image<I>& target, const Image<J>& model)
  {
    trace::entering("core::initialize");
    mln_precondition(! exact(target).has_data());
    mln_precondition(exact(model).has_data());

    trace::stop();
    init_(tag::image, exact(target), exact(model));
    trace::resume();

    mln_postcondition(exact(target).has_data());
    trace::exiting("core::initialize");
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_ROUTINE_INITIALIZE_HH
