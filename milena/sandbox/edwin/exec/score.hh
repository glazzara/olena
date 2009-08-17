#ifndef SCORE_HH_
# define SCORE_HH_

#include <mln/accu/internal/base.hh>
#include <mln/accu/stat/variance.hh>

#include <mln/value/int_u8.hh>

namespace mln
{

  namespace morpho
  {

    namespace attribute
    {
      // forward
      template <typename I>
      struct score_word;

      namespace internal
      {

	/// \param I An edge image.
	/// \param E Exact type.
	///
	/// FIXME: how to staticaly check that I is an edge_image which is not a concept.
	template <typename I, typename E>
	struct score_base : public mln::accu::internal::base< float, E >
	{
	  typedef mln_psite(I) argument;

	  score_base();

	  score_base(const mln_ch_value(I, value::int_u8)&	dist_color,
		     const mln_ch_value(I, unsigned)&		dist_geo,
		     const mln_ch_value(I, float)&		orientation);

	  void init();
	  void take_as_init(const argument& t);
	  void take(const argument& t);
	  void take(const E& other);

	  unsigned count() const;

	  bool is_valid() const;

	  operator float () const;

	protected:
	  const mln_ch_value(I, value::int_u8)*	dist_color_;
	  const mln_ch_value(I, unsigned)*	dist_geo_;
	  const mln_ch_value(I, float)*		orientation_; // [-Pi/2, Pi/2]

	  accu::stat::variance<value::int_u8>	var_color_;
	  accu::stat::variance<unsigned>	var_geo_;
	  accu::stat::variance<float>		var_orientation_;

	  unsigned				count_;
	};

      } // end of namespace mln::morpho::attribute::internal

    } // end of namespace mln::morpho::attribute

  }  // end of namespace mln::morpho

  // Traits.
  namespace trait
  {

    template <typename I, typename E>
    struct accumulator_< morpho::attribute::internal::score_base<I, E> >
    {
      typedef accumulator::has_untake::no     has_untake;
      typedef accumulator::has_set_value::no  has_set_value;
      typedef accumulator::has_stop::no       has_stop;
      typedef accumulator::when_pix::use_p    when_pix;
    };

    template <typename I>
    struct accumulator_ < morpho::attribute::score_word<I> > :
      accumulator_< morpho::attribute::internal::score_base< I, morpho::attribute::score_word<I> > >
    {
      typedef accumulator_< morpho::attribute::internal::score_base< I, morpho::attribute::score_word<I> > > super;
      using super::has_untake;
      using super::has_set_value;
      using super::has_stop;
      using super::when_pix;
    };

  } // end of namespace mln::trait


  namespace morpho
  {

    namespace attribute
    {

      template <typename I>
      struct score_word : internal::score_base< I, score_word<I> >
      {
	typedef float result;

	// coef
	static const float c_dist_geo = 1;
	static const float c_dist_geo_var = 1;
	static const float c_dist_color = 1;
	static const float c_dist_color_var = 1;
	static const float c_orientation = 1;
	static const float c_orientation_var = 1;

	score_word();

	score_word(const mln_ch_value(I, value::int_u8)&	dist_color,
		   const mln_ch_value(I, unsigned)&		dist_geo,
		   const mln_ch_value(I, float)&		orientation);

	// Word caracterization::
	// * short distance between letters + equidistance:
	//    -> min distance and min variance
	//    -> To Fix: letters that have been merged ??
	// * letters have the same color
	//    -> short color distance + short variance
	// * letters mass centers are aligned
	//    -> orientation: 0 + short variance
	result to_result() const;

	operator result () const
	{
	  return to_result();
	}

      protected:
	using internal::score_base< I, score_word<I> >::var_geo_;
	using internal::score_base< I, score_word<I> >::var_color_;
	using internal::score_base< I, score_word<I> >::var_orientation_;

      };


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	template <typename I, typename E>
	inline
	score_base<I, E>::score_base(const mln_ch_value(I, value::int_u8)&	dist_color,
				     const mln_ch_value(I, unsigned)&		dist_geo,
				     const mln_ch_value(I, float)&		orientation) :
	  dist_color_ (&dist_color),
	  dist_geo_ (&dist_geo),
	  orientation_ (&orientation)
	{
	}

	template <typename I, typename E>
	inline
	score_base<I, E>::score_base() :
	  dist_color_ (0),
	  dist_geo_ (0),
	  orientation_ (0)
	{
	}

	template <typename I, typename E>
	inline
	void
	score_base<I, E>::init()
	{
	  count_ = 0;
	  var_color_.init();
	  var_geo_.init();
	  var_orientation_.init();
	}

	template <typename I, typename E>
	inline
	void
	score_base<I, E>::take_as_init(const mln_psite(I)& t)
	{
	  init();
	  take(t);
	}

	template <typename I, typename E>
	inline
	void
	score_base<I, E>::take(const mln_psite(I)& t)
	{
	  ++count_;
	  var_color_.take((*dist_color_)(t));
	  var_geo_.take((*dist_geo_)(t));
	  var_orientation_.take((*orientation_)(t));
	}

	template <typename I, typename E>
	inline
	void
	score_base<I, E>::take(const E& other)
	{
	  count_ += other.count_;
	  var_color_.take(other.var_color_);
	  var_geo_.take(other.var_geo_);
	  var_orientation_.take(other.var_orientation_);
	}

	template <typename I, typename E>
	inline
	unsigned
	score_base<I, E>::count() const
	{
	  return count_;
	}

	template <typename I, typename E>
	inline
	bool
	score_base<I, E>::is_valid() const
	{
	  return (dist_geo_ && dist_color_ && orientation_ &&
		  var_color_.is_valid() &&
		  var_geo_.is_valid() &&
		  var_orientation_.is_valid());
	}

	template <typename I, typename E>
	inline
	score_base<I, E>::operator float () const
	{
	  return this->to_result();
	}

      } // end of namespace mln::morpho::attribute::internal

      template <typename I>
      inline
      score_word<I>::score_word(const mln_ch_value(I, value::int_u8)&	dist_color,
				const mln_ch_value(I, unsigned)&	dist_geo,
				const mln_ch_value(I, float)&		orientation) :
	internal::score_base< I, score_word<I> >(dist_color, dist_geo, orientation)
      {
      }

      template <typename I>
      inline
      score_word<I>::score_word()
      {
      }

      template <typename I>
      inline
      float
      score_word<I>::to_result() const
	{
	  // Word caracterization::
	  // * short distance between letters + equidistance:
	  //    -> min distance and min variance
	  //    -> To Fix: letters that have been merged ??
	  // * letters have the same color
	  //    -> short color distance + short variance
	  // * letters mass centers are aligned
	  //    -> orientation: 0 + short variance

	  float r = 0;
	  r += var_geo_.mean() + var_geo_.to_result();
	  r += var_color_.mean() + var_color_.to_result();
	  r += var_orientation_.mean() + var_orientation_.to_result();

	  mln_postcondition(r >= 0);
	  return -r;
	}


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln

#endif /* !SCORE_HH_ */
