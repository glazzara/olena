
#ifndef LEVEL_HH
# define LEVEL_HH

/*! \file mln/labeling/level.hh
 *
 * \brief Connected component labeling of the image objects at a given
 * level.
 */

# include <mln/labeling/base.hh>
# include <mln/data/fill.hh>
# include "labeling.hh"

namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the image objects at a given
     * level.
     *
     * \param[in]  input The input image.
     * \param[in]  val   The level to consider for the labeling.
     * \param[in]  nbh   The neighborhood.
     * \param[out] output  The label image.
     * \param[out] nlabels The number of labels.
     *
     * \return Succeed or not.
     */
    template <typename I, typename N, typename O>
    bool level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	       Image<O>& output, unsigned& nlabels);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Functors.

      template <typename I_, typename N_, typename O_>
      struct level_t : base_<I_,N_,O_>
      {
	typedef mln_point(I_) P;

	// requirements from mln::canvas::labeling:

	typedef mln_pset(I_) S;
	const S& s;

 	void init()                            { mln::data::fill(this->output, 0); }
	bool handles(const P& p) const         { return input(p) == val; }
 	bool equiv(const P& n, const P&) const { return input(n) == val; }
 
	// end of requirements

	const mln_value(I_)& val;

	level_t(const I_& input, const mln_value(I_)& val, const N_& nbh, O_& output)
	  : base_<I_,N_,O_>(input, nbh, output),
	    s(input.domain()),
	    val(val)
	{}
      };

      // Routines.

      template <typename I, typename N, typename O>
      bool level_(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
		  Image<O>& output, unsigned& nlabels)
      {
	typedef impl::level_t<I,N,O> F;
	F f(exact(input), val, exact(nbh), exact(output));
	canvas::labeling<F> run(f);
	nlabels = f.nlabels;
	return f.status;
      }

      // FIXME: Add fast versions.

      template <typename I, typename N, typename O>
      bool level_(const Fast_Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
		  Fast_Image<O>& output, unsigned& nlabels)
      {
	typedef impl::level_t<I,N,O> F;
	F f(exact(input), val, exact(nbh), exact(output));
	canvas::labeling_fast<F> run(f);
	nlabels = f.nlabels;
	return f.status;
      }

      // END FIX
    } // end of namespace mln::labeling::impl


    // Facade.

    template <typename I, typename N, typename O>
    bool level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	       Image<O>& output, unsigned& nlabels)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      return impl::level_(exact(input), val, nbh, output, nlabels);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! LEVEL_HH
