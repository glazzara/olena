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

#ifndef SCRIBO_CORE_INTERNAL_LINK_FUNCTOR_BASE_HH
# define SCRIBO_CORE_INTERNAL_LINK_FUNCTOR_BASE_HH

/// \file
///
/// Base class for link functors.

# include <mln/core/concept/image.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/core/object_image.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/concept/link_functor.hh>
# include <scribo/primitive/internal/update_link_array.hh>
# include <scribo/primitive/internal/init_link_array.hh>
# include <scribo/primitive/internal/is_invalid_link.hh>


# define scribo_support(T) typename T::support
# define scribo_support_(T) T::support

namespace scribo
{

  namespace primitive
  {

    namespace link
    {

      namespace internal
      {

	/// \brief Base class for link functors.
	template <typename L, typename E>
	class link_functor_base : public Link_Functor<E>
	{
	public:

	  typedef L support;
	  typedef mln_site(L) P;

	  link_functor_base(const object_image(L)& objects, unsigned nanchors);

	  /// \overload
	  /// \p nanchors is set to 1.
	  link_functor_base(const object_image(L)& objects);


	  unsigned nanchors() const;

	  const object_links<L>& links() const;

	  unsigned link(unsigned object) const;
	  const object_image(L)& objects() const;


	  void initialize_link(unsigned current_object);
	  mln_site(L) finalize_link(unsigned current_object);


	  bool verify_link_criterion(unsigned current_object,
				     const P& start_point, const P& p) const;

	  bool is_potential_link(unsigned current_object,
				 const P& start_point, const P& p) const;

	  bool valid_link(unsigned current_object,
			  const P& start_point, const P& p);



	  //FIXME: there's a probleme here. We may not want to provide
	  //both interfaces (with multiple and single anchors)for
	  //every functors...

	  void validate_link(unsigned current_object,
			     const P& start_point,
			     const P& p,
			     unsigned anchor);

	  /// \overload
	  /// \p anchor is set to 0.
	  void validate_link(unsigned current_object,
			     const P& start_point, const P& p);

	  void invalidate_link(unsigned current_object,
			       const P& start_point, const P& p,
			       unsigned anchor);

	  /// \overload
	  /// \p anchor is set to 0
	  void invalidate_link(unsigned current_object,
			       const P& start_point, const P& p);






	  void compute_next_site(P& p);


	  mln_site(L) start_point(unsigned current_object, unsigned anchor);

	  /// \overload
	  /// \p anchor is set to 0.
	  mln_site(L) start_point(unsigned current_object);


	  void start_processing_object(unsigned current_object);




	  // Default implementation for possibly not overridden
	  // methods.

	  void initialize_link_(unsigned current_object);
	  mln_site(L) finalize_link_(unsigned current_object);

	  bool is_potential_link_(unsigned current_object,
				  const P& start_point, const P& p) const;

	  bool verify_link_criterion_(unsigned current_object,
				      const P& start_point, const P& p) const;

	  bool valid_link_(unsigned current_object,
			   const P& start_point, const P& p);

	  void validate_link_(unsigned current_object,
			      const P& start_point, const P& p,
			      unsigned anchor);

	  void invalidate_link_(unsigned current_object,
				const P& start_point, const P& p,
				unsigned anchor);

	  void compute_next_site_(P& p);

	  void start_processing_object_(unsigned current_object);


	protected:
	  object_links<L> links_;
	  const object_image(L) objects_;
	  unsigned nanchors_;
	};



# ifndef MLN_INCLUDE_ONLY


	template <typename L, typename E>
	inline
	link_functor_base<L,E>::link_functor_base(
	  const object_image(L)& objects,
	  unsigned nanchors)
	  : links_(objects, static_cast<unsigned>(objects.nlabels()) + 1),
	    objects_(objects),
	    nanchors_(nanchors)
	{
	  primitive::internal::init_link_array(links_);
	}


	template <typename L, typename E>
	inline
	link_functor_base<L,E>::link_functor_base(
	  const object_image(L)& objects)
	  : links_(objects, static_cast<unsigned>(objects.nlabels()) + 1),
	    objects_(objects),
	    nanchors_(1)
	{
	  primitive::internal::init_link_array(links_);
	}


	template <typename L, typename E>
	inline
	unsigned
	link_functor_base<L,E>::nanchors() const
	{
	  return nanchors_;
	}


	template <typename L, typename E>
	inline
	const object_links<L>&
	link_functor_base<L,E>::links() const
	{
	  return links_;
	}

	template <typename L, typename E>
	inline
	unsigned
	link_functor_base<L,E>::link(unsigned object) const
	{
	  return links_[object];
	}

	template <typename L, typename E>
	inline
	const object_image(L)&
	link_functor_base<L,E>::objects() const
	{
	  return objects_;
	}




	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::is_potential_link(unsigned current_object,
						  const P& start_point,
						  const P& p) const
	{
	  (void) start_point;
	  return this->objects_(p) != literal::zero  // Not the background
	    && this->objects_(p) != current_object // Not the current component
	    && this->links_[this->objects_(p)] != current_object; // No loops
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::initialize_link(unsigned current_object)
	{
	  exact(this)->initialize_link_(current_object);
	}

	template <typename L, typename E>
	inline
	mln_site(L)
	link_functor_base<L,E>::finalize_link(unsigned current_object)
	{
	  return exact(this)->finalize_link_(current_object);
	}



	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::verify_link_criterion(unsigned current_object,
						      const P& start_point,
						      const P& p) const
	{
	  return exact(this)->verify_link_criterion_(current_object,
						     start_point, p);
	}

	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::valid_link(unsigned current_object,
					   const P& start_point,
					   const P& p)
	{
	  return this->objects_.domain().has(p)
	    && exact(this)->valid_link_(current_object, start_point, p);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::validate_link(unsigned current_object,
					      const P& start_point,
					      const P& p,
					      unsigned anchor)
	{
	  exact(this)->validate_link_(current_object, start_point, p, anchor);
	}



	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::validate_link(unsigned current_object,
					      const P& start_point,
					      const P& p)
	{
	  validate_link(current_object, start_point, p, 0);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::invalidate_link(unsigned current_object,
						const P& start_point,
						const P& p,
						unsigned anchor)
	{
	  exact(this)->invalidate_link_(current_object, start_point, p, anchor);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::invalidate_link(unsigned current_object,
						const P& start_point,
						const P& p)
	{
	  invalidate_link(current_object, start_point, p, 0);
	}





	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::compute_next_site(P& p)
	{
	  // No-op
	  exact(this)->compute_next_site_(p);
	}


	template <typename L, typename E>
	inline
	mln_site(L)
	link_functor_base<L,E>::start_point(unsigned current_object,
					    unsigned anchor)
	{
	  return exact(this)->start_point_(current_object, anchor);
	}


	template <typename L, typename E>
	inline
	mln_site(L)
	link_functor_base<L,E>::start_point(unsigned current_object)
	{
	  return start_point(current_object, 0);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::start_processing_object(
	  unsigned current_object)
	{
	  (void) current_object;
	  // No-Op
	  exact(this)->start_processing_object_(current_object);
	}




	// Default implementation for delegated methods.

	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::initialize_link_(unsigned current_object)
	{
	  (void) current_object;
	  // No-Op
	}


	template <typename L, typename E>
	inline
	mln_site(L)
	link_functor_base<L,E>::finalize_link_(unsigned current_object)
	{
	  (void) current_object;
	  // No-Op
	  return P();
	}


	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::is_potential_link_(unsigned current_object,
						   const P& start_point,
						   const P& p) const
	{
	  (void) current_object;
	  (void) start_point;
	  (void) p;
	  // No-Op
	  return true;
	}

	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::verify_link_criterion_(unsigned current_object,
						       const P& start_point,
						       const P& p) const
	{
	  (void) current_object;
	  (void) start_point;
	  (void) p;
	  // No-Op
	  return true;
	}


	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::valid_link_(unsigned current_object,
					    const P& start_point,
					    const P& p)
	{
	  return is_potential_link(current_object, start_point, p)
	    && verify_link_criterion(current_object, start_point, p);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::validate_link_(unsigned current_object,
					       const P& start_point,
					       const P& p,
					       unsigned anchor)
	{
	  (void) start_point;
	  (void) anchor;
	  this->links_[current_object] = this->objects_(p);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::invalidate_link_(unsigned current_object,
						 const P& start_point,
						 const P& p,
						 unsigned anchor)
	{
	  (void) current_object;
	  (void) start_point;
	  (void) p;
	  (void) anchor;
	  // No-op
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::compute_next_site_(P& p)
	{
	  (void) p;
	  // No-Op
	}

	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::start_processing_object_(
	  unsigned current_object)
	{
	  // No-Op
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_CORE_INTERNAL_LINK_FUNCTOR_BASE_HH
