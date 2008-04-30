/*!
 * \file   canevas_dt.hh
 * \author ornthalas <ornthalas@gmail.com>
 */

#ifndef CANEVAS_DT_HH
# define CANEVAS_DT_HH

# include <queue>
# include <map>
# include <mln/core/concept/image.hh>
# include <mln/make/w_window.hh>
# include <mln/literal/zero.hh>

# include <mln/core/concept/neighborhood.hh>

namespace mln
{

  namespace canvas
  {

    // General version.

    template <typename F>
    struct dt
    {
      // Functor.
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::L L;
      typedef mln_point(I) point;

      mln_ch_value(I, L) output;

      // Ctor.
      dt(F& f);

      void init();
      void run();

      std::map<unsigned, std::queue<point> > bucket;
      unsigned bucket_size;

      unsigned current_distance;
    };

    template <typename F>
    struct dt_fastest
    {
      // Functor.
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::L L;
      typedef mln_point(I) point;

      mln_ch_value(I, L) output;

      // Ctor.
      dt_fastest(F& f);

      void init();
      void run();

      std::map<unsigned, std::queue<point> > bucket;
      unsigned bucket_size;

      unsigned current_distance;
    };

# ifndef MLN_INCLUDE_ONLY

    /*-------------.
    | canvas::dt.  |
    `-------------*/

    template <typename F>
    dt<F>::dt(F& f)
      : f(f),
	bucket_size(0),
	current_distance(0)
    {
      trace::entering("canvas::dt");

      this->init();
      this->run();

      trace::exiting("canvas::dt");
    }

    template <typename F>
    void
    dt<F>::init()
    {
      this->f.init();
      initialize(this->output, this->f.input);

      mln_fwd_piter(I) p(this->f.input.domain());
      for_all(p)
	if (this->f.input(p))
	{
	  this->output(p) = literal::zero;
	  this->bucket[0].push(p);
	  ++this->bucket_size;
	}
	else
	  this->output(p) = this->max;
    }

    template <typename F>
    void
    dt<F>::run()
    {
      while (this->bucket_size != 0)
      {
	std::queue<point> bucket_d = this->bucket[this->current_distance];
	while (! bucket_d.empty())
	{
	  point p = bucket_d.front();
	  bucket_d.pop();
	  --bucket_size;

	  if (this->output(p) == this->current_distance)
	  {
	    mln_qiter(N) n(this->nbh, p);

	    for_all(n)
	      if (this->output.has(n))
	      {
		unsigned new_out = this->output(p) + n.w();

		if (new_out < this->output(n))
		{
		  this->output(n) = new_out;
		  this->bucket[new_out].push(n);
		  ++this->bucket_size;
		}
	      }
	  }
	}
	this->bucket.erase(this->current_distance);
	++this->current_distance;
      }
    }

    /*---------------------.
    | canvas::dt_fastest.  |
    `---------------------*/
    template <typename F>
    dt_fastest<F>::dt_fastest(F& f)
      : f(f),
	bucket_size(0),
	current_distance(0)
    {
      trace::entering("canvas::dt");

      this->init();
      this->run();

      trace::exiting("canvas::dt");
    }

    template <typename F>
    void
    dt_fastest<F>::init()
    {
      this->f.init();
      initialize(this->output, this->f.input);

      mln_fwd_piter(I) p(this->f.input.domain());
      for_all(p)
	if (this->f.input(p))
	{
	  this->output(p) = literal::zero;
	  this->bucket[0].push(p);
	  ++this->bucket_size;
	}
	else
	  this->output(p) = this->max;
    }

    template <typename F>
    void
    dt_fastest<F>::run()
    {
      while (this->bucket_size != 0)
      {
	std::queue<point> bucket_d = this->bucket[this->current_distance];
	while (! bucket_d.empty())
	{
	  point p = bucket_d.front();
	  bucket_d.pop();
	  --bucket_size;

	  if (this->output(p) == this->current_distance)
	  {
	    mln_qiter(N) n(this->nbh, p);

	    for_all(n)
	      if (this->output.has(n))
	      {
		unsigned new_out = this->output(p) + n.w();

		if (new_out < this->output(n))
		{
		  this->output(n) = new_out;
		  this->bucket[new_out].push(n);
		  ++this->bucket_size;
		}
	      }
	  }
	}
	this->bucket.erase(this->current_distance);
	++this->current_distance;
      }
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_DT_HH
