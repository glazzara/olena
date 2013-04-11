// Copyright (C) 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_ARRAY_HH
# define MLN_UTIL_ARRAY_HH

/// \file
///
/// Definition of mln::util::array.
///
/// \todo Zed: Add a lazy removal method (based on an extra attribute
/// std::vector<bool> has_).  Then add a purge/compress method.

# include <vector>
# include <iostream>
# include <algorithm>

# include <mln/core/alias/box1d.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/iterator.hh>

# include <mln/fun/internal/selector.hh>


namespace mln
{

  // Forward declarations.
  namespace fun {
    namespace i2v {
      template <typename T> class array;
    }
  }
  template <typename V> struct image1d;
  // End of forward declarations


  namespace util
  {

    // Forward declarations.
    template <typename T> class array_fwd_iter;
    template <typename T> class array_bkd_iter;


    /// \brief A dynamic array class.
    ///
    /// Elements are stored by copy.  Implementation is lazy.
    ///
    /// The parameter \c T is the element type, which shall not be
    /// const-qualified.
    ///
    /// \ingroup modutil
    //
    template <typename T>
    class array
      : public mln::fun::internal::selector_from_result_<T, array<T> >::ret

    // public Function_v2v< mln::util::array<T> >
    {
    public:

      /// Element associated type.
      typedef T element;

      /// Returned value types.
      /// Related to the Function_v2v concept.
      /// @{
      typedef T result;
      typedef typename std::vector<T>::const_reference ro_result;
      typedef typename std::vector<T>::reference mutable_result;
      /// @}


      /// Iterator types
      /// @{
      /// Forward iterator associated type.
      typedef array_fwd_iter<T> fwd_eiter;

      /// Backward iterator associated type.
      typedef array_bkd_iter<T> bkd_eiter;

      /// Iterator associated type.
      typedef fwd_eiter eiter;
      /// @}


      /// Constructors
      /// @{
      /// Constructor without arguments.
      array();

      /// Construct a new array and resize it to \n elements.
      array(unsigned n);

      /// Construct a new array, resize it to \n elements and fill it
      /// with \p default_value.
      array(unsigned n, const T& value);
      /// @}

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

      /// Return the number of elements of the array.
      /// Added for compatibility with fun::i2v::array.
      /// \sa nelements
      unsigned size() const;

      /// Test if the array is empty.
      bool is_empty() const;


      /// \brief Return the \p i-th element of the array.
      /// \pre i < nelements()
      ro_result operator()(unsigned i) const;

      /// \brief Return the \p i-th element of the array.
      /// \pre i < nelements()
      mutable_result operator()(unsigned i);

      /// \brief Return the \p i-th element of the array.
      /// \pre i < nelements()
      ro_result operator[](unsigned i) const;

      /// \brief Return the \p i-th element of the array.
      /// \pre i < nelements()
      mutable_result operator[](unsigned i);

      /// \brief Return the last element.
      ro_result last() const;

      /// \brief Return the last element.
      mutable_result last();

      /// Empty the array.  All elements contained in the array are
      /// destroyed.  \post is_empty() == true
      void clear();

      /// Fill the whole array with value \p value.
      void fill(const T& value);

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
    /// \relates util::array
    template <typename T>
    std::ostream& operator<<(std::ostream& ostr,
			     const array<T>& a);

    /// Operator==
    /// \relates util::array
    template <typename T>
    bool operator==(const array<T>& lhs,
		    const array<T>& rhs);


    /// \cond INTERNAL_API
    /// Conversion: array -> array
    template <typename T1, typename T2>
    void
    from_to_(const array<T1>& from, array<T2>& to);

    /// Conversion: array<T> -> fun::i2v::array<T>
    template <typename T>
    inline
    void
    from_to_(const array<T>& from, fun::i2v::array<T>& to);

    /// Conversion: array<T> -> fun::i2v::array<U>
    template <typename T, typename U>
    inline
    void
    from_to_(const array<T>& from, fun::i2v::array<U>& to);

    ///Conversion: util::array -> image1d.
    template <typename V, typename T>
    void from_to_(const util::array<V>& from, image1d<T>& to);
    /// \endcond

    // array_fwd_iter<T>

    template <typename T>
    class array_fwd_iter : public Proxy< array_fwd_iter<T> >,
			   public mln::internal::proxy_impl< const T&,
							     array_fwd_iter<T> >
    {
    public:
      typedef typename array<T>::ro_result subj_t;

      /// Constructors
      /// @{
      /// Constructor without argument.
      array_fwd_iter();

      /// Constructor from an array \p a.
      array_fwd_iter(const array<T>& a);
      /// @}

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
      T element() const;

      // As a Proxy.
      subj_t subj_();

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
      typedef typename array<T>::ro_result subj_t;

      /// Constructors
      /// @{
      /// Constructor without argument.
      array_bkd_iter();

      /// Constructor from an array \p a.
      array_bkd_iter(const array<T>& a);
      /// @}

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
      T element() const;

      // As a Proxy.
      subj_t subj_();

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
      typedef typename util::array<T>::ro_result ro_result;

      unsigned nelements() const;
      unsigned size() const;
      bool is_empty() const;
      ro_result operator()(unsigned i) const;
      ro_result operator[](unsigned i) const;
      const std::vector<T>& std_vector() const;

      private:
      const E& exact_() const;
    };


    template <typename T, typename E>
    struct subject_impl<util::array<T>&, E>
      : subject_impl<const util::array<T>&, E>
    {
      typedef typename util::array<T>::mutable_result mutable_result;

      void reserve(unsigned n);
      void resize(unsigned n);
      void resize(unsigned n, const T& value);

      util::array<T>& append(const T& elt);

      template <typename U>
      util::array<T>& append(const util::array<U>& other);

      mutable_result operator()(unsigned i);
      mutable_result operator[](unsigned i);

      void clear();

      void fill(const T& value);

      std::vector<T>& hook_std_vector_();

      private:
      E& exact_();
    };


  } // end of namespace mln::internal


# ifndef MLN_INCLUDE_ONLY


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
    void
    array<T>::fill(const T& value)
    {
      std::fill(v_.begin(), v_.end(), value);
    }

    template <typename T>
    inline
    unsigned
    array<T>::size() const
    {
      return nelements();
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
    typename array<T>::ro_result
    array<T>::operator()(unsigned i) const
    {
      return (*this)[i];
    }

    template <typename T>
    inline
    typename array<T>::mutable_result
    array<T>::operator()(unsigned i)
    {
      return (*this)[i];
    }

    template <typename T>
    inline
    typename array<T>::ro_result
    array<T>::operator[](unsigned i) const
    {
      mln_precondition(i < nelements());
      return v_[i];
    }

    template <typename T>
    inline
    typename array<T>::mutable_result
    array<T>::operator[](unsigned i)
    {
      mln_precondition(i < nelements());
      return v_[i];
    }

    template <typename T>
    inline
    typename array<T>::ro_result
    array<T>::last() const
    {
      return v_[nelements() - 1];
    }

    template <typename T>
    inline
    typename array<T>::mutable_result
    array<T>::last()
    {
      return v_[nelements() - 1];
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


    // Conversions

    template <typename T1, typename T2>
    void
    from_to_(const array<T1>& from, array<T2>& to)
    {
      to.resize(from.nelements());

      for (unsigned i = 0; i < from.nelements(); ++i)
	to[i] = convert::to<T2>(from[i]);
    }

    template <typename T>
    void
    from_to_(const array<T>& from, fun::i2v::array<T>& to)
    {
      to = fun::i2v::array<T>(from);
    }

    template <typename T, typename U>
    void
    from_to_(const array<T>& from, fun::i2v::array<U>& to)
    {
      to.resize(from.nelements());
      for (unsigned i = 0; i < from.nelements(); ++i)
	to(i) = convert::to<U>(from[i]);
    }

    template <typename V, typename T>
    void
    from_to_(const array<V>& from, image1d<T>& to)
    {
      to.init_(make::box1d(from.nelements()));
      for (unsigned i = 0; i < from.nelements(); ++i)
	from_to(from[i], to(point1d(i)));
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
    T
    array_fwd_iter<T>::element() const
    {
      mln_precondition(is_valid());
      return a_->operator[](i_);
    }

    template <typename T>
    inline
    typename array_fwd_iter<T>::subj_t
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
    T
    array_bkd_iter<T>::element() const
    {
      mln_precondition(is_valid());
      return a_->operator[](i_);
    }

    template <typename T>
    inline
    typename array_bkd_iter<T>::subj_t
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
    typename subject_impl<util::array<T>&, E>::mutable_result
    subject_impl<util::array<T>&, E>::operator()(unsigned i)
    {
      return exact_().get_subject()(i);
    }

    template <typename T, typename E>
    inline
    typename subject_impl<util::array<T>&, E>::mutable_result
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
    void
    subject_impl<util::array<T>&, E>::fill(const T& value)
    {
      exact_().get_subject().fill(value);
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
    subject_impl<const util::array<T>&, E>::size() const
    {
      return exact_().get_subject().size();
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
    typename subject_impl<const util::array<T>&, E>::ro_result
    subject_impl<const util::array<T>&, E>::operator()(unsigned i) const
    {
      return exact_().get_subject()(i);
    }

    template <typename T, typename E>
    inline
    typename subject_impl<const util::array<T>&, E>::ro_result
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
