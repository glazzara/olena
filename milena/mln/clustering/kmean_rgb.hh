// Copyright (C) 2010, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_CLUSTERING_KMEAN_RGB_HH
# define MLN_CLUSTERING_KMEAN_RGB_HH

/// \file
///
/// \brief Implements the optimized kmean algorithm in the 3d-RGB
/// space.
///
/// This algorithm is optimized in the way it proceeds directly with
/// the rgb value inspite of the pixel attribute. The algorithm is
/// independant from the image dimension. But, we have to compute one
/// time the histogram. In fact, we move a recurrent cost by a fix
/// cost in the complexity. This version is very adapted to images
/// with small quantification. But, in 3d, the execution becomes very
/// slow. It's just normal because the quantification is n bits per
/// axis. So the actual histogram may be bigger than the image.
///
/// Take care to the following point: The within variance is still a
/// scalar value because we take the distance between two points and
/// the result is a scalar from the geometrical point of view. An
/// alternative implementation could study the variance/covariance
/// matrix of each sub data clouds and works with the trace of the
/// within variance matrix (as we do for the fisher criteria in N-d).
///
/// The following sample is a typical use of the functional (versus
/// object) kmean implementation.
///
/// #include <iostream>
/// #include <mln/clustering/kmean_rgb.hh>
/// #include <mln/core/image/image2d.hh>
/// #include <mln/data/transform.hh>
/// #include <mln/fun/v2v/rgb8_to_rgbn.hh>
/// #include <mln/img_path.hh>
/// #include <mln/io/ppm/load.hh>
/// #include <mln/value/label_8.hh>
/// #include <mln/value/rgb.hh>
/// #include <mln/value/rgb8.hh>
///
/// int main()
/// {
///   typedef mln::value::label_8                  t_lbl8;
///   typedef mln::value::rgb8                     t_rgb8;
///   typedef mln::value::rgb<5>                   t_rgb5;
///   typedef mln::image2d<t_rgb8>                 t_image2d_rgb8;
///   typedef mln::image2d<t_rgb5>                 t_image2d_rgb5;
///   typedef mln::image2d<t_lbl8>                 t_image2d_lbl8;
///   typedef mln::fun::v2v::rgb8_to_rgbn<5>       t_rgb8_to_rgb5;
///
///   t_image2d_rgb8                               img_rgb8;
///   t_image2d_rgb5                               img_rgb5;
///   t_image2d_lbl8                               img_lbl8;
///
///   mln::io::ppm::load(img_rgb8, OLENA_IMG_PATH"/house.ppm");
///
///   img_rgb5 = mln::data::transform(img_rgb8, t_rgb8_to_rgb5());
///   img_lbl8 = mln::clustering::kmean_rgb<double,5>(img_rgb5, 3);
///
///   return 0;
/// }
///
/// \todo The execution shows a bug in printing outputs and it seems
/// severe.
///
/// The last execution with the following set of parameters
/// {house.ppm,3,10,10} shows that if the binary starts correctly, it
/// ends before returning the label image and with disturbing outputs.
/// Dumping the outputs in a file reveals that the number of
/// mln_trace differs from the number of trace::exiting. May the
/// program exit from a loop without ending a trace ???

# include <limits.h>
# include <iostream>

# include <mln/accu/stat/histo3d_rgb.hh>

# include <mln/algebra/vec.hh>

# include <mln/core/concept/image.hh>
# include <mln/core/contract.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/macros.hh>

# include <mln/data/compute.hh>
# include <mln/data/fill.hh>
# include <mln/data/transform.hh>

# include <mln/debug/println.hh>

# include <mln/io/ppm/save.hh>
# include <mln/io/pgm/save.hh>

# include <mln/labeling/colorize.hh>
# include <mln/labeling/mean_values.hh>

# include <mln/literal/zero.hh>
# include <mln/literal/one.hh>

# include <mln/math/min.hh>
# include <mln/math/sqr.hh>

# include <mln/norm/l2.hh>

# include <mln/opt/at.hh>

# include <mln/debug/trace.hh>

# include <mln/trait/value_.hh>

# include <mln/util/array.hh>

# include <mln/value/int_u.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/label_8.hh>


//--------------------------------------------------------------------------
// FUNCTIONAL CODE
//--------------------------------------------------------------------------


namespace mln
{

  namespace clustering
  {
    /// \brief Implements the functional kmean algorithm.
    ///
    /// This functional version of the kmean is very specific. All the
    /// code is view as a one-block function. This code is provided as
    /// is. I (YJ) don't know where i stopped this version. It may not
    /// work. Debugging tools are not yet provided. This code is just
    /// the functional transcription of the kmean3d version. The code
    /// has the very experimental status.
    ///
    /// T is the type used for computations (float or double).
    /// n is the quantification for the rgb image.
    ///
    /// \param[in] point     : the image as the population of pixels.
    /// \param[in] k_center  : the number of centers.
    /// \param[in] watch_dog : the limit to observe the convergence (10).
    /// \param[in] n_times   : the number of times that we executed kmean(10).
    ///
    /// \return an image which represents the pixel classification.
    template <typename T, unsigned n, typename I>
    inline
    mln_ch_value(I,value::label_8)
    kmean_rgb(const Image<I>& point,
	      const unsigned  k_center,
	      const unsigned  watch_dog,
	      const unsigned  n_times);

  } // end of namespace mln::clustering

  namespace clustering
  {


# ifndef MLN_INCLUDE_ONLY

    //--------------------------------------------------------------------------
    // Internal.
    //--------------------------------------------------------------------------

    namespace internal
    {

      //------------------------------------------------------------------------
      // Debugging tools
      //------------------------------------------------------------------------

      /*
    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::build_label_dbg()
    {
      mln_trace("mln::clustering::kmean3d_a::build_label_dbg");

      mln_piter(t_point_img) pi(_point.domain());
      mln_piter(t_label_dbg) po(_label_dbg.domain());

      for_all_2(pi, po)
      {
	  t_value    val = _point(pi);
	  t_label    grp = _group(point3d(val.blue(), val.red(), val.green()));

	  // As label zero has got a particular semantic, the first label is one
	  _label_dbg(po) = ++grp;
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::build_mean_dbg()
    {
      mln_trace("mln::clustering::kmean3d_a::build_mean_dbg");

      mln_piter(t_mean_dbg)  p(_mean_dbg.domain());

      for_all(p)
      {
	_mean_dbg(p).red()   = static_cast<unsigned>(_mean[_label_dbg(p)][0]);
	_mean_dbg(p).green() = static_cast<unsigned>(_mean[_label_dbg(p)][1]);
	_mean_dbg(p).blue()  = static_cast<unsigned>(_mean[_label_dbg(p)][2]);
      }

    }


    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::build_all_dbg()
    {
      mln_trace("mln::clustering::kmean3d_a::build_all_dbg");
      build_label_dbg();
      //build_mean_dbg();
      _mean_dbg  = labeling::mean_values(_point, _label_dbg, _k_center);
      _color_dbg = labeling::colorize(value::rgb8(), _label_dbg);

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::update_cnv()
    {
      mln_trace("mln::clustering::kmean3d_a::update_cnv");

      _variance_cnv[_current_launching](point1d(_current_step))
	= _within_variance;

      mln_eiter(t_mean_img) l(_mean);

      for_all(l)
      {
	_mean_cnv[l.index_()][_current_launching](point1d(_current_step))
	  = _mean[l.index_()];
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::finalize_cnv()
    {
      mln_trace("mln::clustering::kmean3d_a::finalize_cnv");

      // saturate the curv with the within variance
      for (unsigned i = _current_step; i < _watch_dog; ++i)
	_variance_cnv[_current_launching](point1d(i)) = _within_variance;

      for (unsigned i = _current_step; i < _watch_dog; ++i)
      {
	mln_eiter(t_mean_img) l(_mean);

	for_all(l)
	{
	  _mean_cnv[l.index_()][_current_launching](point1d(i))
	    = _mean[l.index_()];
	}
      }

    }




    //--------------------------------------------------------------------------
    // Printing temporary results
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::print_mean()
    {
      mln_trace("mln::clustering::kmean3d_a::print_mean");

      mln_eiter(t_mean_img)  l(_mean);

      for_all(l)
      {
	std::cout << "mean("      << l.index_();
	std::cout << ") = [r="    << _mean[l.index_()][0];
	std::cout << ", g="       << _mean[l.index_()][1];
	std::cout << ", b="       << _mean[l.index_()][2];
	std::cout << "]"          << std::endl;
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::print_number()
    {
      mln_trace("mln::clustering::kmean3d_a::print_number");

      mln_eiter(t_number_img)  l(_number);

      for_all(l)
      {
	std::cout << "number(" << l.index_();
	std::cout << ") = "    << _number[l.index_()];
	std::cout << std::endl;
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::print_variance()
    {
      mln_trace("mln::clustering::kmean3d_a::print_variance");

      mln_eiter(t_variance_img)  l(_number);

      for_all(l)
      {
	std::cout << "variance(" << l.index_();
	std::cout << ") = "      << _variance[l.index_()];
	std::cout << std::endl;
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::print_histo()
    {
      mln_trace("mln::clustering::kmean3d_a::print_histo");

      mln_piter(t_histo_img)  rgb(_histo.domain());

      for_all(rgb)
      {
	if (0 < _histo(rgb))
	{
	  std::cout << "histo(r="  << rgb.row();
	  std::cout << ", g="      << rgb.col();
	  std::cout << ", b="      << rgb.sli();
	  std::cout << ")= "       << _histo(rgb);
	  std::cout << std::endl;
	}
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::print_group()
    {
      mln_trace("mln::clustering::kmean3d_a::print_group");

      mln_piter(t_group_img)  rgb(_group.domain());

      for_all(rgb)
      {
	if (0 < _histo(rgb))
	{
	  std::cout << "group(r="  << rgb.row();
	  std::cout << ", g="      << rgb.col();
	  std::cout << ", b="      << rgb.sli();
	  std::cout << ")= "       << _group(rgb);
	  std::cout << std::endl;
	}
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::print_distance()
    {
      mln_trace("mln::clustering::kmean3d_a::print_distance");

      mln_eiter(t_distance_img)  l(_distance);

      for_all(l)
      {
	mln_piter(t_distance_val)  rgb(_distance[l.index_()].domain());

	for_all(rgb)
	{
	  if (0 < _histo(rgb))
	  {
	    std::cout << "distance(l="  << l.index_();
	    std::cout << ",r="          << rgb.row();
	    std::cout << ", g="         << rgb.col();
	    std::cout << ", b="         << rgb.sli();
	    std::cout << ")= "          << _distance[l.index_()](rgb);
	    std::cout << std::endl;
	  }
	}
      }

    }

    template <typename T, unsigned n>
    inline
    void kmean3d_a<T,n>::print_point()
    {
      mln_trace("mln::clustering::kmean3d_a::print_point");

      mln_piter(t_point_img)  p(_point.domain());

      for_all(p)
      {
	std::cout << "point(r="  << p.row();
	std::cout << ", c="      << p.col();
	std::cout << ")= "       << _point(p);
	std::cout << std::endl;
      }

    }



      template <typename T, unsigned n>
      inline
      void rgb_rand_init(t_mean_img mean)
      {
	typedef value::rgb<n>                      t_value;
	typedef mln_trait_value_comp(t_value,0)    t_value_comp0;
	typedef mln_trait_value_comp(t_value,1)    t_value_comp1;
	typedef mln_trait_value_comp(t_value,2)    t_value_comp2;
	typedef algebra::vec<3,T>                  t_result3d;
	typedef util::array<t_result3d>            t_mean_img;

	t_value_comp0         min_comp0 = mln_min(t_value_comp0);
	t_value_comp0         max_comp0 = mln_max(t_value_comp0);
	t_value_comp1         min_comp1 = mln_min(t_value_comp1);
	t_value_comp1         max_comp1 = mln_max(t_value_comp1);
	t_value_comp2         min_comp2 = mln_min(t_value_comp2);
	t_value_comp2         max_comp2 = mln_max(t_value_comp2);
	mln_eiter(t_mean_img) l(mean);

	for_all(l)
	{
	  mean[l.index_()][0] = (rand() % (max_comp0 - min_comp0)) + min_comp0;
	  mean[l.index_()][1] = (rand() % (max_comp1 - min_comp1)) + min_comp1;
	  mean[l.index_()][2] = (rand() % (max_comp2 - min_comp2)) + min_comp2;
	}

	return mean;
      }

      */

    } // end of namespace mln::clustering::internal


    //--------------------------------------------------------------------------
    // Impl.
    //--------------------------------------------------------------------------

    namespace impl
    {

      //------------------------------------------------------------------------
      // kmean_image2d_rgb(const t_point_img& point,
      //                   const unsigned     k_center,
      //                   const unsigned     watch_dog = 10,
      //                   const unsigned     n_times   = 10)
      //------------------------------------------------------------------------

      template <unsigned n>
      struct rgbn_to_lbl8 : Function_v2v< rgbn_to_lbl8<n> >
      {
	typedef value::rgb<n>    argument;
	typedef value::label_8   result;
	typedef value::label_8   t_label;
	typedef image3d<t_label> t_group_img;

	t_group_img _group;

	rgbn_to_lbl8(t_group_img group) : _group(group) {}

	result operator()(const argument& c) const
	{
	  value::label_8 tmp = opt::at(_group, c.blue(), c.red(), c.green());

	  // FIXME WHY DO WE NOT USE +1
	  return ++tmp;
	}
      };

      template <typename T, unsigned n>
      struct rgb_to_dist : Function_v2v< rgb_to_dist<T,n> >
      {
	typedef value::rgb<n>       argument;
	typedef T                   result;
	typedef T                   t_result1d;
	typedef algebra::vec<3,T>   t_result3d;
	typedef image3d<unsigned>   t_histo_img;

	t_result3d  _mean;
	t_histo_img _histo;

	rgb_to_dist(t_result3d mean, t_histo_img histo) : _mean(mean),
							  _histo(histo) {}

	result operator()(const argument& c) const
	{
	  t_result1d diff2_row = math::sqr(c.row() - _mean[0]);
	  t_result1d diff2_col = math::sqr(c.col() - _mean[1]);
	  t_result1d diff2_sli = math::sqr(c.sli() - _mean[2]);
	  t_result1d tmp       = _histo(c)*(diff2_row + diff2_col + diff2_sli);

	  return tmp;
	}
      };

      template <typename T, unsigned n, typename I>
      inline
      mln_ch_value(I,value::label_8)
      kmean_image2d_rgb(const Image<I>& point__,
			const unsigned  k_center,
			const unsigned  watch_dog = 10,
			const unsigned  n_times   = 10)
      {
	mln_trace("mln::clustering::impl::kmean_image2d_rgb");

	const I& point = exact(point__);
	typedef mln_value(I) V;
	mlc_is(V, value::rgb<n>)::check();
	mlc_bool(mln_site_(I)::dim == 2u)::check();
	mln_precondition(point.is_valid());

	// BEGIN TYPEDEF
	typedef value::rgb<8>                      t_rgb;
	typedef value::label<8>                    t_label;
	typedef value::rgb<n>                      t_value;
	typedef mln_trait_value_comp(t_value,0)    t_value_comp0;
	typedef mln_trait_value_comp(t_value,1)    t_value_comp1;
	typedef mln_trait_value_comp(t_value,2)    t_value_comp2;
	typedef T                                  t_result1d;
	typedef algebra::vec<3,T>                  t_result3d;

	typedef I                                  t_point_img;
	typedef image3d<unsigned>                  t_histo_img;
	typedef util::array<t_result1d>            t_number_img;
	typedef util::array<t_result3d>            t_mean_img;
	typedef util::array<t_result1d>            t_variance_img;

	typedef image3d<t_label>                   t_group_img;
	typedef image3d<t_result1d>                t_distance_val;
	typedef util::array<t_distance_val>        t_distance_img;

	typedef mln_ch_value(I,t_label)            t_label_dbg;
	typedef image2d<t_rgb>                     t_color_dbg;
	typedef image2d<t_value>                   t_mean_dbg;

	typedef image1d<t_result3d>                t_mean_val;
	typedef util::array<t_mean_val>            t_mean_set;
	typedef util::array<t_mean_set>            t_mean_cnv;
	typedef image1d<t_result1d>                t_variance_val;
	typedef util::array<t_variance_val>        t_variance_cnv;
	// END TYPEDEF

	// BEGIN INITIALISATION
	mln_precondition(point.is_valid());

	static const unsigned _N_TRIES    = 3;

	typedef accu::meta::stat::histo3d_rgb t_histo3d_rgb;

	t_result1d     _within_variance;

	unsigned       _k_center          = k_center;
	unsigned       _watch_dog         = watch_dog;
	unsigned       _n_times           = n_times;
	t_point_img    _point             = point;

	// HISTOGRAM INIT
	t_histo_img    _histo             = data::compute(t_histo3d_rgb(),
							  _point);

	// CENTER STATS INIT
	t_number_img   _number;
	t_mean_img     _mean;
	t_variance_img _variance;

	for (unsigned i = 0; i < _k_center; ++i)
	{
	  _number.append(literal::zero);
	  _mean.append(literal::zero);
	  _variance.append(literal::zero);
	}


	unsigned       _current_step      = 0;
	unsigned       _current_launching = 0;
	bool           _is_number_valid   = false;

	unsigned       _launching_min;
	t_result1d     _variance_min;
	t_mean_img     _mean_min;



	t_group_img    _group;
	t_distance_img _distance;


	t_label_dbg    _label_dbg;
	t_color_dbg    _color_dbg;
	t_mean_dbg     _mean_dbg;


	t_mean_cnv     _mean_cnv;
	t_variance_cnv _variance_cnv;




	_group.init_(box3d(point3d(mln_min(t_value_comp2),
				   mln_min(t_value_comp0),
				   mln_min(t_value_comp1)),
			   point3d(mln_max(t_value_comp2),
				   mln_max(t_value_comp0),
				   mln_max(t_value_comp1))));

	for (unsigned i = 0; i < _k_center; ++i)
	{
	  t_distance_val img(box3d(point3d(mln_min(t_value_comp2),
					   mln_min(t_value_comp0),
					   mln_min(t_value_comp1)),
				   point3d(mln_max(t_value_comp2),
					   mln_max(t_value_comp0),
					   mln_max(t_value_comp1))));

	  _distance.append(img);
	}

	// Debugging, calibrating and testing
	initialize(_label_dbg, _point);
	initialize(_color_dbg, _point);
	initialize(_mean_dbg,  _point);

	// Observing the convergence

	for (unsigned i = 0; i < _n_times; ++i)
	{
	  t_variance_val img(box1d(point1d(0), point1d(_watch_dog-1)));

	  data::fill(img, literal::zero);

	  _variance_cnv.append(img);
	}

	for (unsigned i = 0; i < _k_center; ++i)
	{
	  t_mean_set mean_set;

	  for (unsigned j = 0; j < _n_times; ++j)
	  {
	    t_mean_val img(box1d(point1d(0), point1d(_watch_dog-1)));

	    data::fill(img, literal::zero);

	    mean_set.append(img);
	  }

	  _mean_cnv.append(mean_set);
	}
	// END INITIALISATION

	// BEGIN LOOP N TIMES
	{
	  unsigned tries     = 0;
	  _variance_min      = mln_max(t_result1d);
	  _current_launching = 0;

	  while (_current_launching < _n_times)
	  {
	    // BEGIN LAUNCH ONE TIME
	    {
	      t_result1d old_variance = mln_max(t_result1d);
	      _within_variance        = mln_max(t_result1d);
	      _current_step           = 0;

	      // BEGIN INIT_MEAN
	      {
		t_value_comp0           min_comp0 = mln_min(t_value_comp0);
		t_value_comp0           max_comp0 = mln_max(t_value_comp0);
		t_value_comp1           min_comp1 = mln_min(t_value_comp1);
		t_value_comp1           max_comp1 = mln_max(t_value_comp1);
		t_value_comp2           min_comp2 = mln_min(t_value_comp2);
		t_value_comp2           max_comp2 = mln_max(t_value_comp2);
		mln_eiter(t_mean_img)   l(_mean);

		for_all(l)
		{
		  _mean[l.index_()][0]=(rand()%(max_comp0-min_comp0))+min_comp0;
		  _mean[l.index_()][1]=(rand()%(max_comp1-min_comp1))+min_comp1;
		  _mean[l.index_()][2]=(rand()%(max_comp2-min_comp2))+min_comp2;
		}
	      }
	      // END INIT MEAN


	      // UPDATE DISTANCE

	      for (unsigned i = 0; i < _k_center; ++i)
	      {

		// _distance[i] = data::transform(_histo,
 		//			       rgb_to_dist<T,n>(_mean[i],
 		//						_histo));

		mln_piter(t_distance_val) d(_distance[i].domain());

		for_all(d)
		{
		  t_result1d diff2_row = math::sqr(d.row() - _mean[i][0]);
		  t_result1d diff2_col = math::sqr(d.col() - _mean[i][1]);
		  t_result1d diff2_sli = math::sqr(d.sli() - _mean[i][2]);
		  _distance[i](d)      = _histo(d)*
		    (diff2_row + diff2_col + diff2_sli);
		}
	      }

	      // END UPDATE DISTANCE

	      do
	      {
		old_variance = _within_variance;

		// BEGIN UPDATE GROUP
		{
		    mln_piter(t_group_img) rgb(_group.domain());

		    for_all(rgb)
		    {
		      mln_eiter(t_distance_img) l(_distance);
		      t_result1d                min   = mln_max(t_result1d);
		      t_label                   label = mln_max(t_label);

		      for_all(l)
		      {
			if (min > _distance[l.index_()](rgb))
			{
			  min   = _distance[l.index_()](rgb);
			  label = l.index_();
			}
		      }

		      _group(rgb) =  label;
		    }

		}
		// END UPDATE GROUP

		// BEGIN UPDATE MEAN
		{
		  mln_eiter(t_number_img) en(_number);
		  mln_eiter(t_mean_img)   em(_mean);

		  for_all_2(en,em)
		  {
		    _number[en.index_()] = literal::zero;
		    _mean[em.index_()]   = literal::zero;
		  }

		  mln_piter(t_group_img)  rgb(_group.domain());

		  for_all(rgb)
		  {
		    _mean[_group(rgb)][0] += rgb.row() * _histo(rgb);
		    _mean[_group(rgb)][1] += rgb.col() * _histo(rgb);
		    _mean[_group(rgb)][2] += rgb.sli() * _histo(rgb);
		    _number(_group(rgb))  += _histo(rgb);
		  }

		  mln_eiter(t_mean_img)   l(_mean);

		  for_all(l)
		  {
		    _is_number_valid = (0 != _number[l.index_()]);

		    if (!_is_number_valid)
		      break;

		      _mean[l.index_()] /= _number[l.index_()];
		  }
		}
		// END UPDATE MEAN


		// Stopping Nan propagation
		if (!_is_number_valid)
		  break;

		// UPDATE DISTANCE

		for (unsigned i = 0; i < _k_center; ++i)
		{
		  mln_piter(t_distance_val) d(_distance[i].domain());

		  for_all(d)
		  {
		    // the square distance
		    t_result1d diff2_row = math::sqr(d.row() - _mean[i][0]);
		    t_result1d diff2_col = math::sqr(d.col() - _mean[i][1]);
		    t_result1d diff2_sli = math::sqr(d.sli() - _mean[i][2]);
		    _distance[i](d)      = _histo(d)*
		      (diff2_row + diff2_col + diff2_sli);
		  }
		}
		// END UPDATE DISTANCE

		// BEGIN UPDATE VARIANCE
		{
		  _within_variance          = literal::zero;
		  mln_eiter(t_variance_img) l(_variance);

		  for_all(l)
		  {
		    _variance[l.index_()] = literal::zero;

		    mln_piter(t_group_img) rgb(_group.domain());

		    for_all(rgb)
		    {
		      if (l.index_() == _group(rgb))
			_variance[l.index_()] += _distance[l.index_()](rgb);
		    }

		    _within_variance += _variance[l.index_()];
		  }

		}
		// END UPDATE VARIANCE

		//update_cnv();

		++_current_step;
	      }
	      while (_current_step < _watch_dog &&
		     _within_variance < old_variance);

	      //finalize_cnv();
	      //build_all_dbg();
	    }
	    // END LAUNCH ONE TIME

	    if ((_is_number_valid && (_current_step < _watch_dog))||
		_N_TRIES < tries)
	    {
	      if (_within_variance < _variance_min)
	      {
		_variance_min  = _within_variance;
		_mean_min      = _mean;
		_launching_min = _current_launching;
	      }

	      // Reinitialize the number of echecs possible
	      tries = 0;

	      //std::cout << "_current_launching : " << _current_launching
	      //	    << std::endl;

	      //std::cout << "within_variance[" << _current_launching << "] = "
	      //	    << _within_variance << std::endl;

	      ++_current_launching;
	    }
	    else
	      ++tries;
	    }

	  //Debugging code
	  //build_all_dbg();

	}
	// END LOOP N TIMES

	// BEGIN BUILD LABEL IMAGE
	_label_dbg = data::transform(_point, rgbn_to_lbl8<n>(_group));

// 	{
// 	  mln_piter(t_point_img) pi(_point.domain());
// 	  mln_piter(t_label_dbg) po(_label_dbg.domain());

// 	  for_all_2(pi, po)
// 	  {
// 	    t_value    val = _point(pi);
// 	    t_label    grp = _group(point3d(val.blue(),val.red(),val.green()));

// 	    _label_dbg(po) = ++grp;
// 	  }
// 	}

	// END BUILD LABEL IMAGE

	return _label_dbg;

	}

    } // end of namespace mln::clustering::impl





    //--------------------------------------------------------------------------
    // Internal.
    //--------------------------------------------------------------------------

    namespace internal
    {

      template <typename T, unsigned n, typename I>
      inline
      mln_ch_value(I,value::label_8)
      kmean_rgb_dispatch(const Image<I>& img,
			 const unsigned  k_center,
			 const unsigned  watch_dog,
			 const unsigned  n_times,
			 const value::rgb<n>& /* v */,
			 const point2d& /* p */)
      {
	return impl::kmean_image2d_rgb<T,n>(img, k_center, watch_dog, n_times);
      }

      template <typename T, unsigned n, typename I, typename V, typename P>
      inline
      mln_ch_value(I,value::label_8)
      kmean_rgb_dispatch(const Image<I>& /* img */,
			 const unsigned /* k_center */,
			 const unsigned /* watch_dog */,
			 const unsigned /* n_times */,
			 const V& /* v */,
			 const P& /* p */)
      {
	// No kmean implementation found.
	mlc_abort(I)::check();

	typedef mln_ch_value(I, value::label_8) output_t;
	return output_t();
      }


      template <typename T, unsigned n, typename I>
      inline
      mln_ch_value(I,value::label_8)
      kmean_rgb_dispatch(const Image<I>& img,
			 const unsigned  k_center,
			 const unsigned  watch_dog,
			 const unsigned  n_times)
      {
	typedef mln_value(I) V;
	typedef mln_site(I) P;
	return kmean_rgb_dispatch<T,n>(img, k_center, watch_dog,
				       n_times, V(), P());
      }


    } // end of namespace mln::clustering::internal


    //--------------------------------------------------------------------------
    // Facade.
    //--------------------------------------------------------------------------

    template <typename T, unsigned n, typename I>
    inline
    mln_ch_value(I,value::label_8)
    kmean_rgb(const Image<I>& point,
	      const unsigned  k_center,
	      const unsigned  watch_dog,
	      const unsigned  n_times)
    {
      mln_trace("mln::clustering::kmean_rgb");

      mln_ch_value(I, value::label_8)
	output = internal::kmean_rgb_dispatch<T,n>(point, k_center,
						   watch_dog, n_times);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::clustering

} // end of namespace mln

#endif // ! MLN_CLUSTERING_KMEAN_RGB_HH
