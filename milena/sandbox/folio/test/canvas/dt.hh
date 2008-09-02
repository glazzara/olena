/*!
 * \file   psn.cc
 * \author ornthalas <ornthalas@gmail.com>
 */

#ifndef DT_HH
# define DT_HH

// The 'fastest' specialization is in:
# include "dt.spe.hh"

namespace mln
{

  namespace dt
  {

    template<typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    dt(const Image<I>& input_, const N& nbh, const unsigned max);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic functor.

      template <typename I_, typename N_, typename L_>
      struct dt_functor
      {
	typedef I_ I;
	typedef N_ N;
	typedef L_ L;

	const I& input;
	const N& nbh;
	unsigned max;

	dt_functor(const I_& input, const N_& nbh, const unsigned max)
	  : input(input),
	    nbh(nbh),
	    max(max)
	{}

	void init() {}
      };


      // Generic implementation.

      namespace generic
      {

	template<typename I, typename N, typename L>
	inline
	mln_ch_value(I, L)
	dt_(const Image<I>& input, const N& nbh, const unsigned max)
	{
	  trace::entering("dt::impl::generic::dt_");

	  typedef dt_functor<I, N, L> F;
	  F f(input, nbh, max);
	  canvas::dt<F> run(f);

	  trace::exiting("dt::impl::generic::dt_");
	  return run.output;
	}

      } // end of namespace mln::dt::impl::generic

    } // end of namespace mln::dt::impl


    // Facade.

    template<typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    dt(const Image<I>& input, const N& nbh, const unsigned max)
    {
      trace::entering("dt::dt");
      mln_precondition(exact(input).has_data());

      mln_ch_value(I, L) output =
	impl::dt_(mln_trait_image_speed(I)(),
		  exact(input), nbh, max);

      trace::exiting("dt::dt");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln

#endif // !DT_HH
