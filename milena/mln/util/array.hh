// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_ARRAY_HH
# define MLN_UTIL_ARRAY_HH

/// \file mln/util/array.hh
///
/// Definition of mln::util::array.
///
/// \todo Zed: Add a lazy removal method (based on an extra attribute
/// std::vector<bool> has_).  Then add a purge/compress method.

# include <vector>
# include <iostream>

# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/iterator.hh>



namespace mln
{

  namespace util
  {

    /// Forward declaration.
    template <typename T>
    class array;

  } // end of namespace mln::util


  namespace convert
  {

    namespace over_load
    {

      template <typename T1, typename T2>
      void
      from_to_(const util::array<T1>& from, util::array<T2>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



  namespace util
  {

    // Forward declarations.
    template <typename T> class array_fwd_iter;
    template <typename T> class array_bkd_iter;


    /// A dynamic array class.
    ///
    /// Elements are stored by copy.  Implementation is lazy.
    ///
    /// The parameter \c T is the element type, which shall not be
    /// const-qualified.
    template <typename T>
    class array : public Object< mln::util::array<T> >
    {
    public:

      /// Element associated type.
      typedef T element;


      /// Forward iterator associated type.
      typedef array_fwd_iter<T> fwd_eiter;

      /// Backward iterator associated type.
      typedef array_bkd_iter<T> bkd_eiter;

      /// Iterator associated type.
      typedef fwd_eiter eiter;


      /// Constructor without arguments.
      array();

      /// Construct a new array and resize it to \n elements.
      array(unsigned n);

      /// Construct a new array, resize it to \n elements and fill it
      /// with \p default_value.
      array(unsigned n, const T& value);


      /// Reserve memory for \p n elements.
      void reserve(unsigned n);

      /// Resize this array to \p n elements.
      void resize(unsigned n);

      /// Resize this array to \p n elements with \p value as value.
      void resize(unsigned n, const T& value);


      /// Add the element \p elt at the end of this array.
      array<T>& append(const T& elt);

      /// Add the elements of \p other at the end of this array.
      template <typename U>
      array<T>& append(const array<U>& other);


      /// Return the number of elements of the array.
      unsigned nelements() const;

      /// Test if the array is empty.
      bool is_empty() const;


      /// \brief Return the \p i-th element of the array.
      /// \pre i < nelements()
      const T& operator[](unsigned i) const;

      /// \brief Return the \p i-th element of the array.
      /// \pre i < nelements()
      T& operator[](unsigned i);


      /// Empty the array.  All elements contained in the array are
      /// destroyed.  \post is_empty() == true
      void clear();


      /// Return the corresponding std::vector of elements.
      const std::vector<T>& std_vector() const;

      /// Hook to the mutable std::vector of elements.
      std::vector<T>& hook_std_vector_();

      /// Return the size of this array in memory.
      std::size_t memory_size() const;

    private:

      std::vector<T> v_;
    };


    /// Operator<<.
    template <typename T>
    std::ostream& operator<<(std::ostream& ostr,
			     const array<T>& a);

    /// Operator==
    template <typename T>
    bool operator==(const array<T>& lhs,
		    const array<T>& rhs);


    // array_fwd_iter<T>

    template <typename T>
    class array_fwd_iter : public Proxy< array_fwd_iter<T> >,
			   public mln::internal::proxy_impl< const T&,
							     array_fwd_iter<T> >
    {
    public:

      /// Constructor without argument.
      array_fwd_iter();

      /// Constructor from an array \p a.
      array_fwd_iter(const array<T>& a);

      /// Change the array it iterates on to \p a.
      void change_target(const array<T>& a);

      /// Start an iteration.
      void start();

      /// Go to the next element.
      void next();

      /// Returns true if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Give the element the iterator designates.
      const T& element() const;

      // As a Proxy.
      const T& subj_();

      /// Give the current index.
      unsigned index_() const;

    protected:
      unsigned i_;
      const array<T>* a_;
    };




    // array_bkd_iter<T>

    template <typename T>
    class array_bkd_iter : public Proxy< array_bkd_iter<T> >,
			   public mln::internal::proxy_impl< const T&,
							     array_bkd_iter<T> >
    {
    public:

      /// Constructor without argument.
      array_bkd_iter();

      /// Constructor from an array \p a.
      array_bkd_iter(const array<T>& a);

      /// Change the array it iterates on to \p a.
      void change_target(const array<T>& a);

      /// Start an iteration.
      void start();

      /// Go to the next element.
      void next();

      /// Returns true if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Give the element the iterator designates.
      const T& element() const;

      // As a Proxy.
      const T& subj_();

      /// Give the current index.
      unsigned index_() const;

    protected:
      unsigned i_;
      const array<T>* a_;
    };

  } // end of namespace mln::util


  namespace internal
  {

    template <typename T, typename E>
    struct subject_impl<const util::array<T>&, E>
    {
      unsigned nelements() const;
      bool is_empty() const;
      const T& operator[](unsigned i) const;
      const std::vector<T>& std_vector() const;

      private:
      const E& exact_() const;
    };


    template <typename T, typename E>
    struct subject_impl<util::array<T>&, E>
      : subject_impl<const util::array<T>&, E>
    {
      void reserve(unsigned n);
      void resize(unsigned n);
      void resize(unsigned n, const T& value);

      util::array<T>& append(const T& elt);

      template <typename U>
      util::array<T>& append(const util::array<U>& other);

      T& operator[](unsigned i);

      void clear();

      std::vector<T>& hook_std_vector_();

      private:
      E& exact_();
    };


  } // end of namespace mln::internal


# ifndef MLN_INCLUDE_ONLY


  // convert::from_to_

  namespace convert
  {

    namespace over_load
    {

      template <typename T1, typename T2>
      void
      from_to_(const util::array<T1>& from, util::array<T2>& to)
      {
	mlc_converts_to(T1,T2)::check();

        to.reserve(from.nelements());

        for (unsigned i = 0; i < from.nelements(); ++i)
	  to.append(convert::to<T2>(from[i]));
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  namespace util
  {

    // util::array<T>


    template <typename T>
    inline
    array<T>::array()
    {
    }

    template <typename T>
    inline
    array<T>::array(unsigned n)
      : v_(n)
    {
    }

    template <typename T>
    inline
    array<T>::array(unsigned n, const T& value)
      : v_(n, value)
    {
    }

    template <typename T>
    inline
    void
    array<T>::reserve(unsigned n)
    {
      v_.reserve(n);
    }

    template <typename T>
    inline
    void
    array<T>::resize(unsigned n)
    {
      v_.resize(n);
    }

    template <typename T>
    inline
    void
    array<T>::resize(unsigned n, const T& value)
    {
      v_.resize(n, value);
    }

    template <typename T>
    inline
    array<T>&
    array<T>::append(const T& elt)
    {
      v_.push_back(elt);
      return *this;
    }

    template <typename T>
    template <typename U>
    inline
    array<T>&
    array<T>::append(const array<U>& other)
    {
      if (other.is_empty())
	// No-op.
	return *this;
      v_.insert(v_.end(),
		other.std_vector().begin(), other.std_vector().end());
      return *this;
    }

    template <typename T>
    inline
    void
    array<T>::clear()
    {
      v_.clear();
      mln_postcondition(is_empty());
    }

    template <typename T>
    inline
    unsigned
    array<T>::nelements() const
    {
      return v_.size();
    }

    template <typename T>
    inline
    const T&
    array<T>::operator[](unsigned i) const
    {
      mln_precondition(i < nelements());
      return v_[i];
    }

    template <typename T>
    inline
    T&
    array<T>::operator[](unsigned i)
    {
      mln_precondition(i < nelements());
      return v_[i];
    }

    template <typename T>
    inline
    bool
    array<T>::is_empty() const
    {
      return nelements() == 0;
    }

    template <typename T>
    inline
    const std::vector<T>&
    array<T>::std_vector() const
    {
      return v_;
    }

    template <typename T>
    inline
    std::vector<T>&
    array<T>::hook_std_vector_()
    {
      return v_;
    }

    template <typename T>
    inline
    std::size_t
    array<T>::memory_size() const
    {
      return sizeof(*this) + nelements() * sizeof(T);
    }



    // util::array_fwd_iter<T>


    template <typename T>
    inline
    array_fwd_iter<T>::array_fwd_iter()
    {
      a_ = 0;
    }

    template <typename T>
    inline
    array_fwd_iter<T>::array_fwd_iter(const array<T>& a)
    {
      change_target(a);
    }

    template <typename T>
    inline
    void
    array_fwd_iter<T>::change_target(const array<T>& a)
    {
      a_ = &a;
      invalidate();
    }

    template <typename T>
    inline
    void
    array_fwd_iter<T>::start()
    {
      mln_precondition(a_ != 0);
      i_ = 0;
    }

    template <typename T>
    inline
    void
    array_fwd_iter<T>::next()
    {
      mln_precondition(is_valid());
      ++i_;
    }

    template <typename T>
    inline
    bool
    array_fwd_iter<T>::is_valid() const
    {
      return a_ != 0 && i_ != a_->nelements();
    }

    template <typename T>
    inline
    void
    array_fwd_iter<T>::invalidate()
    {
      if (a_ != 0)
	i_ = a_->nelements();
      mln_postcondition(! is_valid());
    }

    template <typename T>
    inline
    const T&
    array_fwd_iter<T>::element() const
    {
      mln_precondition(is_valid());
      return a_->operator[](i_);
    }

    template <typename T>
    inline
    const T&
    array_fwd_iter<T>::subj_()
    {
      mln_precondition(is_valid());
      return a_->operator[](i_);
    }

    template <typename T>
    inline
    unsigned
    array_fwd_iter<T>::index_() const
    {
      return i_;
    }



    // util::array_bkd_iter<T>


    template <typename T>
    inline
    array_bkd_iter<T>::array_bkd_iter()
    {
      a_ = 0;
    }

    template <typename T>
    inline
    array_bkd_iter<T>::array_bkd_iter(const array<T>& a)
    {
      change_target(a);
    }

    template <typename T>
    inline
    void
    array_bkd_iter<T>::change_target(const array<T>& a)
    {
      a_ = &a;
      invalidate();
    }

    template <typename T>
    inline
    void
    array_bkd_iter<T>::start()
    {
      mln_precondition(a_ != 0);
      if (! a_->is_empty())
	i_ = a_->nelements() - 1;
    }

    template <typename T>
    inline
    void
    array_bkd_iter<T>::next()
    {
      mln_precondition(is_valid());
      if (i_ == 0)
	invalidate();
      else
	--i_;
    }

    template <typename T>
    inline
    bool
    array_bkd_iter<T>::is_valid() const
    {
      return a_ != 0 && i_ != a_->nelements();
    }

    template <typename T>
    inline
    void
    array_bkd_iter<T>::invalidate()
    {
      if (a_ != 0)
	i_ = a_->nelements();
      mln_postcondition(! is_valid());
    }

    template <typename T>
    inline
    const T&
    array_bkd_iter<T>::element() const
    {
      mln_precondition(is_valid());
      return a_->operator[](i_);
    }

    template <typename T>
    inline
    const T&
    array_bkd_iter<T>::subj_()
    {
      mln_precondition(is_valid());
      return a_->operator[](i_);
    }

    template <typename T>
    inline
    unsigned
    array_bkd_iter<T>::index_() const
    {
      return i_;
    }



    // Operator <<.

    template <typename T>
    std::ostream& operator<<(std::ostream& ostr,
			     const array<T>& a)
    {
      ostr << '[';
      const unsigned n = a.nelements();
      for (unsigned i = 0; i < n; ++i)
	{
	  ostr << a[i];
	  if (i != n - 1)
	    ostr << ", ";
	}
      ostr << ']';
      return ostr;
    }


    // Operator <<.

    template <typename T>
    bool operator==(const array<T>& lhs,
		    const array<T>& rhs)
    {
      return lhs.std_vector() == rhs.std_vector();
    }

  } // end of namespace mln::util


  namespace internal
  {

    template <typename T, typename E>
    inline
    void
    subject_impl<util::array<T>&, E>::reserve(unsigned n)
    {
      exact_().get_subject().reserve(n);
    }

    template <typename T, typename E>
    inline
    void
    subject_impl<util::array<T>&, E>::resize(unsigned n)
    {
      exact_().get_subject().resize(n);
    }

    template <typename T, typename E>
    inline
    void
    subject_impl<util::array<T>&, E>::resize(unsigned n, const T& value)
    {
      exact_().get_subject().resize(n, value);
    }

    template <typename T, typename E>
    inline
    util::array<T>&
    subject_impl<util::array<T>&, E>::append(const T& elt)
    {
      return exact_().get_subject().append(elt);
    }

    template <typename T, typename E>
    template <typename U>
    inline
    util::array<T>&
    subject_impl<util::array<T>&, E>::append(const util::array<U>& other)
    {
      return exact_().get_subject().append(other);
    }

    template <typename T, typename E>
    inline
    T&
    subject_impl<util::array<T>&, E>::operator[](unsigned i)
    {
      return exact_().get_subject()[i];
    }

    template <typename T, typename E>
    inline
    void
    subject_impl<util::array<T>&, E>::clear()
    {
      exact_().get_subject().clear();
    }

    template <typename T, typename E>
    inline
    std::vector<T>&
    subject_impl<util::array<T>&, E>::hook_std_vector_()
    {
      return exact_().get_subject().hook_std_vector_();
    }

    template <typename T, typename E>
    inline
    E&
    subject_impl<util::array<T>&, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }


    template <typename T, typename E>
    inline
    unsigned
    subject_impl<const util::array<T>&, E>::nelements() const
    {
      return exact_().get_subject().nelements();
    }

    template <typename T, typename E>
    inline
    bool
    subject_impl<const util::array<T>&, E>::is_empty() const
    {
      return exact_().get_subject().is_empty();
    }

    template <typename T, typename E>
    inline
    const T&
    subject_impl<const util::array<T>&, E>::operator[](unsigned i) const
    {
      return exact_().get_subject()[i];
    }

    template <typename T, typename E>
    inline
    const std::vector<T>&
    subject_impl<const util::array<T>&, E>::std_vector() const
    {
      return exact_().get_subject().std_vector();
    }

    template <typename T, typename E>
    inline
    const E&
    subject_impl<const util::array<T>&, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }


  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_UTIL_ARRAY_HH
