// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_ACCU_STAT_VAR_HH
# define MLN_ACCU_STAT_VAR_HH

/// \file mln/accu/stat/var.hh
///
/// Define an accumulator that computes the variance matrix of a set
/// of vectors.

# include <cmath>
# include <mln/accu/internal/base.hh>
# include <mln/algebra/vec.hh>
# include <mln/algebra/mat.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {

    namespace stat
    {

      /// \brief Var accumulator class.
      ///
      /// Parameter \c T is the type of vectors
      ///
      /// \ingroup modaccuvalues
      //
      template <typename T>
      struct var : public mln::accu::internal::base< algebra::mat< T::dim, T::dim, float >,
						     var<T> >
      {
	enum { dim = T::dim };
	typedef T argument;
	typedef algebra::mat<dim,dim,float> result;

	var();

	/// Manipulators.
	/// \{
	void init();
	void take(const argument& v);
	void take(const var<T>& other);

	void take_as_init_(const argument& v);
	void take_n_times_(unsigned n_times, const argument& v);
	/// \}

	/// Get the accumulator result (the var value).
	result to_result() const;

	/// Get the variance matrix.
	result variance() const;

	/// Get the number of items.
	unsigned n_items() const;


	/// Type equipment.
	typedef algebra::vec<dim,float> mean_t;
	// ...

	/// Get the mean vector.
	mean_t mean() const;

	/// Check whether this accu returns a valid result.
	bool is_valid() const;

      protected:

	unsigned n_;
	algebra::vec<dim,float>     sum_;
	algebra::mat<dim,dim,float> cov_;
      };



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    var<T>::var()
    {
      init();
    }

    template <typename T>
    inline
    void
    var<T>::init()
    {
      n_ = 0;
      sum_.set_all(0);
      cov_.set_all(0);
    }

    template <typename T>
    inline
    void
    var<T>::take(const argument& v)
    {
      ++n_;
      sum_ += v;
      cov_ += v * v.t();
    }

    template <typename T>
    inline
    void
    var<T>::take(const var<T>& other)
    {
      n_   += other.n_;
      cov_ += other.cov_;
    }

    template <typename T>
    inline
    void
    var<T>::take_as_init_(const argument& v)
    {
      n_ = 1;
      sum_ = v;
      cov_ = v * v.t();
    }

    template <typename T>
    inline
    void
    var<T>::take_n_times_(unsigned n_times, const argument& v)
    {
      n_ += n_times;
      sum_ += n_times * v;
      cov_ += n_times * v * v.t();
    }


    template <typename T>
    inline
    mln_result(var<T>)
    var<T>::to_result() const
    {
      static result null_ = literal::zero;

      if (n_ == 0u)
	return null_; // Safety.

      return (cov_ - sum_ * sum_.t() / n_) / n_;
      // Shorter than:
      // mean_ = sum_ / n_
      // var_  = cov_ / n_ - mean_ * mean_.t() 
    }

    template <typename T>
    inline
    mln_result(var<T>)
    var<T>::variance() const
    {
      return to_result();
    }

    template <typename T>
    inline
    unsigned
    var<T>::n_items() const
    {
      return n_;
    }

    template <typename T>
    inline
    typename var<T>::mean_t
    var<T>::mean() const
    {
      static algebra::vec<dim,float> null_ = literal::zero;

      if (n_ == 0u)
	return null_; // Safety.

      return sum_ / n_;
    }

    template <typename T>
    inline
    bool
    var<T>::is_valid() const
    {
      return n_ != 0;
    }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_VAR_HH
