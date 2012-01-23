// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_CONVERT_HH
# define MLN_DATA_CONVERT_HH

/// \file
///
/// Convert the contents of an image into another one.
///
/// \todo Re-write doc.

# include <mln/core/routine/duplicate.hh>
# include <mln/fun/v2v/convert.hh>
# include <mln/data/transform.hh>


namespace mln
{

  namespace data
  {

    /*! \brief Convert the image \p input by changing the value type.
     *
     * \param[in] v A value of the destination type.
     * \param[in] input The input image.
     *
     * \ingroup mlndata
     */
    template <typename V, typename I>
    mln_ch_value(I, V)
    convert(const V& v, const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename V, typename I>
      inline
      void
      convert_tests(const V&, const Image<I>& input)
      {
	mln_precondition(exact(input).is_valid());
	(void) input;
      }

    } // using namespace mln::data::internal

    namespace impl
    {

      namespace generic
      {

        template <typename V, typename I>
        inline
	mln_ch_value(I, V)
	convert(const V& v, const Image<I>& input)
	{
	  trace::entering("data::impl::generic::convert");
	  internal::convert_tests(v, input);

	  fun::v2v::convert<V> f;
	  mln_ch_value(I, V) output = data::transform(input, f);

	  trace::exiting("data::impl::generic::convert");
	  return output;
	}

      } // end of namespace mln::data::impl::generic


      template <typename V, typename I>
      inline
      mln_ch_value(I,V)
      convert_identity(const V& v, const Image<I>& input)
      {
	trace::entering("data::impl::convert_identity");
	internal::convert_tests(v, input);

	mln_concrete(I) output = duplicate(input);

	trace::exiting("data::impl::convert_identity");
	return output;
      }


    } // end of namespace mln::data::impl

    namespace internal
    {

      template <typename V, typename I>
      inline
      mln_ch_value(I, V)
      convert_dispatch(metal::bool_<true>,
		       const V& v, const Image<I>& input)
      {
	return impl::convert_identity(v, input);
      }

      template <typename V, typename I>
      inline
      mln_ch_value(I, V)
      convert_dispatch(metal::bool_<false>,
		       const V& v, const Image<I>& input)
      {
	return impl::generic::convert(v, input);
      }

      template <typename V, typename I>
      inline
      mln_ch_value(I, V)
      convert_dispatch(const V& v, const Image<I>& input)
      {
	enum {
	  test = mlc_equal(V, mln_value(I))::value
	};
	return convert_dispatch(metal::bool_<test>(),
				v, input);
      }

    } // end of namespace mln::data::internal

    // Facade.

    template <typename V, typename I>
    inline
    mln_ch_value(I, V)
    convert(const V& v, const Image<I>& input)
    {
      trace::entering("data::convert");

      internal::convert_tests(v, input);

      mln_ch_value(I, V) output = internal::convert_dispatch(v, input);

      trace::exiting("data::convert");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_CONVERT_HH
