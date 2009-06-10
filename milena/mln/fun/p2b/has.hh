// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_P2B_HAS_HH
# define MLN_FUN_P2B_HAS_HH

/// \file
///
/// Function "psite-> bool" that maps the call "ima.has(p)".


# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/metal/const.hh>
# include <mln/metal/unconst.hh>


namespace mln
{

  namespace fun
  {

    namespace p2b
    {

      // FIXME: Doc!

      template <typename I>
      struct has

	: public Function_v2b< has<I> >,
	  private mlc_is_a(I, Image)::check_t
      {
	/// Result associated type.
	typedef bool result;

	/// Apply the function on the psite \p p.
	bool operator()(const mln_psite(I)& p) const;


	/// Constructor without argument.
	has();

	/// Constructor from an image \p ima.
	has(mlc_const(I)& ima);


	/// Copy constructor.
	has(const has<I>& rhs);

	/// Assignment operator.
	has<I>& operator=(const has<I>& rhs);

      private:
	mlc_const(I) ima_;
	typedef Function_v2b< has<I> > super_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      bool
      has<I>::operator()(const mln_psite(I)& p) const
      {
	return ima_.has(p);
      }

      template <typename I>
      inline
      has<I>::has()
	: ima_()
      {
      }

      template <typename I>
      inline
      has<I>::has(mlc_const(I)& ima)
	: ima_(ima)
      {
      }

      template <typename I>
      inline
      has<I>::has(const has<I>& rhs)
	: Function_v2v< has<I> >(), // Please g++.
	  super_(),
	  ima_(rhs.ima_)
      {
      }

      template <typename I>
      inline
      has<I>&
      has<I>::operator=(const has<I>& rhs)
      {
	if (&rhs == this)
	  return *this;
	const_cast<mlc_unconst(I)&>(this->ima_) = rhs.ima_;
	return *this;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2B_HAS_HH
