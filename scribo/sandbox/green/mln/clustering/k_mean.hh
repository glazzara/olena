// Copyright (C) 2007,2008,2009,2010, 2011 EPITA LRDE
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

#ifndef MLN_CLUSTERING_K_MEAN_HH
# define MLN_CLUSTERING_K_MEAN_HH

/// \file
///
/// \brief Implement the K MEAN algorithm with matrix and vectors.
///
/// This is the first impementation of the kmean algorithm. There is no
/// specific optimisation but every part of the algorithm is here and
/// it works. As it used vectors and matrices, it is not very convenient to
/// work with this version.
///
/// \verbatim
/// This is the main lines of the kmean algorithm:
///
/// PROCEDURE MAIN
/// BEGIN
///   DO
///     - CALL ASSIGNEMENT STEP
///     - CALL UPDATE STEP
///   LOOP UNTIL CONVERGENCE
/// END
///
/// PROCEDURE ASSIGNEMENT STEP
/// BEGIN
///   - COMPUTE THE DISTANCE MATRIX FROM POINTS TO CENTERS
///   - COMPUTE GROUP MATRIX WHERE EACH POINT IS ASSIGNED TO THE NEAREST CENTER
/// END
///
/// PROCEDURE UPDATE STEP
/// BEGIN
///   - COMPUTE THE MEAN OF THE GROUPS WHICH ARE NOW THE NEW CENTERS
/// END
/// \endvarbitim
///
/// The following sample is the standard use of this first
/// implementation. The number of centers is a decision left to the
/// final user. As the representation uses vectors and matices, size
/// of each objects must be known at compile time, and so NB_POINT must be
/// a constant expression (mln::geom::nsites couldn't be called).
///
/// #include <mln/clustering/k_mean.hh>
/// #include <mln/core/image/image2d.hh>
/// #include <mln/img_path.hh>
/// #include <mln/io/ppm/load.hh>
/// #include <mln/trait/value_.hh>
/// #include <mln/value/rgb8.hh>
///
/// #define NB_CENTER  9
/// #define NB_POINT   (128*128)
/// #define POINT_SIZE mln_dim(mln::value::rgb8)
/// #define POINT_TYPE double
///
/// int main()
/// {
///   typedef mln::value::rgb8  t_rgb8;
///   mln::image2d<t_rgb8>      img_rgb8;
///
///   mln::io::ppm::load(img_ref, OLENA_IMG_PATH"/house.ppm");
///   mln::clustering::k_mean<NB_POINT,NB_CENTER,POINT_SIZE,POINT_TYPE> kmean;
///
///   kmean.init_point(img_rgb8);
///   kmean.loop(img_rgb8);
///
///   return 0;
/// }

# include <limits.h>
# include <iostream>

# include <mln/algebra/mat.hh>
# include <mln/algebra/vec.hh>

# include <mln/core/concept/image.hh>
# include <mln/core/contract.hh>
# include <mln/core/macros.hh>

# include <mln/math/min.hh>
# include <mln/norm/l2.hh>

# include <mln/trace/entering.hh>
# include <mln/trace/exiting.hh>

# include <mln/trait/value_.hh>

namespace mln
{

  namespace clustering
  {

    // Forward declaration.
    template <unsigned n, unsigned k, unsigned p, typename T>
    struct k_mean;

  } // end of namespace mln::clustering

  namespace clustering
  {

    /// \brief Implements the K MEAN algorithm with matrix and vectors.
    ///
    /// Param n is the size of the sample (the number of point to be
    /// classified). Param k is the number of centers, at the end,
    /// each point is link to one the nearest center.  Param p is the
    /// number of attributes of a point, in fact its dimension (int_u8
    /// = 1, rgb8 = 3, etc ...). Param T is the type used for
    /// computations, the type of the point, typiccaly float or
    /// double.
    ///
    /// \ingroup modclustering
    template <unsigned n, unsigned k, unsigned p, typename T>
    struct k_mean
    {

      /// Constructors and destructors.
      /// \{
      /// \brief Allocate or deallocate the data structure.
      ///
      /// Allocate or deallocate the _point, _distance, _group,
      /// _center, _variance data structure with the correct size.
      k_mean();
      ~k_mean();
      /// \}


      /// Accessors.
      /// \{
      /// \brief Hack to view temporary results of the kmean algorithm.
      ///
      /// These methods are necessary for unitary testing and debugging.
      algebra::mat<n, p, T>& get_point();
      algebra::mat<k, p, T>& get_center();
      algebra::mat<n, k, T>& get_distance();
      algebra::mat<n, k, T>& get_group();
      algebra::vec<k, T>&    get_variance();
      /// \}

      /// Tools.
      /// \{
      /// \brief Define elementary function that are not available from milena.
      ///
      /// These methods are not interesting from the kmean'point of
      /// view but are required for the gluing process.


      /// \brief Return the euclidian distance beetween two vectors x and y.
      ///
      /// Compute the f$\sqrt(\sum_{i=0}^P ((x_i - y_i)^2)$f
      ///
      /// \param[in] x a vector of dimension p.
      /// \param[in] y a second vector of dimension p.
      /// \return the distance between x and y.
      mln_sum_product(T,T) euclidian_distance(const algebra::vec<p,T>& x,
					      const algebra::vec<p,T>& y) const;


      /// \brief Return a stack copy of the ith column.
      ///
      /// \param[in] m a matrix of dimension r x q.
      /// \param[in] _col the index of the selected column.
      /// \return a stack copy of the ith column as a vector of dimension q.
      template <unsigned q, unsigned r>
      algebra::vec<q,T> col(const algebra::mat<r, q, T>& m,
			    const unsigned _col) const;


      /// \brief Return a heap copy of the ith col.
      ///
      /// \param[in] m a matrix of dimension r x q.
      /// \param[in] _col the index of the selected column.
      /// \return a heap copy of the ith column as a vector of dimension q.
      template <unsigned q, unsigned r>
      algebra::vec<q,T>* ptr_col(const algebra::mat<r, q, T>& m,
				 const unsigned _col) const;


      /// \brief Return a stack copy of the ith row.
      ///
      /// \param[in] m a matrix of dimension r x q.
      /// \param[in] _row the index of the selected row.
      /// \return a stack copy of the ith row as a vector of dimension q.
      template <unsigned q, unsigned r>
      algebra::vec<r,T> row(const algebra::mat<r, q, T>& m,
			    const unsigned _row) const;


      /// \brief Divide inplace a column of the matrix by a scalar.
      ///
      /// \param[in] m a matrix of dimension r x q.
      /// \param[in] _col the index of the selected column.
      /// \param[in] value the scalar by which dividing the column.
      template <unsigned q, unsigned r>
      void div_col(algebra::mat<r, q, T>& m,
		   const unsigned         _col,
		   const T                value);


      /// \brief Sum all the elements of one row.
      ///
      /// \param[in] m a matrix of dimension r x q.
      /// \param[in] _row the index of the selected row.
      /// \return the sum of every attributes of the row.
      template <unsigned q, unsigned r>
      mln_sum(T) sum_row(const algebra::mat<r, q, T>& m,
			 const unsigned _row) const;


      /// \brief Return the minimum of a vector.
      ///
      /// \param[in] x a vector of dimension q.
      /// \return the mininum of that input vector.
      template <unsigned q, typename M>
      M min_col(const algebra::vec<q, M>& x) const;
      /// \}


      /// Initializations.
      /// \{
      /// \brief Initialize the _point and _center data structure.
      ///
      /// The _point data structure is fed with the value of the input
      /// image. The _center data structure is fed with random points
      /// taken from the input image.
      template <typename I>
      void init_point(const Image<I>& _input);
      void init_center();
      /// \}


      /// Computations.
      /// \{
      /// \brief Define the core routines of the kmean algorithm.
      ///
      /// The update_distance, update_group, update_center and update_variance
      /// define the core kmean routines. If some optimization muste be done,
      /// one may looks here.


      /// \brief Update the _distance data structure.
      ///
      /// Compute the square distance between each point of the data
      /// cloud and each center.
      void update_distance();

      /// \brief Update the _group data structure.
      ///
      /// For each point of the data cloud, find the group which has
      /// the minimum distance from it.
      void update_group();

      /// \brief Update the _center data structure.
      ///
      /// For each group, compute the center of mass in therm of
      /// points of the data cloud.
      void update_center();

      /// \brief Update the _variance.
      ///
      /// Compute the within variance. Sum every shortest distance.
      T    update_variance();
      /// \}

      /// \brief Define the orchestration of the kmean core routines.
      ///
      /// \param[in] _input the initial image which initialize the data cloud.
      ///
      /// The orchestration is in a fact a loop which call one after
      /// each other the core routines. Ending the loop means managing
      /// the statistical convergence or having some clues of
      /// divergence (watch_dog).
      template <typename I>
      void loop(const Image<I>& _input);

    private:
      /// \brief watch_dog define the maximum of iterations that can be done.
      ///
      /// That constant help us to decide if divergence has been
      /// occurred or not.
      static const unsigned watch_dog = 100;


      /// \brief _points contains the concatenation of every data points.
      ///
      /// One data point is a vector 1 x p where p is the number of attributes.
      /// So _points is a matrix n x P where n is the number of data points.
      algebra::mat<n, p, T>* _point;


      /// \brief _distance contains all the euclidian distances between points
      /// and the centers.
      ///
      /// _distance is a matrix n x k where n is the number of data points and
      /// k the number of centers.
      algebra::mat<n, k, mln_sum_product(T,T)>* _distance;


      /// \brief _group contains the point assignement to one center.
      ///
      /// _group is a boolean matrix n x k where n is the number of data points
      /// and k the number of centers.
      algebra::mat<n, k, T>* _group;


      /// \brief _center contains the coordonnate of the k centers.
      ///
      /// _center is a matrix k x p where k is the number of centers
      /// and p is the number of attributes.
      algebra::mat<k, p, T>* _center;


      /// \brief _variance contains the variance of each group.
      ///
      /// _variance is a vector 1 x k where k is the number of centers.
      algebra::vec<k, T>* _variance;
    };

#ifndef MLN_INCLUDE_ONLY

    //--------------------------------------------------------------------------
    // Constructor and destructor
    //--------------------------------------------------------------------------

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    k_mean<n,k,p,T>::k_mean()
    {
      trace::entering("mln::clustering::k_mean::cstor");

      _point    = new algebra::mat<n, p, T>();
      _distance = new algebra::mat<n, k, mln_sum_product(T,T)>();
      _group    = new algebra::mat<n, k, T>();
      _center   = new algebra::mat<k, p, T>();
      _variance = new algebra::vec<k,T>();

      mln_postcondition(_point    != 0);
      mln_postcondition(_distance != 0);
      mln_postcondition(_group    != 0);
      mln_postcondition(_center   != 0);
      mln_postcondition(_variance != 0);

      trace::exiting("mln::clustering::k_mean::cstor");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    k_mean<n,k,p,T>::~k_mean()
    {
      trace::entering("mln::clustering::k_mean::dstor");

      delete _point;
      delete _distance;
      delete _group;
      delete _center;
      delete _variance;

      trace::exiting("mln::clustering::k_mean::dstor");
    }

    //--------------------------------------------------------------------------
    // Accessors
    //--------------------------------------------------------------------------

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    algebra::mat<n, p, T>&
    k_mean<n,k,p,T>::get_point()
    {
      trace::entering("mln::clustering::k_mean::get_point");
      trace::exiting("mln::clustering::k_mean::get_point");

      return *_point;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    algebra::mat<k, p, T>&
    k_mean<n,k,p,T>::get_center()
    {
      trace::entering("mln::clustering::k_mean::get_center");
      trace::exiting("mln::clustering::k_mean::get_center");

      return *_center;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    algebra::mat<n, k, T>&
    k_mean<n,k,p,T>::get_distance()
    {
      trace::entering("mln::clustering::k_mean::get_distance");
      trace::exiting("mln::clustering::k_mean::get_distance");

      return *_distance;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    algebra::mat<n, k, T>&
    k_mean<n,k,p,T>::get_group()
    {
      trace::entering("mln::clustering::k_mean::get_group");
      trace::exiting("mln::clustering::k_mean::get_group");

      return *_group;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    algebra::vec<k, T>&
    k_mean<n,k,p,T>::get_variance()
    {
      trace::entering("mln::clustering::k_mean::get_variance");
      trace::exiting("mln::clustering::k_mean::get_variance");

      return *_variance;
    }

    //--------------------------------------------------------------------------
    // Tools
    //--------------------------------------------------------------------------

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <unsigned q, unsigned r>
    inline
    algebra::vec<r, T> k_mean<n,k,p,T>::row(const algebra::mat<r, q, T>& m,
					    const unsigned _row) const
    {
      trace::entering("mln::clustering::k_mean::row");
      mln_precondition(q > _row);

      algebra::vec<r, T> result;

      for (unsigned i = 0; i < r; ++i)
	result[i] = m(i, _row);

      trace::exiting("mln::clustering::k_mean::row");
      return result;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <unsigned q, unsigned r>
    inline
    algebra::vec<q,T> k_mean<n,k,p,T>::col(const algebra::mat<r, q, T>& m,
					   const unsigned _col) const
    {
      trace::entering("mln::clustering::k_mean::col");
      mln_precondition(r > _col);

      algebra::vec<q, T> result;

      for (unsigned j = 0; j < q; ++j)
	result[j] = m(_col, j);

      trace::exiting("mln::clustering::k_mean::col");
      return result;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <unsigned q, unsigned r>
    inline
    algebra::vec<q,T>* k_mean<n,k,p,T>::ptr_col(const algebra::mat<r, q, T>& m,
						const unsigned _col) const
    {
      trace::entering("mln::clustering::k_mean::ptr_col");
      mln_precondition(r > _col);

      algebra::vec<q, T> *result = new algebra::vec<q, T>();

      for (unsigned j = 0; j < q; ++j)
	(*result)[j] = m(_col, j);

      trace::exiting("mln::clustering::k_mean::ptr_col");
      return result;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <unsigned q, unsigned r>
    inline
    mln_sum(T) k_mean<n,k,p,T>::sum_row(const algebra::mat<r, q, T>& m,
					const unsigned               _row) const
    {
      trace::entering("mln::clustering::k_mean::sum_row");
      mln_precondition(q > _row);

      mln_sum(T) result;

      for (unsigned j = 0; j < r; ++j)
	result += m(j, _row);

      trace::exiting("mln::clustering::k_mean::sum_row");
      return result;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <unsigned q, unsigned r>
    inline
    void k_mean<n,k,p,T>::div_col(algebra::mat<r, q, T>& m,
				  const unsigned _col,
				  const T        value)
    {
      trace::entering("mln::clustering::k_mean::div_col");
      mln_precondition(r > _col);

      for (unsigned j = 0; j < q; ++j)
	m(_col, j) /= value;

      trace::exiting("mln::clustering::k_mean::div_col");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <unsigned q, typename M>
    inline
    M k_mean<n,k,p,T>::min_col(const algebra::vec<q, M>& x) const
    {
      trace::entering("mln::clustering::k_mean::min_col");

      M result = x[0];

      for (unsigned i = 1; i < q; ++i)
	result = math::min(result, x[i]);

      trace::exiting("mln::clustering::k_mean::min_col");
      return result;
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    mln_sum_product(T,T)
    k_mean<n,k,p,T>::euclidian_distance(const algebra::vec<p, T>& x,
					const algebra::vec<p, T>& y) const
    {
      trace::entering("mln::clustering::k_mean::euclidian_distance");
      //vec<p, double> tmp = norm::l2(x - y);
      const algebra::vec<p, double> tmp    = (x - y);
      mln_sum_product(T,T)          result = tmp*tmp;

      trace::exiting("mln::clustering::k_mean::euclidian_distance");
      return result;
    }


    //--------------------------------------------------------------------------
    // Initializations of points and centers
    //--------------------------------------------------------------------------

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <typename I>
    inline
    void k_mean<n,k,p,T>::init_point(const Image<I>& _input)
    {
      trace::entering("mln::clustering::k_mean::init");

      const I&               input = exact(_input);
      algebra::mat<n, p, T>& point = *_point;

      mln_precondition(input.is_valid());
      mln_precondition(n == input.nrows() * input.ncols());
      //mln_precondition(n == input.nrows());
      //mln_precondition(n == input.nrows() * input.ncols() * input.nslis());

      mln_piter(I) pi(input.domain());
      unsigned     i = -1;

      for_all(pi)
      {
	//std::cout << pi << std::endl;

	++i;
	for (unsigned j = 0; j < p; ++j)
	{

	  point(i,j) = input(pi).comp(j);
	  //point(i,j) = input(pi);
	}
      }

      trace::exiting("mln::clustering::k_mean::init");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::init_center()
    {
      trace::entering("mln::clustering::k_mean::init_center");

      algebra::mat<n, p, T>& point  = *_point;
      algebra::mat<k, p, T>& center = *_center;

      // A random point is choosen to be the initial value for a center.
      for (unsigned i = 0; i < k; ++i)
      {
	unsigned random = rand() % n;

	for (unsigned j = 0; j < p; ++j)
	{
	  center(i,j) = point(random, j);
	}

	//std::cout << "center(" << i << ")" << col(center, i) << std::endl;
      }

      trace::exiting("mln::clustering::k_mean::init_center");
    }


    //--------------------------------------------------------------------------
    // Computations of distance, group, center, within variance
    //--------------------------------------------------------------------------

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::update_distance()
    {
      trace::entering("mln::clustering::k_mean::update_distance");
      //mln::trace::quiet = true;

      // the result is stored in _distance matrix.
      algebra::mat<n, p, T>& point    = *_point;
      algebra::mat<n, k, T>& distance = *_distance;
      algebra::mat<k, p, T>& center   = *_center;

      // for all points in the data cloud.
      for (unsigned i = 0; i < n; ++i)
      {
	// for every center
	for (unsigned j = 0; j < k; ++j)
	{
	  distance(i,j) = euclidian_distance(col(point,i),col(center,j));
	}
      }
      //mln::trace::quiet = false;
      trace::exiting("mln::clustering::k_mean::update_distance");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::update_group()
    {
      trace::entering("mln::clustering::k_mean::update_group");

      algebra::mat<n, k, mln_sum_product(T,T)>& distance = *_distance;
      algebra::mat<n, k, T>&                    group    = *_group;

      // for each point
      for (unsigned i = 0; i < n; ++i)
      {
	// tell us the minimum distance from a specific point to a group
	mln_sum_product(T,T) min_dist = min_col(col(distance, i));

	// for each center
	for (unsigned j = 0; j < k; ++j)
	  group(i,j) = (min_dist == distance(i,j))? 1.0 : 0.0;
      }

      // mln_postcondition(sum(col(distance(i,j)) == 1) Vi
      trace::exiting("mln::clustering::k_mean::update_group");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::update_center()
    {
      trace::entering("mln::clustering::k_mean::update_center");

      algebra::mat<n, p, T>& point  = *_point;
      algebra::mat<k, p, T>& center = *_center;
      algebra::mat<n, k, T>& group  = *_group;

      //      center = (group.t() * point);

      for (unsigned i = 0; i < k; ++i)
      {
	for (unsigned j = 0; j < p; ++j)
	{
	  center(i,j) = literal::zero;

	  for (unsigned l = 0; l < n; ++l)
	    center(i,j) += group(l,i) * point(l,j);
	}
      }

      for (unsigned i = 0; i < k; ++i)
	div_col(center, i, sum_row(group, i));

      trace::exiting("mln::clustering::k_mean::update_center");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    T k_mean<n,k,p,T>::update_variance()
    {
      trace::entering("mln::clustering::k_mean::update_variance");

      algebra::vec<k, T>&    variance = *_variance;
      algebra::mat<n, k, T>& distance = *_distance;
      algebra::mat<n, k, T>& group    = *_group;
      T                      result   = literal::zero;

      // for every group
      for (unsigned i = 0; i < k; ++i)
      {
	variance[i] = row(group, i) * row(distance, i);
	result     += variance[i];
      }

      trace::exiting("mln::clustering::k_mean::update_variance");
      return result;
    }


    //--------------------------------------------------------------------------
    // Main loop
    //--------------------------------------------------------------------------

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <typename I>
    inline
    void k_mean<n,k,p,T>::loop(const Image<I>& _input)
    {
      trace::entering("mln::clustering::k_mean::loop");

      T within_variance = INT_MAX-1;
      T old_variance    = INT_MAX;

      init_point(_input);
      init_center();

      std::cout << "LOOP" << std::endl;

      for (unsigned i = 0; i < watch_dog && within_variance < old_variance; ++i)
      {
	update_distance();

	std::cout << "DISTANCE DONE" << std::endl;

	update_group();

	std::cout << "GROUP DONE" << std::endl;

	update_center();

	std::cout << "UPDATE CENTER DONE" << std::endl;

	old_variance    = within_variance;
	within_variance = update_variance();

	std::cout << i << " : " << within_variance << std::endl;
      }

      trace::exiting("mln::clustering::k_mean::loop");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::clustering

} // end of namespace mln::clustering

#endif // ! MLN_CLUSTERING_K_MEAN_HH
