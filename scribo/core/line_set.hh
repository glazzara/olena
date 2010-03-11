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
# include <scribo/core/line_stats_extra.hh>


namespace scribo
{

  // Forward declaration.
  template <typename L> struct line_set;


  namespace internal
  {
    /// Data structure for \c scribo::line_set<I>.
    template <typename L>
    struct line_set_data
    {
      line_set_data();
      line_set_data(const mln::util::array<scribo::line_info>& infos,
		    const component_set<L>& comp_set);

      mln::util::array<scribo::line_info> infos_;
      component_set<L> comp_set_;
    };

  } // end of namespace scribo::internal



  template <typename L>
  class line_set
  {
  public:

    /// Constructors
    /// @{
    /// Constructor without argument.
    line_set();

    /// Constructor from object groups.
    line_set(const object_links<L>& links, const object_groups<L>& groups);
    /// @}

    /// Compute line stats and fill the underlying information.
    util::array<line_stats_extra> compute_lines(const object_links<L>& links,
						const object_groups<L>& groups);

    /// Return the line count.
    mln_value(L) nelements() const;

    /// Return line information for a given line id \p id.
    const line_info& info(const mln_value(L)& id) const;

    /// Return line information for a given line id \p id.
    line_info& info(const mln_value(L)& id);

    /// Return line information for a given line id \p id.
    line_info& operator()(const line_id_t& id);

    /// Return line information for a given line id \p id.
    const line_info& operator()(const line_id_t& id) const;

    /// Update tag of lines set to 'false' in \p f with \p tag.
    template <typename F>
    void update_tags(const mln::Function_v2b<F>& f, line::Tag tag);

    /// Create a copy of this line_set<L>
    line_set<L> duplicate() const;

    /// Return the underlying component set.
    const component_set<L>& component_set_() const;

    /// Internal methods
    /// @{

    /// Return all the line infos.
    const mln::util::array<scribo::line_info>& infos_() const;

    /// @}

  private:
    /// Duplicate the underlying image and create a new line_set.
    void init_(const line_set<L>& model);

    mln::util::tracked_ptr< internal::line_set_data<L> > data_;
  };


  namespace make
  {

    template <typename L>
    scribo::line_set<L>
    line_set(const object_links<L>& links,
	     const object_groups<L>& groups);

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
    line_set_data<L>::line_set_data(const mln::util::array<scribo::line_info>& infos,
				    const component_set<L>& comp_set)
      : infos_(infos), comp_set_(comp_set)
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
  line_set<L>::line_set(const object_links<L>& links,
			const object_groups<L>& groups)
  {
    compute_lines(links, groups);
  }


  // FIXME: groups should have a reference to the links data and we
  // should only required groups as argument.
  template <typename L>
  util::array<line_stats_extra>
  line_set<L>::compute_lines(const object_links<L>& links,
			     const object_groups<L>& groups)
  {
    data_ = new internal::line_set_data<L>();

    typedef mln_site(L) P;
    data_->comp_set_ = groups.component_set_();
    const component_set<L>& comp_set = groups.component_set_();

    mln_value(L) n_groups = groups.nelements() - 1;
    mln::fun::i2v::array<mln_value(L)>
      packed_groups = mln::make::relabelfun(groups.comp_to_group(),
					    n_groups, n_groups);

    // FIXME: object_groups should store the relation 'group -> comp'.
    //        it would avoid the use of accumulator arrays.

    // FIXME: int_u<11> may not be enought but we can't use unsigned
    // or any other larger types since there is no median
    // implementation for high quantification types...

    // Init.
    typedef mln::value::int_u<12> median_data_t;
    typedef mln::accu::stat::median_h<median_data_t> median_t;
    util::array<median_t>
      absolute_median(static_cast<unsigned>(n_groups) + 1),
      absolute_baseline(static_cast<unsigned>(n_groups) + 1),
      char_space(static_cast<unsigned>(n_groups) + 1),
      char_width(static_cast<unsigned>(n_groups) + 1);

    util::array<mln::accu::shape::bbox<P> >
      bbox(static_cast<unsigned>(n_groups) + 1);

    util::array< util::array<component_id_t> >
      comps(static_cast<unsigned>(n_groups) + 1);

    // 1st pass - Compute data.
    for (unsigned i = 1; i < packed_groups.size(); ++i)
      if (comp_set(i).tag() != component::Ignored)
      {
	unsigned group_id = packed_groups(i);
	if (group_id != 0) // Is this component part of a group?
	{
	  const box2d& bb = comp_set(i).bbox();

	  // Space between characters.
	  int space = bb.pmin().col()
	    - comp_set(links[i]).bbox().pmax().col();
	  // -- Ignore overlapped characters.
	  if (space > 0)
	    char_space(group_id).take(space);

	  // Character width
	  // -- Ignore too large components.
	  if (bb.width() <= 1000)
	    char_width(group_id).take(bb.width());

	  // Median (compute an absolute value, from the top left
	  // corner of the image).
	  absolute_median(group_id).take(bb.pmin().row());

	  // Baseline (compute an absolute value, from the top left
	  // corner of the image).
	  absolute_baseline(group_id).take(bb.pmax().row());

	  // Bounding box.
	  bbox(group_id).take(bb);

	  // Component id.
	  comps(group_id).append(i);
	}
      }

    // 2nd pass - Store data.
    data_->infos_.reserve(groups.nelements());
    data_->infos_.append(line_info()); // line with id 0 is invalid.

    util::array<line_stats_extra> stats_extra;
    stats_extra.reserve(static_cast<unsigned>(n_groups) + 1);
    stats_extra.append(line_stats_extra());

    for (unsigned i = 1; i <= n_groups; ++i)
    {
      // FIXME: should be removable when object_group will store the
      // relation 'group -> comp'
      // Special cases for lines with 1 or 2 components.
      // {
      int
	cspace,
	cwidth;
      if (comps(i).nelements() == 1)
	cspace = 0;
      else
	cspace = char_space(i).to_result();
      if (comps(i).nelements() == 2)
	cwidth = (comp_set(comps(i)[0]).bbox().width()
		  + comp_set(comps(i)[1]).bbox().width()) / 2;
      else
	cwidth = char_width(i).to_result();
      // }


      // Add line info.
      line_info info(i,
		     bbox(i).to_result(),
		     comps(i),
		     absolute_median(i).to_result(),
		     absolute_baseline(i).to_result(),
		     cspace,
		     cwidth);
      data_->infos_.append(info);


      // Prepare extra stats to be returned.
      line_stats_extra stats(absolute_median(i) * absolute_median(i).card(),
			     absolute_median(i).card(),
			     absolute_baseline(i) * absolute_baseline(i).card(),
			     absolute_baseline(i).card(),
			     char_space(i) * char_space(i).card(),
			     char_space(i).card(),
			     char_width(i) * char_width(i).card(),
			     char_width(i).card());
      stats_extra.append(stats);
    }

    return stats_extra;
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
  const line_info&
  line_set<L>::info(const mln_value(L)& id) const
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  line_info&
  line_set<L>::info(const mln_value(L)& id)
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  const line_info&
  line_set<L>::operator()(const line_id_t& id) const
  {
    return this->data_->infos_[id];
  }

  template <typename L>
  inline
  line_info&
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

    for_all_elements(i, data_->infos_)
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
  line_set<L>::component_set_() const
  {
    return data_->comp_set_;
  }


  template <typename L>
  inline
  const mln::util::array<scribo::line_info>&
  line_set<L>::infos_() const
  {
    return data_->infos_;
  }

  template <typename L>
  inline
  void
  line_set<L>::init_(const line_set<L>& set)
  {
    data_ = new internal::line_set_data<L>(set.infos_(), set.component_set_());
  }


  // Make routines.

  namespace make
  {

    template <typename L>
    scribo::line_set<L>
    line_set(const object_links<L>& links,
	     const object_groups<L>& groups)
    {
      mln_precondition(exact(ima).is_valid());
      scribo::line_set<L> tmp(links, groups);
      return tmp;
    }

    template <typename L>
    scribo::line_set<L>
    line_set(const object_links<L>& links,
	     const object_groups<L>& groups,
	     util::array<line_stats_extra>& line_stats)
    {
      mln_precondition(exact(ima).is_valid());
      scribo::line_set<L> tmp;
      line_stats = tmp.compute_lines(links, groups);
      return tmp;
    }

  } // end of namespace scribo::make


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_LINE_SET_HH
