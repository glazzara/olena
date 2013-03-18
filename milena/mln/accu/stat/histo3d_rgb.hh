// Copyright (C) 2009, 2010, 2013 EPITA Research and Development
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

#ifndef MLN_ACCU_STAT_HISTO3D_RGB_HH
# define MLN_ACCU_STAT_HISTO3D_RGB_HH

/// \file
///
/// \brief Define a histogram as accumulator which returns an image3d.
///
/// This source implements the discrete histogram version. It was
/// created for images which the values are derived from integer
/// type. The number of bins is directly infered from the cardinality
/// of the image value. It works for RGB space. See histo1d.hh for limitations
/// of such implementation. 8 bits quantification is very
/// expensive, it produces image3d with [0..255,0..255,0..255] as domain.
///
/// The following sample is a typical use of the histogram.
///
/// #include <mln/accu/stat/histo3d_rgb.hh>
/// #include <mln/core/image/image2d.hh>
/// #include <mln/core/image/image3d.hh>
/// #include <mln/data/compute.hh>
/// #include <mln/data/transform.hh>
/// #include <mln/fun/v2v/rgb8_to_rgbn.hh>
/// #include <mln/img_path.hh>
/// #include <mln/io/ppm/load.hh>
/// #include <mln/value/rgb.hh>
/// #include <mln/value/rgb8.hh>
///
/// int main()
/// {
///   typedef mln::value::rgb8   t_rgb8;
///   typedef mln::value::rgb<7> t_rgb7;
///   mln::image2d<t_rgb8>       img_rgb8;
///   mln::image2d<t_rgb7>       img_rgb7;
///   mln::image3d<unsigned>     histo;
///
///   mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/lena.ppm");
///   img_rgb7 =mln::data::transform(img_rgb8,mln::fun::v2v::rgb8_to_rgbn<7>());
///   histo = mln::data::compute(mln::accu::meta::stat::histo3d_rgb(),img_rgb7);
///
///   return 0;
/// }


# include <iostream>

# include <mln/accu/internal/base.hh>

# include <mln/arith/plus.hh>

# include <mln/core/macros.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/alias/point3d.hh>
# include <mln/core/alias/box3d.hh>

# include <mln/literal/zero.hh>

# include <mln/debug/trace.hh>

# include <mln/trait/value/comp.hh>

# include <mln/value/ops.hh>

namespace mln
{

  namespace accu
  {

    namespace stat
    {

      // Forward declaration
      template <typename V>
      struct histo3d_rgb;

    } // end of namespace mln::accu::stat

    namespace meta
    {

      namespace stat
      {

	struct histo3d_rgb : public Meta_Accumulator<histo3d_rgb>
	{
	  template <typename V>
	  struct with
	  {
	    typedef accu::stat::histo3d_rgb<V> ret;
	  };
	};

      } // end of namespace mln::accu::meta::stat

    } // end of namespace mln::accu::meta

  } // end of namespace mln::accu


  namespace trait
  {

    template <typename V>
    struct accumulator_< mln::accu::stat::histo3d_rgb<V> >
    {
      typedef accumulator::has_untake::no    has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

    template <typename V>
    struct set_precise_binary_<op::eq,
			       accu::stat::histo3d_rgb<V>,
			       accu::stat::histo3d_rgb<V> >
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
      /// Param V defines the type of the input image value. It is in
      /// this space that we count the values. For instance, this
      /// histogram works well for image2d< rgb<2> > or with image2d<
      /// rgb<7> >. The number of bins depends directly the values V.
      /// For 8 bits there is 256x3 bins. Note that less
      /// quantification works too.
      ///
      /// \ingroup modaccuvalues

      template <typename V>
      struct histo3d_rgb :
	public mln::accu::internal::base<image3d<unsigned>, histo3d_rgb<V> >
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
	histo3d_rgb();
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
	void take(const histo3d_rgb<V>& other);
	/// \}

	/// Accessors.
	/// \{
	/// \brief Return the histogram as an RGB image3d.
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
	result  count_;
      };

      /// \brief Check wethever an histogram is equal to an other one.
      /// \param[in] histo1 the first histogram to compare with.
      /// \param[in] histo2 the second histogram.
      ///
      /// The operator compares all the bins from the two histograms.
      /// Nobody uses this method unless unitary tests.
      template <typename V>
      bool operator==(const histo3d_rgb<V>& histo1,
		      const histo3d_rgb<V>& histo2);

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      histo3d_rgb<V>::histo3d_rgb()
      {
	mln_trace("mln::accu::stat::histo3d_rgb::cstor");

	typedef mln_trait_value_comp(V,0) comp0;
	typedef mln_trait_value_comp(V,1) comp1;
	typedef mln_trait_value_comp(V,2) comp2;

	// comp keep a trace of the dimension of the theorical image.
	// mln_min(comp) --> 0
	// mln_max(comp) --> 2^(n-1) [127 for n=7][255 for n=8] ...

	count_.init_(box3d(point3d(mln_min(comp0),
				   mln_min(comp1),
				   mln_min(comp2)),
			   point3d(mln_max(comp0),
				   mln_max(comp1),
				   mln_max(comp2))));

      }

      template <typename V>
      inline
      void histo3d_rgb<V>::init()
      {
	data::fill(count_, literal::zero);
      }

      template <typename V>
      inline
      void histo3d_rgb<V>::take(const argument& t)
      {
	// Just convert a greyscale value (int_u8 like) to a position for an
	// iterator on the resulting image.
	// Take care to the constructor : Point(slice, row, column)
	++count_(point3d(t.blue(), t.red(), t.green()));
      }


      template <typename V>
      inline
      void histo3d_rgb<V>::take(const histo3d_rgb<V>& other)
      {
	count_ += other.count_;
      }

      template <typename V>
      inline
      typename histo3d_rgb<V>::result histo3d_rgb<V>::to_result() const
      {
	return count_;
      }

      template <typename V>
      inline
      histo3d_rgb<V>::operator result() const
      {
	return count_;
      }

      template <typename V>
      inline
      bool histo3d_rgb<V>::is_valid() const
      {
	bool result = count_.is_valid();

	return result;
      }

      template <typename V>
      bool operator==(const histo3d_rgb<V>& histo1,
		      const histo3d_rgb<V>& histo2)
      {
	mln_trace("mln::accu::stat::histo3d_rgb::operator==");

	bool  result                  = true;
	const image3d<unsigned>& res1 = histo1.to_result();
	const image3d<unsigned>& res2 = histo2.to_result();

	mln_precondition(res1.is_valid());
	mln_precondition(res2.is_valid());

	mln_piter(image3d<unsigned>)      p1(res1.domain());
	mln_piter(image3d<unsigned>)      p2(res2.domain());

	for_all_2(p1, p2)
	  result &= (res1(p1) == res2(p2));

	return result;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_ACCU_STAT_HISTO3D_RGB_HH
