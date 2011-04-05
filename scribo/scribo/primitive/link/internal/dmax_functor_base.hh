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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_DMAX_FUNCTOR_BASE_HH
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_DMAX_FUNCTOR_BASE_HH

/// \file
///
/// Base class for dmax functors.

# include <mln/core/alias/box2d.hh>

# include <mln/math/max.hh>

# include <scribo/core/concept/dmax_functor.hh>

namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {

	using namespace mln;


	/// \brief Base class for dmax functors.
	template <typename E>
	class dmax_functor_base : public DMax_Functor<E>
	{

	public:
	  dmax_functor_base(float dmax_factor);
	  float operator()(const box2d& b) const;

	protected:
	  float dmax_factor_;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename E>
	dmax_functor_base<E>::dmax_functor_base(float dmax_factor)
	  : dmax_factor_(dmax_factor)
	{
	}


	template <typename E>
	float
	dmax_functor_base<E>::operator()(const box2d& b) const
	{
	  return exact(this)->compute_(b);
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_LINK_INTERNAL_DMAX_FUNCTOR_BASE_HH
