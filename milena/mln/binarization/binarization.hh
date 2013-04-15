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

#ifndef MLN_BINARIZATION_BINARIZATION_HH
# define MLN_BINARIZATION_BINARIZATION_HH

/// \file
///
/// \brief Threshold an image.
///
/// \todo Rename and/or remove (that is this?)

# include <mln/core/concept/function.hh>
# include <mln/data/transform.hh>


namespace mln
{

  namespace binarization
  {

    /// Thresholds the values of \p input so that they can be stored in
    ///  the \p output binary image.
    ///
    /// \param[in] input The input image.
    /// \param[in] fun The thresholding function, from value(I) to bool.
    ///
    /// for_all(p), output(p) = fun(p)
    template <typename I, typename F>
    inline
    mln_ch_value(I, bool)
    binarization(const Image<I>& input, const Function_v2b<F>& fun);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename F>
      inline
      mln_ch_value(I, bool)
      binarization_(const I& input, const Function_v2b<F>& fun)
      {
	mln_trace("binarization::impl::binarization_");
	mln_ch_value(I, bool) output(input.domain());

	output = data::transform(input, fun);

	return output;
      }

    } // end of namespace mln::binarization::impl



    // Facade

    template <typename I, typename F>
    inline
    mln_ch_value(I, bool)
    binarization(const Image<I>& input, const Function_v2b<F>& fun)
    {
      mln_trace("binarization::binarization");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, bool) output(exact(input).domain());
      output = impl::binarization_(exact(input), fun);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::binarization

} // end of namespace mln


#endif // ! MLN_BINARIZATION_BINARIZATION_HH
