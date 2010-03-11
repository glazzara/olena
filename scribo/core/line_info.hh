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

#ifndef SCRIBO_CORE_LINE_INFO_HH
# define SCRIBO_CORE_LINE_INFO_HH

/// \file
///
/// \brief Line information data structure.


# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/accu/stat/median_h.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/util/object_id.hh>

# include <scribo/core/tag/component.hh>
# include <scribo/core/tag/line.hh>

// # include <scribo/filter/object_links_bottom_aligned.hh>
// # include <scribo/filter/object_links_top_aligned.hh>


namespace scribo
{

  typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;

  template <typename L>
  class line_info
  {
    typedef mln::util::object_id<scribo::ComponentId, unsigned> component_id_t;
    typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;

  public:
    line_info();
    line_info(const line_id_t& id,
	      const mln::box2d& bbox,
	      const mln::util::array<component_id_t>& comps,
	      unsigned absolute_median,
	      unsigned absolute_baseline,
	      unsigned char_space,
	      unsigned char_width);
    line_info(const object_links<L>& links,
	      const line_id_t& id,
	      const mln::util::array<component_id_t>& comps);

    line_id_t id() const;

    line::Tag tag() const;
    void update_tag(line::Tag tag);

    const mln::box2d& bbox() const;

    const mln::util::array<component_id_t>& components() const;
    unsigned card() const;

    unsigned baseline() const;
    unsigned median() const;
    int x_height() const;
    int d_height() const;
    int a_height() const;

    unsigned char_space() const;
    unsigned char_width() const;

    unsigned word_space() const;

    line::ReadingDirection reading_direction() const;
    line::Type type() const;
    bool reverse_video() const;

    float orientation() const;
    float reading_orientation() const;

    bool indented() const;


    bool is_valid() const;


    /// Merge related routines.
    /// @{

    /// This merge only updates the component list and the bounding box.
    ///
    /// After this merge, the line is tagged with
    /// line::Needs_Precise_Stats_Update.
    //
    void fast_merge(line_info<L>& other);

    /// This merge updates the component list and recompute from
    /// scratch statistics, bounding box and other line attributes.
    ///
    /// After this merge, the line is tagged with line::None.
    //
    void precise_merge(line_info<L>& other);

    /// @}


    /// Force a new computation of statistics.
    void force_stats_update();


  private:
    line_id_t id_;
    line::Tag tag_;
    mln::box2d bbox_;
    mln::util::array<component_id_t> components_;

    // Value relative to the line bbox.
    unsigned baseline_;
    unsigned median_;

    // Values relative to the baseline.
    int x_height_;
    int d_height_;
    int a_height_;


    // Character related stats.
    unsigned char_space_;
    unsigned char_width_;

    // Words related stats.
    unsigned word_space_;

    // Reading direction
    line::ReadingDirection reading_direction_;

    // Line type
    line::Type type_;

    // Is this line in reverse video?
    bool reverse_video_;

    // Text orientation
    float orientation_;

    // Text reading orientation
    float reading_orientation_;

    bool indented_;

    // Related object links information.
    const object_links<L>* links_;
  };


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_info<L>& info);


# ifndef MLN_INCLUDE_ONLY


  template <typename L>
  line_info<L>::line_info()
    : id_(0)
  {

  }


  /*!

    Exemple:

    ---------------------
    | | | |x| | | | | | |   ----> a_height = 4
    ---------------------
    | | | |x| | | | | | |
    ---------------------
    | |x|x|x| | |x|x|x| |   ----> Median = 2      ^
    ---------------------                         |
    | |x| |x| | |x| |x| |                         | x_height = 3
    ---------------------                         |
    | |x|x|x| | |x|x|x| |   ----> Baseline = 4    v
    ---------------------
    | | | | | | |x| | | |
    ---------------------
    | | | | | | |x| | | |   ----> d_height = -2
    ---------------------

    All the metrics are computed relatively to the Baseline.

    The baseline is defined as an absolute row index.

  */

  template <typename L>
  line_info<L>::line_info(const object_links<L>& links,
			  const line_id_t& id,
			  const mln::util::array<component_id_t>& comps)
    : id_(id), tag_(line::None), components_(comps), links_(&links)
  {
    force_stats_update();


//     typedef mln_site(L) P;
//     const component_set<L>& comp_set = links_->component_set_();
//     mln::accu::shape::bbox<P> bbox;
//     for_all_elements(i, components_)
//     {
//       unsigned c = components_(i);
//       const box2d& bb = comp_set(c).bbox();
//       // Bounding box.
//       bbox.take(bb);
//     }
//     bbox_ = bbox.to_result();




    // FIXME: set valid information for these attributes.
    word_space_ = 0;
    reading_direction_ = line::LeftToRight;
    type_ = line::Paragraph;
    reverse_video_ = false;

    orientation_ = 0.;
    reading_orientation_ = 0.;

    indented_ = false;
  }


  template <typename L>
  typename line_info<L>::line_id_t
  line_info<L>::id() const
  {
    return id_;
  }

  template <typename L>
  line::Tag
  line_info<L>::tag() const
  {
    return tag_;
  }


  template <typename L>
  void
  line_info<L>::update_tag(line::Tag tag)
  {
    tag_ = tag;
  }


  template <typename L>
  const mln::box2d&
  line_info<L>::bbox() const
  {
    return bbox_;
  }


  template <typename L>
  const mln::util::array<typename line_info<L>::component_id_t>&
  line_info<L>::components() const
  {
    return components_;
  }

  template <typename L>
  unsigned
  line_info<L>::card() const
  {
    return components_.size();
  }


  template <typename L>
  unsigned
  line_info<L>::baseline() const
  {
    return baseline_;
  }


  template <typename L>
  unsigned
  line_info<L>::median() const
  {
    return median_;
  }

  template <typename L>
  int
  line_info<L>::x_height() const
  {
    return x_height_;
  }


  template <typename L>
  int
  line_info<L>::d_height() const
  {
    return d_height_;
  }


  template <typename L>
  int
  line_info<L>::a_height() const
  {
    return a_height_;
  }


  template <typename L>
  unsigned
  line_info<L>::char_space() const
  {
    return char_space_;
  }


  template <typename L>
  unsigned
  line_info<L>::char_width() const
  {
    return char_width_;
  }


  template <typename L>
  unsigned
  line_info<L>::word_space() const
  {
    return word_space_;
  }


  template <typename L>
  line::ReadingDirection
  line_info<L>::reading_direction() const
  {
    return reading_direction_;
  }

  template <typename L>
  line::Type
  line_info<L>::type() const
  {
    return type_;
  }


  template <typename L>
  bool
  line_info<L>::reverse_video() const
  {
    return reverse_video_;
  }


  template <typename L>
  float
  line_info<L>::orientation() const
  {
    return orientation_;
  }


  template <typename L>
  float
  line_info<L>::reading_orientation() const
  {
    return reading_orientation_;
  }


  template <typename L>
  bool
  line_info<L>::indented() const
  {
    return indented_;
  }


  template <typename L>
  bool
  line_info<L>::is_valid() const
  {
    return id_ != 0u;
  }


  template <typename L>
  void
  line_info<L>::fast_merge(line_info<L>& other)
  {
    tag_ = line::Needs_Precise_Stats_Update;
    other.update_tag(line::Merged);

    bbox_.merge(other.bbox());
    components_.append(other.components());
  }


  template <typename L>
  void
  line_info<L>::precise_merge(line_info<L>& other)
  {
    fast_merge(other);
    force_stats_update();
  }

  template <typename L>
  void
  line_info<L>::force_stats_update()
  {
    typedef mln_site(L) P;
    const component_set<L>& comp_set = links_->component_set_();

    // FIXME: int_u<12> may not be enought but we can't use unsigned
    // or any other larger types since there is no median
    // implementation for high quantification types...

    // Init.
    typedef mln::value::int_u<12> median_data_t;
    typedef mln::accu::stat::median_h<median_data_t> median_t;
    median_t
      absolute_median,
      absolute_baseline,
      char_space,
      char_width;

    mln::accu::shape::bbox<P> bbox;

    for_all_elements(i, components_)
    {
      unsigned c = components_(i);

      const box2d& bb = comp_set(c).bbox();

      // Space between characters.
      int space = bb.pmin().col()
	- comp_set((*links_)[c]).bbox().pmax().col();
      // -- Ignore overlapped characters.
      if (space > 0)
	char_space.take(space);

      // Character width
      // -- Ignore too large components.
      if (bb.width() <= 1000)
	char_width.take(bb.width());

      // Median (compute an absolute value, from the top left
      // corner of the image).
      absolute_median.take(bb.pmin().row());

      // Baseline (compute an absolute value, from the top left
      // corner of the image).
      absolute_baseline.take(bb.pmax().row());

      // Bounding box.
      bbox.take(bb);
    }

    // Finalization
    {
      tag_ = line::None;
      bbox_ = bbox.to_result();

      // Char space
      if (card() == 1)
	char_space_ = 0;
      else
	char_space_ = char_space.to_result();

      // Char width
      if (card() == 2)
	char_width_ = (comp_set(components_[0]).bbox().width()
		       + comp_set(components_[1]).bbox().width()) / 2;
      else
	char_width_ = char_width.to_result();


      baseline_ = absolute_baseline.to_result();
      median_   = absolute_baseline - absolute_median;
      x_height_ = absolute_baseline - absolute_median + 1;
      d_height_ = absolute_baseline - bbox.to_result().pmax().row();
      a_height_ = absolute_baseline - bbox.to_result().pmin().row() + 1;

      //FIXME
      //
      //word_space_ = ...;
      //reading_direction_ = ...;
      //type_ = ...;
      //reverse_video_ = ...;
      //orientation_ = ...;
      //reading_orientation_ = ...;
      //indented_ = ...;
    }
  }


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_info<L>& info)
  {
    return ostr << "line_info("
		<< "id=" << info.id()
		<< ", tag=" << info.tag()
		<< ", bbox=" << info.bbox()
		<< ", components=" << info.components()
		<< ", baseline=" << info.baseline()
		<< ", median=" << info.median()
		<< ", x_height=" << info.x_height()
		<< ", d_height=" << info.d_height()
		<< ", a_height=" << info.a_height()
		<< ", char_space=" << info.char_space()
		<< ", char_width=" << info.char_width()
		<< ", word_space=" << info.word_space()
		<< ", reading_direction=" << info.reading_direction()
		<< ", type=" << info.type()
		<< ", reverse_video=" << info.reverse_video()
		<< ", orientation=" << info.orientation()
		<< ", reading_orientation=" << info.reading_orientation()
		<< ", indented=" << info.indented()
		<< ")" << std::endl;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_LINE_INFO_HH
