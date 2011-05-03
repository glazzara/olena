// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_CORE_OBJECT_GROUPS_HH
# define SCRIBO_CORE_OBJECT_GROUPS_HH

/// \file
///
/// \brief Object groups representation.

# include <mln/util/array.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>

# include <scribo/core/group_info.hh>
# include <scribo/core/internal/sort_comp_ids.hh>
# include <scribo/core/concept/serializable.hh>

// Not to include.
//#include <scribo/core/line_info.hh>

namespace scribo
{

  using namespace mln;


  // Forward declaration.
  template <typename L> class object_groups;

  namespace internal
  {
    /// Data structure for \c scribo::object_groups<I>.
    template <typename L>
    struct object_groups_data
    {
      object_groups_data();
      object_groups_data(const object_links<L>& links);
      object_groups_data(const object_links<L>& links,
			 const mln::util::array<group_info>& info);

      mln::util::array<unsigned> comp_to_group_;
      mln::util::array<group_info> group_info_;

      component_set<L> components_;
      object_links<L> links_;
    };

  } // end of namespace scribo::internal




  /// \brief Object group representation.
  //
  template <typename L>
  class object_groups : public Serializable<object_groups<L> >
  {
    typedef internal::object_groups_data<L> data_t;

  public:
    object_groups();
    object_groups(const object_links<L>& links);
    // Used for incremental construction (xml loading)
    object_groups(const object_links<L>& links,
		  const mln::util::array<group_info>& info);

    const component_set<L>& components() const;
    const object_links<L>& links() const;

    bool is_valid() const;


    // Return the number of groups
    unsigned nelements() const;


    /// Return the group id of the component \p comp_id.
    const group_info& group_of(unsigned comp_id) const;
    group_info& group_of(unsigned comp_id);

    /// Return group info data for group with id \p group_id.
    /// Valid id starts from 1.
    const group_info& operator()(unsigned group_id) const;
    group_info& operator()(unsigned group_id);

    // Map component ids to group ids.
    const mln::util::array<unsigned>& comp_to_group() const;

    object_groups<L> duplicate() const;


  private: // attributes
    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_groups<L>& groups);

  template <typename L>
  bool
  operator==(const object_groups<L>& lhs, const object_groups<L>& rhs);


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {


    /// Data structure for \c scribo::object_groups<I>.
    template <typename L>
    object_groups_data<L>::object_groups_data()
    {
    }


    template <typename L>
    object_groups_data<L>::object_groups_data(const object_links<L>& links)
      : comp_to_group_(unsigned(links.nelements())),
	components_(links.components()), links_(links)
    {
      comp_to_group_ = links.comp_to_link();

      unsigned ngroups = 0;
      util::array<unsigned> new_id(comp_to_group_.nelements(), 0);
      mln::util::array<mln::util::array<component_id_t> > comp_ids(1);
      mln::util::array<accu::shape::bbox<mln_site(L)> > bboxes(1);
      mln::util::array<unsigned> pixel_areas(1);

      // Remove potential loops in linking
      // FIXME: we may try to avoid loops while linking...
      {
      	util::array<bool> deja_vu(comp_to_group_.nelements());
      	for_all_elements(e, comp_to_group_)
      	  if (comp_to_group_(e) != e && comp_to_group_(e) != 0)
      	  {
	    deja_vu.fill(false); // FIXME: ugly!
      	    unsigned cur = e;
	    deja_vu(cur) = true;
      	    while (comp_to_group_(cur) != cur && !deja_vu(comp_to_group_(cur)))
      	    {
      	      cur = comp_to_group_(cur);
      	      deja_vu(cur) = true;
      	    }
      	    // Break the loop!
      	    if (comp_to_group_(cur) != cur && deja_vu(comp_to_group_(cur)))
      	      comp_to_group_(cur) = cur;
      	  }
      }

      for_all_elements(e, comp_to_group_)
	if (comp_to_group_(e) != 0)
	{
	  // Make sure there is no intermediate ids to reach the root.
	  // FIXME: useful?
	  unsigned e_root = internal::find_root(comp_to_group_, e);

	  if (! new_id(e_root))
	  {
	    new_id(e_root) = ++ngroups;
	    comp_ids.resize(comp_ids.size() + 1);
	    bboxes.resize(bboxes.size() + 1);
	    pixel_areas.resize(pixel_areas.size() + 1, 0);
	  }

	  unsigned nid = new_id(e_root);
	  comp_ids(nid).append(e);

	  bboxes(nid).take(components_(e).bbox());
	  pixel_areas(nid) += components_(e).card();
	}

      group_info_.resize(1);
      group_info_.reserve(ngroups);
      util::array<unsigned> group_idx(ngroups + 1, 0);

      for (unsigned i = 1; i < new_id.nelements(); ++i)
	if (new_id(i))
	{
	  unsigned id = new_id(i);

	  // Order component ids according to component localization (left
	  // to right).
	  std::sort(comp_ids(id).hook_std_vector_().begin(),
		    comp_ids(id).hook_std_vector_().end(),
		    internal::sort_comp_ids<L>(components_));

	  group_idx(id) = group_info_.size();
	  group_info_.append(group_info(group_info_.size(), comp_ids(id), pixel_areas(id), bboxes(id)));
	}

      // Update mapping comp/group with new ids.  Note: group id is
      // different from its location in group_info array during
      // construction.
      for (unsigned i = 0; i < comp_to_group_.nelements(); ++i)
	comp_to_group_(i) = group_idx(new_id(comp_to_group_(i)));
    }

    template <typename L>
    object_groups_data<L>::object_groups_data(const object_links<L>& links,
					      const mln::util::array<group_info>& info)
      : comp_to_group_(unsigned(links.nelements())), group_info_(info),
	components_(links.components()), links_(links)
    {
      for_all_groups(g, group_info_)
	for_all_elements(e, group_info_(g).component_ids())
	  comp_to_group_(group_info_(g).component_ids()(e)) = group_info_(g).id();
    }

  } // end of namespace scribo::internal


  template <typename L>
  object_groups<L>::object_groups()
  {
  }

  template <typename L>
  object_groups<L>::object_groups(const object_links<L>& links)
  {
    data_ = new data_t(links);
  }

  template <typename L>
  object_groups<L>::object_groups(const object_links<L>& links,
				  const mln::util::array<group_info>& info)
  {
    data_ = new data_t(links, info);
  }

  template <typename L>
  const component_set<L>&
  object_groups<L>::components() const
  {
    return data_->components_;
  }

  template <typename L>
  const object_links<L>&
  object_groups<L>::links() const
  {
    mln_assertion(data_ != 0);
    return data_->links_;
  }

  template <typename L>
  bool
  object_groups<L>::is_valid() const
  {
    mln_assertion(data_->components_.nelements() == data_->comp_to_group_.nelements() - 1);
    return data_->links_.is_valid();
  }

  template <typename L>
  unsigned
  object_groups<L>::nelements() const
  {
    return data_->group_info_.nelements();
  }

  template <typename L>
  const group_info&
  object_groups<L>::group_of(unsigned comp_id) const
  {
    mln_precondition(comp_id < data_->comp_to_group_.nelements());
    mln_assertion(data_->group_info_(data_->comp_to_group_(comp_id)).id()
		  == data_->comp_to_group_(comp_id));
    return data_->group_info_(data_->comp_to_group_(comp_id));
  }

  template <typename L>
  group_info&
  object_groups<L>::group_of(unsigned comp_id)
  {
    mln_precondition(comp_id < data_->comp_to_group_.nelements());
    mln_assertion(data_->group_info_(data_->comp_to_group_(comp_id)).id()
		  == data_->comp_to_group_(comp_id));
    return data_->group_info_(data_->comp_to_group_(comp_id));
  }

  template <typename L>
  const util::array<unsigned>&
  object_groups<L>::comp_to_group() const
  {
    return data_->comp_to_group_;
  }


  template <typename L>
  const group_info&
  object_groups<L>::operator()(unsigned group_id) const
  {
    mln_precondition(group_id < data_->group_info_.nelements());
    return data_->group_info_(group_id);
  }


  template <typename L>
  group_info&
  object_groups<L>::operator()(unsigned group_id)
  {
    mln_precondition(group_id < data_->group_info_.nelements());
    return data_->group_info_(group_id);
  }


  template <typename L>
  inline
  object_groups<L>
  object_groups<L>::duplicate() const
  {
    object_groups<L> output;
    output.data_ = new data_t();

    *(output.data_.ptr_) = *(data_.ptr_);
    return output;
  }

  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_groups<L>& groups)
  {
    ostr << "object_groups[";

    for_all_groups(g, groups)
      ostr << groups(g) << ", ";

    ostr << " | comp_to_group=" << groups.comp_to_group();

    ostr << "]";

    return ostr;
  }


  template <typename L>
  bool
  operator==(const object_groups<L>& lhs, const object_groups<L>& rhs)
  {
    if (! (lhs.components() == rhs.components()))
    {
      std::cout << "group.comp" << std::endl;
      return false;
    }

    if (!( lhs.comp_to_group() == rhs.comp_to_group() && lhs.nelements() == rhs.nelements()))
    {
      std::cout << "group.comp_to_group" << std::endl;
      return false;
    }


    for_all_groups(g, lhs)
      if (! (lhs(g) == rhs(g)))
      {
	std::cout << "group.info" << std::endl;
	return false;
      }

    return true;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_OBJECT_GROUPS_HH
