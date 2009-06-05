# include <mln/accu/stat/max.hh>
# include <mln/accu/stat/min_max.hh>
# include <mln/labeling/regional_minima.hh>

# include <mln/core/site_set/p_queue_fast.hh>


namespace mln
{

  namespace cmorpho
  {

    template <typename T>
    T abs_diff(T t1, T t2)
    {
      return t1 > t2 ? t1 - t2 : t2 - t1;
    }

    template <typename I>
    void gradient(I& input)
    {
      mln_piter(I) p(input.domain());
      for_all(p)
	{
	  mln_psite(I) p_ = p;
	  input(p) =
	    p_.row() % 2 ?
	    // Odd so horizontal edge and vertical cells.
	    abs_diff(input(p_ + left), input(p_ + right)) :
	    // Otherwise
	    abs_diff(input(p_ + up), input(p_ + down));
	}
    }


    template <typename I, typename N>
    inline
    mln_concrete(I) gradient(const Image<I>& input_,
			     const Neighborhood<N>& nbh_)
    {
      const I& input = exact(input_);
      const N& nbh   = exact(nbh_);

      mln_concrete(I) output;
      initialize(output, input);

      accu::stat::min_max<mln_value(I)> m;
	
      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
	{
	  m.init();
	  for_all(n) if (input.has(n))
	    m.take(input(n));
	  output(p) = m.second() - m.first();
	}
      return output;
    }


    class dbl_niter;

    struct dbl_neighb2d : public Neighborhood< dbl_neighb2d >
    {
      typedef dpoint2d dpoint;
      typedef point2d point;

      typedef dbl_niter fwd_niter;
      typedef dbl_niter bkd_niter;
      typedef dbl_niter niter;

      dbl_neighb2d()
      {
      }

      dbl_neighb2d& insert_odd(const dpoint2d& dp)
      {
	odds.push_back(dp);
	odds.push_back(-dp);
	return *this;
      }
      dbl_neighb2d& insert_even(const dpoint2d& dp)
      {
	evens.push_back(dp);
	evens.push_back(-dp);
	return *this;
      }

      std::vector<dpoint2d> odds, evens;
    };


    class dbl_niter 
      : public internal::point_iterator_base_< point2d, dbl_niter >
    {
    public:

      typedef dpoint2d D;

      template <typename N, typename Pref>
      dbl_niter(const N& nbh, const Point_Site<Pref>& p_ref) :
	dps_odd(exact(nbh).odds),
	dps_even(exact(nbh).evens),
	p_ref_(exact(p_ref).to_point())
      {

	invalidate();
      }
      
      /// Conversion to point.
      operator point2d () const
      {
	mln_precondition(is_valid());
	return p_;
      }

      /// Reference to the corresponding point.
      const point2d& to_point() const
      {
	return p_;
      }

      /// Test the iterator validity.
      bool is_valid() const
      {
	return i_ != dps_odd.size();
      }

      /// Invalidate the iterator.
      void invalidate()
      {
	i_ = dps_odd.size();
      }

      /// Start an iteration.
      void start()
      {
	i_ = 0;
	update();
      }

      /// Go to the next point.
      void next_()
      {
	++i_;
	update();
      }

      /// Give the i-th coordinate.
      int operator[](unsigned i) const
      {
	mln_precondition(is_valid());
	return p_[i];
      }

      /// The point around which this iterator moves.
      const point2d& center_point() const
      {
	return p_ref_;
      }

      /// Force this iterator to update its location to take into
      /// account that its center point may have moved.
      void update()
      {
	if (is_valid())
	  p_ = p_ref_ + (p_ref_.row() % 2 ? dps_odd[i_] : dps_even[i_]);
      }

    protected:

      const std::vector<dpoint2d>& dps_odd, dps_even;
      const point2d& p_ref_; // reference point (or "center point")

      unsigned i_;
      point2d p_; // location of this iterator; p_ makes this iterator be
      // itself a potential center point.
    };


    template <typename L, typename I, typename N>
    mln_ch_value(I, L)
      meyer_wst(const Image<I>& input_, const Neighborhood<N>& nbh_,
		L& nbasins)
    {
      trace::entering("morpho::meyer_wst");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      
      /* FIXME: Ensure the input image has scalar values.  */

      const L unmarked = 0;

      // Initialize the output with the markers (minima components).
      mln_ch_value(I, L) output;
      output = labeling::regional_minima(input, nbh, nbasins);

      // Ordered queue.
      std::vector< p_queue_fast<mln_psite(I)> > qu(256);
      unsigned qu_size = 0;

      // Insert every neighbor P of every marked area in a
      // hierarchical queue, with a priority level corresponding to
      // the grey level input(P).
      {
	mln_piter(I) p(output.domain());
	mln_niter(N) n(nbh, p);
	for_all(p)
	  if (output(p) == unmarked)
	    for_all(n)
	      if (output.has(n) && output(n) != unmarked)
		{
		  qu[input(p)].push_force(p);
		  ++qu_size;
		  break;
		}
      }

      /* Until the queue is empty, extract a psite p from the
         hierarchical queue, at the highest priority level, that is,
         the lowest level.  */
      mln_psite(I) p;
      mln_niter(N) n(nbh, p);

      while (qu_size != 0)
	{

	  for (unsigned l = 0; l < 256; ++l)
	    if (! qu[l].is_empty())
	      {
		p = qu[l].pop_front();
		// qu[l].pop();
		break;
	      }
	  --qu_size;

	  // Last seen marker adjacent to P.
	  L adjacent_marker = unmarked;
	  // Has P a single adjacent marker?
	  bool single_adjacent_marker_p = true;
	  for_all(n)
	    if (output.has(n) && output(n) != unmarked)
	      {
		if (adjacent_marker == unmarked)
		  {
		    adjacent_marker = output(n);
		    single_adjacent_marker_p = true;
		  }
		else
		  if (adjacent_marker != output(n))
		    {
		      single_adjacent_marker_p = false;
		      break;
		    }
	      }

	  /* If the neighborhood of P contains only psites with the
	     same label, then P is marked with this label, and its
	     neighbors that are not yet marked are put into the
	     hierarchical queue.  */
	  if (single_adjacent_marker_p)
	    {
	      output(p) = adjacent_marker;
	      for_all(n)
		if (output.has(n) && output(n) == unmarked)
		  {
		    qu[input(n)].push_force(n);
		    ++qu_size;
		  }
	    }
	}

      trace::exiting("morpho::meyer_wst");
      return output;
    }

  } // mln::cmorpho


  namespace accu
  {

    template <typename A_, typename I, typename L>
    inline
    std::vector<mln_accu_with(A_, mln_value(I))::result>
    compute(const Image<I>& input_,
	    const Image<L>& label_,
	    mln_value(L) n)
    {
      mlc_is_a(A_, Meta_Accumulator)::check();      

      trace::entering("accu::compute");

      const I& input = exact(input_);
      const L& label = exact(label_);

      typedef mln_accu_with(A_, mln_value(I)) A;
      std::vector<A> a(n + 1);

      mln_piter(I) p(input.domain());
      for_all(p)
	a[label(p)].take(input(p));

      typedef typename A::result R;
      std::vector<R> v(n + 1);
      for (unsigned l = 1; l <= n; ++l)
	v[l] = a[l].to_result();

      trace::exiting("accu::compute");
      return v;
    }


    template <typename A_, typename I, typename L, typename R>
    inline
    void
    compute(const Image<I>& input_,
	    const Image<L>& label_,
	    std::vector<R>& v)
    {
      mlc_is_a(A_, Meta_Accumulator)::check();      

      trace::entering("accu::compute");

      const I& input = exact(input_);
      const L& label = exact(label_);

      typedef mln_accu_with(A_, mln_value(I)) A;
      const unsigned n = v.size();
      std::vector<A> a(n);

      mln_piter(I) p(input.domain());
      for_all(p)
	a[label(p)].take(input(p));

      for (unsigned l = 1; l < n; ++l)
	v[l] = a[l].to_result();

      trace::exiting("accu::compute");
    }

  } // mln::accu


  namespace data
  {

    template<typename I, typename R>
    mln_ch_value(I, R)
      transform(const Image<I>& input_, const std::vector<R>& v)
    {
      trace::entering("data::transform");
      const I& input = exact(input_);

      mln_ch_value(I, R) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      for_all(p)
	output(p) = v[input(p)];

      trace::exiting("data::transform");
      return output;
    }

  } // mln::data


} // mln
