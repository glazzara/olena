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

# include <scribo/core/concept/serializable.hh>

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
      object_groups_data(const object_links<L>& links, unsigned value);

      mln::util::array<unsigned> comp_to_group_;
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
    object_groups(const object_links<L>& links, unsigned value);

    const component_set<L>& components() const;
    const object_links<L>& links() const;

    void init_(const object_links<L>& links);

    bool is_valid() const;
    bool is_valid(unsigned comp_id) const;

    unsigned nelements() const;

    unsigned& operator()(unsigned comp_id);
    const unsigned& operator()(unsigned comp_id) const;

    const mln::util::array<unsigned>& comp_to_group() const;

    object_groups<L> duplicate() const;

    void init();

  private: // attributes
    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_groups<L>& groups);


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
    };


    template <typename L>
    object_groups_data<L>::object_groups_data(const object_links<L>& links,
					      unsigned value)
      : comp_to_group_(unsigned(links.nelements()), value),
	components_(links.components()), links_(links)
    {
    };


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
  object_groups<L>::object_groups(const object_links<L>& links, unsigned value)
  {
    data_ = new data_t(links, value);
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
  void
  object_groups<L>::init_(const object_links<L>& links)
  {
    mln_assertion(data_ != 0);
    data_->comp_to_group_ = links.comp_to_link();
  }

  template <typename L>
  bool
  object_groups<L>::is_valid() const
  {
    mln_assertion(data_->components_.nelements() == (nelements() - 1));
    return data_->links_.is_valid();
  }

  template <typename L>
  bool
  object_groups<L>::is_valid(unsigned comp_id) const
  {
    mln_assertion(is_valid());
    mln_assertion(comp_id < data_->links_.nelements());
    return data_->links_(comp_id) != 0;
  }

  template <typename L>
  unsigned
  object_groups<L>::nelements() const
  {
    return data_->comp_to_group_.nelements();
  }


  template <typename L>
  unsigned&
  object_groups<L>::operator()(unsigned comp_id)
  {
    return data_->comp_to_group_(comp_id);
  }


  template <typename L>
  const unsigned&
  object_groups<L>::operator()(unsigned comp_id) const
  {
    return data_->comp_to_group_(comp_id);
  }

  template <typename L>
  const mln::util::array<unsigned>&
  object_groups<L>::comp_to_group() const
  {
    return data_->comp_to_group_;
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
  void
  object_groups<L>::init()
  {
    for (unsigned i = 0; i < nelements(); ++i)
      data_->comp_to_group_(i) = i;
  }


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_groups<L>& groups)
  {
    ostr << "object_groups[";

    for_all_groups(g, groups)
      ostr << g << "->" << groups.comp_to_group()[g] << ", ";

    ostr << "]";

    return ostr;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_OBJECT_GROUPS_HH
