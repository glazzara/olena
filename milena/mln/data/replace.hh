// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_REPLACE_HH
# define MLN_DATA_REPLACE_HH

/// \file
///
/// Replace the contents of an image into another one.

# include <mln/core/concept/image.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <mln/data/fill.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>


namespace mln
{

  namespace data
  {

    /*! \brief Replace \p old_value by \p new_value in the image \p
     *  input
     *
     * \param[in] input The input image.
     * \param[in] old_value The value to be replaced...
     * \param[in] new_value ...by this one.
     *
     * \ingroup mlndata
     */
    template <typename I>
    void replace(Image<I>& input,
		 const mln_value(I)& old_value, const mln_value(I)& new_value);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	inline
	void replace_(Image<I>& input_, const mln_value(I)& old_value,
		      const mln_value(I)& new_value)
	{
	  trace::entering("data::impl::generic::replace");
	  I& input = exact(input_);

	  data::fill((input | (pw::value(input) == pw::cst(old_value))).rw(),
		      new_value);

	  trace::exiting("data::impl::generic::replace");
	}

      } // end of namespace mln::data::impl::generic

    } // end of namespace mln::data::impl



    // Facade.
    template <typename I>
    void replace(Image<I>& input,
		 const mln_value(I)& old_value, const mln_value(I)& new_value)
    {
      trace::entering("data::replace");

      mln_precondition(exact(input).is_valid());

      impl::generic::replace_<I>(exact(input), old_value, new_value);

      trace::exiting("data::replace");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_REPLACE_HH
