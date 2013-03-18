// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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

#ifndef MLN_LABELING_SUPERPOSE_HH
# define MLN_LABELING_SUPERPOSE_HH

/// \file
///
/// \brief Superpose two labeled images.


# include <mln/core/concept/image.hh>
# include <mln/util/array.hh>
# include <mln/labeling/relabel.hh>
# include <mln/data/paste.hh>
# include <mln/pw/all.hh>

# include <mln/value/next.hh>
# include <mln/value/equiv.hh>

namespace mln
{

  namespace labeling
  {


    /*!
      \brief Superpose two labeled image.

      Labels in \p lhs are preserved in the output. Labels of \p rhs
      are renumbered from the last label value of \p lhs. It avoids
      duplicate label values in several components.

      \param[in] lhs          A labeled image.
      \param[in] lhs_nlabels  The number of labels in \p lhs.
      \param[in] rhs          A labeled image.
      \param[in] rhs_nlabels  The number of labels in \p rhs.
      \param[out] new_nlabels The number of labels in the output image.

      \return An image with all the components of \p rhs and \p lhs.

      @pre \p rhs and lhs must have the same domain.
      @pre \p The value type of rhs must be convertible towards lhs's.

     */
    template <typename I, typename J>
    mln_concrete(I)
    superpose(const Image<I>& lhs, const mln_value(I)& lhs_nlabels,
	      const Image<J>& rhs, const mln_value(J)& rhs_nlabels,
	      mln_value(I)& new_nlabels);




# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename J>
    mln_concrete(I)
    superpose(const Image<I>& lhs_, const mln_value(I)& lhs_nlabels,
	      const Image<J>& rhs_, const mln_value(J)& rhs_nlabels,
	      mln_value(I)& new_nlabels)
    {
      mln_trace("mln::labeling::superpose");

      const I& lhs = exact(lhs_);
      const J& rhs = exact(rhs_);

      mlc_converts_to(mln_value(I), mln_value(J))::check();
      mln_precondition(lhs.is_valid());
      mln_precondition(rhs.is_valid());
      mln_precondition(lhs.domain() == rhs.domain());
      /// Check if there are enough free labels?

      mln_concrete(I) output = duplicate(lhs);

      util::array<mln_value(J)>
	rebase_lbl(value::next(rhs_nlabels));

      rebase_lbl(0) = 0;
      for (unsigned i = 1; i <= rhs_nlabels; ++i)
	rebase_lbl(i) = i + value::equiv(lhs_nlabels);

      data::paste(labeling::relabel(rhs, rhs_nlabels, rebase_lbl)
		  | (pw::value(rhs) != pw::cst(literal::zero)),
		  output);

      new_nlabels = value::equiv(lhs_nlabels)
	            + value::equiv(rhs_nlabels) + 1;

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_SUPERPOSE_HH
