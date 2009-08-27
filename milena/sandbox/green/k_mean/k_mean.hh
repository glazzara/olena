// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
#define MLN_CLUSTERING_K_MEAN_HH

/// \file
///
/// \brief Implements the K MEAN algorithm.
/// DO
/// - ASSIGNEMENT STEP
/// - UPDATE STEP
/// LOOP UNTIL CONVERGENCE
///
/// ASSIGNEMENT STEP
/// BEGIN
/// - COMPUTE THE DISTANCE MATRIX FROM POINTS TO CENTERS
/// - COMPUTE GROUP MATRIX WHERE EACH POINT IS ASSIGNED TO NEAREST CENTER
/// END
///
/// UPDATE STEP
/// BEGIN
/// - COMPUTE THE MEAN OF THE GROUPS WHICH ARE THE NEW CENTERS
/// END

#include <iostream>
#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>

#include <mln/core/contract.hh>

#include <mln/algebra/mat.hh>
#include <mln/algebra/vec.hh>

#include <mln/math/min.hh>
#include <mln/norm/l2.hh>

#include <mln/core/concept/image.hh>
#include <mln/core/macros.hh>

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
    
    //
    // PARAM : DIM VECTOR, NUMBER OF CLASSES
    // INPUT : MATRIX 1..N (SAMPLE)
    // WORK
    //

    // n is the size of the sample.
    // k is the number of centers.
    // p is the number of attributes for a point.
    // T is the type for computations.
    template <unsigned n, unsigned k, unsigned p, typename T>
    struct k_mean
    {
      /// \brief Initialize the matrix _point with the pixels of an image.
      ///
      /// \param[in] input an image which contains the data points.
      template <typename I>
      void init_point(const Image<I>&);
      void init_center();
      
      algebra::mat<n, p, T>& get_point();
      algebra::mat<k, p, T>& get_center();

      k_mean();
      ~k_mean();

      void update_distance();
      void update_group();
      void update_center();

      template <unsigned q, typename M>
      M min(const algebra::vec<q, M>& x) const;

      /// \brief Return the euclidian distance beetween vector x and vector y.
      ///
      /// Compute the f$\sqrt(\sum_{i=0}^P ((x_i - y_i)^2)$f
      ///
      /// \param[in] x a vector of dimension 1xP.
      /// \param[in] y a second vector of dimension 1xP.
      /// \return the distance between x and y.
      
      mln_sum_product(T,T) euclidian_distance(const algebra::vec<p,T>& x,
					      const algebra::vec<p,T>& y) const;
      


      /// \brief Return the ith column.
      ///
      /// \param[in] m a matrix of dimension {N or K}xP.
      /// \return the ith column as a vector of dimension 1xP.
      //template <unsigned q, unsigned r>
      //vec<q, T> row(const mat<r, q, T>& m, unsigned i) const;

      /// \brief Return the ith column.
      ///
      /// \param[in] m a matrix of dimension {N or K}xP.
      /// \return the ith column as a vector of dimension 1xP.
      template <unsigned q, unsigned r>
      algebra::vec<q,T> col(const algebra::mat<r, q, T>& m,
			    const unsigned _col) const;

    private:
      /// \brief _points contains the concatenation of every data points.
      ///
      /// One data point is a vector 1xP where P is the number of attributes.
      /// _points is a matrix NxP where N is the number of data points.
      algebra::mat<n, p, T>* _point;

      /// \brief _distance contains all the euclidian distances between points
      /// and the centers.
      ///
      /// _distance is a matrix NxK where N is the number of data points and
      /// K the number of centers.
      algebra::mat<n, k, mln_sum_product(T,T)>* _distance;

      /// \brief _group contains the point assignement to one center.
      ///
      /// _group is a boolean matrix NxK where N is the number of data points
      /// and K the number of centers.
      algebra::mat<n, k, T>* _group;

      /// \brief _center contains the coordonnate of the K centers.
      ///
      /// _center is a matrix KxP where K is the number of centers and P is the
      /// number of attributes.
      algebra::mat<k, p, T>* _center;
    };

#ifndef MLN_INCLUDE_ONLY

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
    k_mean<n,k,p,T>::k_mean()
    {
      trace::entering("mln::clustering::k_mean::k_mean");

      _point    = new algebra::mat<n, p, T>();
      _distance = new algebra::mat<n, k, mln_sum_product(T,T)>();
      _group    = new algebra::mat<n, k, T>();
      _center   = new algebra::mat<k, p, T>();

      mln_postcondition(_point    != 0);
      mln_postcondition(_distance != 0);
      mln_postcondition(_group    != 0);
      mln_postcondition(_center   != 0);

      trace::exiting("mln::clustering::k_mean::k_mean");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    k_mean<n,k,p,T>::~k_mean()
    {
      trace::entering("mln::clustering::k_mean::~k_mean");

      delete _point;
      delete _distance;
      delete _group;
      delete _center;

      trace::exiting("mln::clustering::k_mean::~k_mean");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <typename I>
    inline
    void k_mean<n,k,p,T>::init_point(const Image<I>& _input)
    {
      trace::entering("mln::clustering::k_mean<n,k,p,T>::init");
      
      const I&               input = exact(_input);
      algebra::mat<n, p, T>& point = *_point;

      mln_precondition(input.is_valid());
      mln_precondition(n == input.nrows() * input.ncols());
      //mln_precondition(n == input.nrows());
      //mln_precondition(n == input.nrows() * input.ncols() * input.nslices());

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

      trace::exiting("mln::clustering::k_mean<n,k,p,T>::init");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::init_center()
    {
      trace::entering("mln::clustering::k_mean<n,k,p,T>::init_center");
      
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

	std::cout << "center(" << i << ")" << col(center, i) << std::endl;
      }

      trace::exiting("mln::clustering::k_mean<n,k,p,T>::init_center");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    template <unsigned q, typename M>
    inline
    M k_mean<n,k,p,T>::min(const algebra::vec<q, M>& x) const
    {
      trace::entering("mln::clustering::k_mean<n,k,p,T>::min");
      
      M result = x[0];

      for (unsigned i = 1; i < q; ++i)
	result = math::min(result, x[i]);

      trace::exiting("mln::clustering::k_mean<n,k,p,T>::min");
      return result;
    }
    
    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::update_group()
    {
      trace::entering("mln::clustering::k_mean<n,k,p,T>::update_group");

      algebra::mat<n, k, mln_sum_product(T,T)>& distance = *_distance;
      algebra::mat<n, k, T>&                    group    = *_group;

      // for each point
      for (unsigned i = 0; i < n; ++i)
      {
	// tell us the minimum distance from a specific point to a group
	mln_sum_product(T,T) min_dist = min(col(distance, i));

	// for each center
	for (unsigned j = 0; j < k; ++j)
	  group(i,j) = (min_dist == distance(i,j))? 1.0 : 0.0;
      }

      // mln_postcondition(sum(col(distance(i,j)) == 1) Vi
      trace::exiting("mln::clustering::k_mean<n,k,p,T>::update_group");
    }

    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::update_center()
    {
      trace::entering("mln::clustering::k_mean<n,k,p,T>::update_center");

      algebra::mat<n, p, T>& point  = *_point;
      algebra::mat<k, p, T>& center = *_center;
      algebra::mat<n, k, T>& group  = *_group;

      center = (group.t() * point) / n;

      // mln_postcondition(sum(col(distance(i,j)) == 1) Vi
      trace::exiting("mln::clustering::k_mean<n,k,p,T>::update_center");
    }

    
    
  /*
    template <typename K>
    template <unsigned q, unsigned r, typename T>
    inline
    vec<q, T> row(const mat<q, r, T>& m, unsigned _row) const
    {
      // check _row is valid

      vec<q, T> result;

      for (int j = 0; j < q; ++j)
	result(j) = m(_row, j);
    }

  */
    //vec<q, T> k_mean<n,k,p,T>::col(const mat<r, q, T>& m, unsigned _col) const
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
    
    template <unsigned n, unsigned k, unsigned p, typename T>
    inline
    void k_mean<n,k,p,T>::update_distance()
    {
      trace::entering("mln::clustering::k_mean::distance");
      mln::trace::quiet = true;
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
      mln::trace::quiet = false;
      trace::exiting("mln::clustering::k_mean::distance");
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::clustering

} // end of namespace mln::clustering

#endif // ! MLN_CLUSTERING_K_MEAN_HH
