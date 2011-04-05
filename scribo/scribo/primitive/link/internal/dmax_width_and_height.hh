// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_DMAX_WIDTH_AND_HEIGHT_HH
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_DMAX_WIDTH_AND_HEIGHT_HH

/// \file
///
/// Dmax functor relying on width only.
///
/// For newspapers like ICDAR documents, use dmax_factor = 1.

# include <scribo/primitive/link/internal/dmax_functor_base.hh>


namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {


	/// \brief Base class for dmax functors.
	class dmax_width_and_height : public dmax_functor_base<dmax_width_and_height>
	{
	  typedef dmax_functor_base<dmax_width_and_height> super_;


	public:
	  dmax_width_and_height(float dmax_factor);
	  float compute_(const box2d&) const;

	protected:
	  using super_::dmax_factor_;
	};


# ifndef MLN_INCLUDE_ONLY


	inline
	dmax_width_and_height::dmax_width_and_height(float dmax_factor)
	  : super_(dmax_factor)
	{
	}


	inline
	float
	dmax_width_and_height::compute_(const box2d& b) const
	{
	  float
	    w = b.width(),
	    h = b.height();

	  return (w / 2.0f) + dmax_factor_ * (h + w);
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_LINK_INTERNAL_DMAX_WIDTH_AND_HEIGHT_HH
