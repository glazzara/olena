// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_CORE_LINE_SET_HH
# define SCRIBO_CORE_LINE_SET_HH

/// \file
///
/// \brief Definition of a line set.

# include <mln/util/array.hh>

# include <mln/accu/pair.hh>
# include <mln/accu/center.hh>
# include <mln/accu/shape/bbox.hh>

# include <mln/labeling/compute.hh>
# include <mln/make/relabelfun.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/convert/from_to.hh>


# include <scribo/core/macros.hh>
# include <scribo/core/line_info.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/object_groups.hh>


namespace scribo
{

  // Forward declaration.
  template <typename L> struct line_set;
  template <typename L> struct line_info;


  typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;

  namespace internal
  {
    /// Data structure for \c scribo::line_set<I>.
    template <typename L>
    struct line_set_data
    {
      line_set_data();
      line_set_data(const object_groups<L>& comp_set);
      line_set_data(const mln::util::array<scribo::line_info<L> >& infos,
		    const object_groups<L>& comp_set);

      mln::util::array<scribo::line_info<L> > infos_;
      component_set<L> components_;
      object_links<L> links_;
      object_groups<L> groups_;
    };

  } // end of namespace scribo::internal


  /*! \brief Lines container.

    Line ids start from 1.

   */
  template <typename L>
  class line_set
  {
  public:

    /// Constructors
    /// @{
    /// Constructor without argument.
    line_set();

    /// Constructor from object groups.
    line_set(const object_groups<L>& groups);

    /// Constructor useful for delayed construction (loading from file).
    line_set(const object_groups<L>& groups,
	     const mln::util::array<line_info<L> >& line_data);
    /// @}

    /// Compute line stats and fill the underlying information.
    void compute_lines(const object_groups<L>& groups);

    /// Return the line count.
    mln_value(L) nelements() const;

    /// Return line information for a given line id \p id.
    const line_info<L>& info(const mln_value(L)& id) const;

    /// Return line information for a given line id \p id.
    line_info<L>& info(const mln_value(L)& id);

    /// Return line information for a given line id \p id.
    line_info<L>& operator()(const line_id_t& id);

    /// Return line information for a given line id \p id.
    const line_info<L>& operator()(const line_id_t& id) const;

    /// Update tag of lines set to 'false' in \p f with \p tag.
    template <typename F>
    void update_tags(const mln::Function_v2b<F>& f, line::Tag tag);

    /// Create a copy of this line_set<L>
    line_set<L> duplicate() const;

    /// Return the underlying component set.
    const component_set<L>& components() const;

    /// Return the underlying component set (non-const version).
    component_set<L>& components_();

    /// Return the underlying component group.
    const object_groups<L>& groups() const;

    /// Return the underlying links.
    const object_links<L>& links() const;


    /// Massive line computation
    /// @{

    void force_stats_update();

    /// @}

    /// Internal methods
    /// @{

    /// Return all the line infos.
    const mln::util::array<scribo::line_info<L> >& infos_() const;

    /// @}


    /// Return false if it is not initialized (built with the default
    /// constructor).
    bool is_valid() const;


    void update_line_data_(const mln::util::array<line_info<L> >& line_data);


  private:
    /// Duplicate the underlying image and create a new line_set.
    void init_(const line_set<L>& model);

    mln::util::tracked_ptr< internal::line_set_data<L> > data_;
  };

  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_set<L>& lines);

  namespace make
  {

    template <typename L>
    scribo::line_set<L>
    line_set(const object_groups<L>& groups);

  } // End of namespace scribo::make



# ifndef MLN_INCLUDE_ONLY


  // line_set_data<L> >

  namespace internal
  {

    // data< line_set<L> >


    template <typename L>
    inline
    line_set_data<L>::line_set_data()
    {
    }


    template <typename L>
    inline
    line_set_data<L>::line_set_data(const object_groups<L>& groups)
      : components_(groups.components()), links_(groups.links()),
	groups_(groups)
    {
    }


    template <typename L>
    inline
    line_set_data<L>::line_set_data(const mln::util::array<scribo::line_info<L> >& infos,
				    const object_groups<L>& groups)
      : infos_(infos), components_(groups.components()),
	links_(groups.links()), groups_(groups)
    {
    }

  } // end of namespace mln::internal




  template <typename L>
  inline
  line_set<L>::line_set()
  {
  }


  template <typename L>
  inline
  line_set<L>::line_set(const object_groups<L>& groups)
  {
    compute_lines(groups);
  }


  template <typename L>
  inline
  line_set<L>::line_set(const object_groups<L>& groups,
			const mln::util::array<line_info<L> >& line_data)
  {
    data_ = new internal::line_set_data<L>(line_data, groups);
  }


  template <typename L>
  void
  line_set<L>::compute_lines(const object_groups<L>& groups)
  {
    data_ = new internal::line_set_data<L>(groups);

    typedef mln_site(L) P;

    mln_value(L) n_groups = groups.nelements() - 1;
    mln::fun::i2v::array<mln_value(L)>
      packed_groups = mln::make::relabelfun(groups.comp_to_group(),
					    n_groups, n_groups);

    // FIXME: object_groups should store the relation 'group -> comp'.
    mln::util::array< mln::util::array<component_id_t> >
      group_to_comps(value::next(n_groups));


    // 1st pass - Compute data.
    for_all_comps(i, data_->components_)
      if (data_->components_(i).is_valid())
      {
	unsigned group_id = packed_groups(i);
	if (group_id != 0) // Is this component part of a group?
	{
	  // Component id.
	  group_to_comps(group_id).append(i);
	}
      }

    // 2nd pass - Store data.
    data_->infos_.reserve(group_to_comps.size());
    data_->infos_.append(line_info<L>()); // line with id 0 is invalid.

    for_all_groups(i, group_to_comps)
    {
      // Add line info.
      line_info<L> info(*this, i, group_to_comps(i));
      data_->infos_.append(info);
    }
  }



  template <typename L>
  inline
  mln_value(L)
  line_set<L>::nelements() const
  {
    return data_->infos_.nelements() - 1;
  }

  template <typename L>
  inline
  const line_info<L>&
  line_set<L>::info(const mln_value(L)& id) const
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  line_info<L>&
  line_set<L>::info(const mln_value(L)& id)
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  const line_info<L>&
  line_set<L>::operator()(const line_id_t& id) const
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  line_info<L>&
  line_set<L>::operator()(const line_id_t& id)
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  template <typename F>
  inline
  void
  line_set<L>::update_tags(const mln::Function_v2b<F>& f_,
				line::Tag tag)
  {
    const F& f = exact(f_);

    for_all_lines_info(i, data_->infos_)
      if (!f(i))
	data_->infos_[i].update_tag(tag);
  }


  template <typename L>
  inline
  line_set<L>
  line_set<L>::duplicate() const
  {
    line_set<L> output;
    output.init_(*this);
    return output;
  }

  template <typename L>
  inline
  const component_set<L>&
  line_set<L>::components() const
  {
    return data_->components_;
  }

  template <typename L>
  inline
  component_set<L>&
  line_set<L>::components_()
  {
    return data_->components_;
  }

  template <typename L>
  inline
  const object_groups<L>&
  line_set<L>::groups() const
  {
    return data_->groups_;
  }

  template <typename L>
  inline
  const object_links<L>&
  line_set<L>::links() const
  {
    return data_->links_;
  }

  template <typename L>
  inline
  void
  line_set<L>::force_stats_update()
  {
    for_all_lines_info(i, data_->infos_)
      if (data_->infos_(i).tag() == line::Needs_Precise_Stats_Update)
	data_->infos_(i).force_stats_update();
  }

  template <typename L>
  inline
  const mln::util::array<scribo::line_info<L> >&
  line_set<L>::infos_() const
  {
    return data_->infos_;
  }

  template <typename L>
  inline
  bool
  line_set<L>::is_valid() const
  {
    return data_ && data_->groups_.is_valid();
  }

  template <typename L>
  inline
  void
  line_set<L>::update_line_data_(const mln::util::array<line_info<L> >& line_data)
  {
    data_->infos_ = line_data;
  }

  template <typename L>
  inline
  void
  line_set<L>::init_(const line_set<L>& set)
  {
    data_ = new internal::line_set_data<L>(set.infos_(), set.groups());
  }



  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_set<L>& lines)
  {
    ostr << "line_set[" << std::endl;
    for_all_lines(i, lines)
      ostr << lines(i);
    ostr << "]" << std::endl;

    return ostr;
  }


  // Make routines.

  namespace make
  {

    template <typename L>
    scribo::line_set<L>
    line_set(const object_groups<L>& groups)
    {
      mln_precondition(groups.is_valid());
      scribo::line_set<L> tmp(groups);
      return tmp;
    }

  } // end of namespace scribo::make


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_LINE_SET_HH
