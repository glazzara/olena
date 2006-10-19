// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_TRACKED_PTR_HH
# define OLN_CORE_INTERNAL_TRACKED_PTR_HH

# include <set>
# include <ostream>


namespace oln
{

  namespace internal
  {

    template <typename T>
    struct tracked_ptr
    {
      typedef tracked_ptr<T> self_t;
      typedef std::set<self_t*> holders_t;

      T* ptr_;
      holders_t* holders_;

      /// Coercion towards Boolean (for arithmetical tests).
      operator bool() const;

      /// Negation (for arithmetical tests).
      bool operator not() const;

      /*! \brief Mimics the behavior of op-> for a pointer in the const case.
      **
      ** \invariant Pointer proxy exists.
      */
      const T*const operator->() const;

      /*! \brief Mimics the behavior of op-> for a pointer in the mutable case.
      **
      ** \invariant Pointer proxy exists.
      */
      T*const operator->();

      /// Ctor.
      tracked_ptr();

      /// Ctor.
      tracked_ptr(T* ptr);

      /// Cpy ctor.
      tracked_ptr(const tracked_ptr<T>& rhs);

      /// Assignment.
      tracked_ptr<T>& operator=(const tracked_ptr<T>& rhs);

      /// Assignment.
      tracked_ptr<T>& operator=(T* ptr);

      /// Dtor.
      ~tracked_ptr();

      void invariant_() const;

      void clean_();

    };




# ifndef OLN_INCLUDE_ONLY

    template <typename T>
    tracked_ptr<T>::operator bool() const
    {
      invariant_();
      return ptr_ != 0;
    }

    template <typename T>
    /// Negation (for arithmetical tests).
    bool tracked_ptr<T>::operator not() const
    {
      invariant_();
      return not bool(*this);
    }

    template <typename T>
    /*! \brief Mimics the behavior of op-> for a pointer in the const case.
    **
    ** \invariant Pointer proxy exists.
    */
    const T*const tracked_ptr<T>::operator->() const
    {
      invariant_();
      precondition(ptr_ != 0);
      return ptr_;
    }

    template <typename T>
    /*! \brief Mimics the behavior of op-> for a pointer in the mutable case.
    **
    ** \invariant Pointer proxy exists.
    */
    T*const tracked_ptr<T>::operator->()
    {
      invariant_();
      precondition(ptr_ != 0);
      return ptr_;
    }

    template <typename T>
    /// Ctor.
    tracked_ptr<T>::tracked_ptr() :
      ptr_(0),
      holders_(0)
    {
      invariant_();
    }

    template <typename T>
    /// Ctor.
    tracked_ptr<T>::tracked_ptr(T* ptr) :
      ptr_(ptr)
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      if (ptr == 0)
	holders_ = 0;
      else
	{
	  holders_ = new holders_t;
	  holders_->insert(this);
	}
      invariant_();
    }

    template <typename T>
    /// Cpy ctor.
    tracked_ptr<T>::tracked_ptr(const tracked_ptr<T>& rhs) :
      ptr_(rhs.ptr_),
      holders_(rhs.holders_)
    {
      rhs.invariant_();
      if (ptr_ != 0)
	holders_->insert(this);
      invariant_();
    }

    template <typename T>
    /// Assignment.
    tracked_ptr<T>& tracked_ptr<T>::operator=(const tracked_ptr<T>& rhs)
    {
      invariant_();
      rhs.invariant_();
      if (&rhs == this or rhs.ptr_ == ptr_)
	// no-op
	return *this;
      clean_();
      ptr_ = rhs.ptr_;
      holders_ = rhs.holders_;
      holders_->insert(this);
      return *this;
    }

    template <typename T>
    /// Assignment.
    tracked_ptr<T>& tracked_ptr<T>::operator=(T* ptr)
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      invariant_();
      if (ptr == ptr_)
	// no-op
	return *this;
      clean_();
      ptr_ = ptr;
      if (ptr == 0)
	holders_ = 0;
      else
	{
	  holders_ = new holders_t;
	  holders_->insert(this);
	}
      return *this;
    }

    /// Dtor.
    template <typename T>
    tracked_ptr<T>::~tracked_ptr()
    {
      clean_();
    }

    template <typename T>
    void tracked_ptr<T>::invariant_() const
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      invariant((ptr_ and holders_) or (not ptr_ and not holders_));
      if (ptr_ == 0)
	return;
      invariant(holders_->size() > 0);
      tracked_ptr<T>* this_ = const_cast<tracked_ptr<T>*>(this);
      invariant(holders_->find(this_) != holders_->end());
      this_ = 0;
      typename holders_t::const_iterator i;
      for (i = holders_->begin(); i != holders_->end(); ++i)
	invariant((*i)->ptr_ == ptr_);
    }

    template <typename T>
    void tracked_ptr<T>::clean_()
    {
      invariant_();
      if (ptr_ == 0)
	// no-op
	return;
      if (holders_->size() == 1)
	{
	  delete ptr_;
	  delete holders_;
	}
      else
	holders_->erase(this);
      ptr_ = 0;
      holders_ = 0;
      invariant_();
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& ostr, const tracked_ptr<T>& tp)
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      ostr << "tracked_ptr @ " << (&tp)
	   << " { ptr = " << tp.ptr_
	   << " / holders = ";
      if (tp.holders_ == 0)
	ostr << "0";
      else
	{
	  typename holders_t::const_iterator i;
	  for (i = tp.holders_->begin(); i != tp.holders_->end(); ++i)
	    ostr << (*i) << ' ';
	}
      ostr << " }";
      return ostr;
    }


# endif

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_TRACKED_PTR_HH
