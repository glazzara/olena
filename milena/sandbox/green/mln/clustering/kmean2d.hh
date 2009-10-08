// Copyright (C) 2008,2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CLUSTERING_KMEAN2D_HH
#define MLN_CLUSTERING_KMEAN2D_HH

/// \file
///
/// \brief Implements the optimized kmean algorithm.
///
/// This algorithm is optimized in the way it proceeds directly with
/// the greylevel attribute inspite of the pixel attribute. The
/// algorithm is independant from the image dimension. But, we have to
/// compute one time the histogram. In fact, we move a recurrent cost
/// to a fix cost in the complexity. This version is very adapted to
/// images with small quantification.

#include <limits.h>
#include <iostream>
#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>

#include <mln/core/contract.hh>
#include <mln/trait/value_.hh>
#include <mln/accu/stat/histo2d.hh>

#include <mln/math/min.hh>
#include <mln/math/sqr.hh>
#include <mln/norm/l2.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/concept/image.hh>
#include <mln/value/int_u.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/rg.hh>
#include <mln/core/macros.hh>

#include <mln/data/compute.hh>
#include <mln/debug/println.hh>
#include <mln/data/fill.hh>
#include <mln/literal/zero.hh>
#include <mln/literal/one.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/mean_values.hh>

#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/util/array.hh>
#include <mln/algebra/vec.hh>

namespace mln
{

  namespace clustering
  {

    // Forward declaration.
    template <typename T, unsigned n>
    struct kmean2d;

  } // end of namespace mln::clustering

  namespace clustering
  {
    /// \brief Implements the kmean algorithm in a specific way.
    ///
    /// This version of the kmean algorithm uses a greyscale image as input,
    /// temporary images for computations and produces images as result. Images
    /// play the role of matrix or vector in standard statistic algoritm.
    ///
    /// T is the type used for computations (float or double).
    /// n is the quantification for the image grayscale.
    template <typename T, unsigned n>
    struct kmean2d
    {
      /// Type definitions.
      /// \brief A few type definitions to limit the refactoring impact.
      ///{

      typedef value::rgb<8>                      t_rgb;
      typedef value::label<8>                    t_label;
      typedef value::rg<n>                       t_value;
      typedef mln_trait_value_comp(t_value,0)    t_value_comp0;
      typedef mln_trait_value_comp(t_value,1)    t_value_comp1;
      typedef T                                  t_result1d;
      typedef algebra::vec<2,T>                  t_result2d;

      /// FIXME t_point n'est pas forcément une image 2d, bien au contraire.
      typedef image2d<t_value>                   t_point_img;
      typedef image2d<unsigned>                  t_histo_img;
      typedef util::array<t_result1d>            t_number_img;
      typedef util::array<t_result2d>            t_mean_img;
      typedef util::array<t_result1d>            t_variance_img;

      typedef image2d<t_label>                   t_group_img;
      typedef image2d<t_result1d>                t_distance_val;
      typedef util::array<t_distance_val>        t_distance_img;

      typedef image2d<t_label>                   t_label_dbg;
      typedef image2d<t_rgb>                     t_color_dbg;
      typedef image2d<t_value>                   t_mean_dbg;

      typedef image1d<t_result2d>                t_mean_val;
      typedef util::array<t_mean_val>            t_mean_set;
      typedef util::array<t_mean_set>            t_mean_cnv;
      typedef image1d<t_result1d>                t_variance_val;
      typedef util::array<t_variance_val>        t_variance_cnv;

      ///}

      //------------------------------------------------------------------------
      // Constructor
      //------------------------------------------------------------------------

      /// \brief Constructor
      /// \param[in] point     : the image as the population of pixels.
      /// \param[in] k_center  : the number of centers.
      /// \param[in] watch_dog : the limit to observe the convergence (10).
      /// \param[in] n_times   : the number of times that we executed kmean(10).

      kmean2d(const t_point_img& point,
	      const unsigned     k_center,
	      const unsigned     watch_dog = 10,
	      const unsigned     n_times   = 10);

      //------------------------------------------------------------------------
      // Accessors
      //------------------------------------------------------------------------

      /// Mutator and accessors.
      /// \{
      /// \brief Mutator and accessors are required for debugging and testing.
      ///
      /// Testing needs to hack the kmean loop process in order to verify each
      /// step of the algorithm.

      void set_point(t_point_img&       point);
      void set_histo(t_histo_img&       histo);
      void set_number(t_number_img&     number);
      void set_mean(t_mean_img&         mean);
      void set_variance(t_variance_img& variance);
      void set_group(t_group_img&       group);
      void set_distance(t_distance_img& distance);

      // Hacking outputs
      t_point_img&    get_point();
      t_histo_img&    get_histo();
      t_number_img&   get_number();
      t_mean_img&     get_mean();
      t_variance_img& get_variance();
      t_group_img&    get_group();
      t_distance_img& get_distance();

      // Testing outputs
      t_color_dbg&    get_color_dbg();
      t_mean_dbg&     get_mean_dbg();
      t_label_dbg&    get_label_dbg();

      t_mean_cnv&     get_mean_cnv();
      t_variance_cnv& get_variance_cnv();

      // Normal outputs
      t_mean_img&     to_result();

      /// \}

      //------------------------------------------------------------------------
      // Printing temporary results
      //------------------------------------------------------------------------

      void print_mean();
      void print_number();
      void print_variance();
      void print_distance();
      void print_group();
      void print_point();
      void print_histo();

      //------------------------------------------------------------------------
      // Initializations of centers
      //------------------------------------------------------------------------

      /// Initialization of centers.
      /// \{
      /// \brief Two ways: Regular greylevel tick or random greylevel value or.
      ///
      /// There is two way to proceed the initialization. First of all, we
      /// divide the greyscale in regular tick and we assigne them to the mean
      /// of the centers. Finaly, we can ask random initialization along the
      /// greyscale axis. The second process is needed to launch_n_times the
      /// kmean and converge to the best descent.

      void init_mean();
      void init_mean_regular();
      void init_mean_random();

      /// \}


      //------------------------------------------------------------------------
      // Computations of distance, group, center, within variance
      //------------------------------------------------------------------------

      /// Computations of distance, group, center, within variance.
      /// \{
      /// \brief Update the statistical information needed by the kmean process.
      ///
      /// The kmean process is a loop where distance from centers to pixels are
      /// first compute. Then we assign the pixels to their nearest center.
      /// The new location of each center can then update. Finaly, hommogeneity
      /// in group is observed by the within variance.

      void update_distance();
      void update_group();
      void update_mean();
      void update_variance();

      /// \}

      //------------------------------------------------------------------------
      // Main loop
      //------------------------------------------------------------------------

      /// kmean main loop
      /// \{
      /// \brief User interface to launch the kmean process.
      ///
      /// There are two ways to launch the kmean process. The first one allow to
      /// run it one time until convergence. As the process is a descent, it
      /// depends on the initial center locations. The second call allow us to
      /// rerun the process many times and to keep the best result (the one
      /// with the smallest within variance).

      void launch_one_time();
      void launch_n_times();

      /// \}

      //------------------------------------------------------------------------
      // Checking the validiy of the results
      //------------------------------------------------------------------------

      /// Checking the validity of the results.
      /// \{
      /// \brief These methods help us to determine the validity of the results.
      ///
      /// After each launching the kmean process one time, we need to know if
      /// the descent was successfull or not. The method is_valid_descent do it
      /// for us. The method looks for a bad center (a class with no greylevel
      /// associate to it) and a failure in the convergence.

      bool is_descent_valid();

      /// \}

      //------------------------------------------------------------------------
      // Debugging tools
      //------------------------------------------------------------------------

      /// Debugging tools
      /// \{
      /// \brief These methods help to interpret results.
      ///
      /// The methods build_label_dbg and build_all_dbg work in the input data
      /// space. The first one build the 2d image of labels. The second call the
      /// first one and then builds the colorize label' image  and the mean
      /// greylevel image. The update_cnv and finalize_cnv methods are used to
      /// trace the convergence. They fill two images with the mean info and
      /// the within variance info along the convergence and the tries.

      void build_label_dbg();
      void build_mean_dbg();
      void build_all_dbg();
      void update_cnv();
      void finalize_cnv();

      /// \}


    private:
      /// Parameters.
      /// \{
      /// \brief These parameters control the convergence of the process.
      ///
      /// The first parameter, k_center, defines the number of center for kmean.
      /// In fact, watch_dog limit the number of iteration that a simple kmean
      /// loop can do. If the process reaches the watch_dog limit, it means
      /// that the process will not converge at all. The second parameter
      /// n_times is the number of times we launch again and again the simple
      /// kmean loop. As the kmean process is a descent, restarting the process
      /// from different location will confort us in that we found a global
      /// minima, not just a local one.

      unsigned       _k_center;
      unsigned       _watch_dog;
      unsigned       _n_times;

      /// \}

      /// Convergence information.
      /// \{
      /// \brief This information is used to follow the convergence.
      ///
      /// The within_variance is the homogeniety indicator for the
      /// kmean process.  The ideal situation is to find the center
      /// with the minimum variance around them. The within variance
      /// is just the sum of all variance around the centers. The
      /// current_step variable allows us to remember the current
      /// iteration in the kmean loop. This information is needed by
      /// is_descent_valid routine which decide if convergence occurs
      /// or not. The current_step info explicit where we are in the
      /// kmean iteration. The last information, current_launching,
      /// traces the progress while iterates kmean loop again and
      /// again. The flag is_number_valid is set when a empty class
      /// appears.  This flag inhibit the process and force to restart
      /// a try.

      t_result1d      _within_variance;
      unsigned       _current_step;
      unsigned       _current_launching;
      bool           _is_number_valid;

      static const unsigned _N_TRIES = 3;

      /// \}

      /// Result of the kmean process.
      /// \{
      /// \brief The center location are the major results of the kmean process.
      ///
      /// The kmean process result is composed by three information. The best
      /// launching iteration, the best within variance obtained and the
      /// location of the centers associated.

      unsigned        _launching_min;
      t_result1d      _variance_min;
      t_mean_img      _mean_min;

      /// Inputs.
      /// \{
      /// \brief The inputs are the distribution of the values and the values.
      ///
      /// The point image is a saving data for the real inputs. In fact, we use
      /// the histogram information in the optimized kmean process.

      t_point_img    _point;
      t_histo_img    _histo;

      ///\}

      /// Centers description.
      /// \{
      /// \brief Centers are described by the first moment of their group.
      ///
      /// Centers are describe by their group attributes like the number of
      /// pixels wich are relying on, the mass center of the group and the
      /// homogeneity of the group. The variance is used as indicator for the
      /// convergence. The number of pixels is used as integrity indicator.
      /// A center with no point is a non sense. Theses informations are updated
      /// after each kmean iteration.

      t_number_img   _number;   // k x 1
      t_mean_img     _mean;     // k x 1
      t_variance_img _variance; // k x 1 within group

      /// \}

      /// Greylevels description.
      /// \{
      /// \brief The information are concerned with the greylevel input image.
      ///
      /// The group image allow us to decide which greylevel (and of course
      /// which pixel) is assigned to a center. The distance image give us a
      /// clue on how a greylevel could contribute to a center. The summation
      /// over the greylevels of a center give us the within variance.

      t_group_img    _group;    // g x 1 because dim(t_value) = 1
      t_distance_img _distance; // label x graylevel

      /// \}

      /// Debugging, calibrating and testing results.
      /// \{
      /// \brief Some exports information to control the results.
      ///
      /// We come back in the input space. Label is the image which associates
      /// each pixel to its center. Color is the image which gives a random rgb
      /// color to each label. We can then visualize the regions of the same
      /// label (assigned to the same center) in the image. The mean image
      /// associate the mean of each pixel center to each pixel. We obtain thus
      /// a rebuilded image.

      t_label_dbg    _label_dbg;
      t_color_dbg    _color_dbg;
      t_mean_dbg     _mean_dbg;

      /// \}

      /// Debugging, calibrating and testing convergence.
      /// \{
      /// \brief Trace the variance and the center evolution.
      ///
      /// The mean_cnv image is a trace of the evolution of the mean' signal.
      /// We can observe the variation of the mean of each center along the
      /// kmean loop or along the different launch. The variance_cnv is a trace
      /// of the within variance along the kmean loop or along the different
      /// launch.

      t_mean_cnv     _mean_cnv;
      t_variance_cnv _variance_cnv;

      /// \}
    };

#ifndef MLN_INCLUDE_ONLY

    //--------------------------------------------------------------------------
    // Constructor
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    kmean2d<T,n>::kmean2d(const t_point_img& point,
			  const unsigned     k_center,
			  const unsigned     watch_dog,
			  const unsigned     n_times)
    {
      trace::entering("mln::clustering::kmean2d::kmean2d");
      mln_precondition(point.is_valid());

      _k_center          = k_center;
      _watch_dog         = watch_dog;
      _n_times           = n_times;
      _point             = point;
      _histo             = data::compute(accu::meta::stat::histo2d(),_point);

      // Results aren't valid since they aren't available
      _is_number_valid   = false;
      _current_step      = 0;
      _current_launching = 0;

      for (unsigned i = 0; i < _k_center; ++i)
      {
	_number.append(literal::zero);
	_mean.append(literal::zero);
	_variance.append(literal::zero);
      }

      _group.init_(box2d(point2d(mln_min(t_value_comp0),
				 mln_min(t_value_comp1)),
			 point2d(mln_max(t_value_comp0),
				 mln_max(t_value_comp1))));

      for (unsigned i = 0; i < _k_center; ++i)
      {
	t_distance_val img(box2d(point2d(mln_min(t_value_comp0),
					 mln_min(t_value_comp1)),
				 point2d(mln_max(t_value_comp0),
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

      trace::exiting("mln::clustering::kmean2d::kmean2d");
    }

    //--------------------------------------------------------------------------
    // Mutators and accessors
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::set_point(t_point_img& point)
    {
      trace::entering("mln::clustering::kmean2d::set_point");

      _point = point;

      trace::exiting("mln::clustering::kmean2d::set_point");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::set_histo(t_histo_img& histo)
    {
      trace::entering("mln::clustering::kmean2d::set_histo");

      _histo = histo;

      trace::exiting("mln::clustering::kmean2d::set_histo");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::set_number(t_number_img& number)
    {
      trace::entering("mln::clustering::kmean2d::set_number");

      _number = number;

      trace::exiting("mln::clustering::kmean2d::set_number");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::set_mean(t_mean_img& mean)
    {
      trace::entering("mln::clustering::kmean2d::set_mean");

      _mean = mean;

      trace::exiting("mln::clustering::kmean2d::set_mean");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::set_variance(t_variance_img& variance)
    {
      trace::entering("mln::clustering::kmean2d::set_variance");

      _variance = variance;

      trace::exiting("mln::clustering::kmean2d::set_variance");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::set_group(t_group_img& group)
    {
      trace::entering("mln::clustering::kmean2d::set_group");

      _group = group;

      trace::exiting("mln::clustering::kmean2d::set_group");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::set_distance(t_distance_img& distance)
    {
      trace::entering("mln::clustering::kmean2d::set_distance");

      _distance = distance;

      trace::exiting("mln::clustering::kmean2d::set_distance");
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_point_img& kmean2d<T,n>::get_point()
    {
      trace::entering("mln::clustering::kmean2d::get_point");

      trace::exiting("mln::clustering::kmean2d::get_point");
      return _point;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_histo_img& kmean2d<T,n>::get_histo()
    {
      trace::entering("mln::clustering::kmean2d::get_histo");

      trace::exiting("mln::clustering::kmean2d::get_histo");
      return _histo;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_number_img& kmean2d<T,n>::get_number()
    {
      trace::entering("mln::clustering::kmean2d::get_number");

      trace::exiting("mln::clustering::kmean2d::get_number");
      return _number;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_mean_img& kmean2d<T,n>::get_mean()
    {
      trace::entering("mln::clustering::kmean2d::get_mean");

      trace::exiting("mln::clustering::kmean2d::get_mean");
      return _mean;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_variance_img& kmean2d<T,n>::get_variance()
    {
      trace::entering("mln::clustering::kmean2d::get_variance");

      trace::exiting("mln::clustering::kmean2d::get_variance");
      return _variance;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_group_img& kmean2d<T,n>::get_group()
    {
      trace::entering("mln::clustering::kmean2d::get_group");

      trace::exiting("mln::clustering::kmean2d::get_group");
      return _group;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_distance_img& kmean2d<T,n>::get_distance()
    {
      trace::entering("mln::clustering::kmean2d::get_distance");

      trace::exiting("mln::clustering::kmean2d::get_distance");
      return _distance;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_color_dbg& kmean2d<T,n>::get_color_dbg()
    {
      trace::entering("mln::clustering::kmean2d::get_color_dbg");

      trace::exiting("mln::clustering::kmean2d::get_color_dbg");
      return _color_dbg;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_mean_dbg& kmean2d<T,n>::get_mean_dbg()
    {
      trace::entering("mln::clustering::kmean2d::get_mean_dbg");

      trace::exiting("mln::clustering::kmean2d::get_mean_dbg");
      return _mean_dbg;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_label_dbg& kmean2d<T,n>::get_label_dbg()
    {
      trace::entering("mln::clustering::kmean2d::get_label_dbg");

      trace::exiting("mln::clustering::kmean2d::get_label_dbg");
      return _label_dbg;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_mean_cnv& kmean2d<T,n>::get_mean_cnv()
    {
      trace::entering("mln::clustering::kmean2d::get_mean_cnv");

      trace::exiting("mln::clustering::kmean2d::get_mean_cnv");
      return _mean_cnv;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_variance_cnv& kmean2d<T,n>::get_variance_cnv()
    {
      trace::entering("mln::clustering::kmean2d::get_variance_cnv");

      trace::exiting("mln::clustering::kmean2d::get_variance_cnv");
      return _variance_cnv;
    }

    template <typename T, unsigned n>
    inline
    typename kmean2d<T,n>::t_mean_img& kmean2d<T,n>::to_result()
    {
      trace::entering("mln::clustering::kmean2d::to_result");

      trace::exiting("mln::clustering::kmean2d::to_result");
      return _mean_min;
    }

    //--------------------------------------------------------------------------
    // Printing temporary results
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::print_mean()
    {
      trace::entering("mln::clustering::kmean2d::print_mean");

      mln_eiter(t_mean_img)  l(_mean);

      for_all(l)
      {
	std::cout << "mean("      << l.index_();
	std::cout << ") = [r="    << _mean[l.index_()][0];
	std::cout << ", g="       << _mean[l.index_()][1];
	std::cout << ", b="       << _mean[l.index_()][2];
	std::cout << "]"          << std::endl;
      }

      trace::exiting("mln::clustering::kmean2d::print_mean");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::print_number()
    {
      trace::entering("mln::clustering::kmean2d::print_number");

      mln_eiter(t_number_img)  l(_number);

      for_all(l)
      {
	std::cout << "number(" << l.index_();
	std::cout << ") = "    << _number[l.index_()];
	std::cout << std::endl;
      }

      trace::exiting("mln::clustering::kmean2d::print_number");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::print_variance()
    {
      trace::entering("mln::clustering::kmean2d::print_variance");

      mln_eiter(t_variance_img)  l(_number);

      for_all(l)
      {
	std::cout << "variance(" << l.index_();
	std::cout << ") = "      << _variance[l.index_()];
	std::cout << std::endl;
      }

      trace::exiting("mln::clustering::kmean2d::print_variance");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::print_histo()
    {
      trace::entering("mln::clustering::kmean2d::print_histo");

      mln_piter(t_histo_img)  rgb(_histo.domain());

      for_all(rgb)
      {
	if (0 < _histo(rgb))
	{
	  std::cout << "histo(r="  << rgb.row();
	  std::cout << ", g="      << rgb.col();
	  std::cout << ")= "       << _histo(rgb);
	  std::cout << std::endl;
	}
      }

      trace::exiting("mln::clustering::kmean2d::print_histo");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::print_group()
    {
      trace::entering("mln::clustering::kmean2d::print_group");

      mln_piter(t_group_img)  rgb(_group.domain());

      for_all(rgb)
      {
	if (0 < _histo(rgb))
	{
	  std::cout << "group(r="  << rgb.row();
	  std::cout << ", g="      << rgb.col();
	  std::cout << ")= "       << _group(rgb);
	  std::cout << std::endl;
	}
      }

      trace::exiting("mln::clustering::kmean2d::print_group");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::print_distance()
    {
      trace::entering("mln::clustering::kmean2d::print_distance");

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
	    std::cout << ")= "          << _distance[l.index_()](rgb);
	    std::cout << std::endl;
	  }
	}
      }

      trace::exiting("mln::clustering::kmean2d::print_distance");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::print_point()
    {
      trace::entering("mln::clustering::kmean2d::print_point");

      mln_piter(t_point_img)  p(_point.domain());

      for_all(p)
      {
	std::cout << "point(r="  << p.row();
	std::cout << ", c="      << p.col();
	std::cout << ")= "       << _point(p);
	std::cout << std::endl;
      }

      trace::exiting("mln::clustering::kmean2d::print_point");
    }



    //--------------------------------------------------------------------------
    // Initialization of centers
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::init_mean_random()
    {
      trace::entering("mln::clustering::kmean2d::init_mean_random");

      t_value_comp0               min_comp0 = mln_min(t_value_comp0);
      t_value_comp0               max_comp0 = mln_max(t_value_comp0);
      t_value_comp1               min_comp1 = mln_min(t_value_comp1);
      t_value_comp1               max_comp1 = mln_max(t_value_comp1);

      mln_eiter(t_mean_img)       l(_mean);

      for_all(l)
      {
	_mean[l.index_()][0] = (rand() % (max_comp0-min_comp0)) + min_comp0;
	_mean[l.index_()][1] = (rand() % (max_comp1-min_comp1)) + min_comp1;
      }

      trace::exiting("mln::clustering::kmean2d::init_mean_random");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::init_mean()
    {
      trace::entering("mln::clustering::kmean2d::init_mean");

      init_mean_random();

      trace::exiting("mln::clustering::kmean2d::init_mean");
    }

    //--------------------------------------------------------------------------
    // Computations of distance, group, center, within variance
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::update_distance()
    {
      trace::entering("mln::clustering::kmean2d::update_distance");

      for (unsigned i = 0; i < _k_center; ++i)
      {
	mln_piter(t_distance_val) d(_distance[i].domain());

	for_all(d)
	{
	  // the square distance
	  t_result1d diff2_row = math::sqr(d.row() - _mean[i][0]);
	  t_result1d diff2_col = math::sqr(d.col() - _mean[i][1]);
	  t_result1d diff2_sli = math::sqr(d.sli() - _mean[i][2]);
	  _distance[i](d)      = _histo(d)*(diff2_row + diff2_col + diff2_sli);
	  /*
	    std::cout << "row      : " << d.row()                  << std::endl;
	    std::cout << "col      : " << d.col()                  << std::endl;
	    std::cout << "histo    : " << _histo(point1d(d.row())) << std::endl;
	    std::cout << "center   : " << _mean(point1d(d.col()))  << std::endl;
	    std::cout << "distance : " << _distance(d)             << std::endl;
	    std::cout << "--------------------------------------"  << std::endl;
	  */
	}
      }

      trace::exiting("mln::clustering::kmean2d::update_distance");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::update_group()
    {
      trace::entering("mln::clustering::kmean2d::update_group");

      mln_piter(t_group_img) rgb(_group.domain());

      for_all(rgb)
      {
	mln_eiter(t_distance_img) l(_distance);
	t_result1d                min   = mln_max(t_result1d);
	t_label                   label = mln_max(t_label);

	//std::cout << "g = " << g << std::endl;

	for_all(l)
	{
	  if (min > _distance[l.index_()](rgb))
	  {
	    min   = _distance[l.index_()](rgb);
	    label = l.index_();
	  }

	  //std::cout << "d" << l << " = " <<
	  //  _distance(point2d(g.ind(), l.ind())) << std::endl;
	}

	//std::cout << "g = " << g << std::endl;

	_group(rgb) =  label;
	//std::cout << "group = " << _group(g) << std::endl;
	//std::cout << "-----------" << std::endl;
      }

      trace::exiting("mln::clustering::kmean2d::update_group");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::update_mean()
    {
      trace::entering("mln::clustering::kmean2d::update_mean");

      /// FIXME VERIFIER QUE L'ON PEUT OBTENIR UNE IMAGE EN NDG SIGNE

      // avec g    le niveau de gris (signed or not signed)
      //      w[g] la classe de g sous forme d'image
      //      h[g] l'histogramme de l'image sous forme d'image
      //
      // Vg in [0..255]
      // si w[g] = l
      //    c[l] += h(g) * g
      //    n[l] += h(g)
      //
      // c[l] /= n


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
	// peut être faut-il le decomposer par composantes
	_mean[_group(rgb)][0] += rgb.row() * _histo(rgb);
	_mean[_group(rgb)][1] += rgb.col() * _histo(rgb);
	_number(_group(rgb))  += _histo(rgb);
      }

      mln_eiter(t_mean_img)   l(_mean);

      /*
      for_all(l)
      {
	std::cout << "c(" << l << ") = " << _mean(l) << std::endl;
	std::cout << "n(" << l << ") = " << _number(l) << std::endl;
      }
      */
      for_all(l)
      {
	// State the stopping propagation Nan flag
	_is_number_valid = (0 != _number[l.index_()]);

	// Emergency exit
	if (!_is_number_valid)
	  break;

	// Compute the mean
	_mean[l.index_()] /= _number[l.index_()];

	// Debugging
	//std::cout << "c" << l << " = " << _mean(l) << std::endl;
      }

      trace::exiting("mln::clustering::kmean2d::update_mean");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::update_variance()
    {
      trace::entering("mln::clustering::kmean2d::update_variance");

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
	//std::cout << "v(" << l << ") = " << _variance(l) << std::endl;
      }

      //std::cout << "result" << result << std::endl;

      trace::exiting("mln::clustering::kmean2d::update_variance");
    }

    //--------------------------------------------------------------------------
    // Debugging tools
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::build_label_dbg()
    {
      trace::entering("mln::clustering::kmean2d::build_label_dbg");

      mln_piter(t_point_img) pi(_point.domain());
      mln_piter(t_label_dbg) po(_label_dbg.domain());

      for_all_2(pi, po)
      {
	  t_value    val = _point(pi);
	  t_label    grp = _group(point2d(val.red(), val.green()));

	  // As label zero has got a particular semantic, the first label is one
	  _label_dbg(po) = ++grp;
      }

      trace::exiting("mln::clustering::kmean2d::build_label_dbg");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::build_mean_dbg()
    {
      trace::entering("mln::clustering::kmean2d::build_mean_dbg");

      mln_piter(t_mean_dbg)  p(_mean_dbg.domain());

      for_all(p)
      {
	_mean_dbg(p).red()   = static_cast<unsigned>(_mean[_label_dbg(p)][0]);
	_mean_dbg(p).green() = static_cast<unsigned>(_mean[_label_dbg(p)][1]);
	_mean_dbg(p).blue()  = static_cast<unsigned>(_mean[_label_dbg(p)][2]);
      }

      trace::exiting("mln::clustering::kmean2d::build_mean_dbg");
    }


    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::build_all_dbg()
    {
      trace::entering("mln::clustering::kmean2d::build_all_dbg");
      build_label_dbg();
      //build_mean_dbg();
      _mean_dbg  = labeling::mean_values(_point, _label_dbg, _k_center);
      _color_dbg = labeling::colorize(value::rgb8(), _label_dbg);

      trace::exiting("mln::clustering::kmean2d::build_all_dbg");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::update_cnv()
    {
      trace::entering("mln::clustering::kmean2d::update_cnv");

      _variance_cnv[_current_launching](point1d(_current_step))
	= _within_variance;

      mln_eiter(t_mean_img) l(_mean);

      for_all(l)
      {
	_mean_cnv[l.index_()][_current_launching](point1d(_current_step))
	  = _mean[l.index_()];
      }

      trace::exiting("mln::clustering::kmean2d::update_cnv");
    }

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::finalize_cnv()
    {
      trace::entering("mln::clustering::kmean2d::finalize_cnv");

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

      trace::exiting("mln::clustering::kmean2d::finalize_cnv");
    }


    //--------------------------------------------------------------------------
    // Checking the validity of the results
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    bool kmean2d<T,n>::is_descent_valid()
    {
      trace::entering("mln::clustering::kmean2d::is_descent_valid");

      bool result = _is_number_valid && (_current_step < _watch_dog);

      trace::exiting("mln::clustering::kmean2d::is_descent_valid");
      return result;
    }


    //--------------------------------------------------------------------------
    // Main loop
    //--------------------------------------------------------------------------

    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::launch_one_time()
    {
      trace::entering("mln::clustering::kmean2d::launch_one_time");

      //std::cout << "----------------------------------------" << std::endl;

      // Initialization to start the descent
      t_result1d old_variance = mln_max(t_result1d);
      _within_variance        = mln_max(t_result1d);
      _current_step           = 0;

      // Build the first group and evaluate its caharacteristics
      init_mean();
      update_distance();

      //print_point();
      //print_histo();
      //print_mean();
      //print_distance();

      //std::cout << "first_variance : " << old_variance << std::endl;

      // Execute the descent
      do
      {
	old_variance = _within_variance;

	update_group();

	//print_group();

	update_mean(); // update _is_number_valid

	//print_mean();

	// Stopping Nan propagation
	if (!_is_number_valid)
	  break;

	update_distance();

	//print_distance();

	update_variance(); // update _within_variance

	//print_variance();

	// Debugging code
	update_cnv();

	std::cout << "_current_step    : " << _current_step    << std::endl;
	std::cout << "_within_variance : " << _within_variance << std::endl;

	++_current_step;
      }
      while (_current_step < _watch_dog && _within_variance < old_variance);

      //std::cout << "----------------------------------------" << std::endl;

      finalize_cnv();
      //build_all_dbg();

      trace::exiting("mln::clustering::kmean2d::launch_one_time");
    }


    template <typename T, unsigned n>
    inline
    void kmean2d<T,n>::launch_n_times()
    {
      trace::entering("mln::clustering::kmean2d::launch_n_times");

      //std::cout << "watch_dog : " << _watch_dog << std::endl;
      //std::cout << "n_times   : " << _n_times   << std::endl;

      // number of times we reexecute launch_one_time without any success
      unsigned tries     = 0;

      _variance_min      = mln_max(t_result1d);
      _current_launching = 0;

      // Execute the descent n times
      while (_current_launching < _n_times)
      {
	launch_one_time();

	if (is_descent_valid() || _N_TRIES < tries)
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
      build_all_dbg();

      trace::exiting("mln::clustering::kmean2d::launch_n_times");
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::clustering

} // end of namespace mln

#endif // ! MLN_CLUSTERING_KMEAN2D_HH
