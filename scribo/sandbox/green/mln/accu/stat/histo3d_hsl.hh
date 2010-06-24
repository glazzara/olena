// Copyright (C) 2007, 2008, 2009, 2010 EPITA LRDE
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

#ifndef MLN_ACCU_STAT_HISTO3D_HSL_HH
# define MLN_ACCU_STAT_HISTO3D_HSL_HH

/// \file
///
/// \brief Define a histogram as accumulator which returns an HSL image3d.
///
/// This source implements the discrete histogram version. It was
/// created for images which the values are derived from integer
/// type. The number of bins is given by the user via the q parameter.
/// It works for HSL space.
///
/// The following sample is a typical use of the histogram.
///
/// #include <mln/accu/stat/histo3d_hsl.hh>
/// #include <mln/core/image/image2d.hh>
/// #include <mln/core/image/image3d.hh>
/// #include <mln/data/compute.hh>
/// #include <mln/data/transform.hh>
/// #include <mln/fun/v2v/rgb_to_hsl.hh>
/// #include <mln/img_path.hh>
/// #include <mln/io/ppm/load.hh>
/// #include <mln/value/hsl.hh>
/// #include <mln/value/rgb8.hh>
///
/// int main()
/// {
///   typedef mln::value::rgb8                    t_rgb;
///   typedef mln::value::hsl_f                   t_hsl;
///   typedef mln::fun::v2v::f_rgb_to_hsl_<t_hsl> t_rgb_to_hsl;
///   mln::image2d<t_rgb>                         img_rgb;
///   mln::image2d<t_hsl>                         img_hsl;
///   mln::image3d<unsigned>                      histo;
///
///   mln::io::ppm::load(img_rgb, OLENA_IMG_PATH"/lena.ppm");
///  img_hsl = mln::data::transform(img_rgb, t_rgb_to_hsl());
///  histo=mln::data::compute(mln::accu::meta::stat::histo3d_hsl<7>(),img_hsl);
///
///  return 0;
/// }
///
/// \fixme This implementation is very doubtfull ! Be carefull !!
/// \fixme This code should be compile, but it doesn't!
/// \fixme The fold interface has changed v2v -> p2p or the inverse.


# include <iostream>

# include <mln/accu/internal/base.hh>

# include <mln/arith/plus.hh>

# include <mln/core/alias/point3d.hh>
# include <mln/core/alias/box3d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/image/dmorph/transformed_image.hh>
# include <mln/core/macros.hh>

# include <mln/fun/p2p/fold.hh>

# include <mln/literal/zero.hh>

# include <mln/trace/entering.hh>
# include <mln/trace/exiting.hh>

# include <mln/trait/value/comp.hh>

# include <mln/value/int_u.hh>
# include <mln/value/ops.hh>

namespace mln
{

  namespace accu
  {

    namespace stat
    {

      // Forward declaration
      template <unsigned q, typename V>
      struct histo3d_hsl;

    } // end of namespace mln::accu::stat

    namespace meta
    {

      namespace stat
      {

	template <unsigned q>
	struct histo3d_hsl : public Meta_Accumulator< histo3d_hsl<q> >
	{
	  template <typename V>
	  struct with
	  {
	    typedef accu::stat::histo3d_hsl<q,V> ret;
	  };
	};

      } // end of namespace mln::accu::meta::stat

    } // end of namespace mln::accu::meta

  } // end of namespace mln::accu

  namespace trait
  {

    template <unsigned q, typename V>
    struct accumulator_< mln::accu::stat::histo3d_hsl<q,V> >
    {
      typedef accumulator::has_untake::no    has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

    template <unsigned q, typename V>
    struct set_precise_binary_<op::eq,
			       accu::stat::histo3d_hsl<q,V>,
			       accu::stat::histo3d_hsl<q,V> >
    {
      typedef bool ret;
    };

  } // end of namespace mln::trait

  namespace accu
  {

    namespace stat
    {


      /// \brief Define a histogram as accumulator which returns an image3d.
      ///
      /// Param q defines the number of bins per axe.
      /// Param V defines the type of the input image value. We assume that
      /// V is a kind of hsl_<float,float,float>.
      ///
      /// \ingroup modaccuvalues
      template <unsigned q, typename V>
      struct histo3d_hsl :

	public mln::accu::internal::base<image3d<unsigned>, histo3d_hsl<q,V> >
      {
	typedef V                 argument;
	typedef image3d<unsigned> result;
	typedef result            q_result;

	/// Constructors.
	/// \{
	/// \brief Infer the size of the resulting image3d domain.
	///
	/// By evaluating the minimum and the maximum of V, we define the domain
	/// of the resulting image3d.
	histo3d_hsl();
	/// \}


	/// Manipulators.
	/// \{
	/// \brief Initialize the histogram with zero value.
	///
	/// This method must be called just before starting the use of the
	/// histogram. If it's not, resulting values won't converge to the
	/// density.
	void init();


	/// \brief Update the histogram with the RGB pixel t.
	/// \param[in] t a graylevel pixel of type V.
	///
	/// The end user shouldn't call this method. In place of it, he can
	/// go through the data compute interface.
	void take(const argument& t);


	/// \brief Update the histogram with an other histogram.
	/// \param[in] other the other histogram.
	///
	/// The end user shouldn't call this method. This is part of
	/// data compute interface mechanism.
	void take(const histo3d_hsl<q,V>& other);
	/// \}

	/// Accessors.
	/// \{
	/// \brief Return the histogram as an HSL image3d.
	///
	/// This is the machinery to communicate with data compute interface.
	/// The end user should'nt use it.
	result to_result() const;
	operator result () const;
	/// \}

	/// \brief Check whethever this accumulator is able to return a result.
	///
	/// Depends if the resulting image1d is valid. We can assume it is quite
	/// always the case.
	bool is_valid() const;

      protected:
	const float min_hue;
	const float max_hue;
	float       step_hue;
	const float min_lum;
	const float max_lum;
	float       step_lum;
	const float min_sat;
	const float max_sat;
	float       step_sat;
	result      count_;
      };

      /// \brief Check wethever an histogram is equal to an other one.
      /// \param[in] histo1 the first histogram to compare with.
      /// \param[in] histo2 the second histogram.
      ///
      /// The operator compares all the bins from the two histograms.
      /// Nobody uses this method unless unitary tests.
      template <unsigned q, typename V>
      bool operator==(const histo3d_hsl<q,V>& histo1,
		      const histo3d_hsl<q,V>& histo2);

# ifndef MLN_INCLUDE_ONLY

      template <unsigned q, typename V>
      inline
      histo3d_hsl<q,V>::histo3d_hsl() : min_hue(0.0), max_hue(360.0),
				        min_lum(0.0), max_lum(1.0),
				        min_sat(0.0), max_sat(1.0)
      {
	trace::entering("mln::accu::stat::histo3d_hsl::cstor");

	/// As there is no info about preceding color space
	/// we ask the end user to specify the quantification he's looking for.

	count_.init_(box3d(point3d(mln_min(value::int_u<q>),
				   mln_min(value::int_u<q>),
				   mln_min(value::int_u<q>)),
			   point3d(mln_max(value::int_u<q>),
				   mln_max(value::int_u<q>),
				   mln_max(value::int_u<q>))));

	/// Make the hue domain cyclic
	fun::p2p::fold<point3d,1,0,0> fold_(count_.domain());
	transform_domain(count_, fold_);

	step_hue = (max_hue - min_hue)/q;
	step_lum = (max_lum - min_lum)/q;
	step_sat = (max_sat - min_sat)/q;

	trace::exiting("mln::accu::stat::histo3d_hsl::cstor");
      }

      template <unsigned q, typename V>
      inline
      void histo3d_hsl<q,V>::init()
      {
	data::fill(count_, literal::zero);
      }

      template <unsigned q, typename V>
      inline
      void histo3d_hsl<q,V>::take(const argument& t)
      {
	// Just convert a greyscale value (int_u8 like) to a position for an
	// iterator on the resulting image.
	//++count_(point3d(t.red(), t.green(), t.blue()));


	// Technical way to access i° component without kwnowing the name
	// mln::trait::value_<argument>::get_comp_0(t);

	// is def::coord1d the type of x, y, z ??
	unsigned x = (t.hue() - min_hue)/step_hue;
	unsigned y = (t.sat() - min_sat)/step_sat;
	unsigned z = (t.lum() - min_lum)/step_lum;
	/*

	std::cout << "H : " << t.hue() << std::endl;
	std::cout << "L : " << t.lum() << std::endl;
	std::cout << "S : " << t.sat() << std::endl;

	std::cout << "step_hue : " << step_hue << std::endl;
	std::cout << "step_lum : " << step_lum << std::endl;
	std::cout << "step_sat : " << step_sat << std::endl;

	std::cout << "X : " << x << std::endl;
	std::cout << "Y : " << y << std::endl;
	std::cout << "Z : " << z << std::endl;
	*/

	++count_(point3d(z, x, y));
      }


      template <unsigned q, typename V>
      inline
      void histo3d_hsl<q,V>::take(const histo3d_hsl<q,V>& other)
      {
	count_ += other.count_;
      }

      template <unsigned q, typename V>
      inline
      typename histo3d_hsl<q,V>::result histo3d_hsl<q,V>::to_result() const
      {
	return count_;
      }

      template <unsigned q, typename V>
      inline
      histo3d_hsl<q,V>::operator result() const
      {
	return count_;
      }

      template <unsigned q, typename V>
      inline
      bool histo3d_hsl<q,V>::is_valid() const
      {
	bool result = count_.is_valid();

	return result;
      }

      template <unsigned q, typename V>
      bool operator==(const histo3d_hsl<q,V>& histo1,
		      const histo3d_hsl<q,V>& histo2)
      {
	trace::entering("mln::accu::stat::histo3d_hsl::operator==");

	bool  result                  = true;
	const image3d<unsigned>& res1 = histo1.to_result();
	const image3d<unsigned>& res2 = histo2.to_result();

	mln_precondition(res1.is_valid());
	mln_precondition(res2.is_valid());

	mln_piter(image3d<unsigned>)      p1(res1.domain());
	mln_piter(image3d<unsigned>)      p2(res2.domain());

	for_all_2(p1, p2)
	  result &= (res1(p1) == res2(p2));

	trace::exiting("mln::accu::stat::histo3d_hsl::operator==");
	return result;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_ACCU_STAT_HISTO3D_HSL_HH
