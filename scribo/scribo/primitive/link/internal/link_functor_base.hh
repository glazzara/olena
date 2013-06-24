// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
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

#ifndef SCRIBO_CORE_INTERNAL_LINK_FUNCTOR_BASE_HH
# define SCRIBO_CORE_INTERNAL_LINK_FUNCTOR_BASE_HH

/// \file
///
/// Base class for link functors.
///
/// FIXME: type used for 'current_object' argument should be of type
/// mln_value(L)

# include <mln/core/concept/image.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <mln/labeling/compute.hh>
# include <mln/accu/center.hh>

# include <scribo/core/tag/anchor.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/concept/link_functor.hh>
# include <scribo/primitive/link/internal/compute_anchor.hh>

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

	  typedef component_set<L> component_set_t;
	  typedef L support;
	  typedef mln_site(L) P;
	  typedef mln::util::couple<anchor::Type, P> couple_t;

	  /// \overload
	  link_functor_base(const component_set<L>& components);


	  const object_links<L>& links() const;

	  unsigned link(unsigned object) const;
	  const component_set<L>& components() const;


	  void initialize_link(unsigned current_object);
	  couple_t finalize_link(unsigned current_object);


	  bool verify_link_criterion(unsigned current_object,
				     const P& start_point, const P& p,
				     anchor::Type anchor) const;

	  bool is_potential_link(unsigned current_object,
				 const P& start_point, const P& p) const;

	  bool valid_link(unsigned current_object,
			  const P& start_point, const P& p,
			  anchor::Type anchor);



	  //FIXME: there's a probleme here. We may not want to provide
	  //both interfaces (with multiple and single anchors)for
	  //every functors...

	  void validate_link(unsigned current_object,
			     const P& start_point,
			     const P& p,
			     anchor::Type anchor);

	  /// \overload
	  /// \p anchor is set to anchor::MassCenter.
	  void validate_link(unsigned current_object,
			     const P& start_point, const P& p);

	  void invalidate_link(unsigned current_object,
			       const P& start_point, const P& p,
			       anchor::Type anchor);

	  /// \overload
	  /// \p anchor is set to anchor::MassCenter
	  void invalidate_link(unsigned current_object,
			       const P& start_point, const P& p);


	  void compute_next_site(P& p);


	  mln_site(L) start_point(unsigned current_object, anchor::Type anchor);

	  /// \overload
	  /// \p anchor is set to anchor::MassCenter.
	  mln_site(L) start_point(unsigned current_object);


	  void start_processing_object(unsigned current_object);




	  // Default implementation for possibly not overridden
	  // methods.

	  void initialize_link_(unsigned current_object);
	  couple_t finalize_link_(unsigned current_object);

	  bool is_potential_link_(unsigned current_object,
				  const P& start_point, const P& p) const;

	  bool verify_link_criterion_(unsigned current_object,
				      const P& start_point, const P& p,
				      anchor::Type anchor) const;

	  bool valid_link_(unsigned current_object,
			   const P& start_point, const P& p,
			   anchor::Type anchor);

	  void validate_link_(unsigned current_object,
			      const P& start_point, const P& p,
			      anchor::Type anchor);

	  void invalidate_link_(unsigned current_object,
				const P& start_point, const P& p,
				anchor::Type anchor);

	  void compute_next_site_(P& p);

	  void start_processing_object_(unsigned current_object);

	  mln_site(L) start_point_(unsigned current_object,
				   anchor::Type anchor);

	  const L& labeled_image() const;

	protected:
	  object_links<L> links_;
	  const component_set<L> components_;
	  const L& labeled_image_;
	};



# ifndef MLN_INCLUDE_ONLY


	template <typename L, typename E>
	inline
	link_functor_base<L,E>::link_functor_base(
	  const component_set<L>& components)
	  : links_(components),
	    components_(components),
	    labeled_image_(this->components_.labeled_image())
	{
	  links_.init();
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
	  return links_(object);
	}

	template <typename L, typename E>
	inline
	const component_set<L>&
	link_functor_base<L,E>::components() const
	{
	  return components_;
	}




	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::is_potential_link(unsigned current_object,
						  const P& start_point,
						  const P& p) const
	{
	  (void) start_point;
	  mln_value(L) v = this->labeled_image_(p);

	  // We found that we are passing through a separator. We stop
	  // looking up for a neighbor by returning true.
	  //
	  // Since we don't want to link with a separator,
	  // verify_link_criterion will invalidate this link.
	  //
	  if (this->components_.has_separators() && this->components_.separators()(p))
	    return true;

	  return v != literal::zero  // Not the background
	    && v != current_object // Not the current component
	    && this->links_(v) != current_object  // No loops
	    && this->components_(v).tag() != component::Ignored; // Not ignored
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
	mln::util::couple<anchor::Type, mln_site(L)>
	link_functor_base<L,E>::finalize_link(unsigned current_object)
	{
	  return exact(this)->finalize_link_(current_object);
	}



	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::verify_link_criterion(unsigned current_object,
						      const P& start_point,
						      const P& p,
						      anchor::Type anchor) const
	{
	  return
	    // Do not link with separators...
	    ! this->components_.separators()(p)
	    // ... and perform custom checks.
	    && exact(this)->verify_link_criterion_(current_object,
						   start_point, p, anchor);
	}

	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::valid_link(unsigned current_object,
					   const P& start_point,
					   const P& p,
					   anchor::Type anchor)
	{
	  return this->labeled_image_.domain().has(p)
	    && exact(this)->valid_link_(current_object, start_point, p, anchor);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::validate_link(unsigned current_object,
					      const P& start_point,
					      const P& p,
					      anchor::Type anchor)
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
	  validate_link(current_object, start_point, p, anchor::MassCenter);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::invalidate_link(unsigned current_object,
						const P& start_point,
						const P& p,
						anchor::Type anchor)
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
	  invalidate_link(current_object, start_point, p, anchor::MassCenter);
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
					    anchor::Type anchor)
	{
	  return exact(this)->start_point_(current_object, anchor);
	}


	template <typename L, typename E>
	inline
	mln_site(L)
	link_functor_base<L,E>::start_point(unsigned current_object)
	{
	  return start_point(current_object, anchor::MassCenter);
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


	template <typename L, typename E>
	inline
	const L&
	link_functor_base<L,E>::labeled_image() const
	{
	  return labeled_image_;
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
	mln::util::couple<anchor::Type, mln_site(L)>
	link_functor_base<L,E>::finalize_link_(unsigned current_object)
	{
	  (void) current_object;
	  // No-Op
	  return mln::make::couple(anchor::Invalid, P());
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
						       const P& p,
						       anchor::Type anchor) const
	{
	  (void) current_object;
	  (void) start_point;
	  (void) p;
	  (void) anchor;
	  // No-Op
	  return true;
	}


	template <typename L, typename E>
	inline
	bool
	link_functor_base<L,E>::valid_link_(unsigned current_object,
					    const P& start_point,
					    const P& p,
					    anchor::Type anchor)
	{
	  return is_potential_link(current_object, start_point, p)
	    && verify_link_criterion(current_object, start_point, p, anchor);
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::validate_link_(unsigned current_object,
					       const P& start_point,
					       const P& p,
					       anchor::Type anchor)
	{
	  (void) start_point;
	  (void) anchor;
	  this->links_.update(current_object, this->labeled_image_(p));
	}


	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::invalidate_link_(unsigned current_object,
						 const P& start_point,
						 const P& p,
						 anchor::Type anchor)
	{
	  (void) current_object;
	  (void) start_point;
	  (void) p;
	  (void) anchor;
	  // No-op
	}


	template <typename L, typename E>
	inline
	mln_site(L)
	link_functor_base<L,E>::start_point_(unsigned current_object,
					     anchor::Type anchor)
	{
	  (void) anchor;
	  return internal::compute_anchor(this->components_,
					  current_object, anchor);
	}

	template <typename L, typename E>
	inline
	void
	link_functor_base<L,E>::start_processing_object_(
	  unsigned /* current_object */)
	{
	  // No-Op
	}


# endif // ! MLN_INCLUDE_ONLY


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_CORE_INTERNAL_LINK_FUNCTOR_BASE_HH
