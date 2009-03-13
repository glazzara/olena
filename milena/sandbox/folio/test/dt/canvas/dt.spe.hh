/*!
 * \file   test_psn.spe.hh
 * \author ornthalas <ornthalas@gmail.com>
 */

#ifndef DT_SPE_HH
# define DT_SPE_HH

# ifndef DT_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! DT_HH

# include "canevas_dt.hh"

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


      // Fwd decl of the Generic version.

      namespace generic
      {

	template<typename I, typename N, typename L>
	inline
	mln_ch_value(I, L)
	dt_(const Image<I>& input_, const N& nbh, const unsigned max);

      } // end of namespace mln::dt::impl::generic


      // Fastest functor.

      template <typename I_, typename N_, typename L_>
      struct dt_fasfunctor
      {
	typedef I_ I;
	typedef N_ N;
	typedef L_ L;

	const I& input;
	const N& nbh;
	unsigned max;

	dt_fasfunctor(const I_& input, const N_& nbh, const unsigned max)
	  : input(input),
	    nbh(nbh),
	    max(max)
	{}

	void init() {}
      };


      // Fastest implementation.

      namespace fastest
      {

	template<typename I, typename N, typename L>
	inline
	mln_ch_value(I, L)
        dt_(const I& input, const N& nbh, const unsigned max)
	{
	  trace::entering("dt::impl::dt_fas");

	  typedef dt_fasfunctor<I,N,L> F;
	  F f(input, nbh, max);
	  canvas::dt_fastest<F> run(f);
b
	  trace::exiting("dt::impl::dt_fas");
	  return run.output;
	}

      } // end of namespace mln::dt::impl::fastest


      // Disjunction between "fastest" and "not fastest".

      template<typename I, typename N, typename L>
      inline
      mln_ch_value(I, L)
      dt_(trait::image::speed::any,
	  const I& input, const N& nbh, const unsigned max)
      {
	return generic::dt_(input, nbh, max);
      }

      template<typename I, typename N, typename L>
      inline
      mln_ch_value(I, L)
      dt_(trait::image::speed::fastest,
	  const I& input, const N& nbh, const unsigned max)
      {
	return fastest::dt_(input, nbh, max);
      }


    } // end of namespace mln::dt::impl


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln


#endif // !DT_SPE_HH
