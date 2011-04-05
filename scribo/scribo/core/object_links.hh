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

#ifndef SCRIBO_CORE_OBJECT_LINKS_HH
# define SCRIBO_CORE_OBJECT_LINKS_HH

/// \file
///
/// \brief Object links representation.


# include <mln/util/array.hh>
# include <mln/util/tracked_ptr.hh>

# include <scribo/core/component_set.hh>

# include <scribo/core/concept/serializable.hh>


namespace scribo
{

  using namespace mln;

  // Forward declaration.
  template <typename L> class object_links;


  namespace internal
  {
    /// Data structure for \c scribo::object_links<I>.
    template <typename L>
    struct object_links_data
    {
      object_links_data();
      object_links_data(const component_set<L>& components, unsigned size);
      object_links_data(const component_set<L>& components,
			unsigned size, unsigned value);

      mln::util::array<unsigned> comp_to_link_;
      component_set<L> components_;
    };

  } // end of namespace scribo::internal




  /// \brief Object group representation.
  //
  template <typename L>
  class object_links : public Serializable<object_links<L> >
  {
    typedef internal::object_links_data<L> data_t;

  public:
    object_links();
    object_links(const component_set<L>& components);
    object_links(const component_set<L>& components, unsigned value);

    const component_set<L>& components() const;

    bool is_valid() const;
    bool is_valid(unsigned comp_id) const;

    unsigned nelements() const;

    unsigned& operator()(unsigned comp_id);
    const unsigned& operator()(unsigned comp_id) const;

    const mln::util::array<unsigned>& comp_to_link() const;

    void init();

    object_links<L> duplicate() const;

  private:
    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_links<L>& links);


# ifndef MLN_INCLUDE_ONLY


  namespace internal
  {


    /// Data structure for \c scribo::object_links<I>.
    template <typename L>
    object_links_data<L>::object_links_data()
    {
    }


    template <typename L>
    object_links_data<L>::object_links_data(const component_set<L>& components,
					    unsigned size)
      : comp_to_link_(size), components_(components)
    {
    };


    template <typename L>
    object_links_data<L>::object_links_data(const component_set<L>& components,
					    unsigned size, unsigned value)
      : comp_to_link_(size, value), components_(components)
    {
    };


  } // end of namespace scribo::internal



  template <typename L>
  object_links<L>::object_links()
  {
    data_ = new data_t();
  }


  template <typename L>
  object_links<L>::object_links(const component_set<L>& components)
  {
    data_ = new data_t(components, value::next(components.nelements()));
  }


  template <typename L>
  object_links<L>::object_links(const component_set<L>& components,
				unsigned value)
  {
    data_ = new data_t(components, value::next(components.nelements()),
		       value);
  }


  template <typename L>
  const component_set<L>&
  object_links<L>::components() const
  {
    return data_->components_;
  }


  template <typename L>
  bool
  object_links<L>::is_valid() const
  {
    return data_->components_.is_valid()
      && data_->components_.nelements() == (this->nelements() - 1);
  }


  template <typename L>
  bool
  object_links<L>::is_valid(unsigned comp_id) const
  {
    mln_precondition(is_valid());
    mln_precondition(comp_id < data_->comp_to_link_.nelements());
    return data_->comp_to_link_(comp_id) != 0;
  }


  template <typename L>
  unsigned
  object_links<L>::nelements() const
  {
    return data_->comp_to_link_.nelements();
  }


  template <typename L>
  unsigned&
  object_links<L>::operator()(unsigned comp_id)
  {
    return data_->comp_to_link_(comp_id);
  }


  template <typename L>
  const unsigned&
  object_links<L>::operator()(unsigned comp_id) const
  {
    return data_->comp_to_link_(comp_id);
  }


  template <typename L>
  const mln::util::array<unsigned>&
  object_links<L>::comp_to_link() const
  {
    return data_->comp_to_link_;
  }


  template <typename L>
  void
  object_links<L>::init()
  {
    for (unsigned i = 0; i < nelements(); ++i)
      if (data_->components_(i).tag() == component::Ignored)
	data_->comp_to_link_(i) = 0;
      else
	data_->comp_to_link_(i) = i;
  }

  template <typename L>
  inline
  object_links<L>
  object_links<L>::duplicate() const
  {
    object_links<L> output;
    output.data_ = new data_t();

    *(output.data_.ptr_) = *(data_.ptr_);
    return output;
  }


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const object_links<L>& links)
  {
    ostr << "object_links[";

    for_all_links(l, links)
      ostr << l << "->" << links.comp_to_link()[l] << ", ";

    ostr << "]";

    return ostr;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_OBJECT_LINKS_HH
