// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_DATA_COMPUTE_HH
# define MLN_DATA_COMPUTE_HH

/// \file
///
/// Compute an accumulator onto image pixel values.

# include <mln/data/update.hh>
# include <mln/core/concept/meta_accumulator.hh>



namespace mln
{

  namespace data
  {

    /// \brief Compute an accumulator onto the pixel values of the
    /// image \p input.  Be ware that the given accumulator won't be
    /// modified and won't store any result.
    ///
    /// \param[in] a An accumulator.
    /// \param[in] input The input image.
    /// \return The accumulator result.
    ///
    /// It fully relies on data::update.
    //
    template <typename A, typename I>
    mln_result(A)
    compute(const Accumulator<A>& a, const Image<I>& input);


    /// \brief Compute an accumulator onto the pixel values of the
    /// image \p input.
    ///
    /// \param[in, out] a An accumulator.
    /// \param[in] input The input image.
    /// \return The accumulator result.
    ///
    /// It fully relies on data::update.
    //
    template <typename A, typename I>
    mln_result(A)
    compute(Accumulator<A>& a, const Image<I>& input);


    /// \brief Compute an accumulator onto the pixel values of the
    /// image \p input.
    ///
    /// \param[in] a A meta-accumulator.
    /// \param[in] input The input image.
    /// \return The accumulator result.
    ///
    template <typename A, typename I>
    mln_meta_accu_result(A, mln_value(I))
    compute(const Meta_Accumulator<A>& a, const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    // Facades.

    template <typename A, typename I>
    inline
    mln_result(A)
    compute(const Accumulator<A>& a_, const Image<I>& input)
    {
      (void) a_;
      A a;
      return compute(a, input);
    }


    template <typename A, typename I>
    inline
    mln_result(A)
    compute(Accumulator<A>& a_, const Image<I>& input)
    {
      trace::entering("data::compute");

      A a = exact(a_);
      data::internal::update_tests(a, input);

      a.init();
      data::internal::update_dispatch(a, input);

      trace::exiting("data::compute");
      return a;
    }


    template <typename A, typename I>
    inline
    mln_meta_accu_result(A, mln_value(I))
    compute(const Meta_Accumulator<A>& a, const Image<I>& input)
    {
      typedef mln_accu_with(A, mln_value(I)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_value(I)());

      return data::compute(a_, input); // Call the previous version.
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_COMPUTE_HH
