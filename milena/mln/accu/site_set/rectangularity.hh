// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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


#ifndef MLN_ACCU_SITE_SET_RECTANGULARITY_HH
# define MLN_ACCU_SITE_SET_RECTANGULARITY_HH

/// \file mln/accu/site_set/rectangularity.hh
///
/// Compute the rectangularity of a site set.

# include <mln/accu/internal/couple.hh>
# include <mln/accu/bbox.hh>
# include <mln/accu/count.hh>

namespace mln
{

  namespace accu
  {

    namespace site_set
    {

      /// \brief Compute the rectangularity of a site set.
      ///
      /// \ingroup modaccusiteset
      //
      template <typename P>
      class rectangularity
	: public accu::internal::couple<accu::bbox<P>,
					accu::count<P>,
					float,
					rectangularity<P> >
      {

      public:

	typedef accu::internal::couple<accu::bbox<P>,
				       accu::count<P>,
				       float,
				       rectangularity<P> > super_;

	typedef accu::bbox<P> A1;
	typedef accu::count<P> A2;

	typedef float result;

	/// Constructor
	rectangularity();

	/// Return the site set bounding box.
	mln_result(A1) bbox() const;
	/// Return the site set area.
	mln_result(A2) area() const;

	/// Return the rectangularity value.
	result to_result() const;

      protected:
	using super_::a1_;
	using super_::a2_;

      };

# ifndef MLN_INCLUDE_ONLY

      template <typename P>
      inline
      rectangularity<P>::rectangularity()
      {
      }

      template <typename P>
      inline
      mln_result(rectangularity<P>::A1)
      rectangularity<P>::bbox() const
      {
	mln_precondition(a1_.is_valid());
	return a1_.to_result();
      }

      template <typename P>
      inline
      mln_result(rectangularity<P>::A2)
      rectangularity<P>::area() const
      {
	mln_precondition(a2_.is_valid());
	return a2_.to_result();
      }

      template <typename P>
      inline
      mln_result(rectangularity<P>)
      rectangularity<P>::to_result() const
      {
	mln_precondition(this->is_valid());
	// Force division return type.
	return static_cast<result>(a2_.to_result()) / a1_.to_result().nsites();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::site_set

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_ACCU_SITE_SET_RECTANGULARITY_HH
