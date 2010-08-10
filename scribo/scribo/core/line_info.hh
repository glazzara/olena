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

#ifndef SCRIBO_CORE_LINE_INFO_HH
# define SCRIBO_CORE_LINE_INFO_HH

/// \file
///
/// \brief Line information data structure.
///
/// \fixme The meanline should not be stored! The user can deduce it
/// from the x_height and the baseline.


# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/accu/stat/median_h.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/util/object_id.hh>
# include <mln/value/int_u.hh>

# include <scribo/core/tag/component.hh>
# include <scribo/core/tag/line.hh>

# include <scribo/core/line_set.hh>
# include <scribo/core/component_set.hh>

namespace scribo
{

  // Forward declarations.
  template <typename L> class line_set;

  typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;

  template <typename L>
  class line_info
  {
    typedef mln::util::object_id<scribo::ComponentId, unsigned> component_id_t;
    typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;

  public:

    /// Constructors
    /// @{

    line_info();

    line_info(const line_set<L>& holder,
	      const line_id_t& id,
	      const mln::util::array<component_id_t>& comps);

    /// The line id of the target instance is preserved if it is valid.
    line_info(const line_info<L>& other);
    /// @}

    /// The line id of the target instance is preserved if it is valid.
    line_info<L>& operator=(const line_info<L>& other);

    /// If the line info is valid, the line id never changes for a
    /// given instance.
    //
    line_id_t id() const;

    line::Tag tag() const;
    void update_tag(line::Tag tag);

    const mln::box2d& bbox() const;

    /// Extended bounding box.
    /// The width is extended with char_width() + char_space() on each side.
    /// The height is adjusted to max(a_height, - d_height) on each side.
    //
    const mln::box2d& ebbox() const;

    const mln::util::array<component_id_t>& components() const;
    unsigned card() const;

    int baseline() const;
    int meanline() const;
    int ascent() const;
    int descent() const;

    unsigned x_height() const;
    int d_height() const;
    int a_height() const;

    unsigned char_space() const;
    unsigned char_width() const;

    unsigned word_space() const;

    line::ReadingDirection reading_direction() const;

    line::Type type() const;
    void update_type(line::Type type);

    bool reverse_video() const;

    float orientation() const;
    float reading_orientation() const;

    bool indented() const;

    bool has_text() const;
    const std::string& text() const;
    void update_text(const std::string& str);


    bool is_valid() const;

    /// Hidden status.
    ///
    /// When a line is hidden, it should not be used in routines
    /// computing data over lines.
    ///
    /// @{
    bool is_hidden() const;
    void set_hidden(bool b);
    /// @}

    /// Merge related routines.
    /// @{

    /// This merge only updates the component list and the bounding box.
    ///
    /// After this merge, the line is tagged with
    /// line::Needs_Precise_Stats_Update.
    ///
    /// The \p other line is tagged with line::Merged and if \p hide
    /// is set to 'True', it is set as hidden as well.
    //
    void fast_merge(line_info<L>& other, bool hide = true);

    /// This merge updates the component list and recompute from
    /// scratch statistics, bounding box and other line attributes.
    ///
    /// After this merge, the line is tagged with line::None.
    //
    void precise_merge(line_info<L>& other, bool hide = true);

    /// @}


    /// Force a new computation of statistics.
    void force_stats_update();


    /// Returns the line set holding this element.
    const line_set<L>& holder() const;

    /// Returns the delta used to compute the extended bbox.
    int delta_of_line() const;

  private: // Members
    void copy_data(const line_info<L>& other);

    /// Enlarge the width of a given bbox \p b with a \p delta.
    mln::box2d enlarge(const mln::box2d& b, int delta) const;

    /// Update bbox and ebbox_ attributes.
    void update_bbox_and_ebox(line_info<L>& other);

    /// Update the extended bbox.
    void update_ebbox();

    mln::box2d merged_ebbox(const scribo::line_info<L>& info_l,
			    const scribo::line_info<L>& info);

    void update_components_type(component::Type type);

  private: // Attributes
    // WARNING: NEVER FORGET TO UPDATE COPY CONSTRUCTOR REDEFINITION!!!!

    line_id_t id_;
    bool hidden_;
    line::Tag tag_;
    mln::box2d bbox_;
    mln::box2d ebbox_;
    mln::util::array<component_id_t> components_;

    // Values relative to the line bbox.
    int baseline_;
    int meanline_;

    // Values relative to the baseline.
    unsigned x_height_;
    int d_height_;
    int a_height_;

    // WARNING: NEVER FORGET TO UPDATE COPY CONSTRUCTOR REDEFINITION!!!!

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

    std::string text_;

    // Line set holding this element.
    line_set<L> holder_;

    // WARNING: NEVER FORGET TO UPDATE COPY CONSTRUCTOR REDEFINITION!!!!
  };


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_info<L>& info);


# ifndef MLN_INCLUDE_ONLY


  template <typename L>
  line_info<L>::line_info()
    : id_(0), hidden_(false)
  {

  }

  template <typename L>
  inline
  void
  line_info<L>::copy_data(const line_info<L>& other)
  {
    // Id MUST NOT change except if this instance have no id.
    if (! is_valid())
      id_ = other.id();

    hidden_ = other.hidden_;

    tag_ = other.tag();
    bbox_ = other.bbox();
    ebbox_ = other.ebbox();
    components_ = other.components();

    baseline_ = other.baseline();
    meanline_ = other.meanline();

    x_height_ = other.x_height();
    d_height_ = other.d_height();
    a_height_ = other.a_height();

    char_space_ = other.char_space();
    char_width_ = other.char_width();

    word_space_ = other.word_space();

    reading_direction_ = other.reading_direction();

    type_ = other.type();

    reverse_video_ = other.reverse_video();

    orientation_ = other.orientation();

    reading_orientation_ = other.reading_orientation();

    indented_ = other.indented();

    text_ = other.text();

    holder_ = other.holder();
  }

  template <typename L>
  inline
  line_info<L>::line_info(const line_info<L>& other)
    : id_(0), hidden_(false)
  {
    copy_data(other);
  }


  /*!

    Exemple:

    \verbatim

      0 1 2 3 4 5 6 7 8 9
     ---------------------
   0 | | | |x| | | | | | |   ----> a_height = 4
     ---------------------
   1 | | | |x| | | | | | |
     ---------------------
   2 | |x|x|x| | |x|x|x| |   ----> Meanline = 2    ^
     ---------------------                         |
   3 | |x| |x| | |x| |x| |                         | x_height = 3
     ---------------------                         |
   4 | |x|x|x| | |x|x|x| |   ----> Baseline = 4    v
     ---------------------
   5 | | | | | | |x| | | |
     ---------------------
   6 | | | | | | |x| | | |   ----> d_height = -2
     ---------------------

     \endverbatim

    The baseline, the meanline, the ascent and the descent are defined
    as an absolute row index.

    All other metrics, such as x_height, are computed relatively to
    the Baseline.

  */

  template <typename L>
  line_info<L>::line_info(const line_set<L>& holder,
			  const line_id_t& id,
			  const mln::util::array<component_id_t>& comps)
    : id_(id), hidden_(false), tag_(line::None), components_(comps),
      type_(line::Undefined), holder_(holder)
  {

    force_stats_update();

    // FIXME: set valid information for these attributes in
    // force_stats_update.
    word_space_ = 0;
    reading_direction_ = line::LeftToRight;
    reverse_video_ = false;

    orientation_ = 0.;
    reading_orientation_ = 0.;

    indented_ = false;
  }


  template <typename L>
  inline
  line_info<L>&
  line_info<L>::operator=(const line_info<L>& other)
  {
    copy_data(other);
    return *this;
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
  const mln::box2d&
  line_info<L>::ebbox() const
  {
    return ebbox_;
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
  int
  line_info<L>::baseline() const
  {
    return baseline_;
  }


  template <typename L>
  int
  line_info<L>::meanline() const
  {
    return meanline_;
  }


  template <typename L>
  int
  line_info<L>::ascent() const
  {
    return baseline_ - a_height() + 1;
  }


  template <typename L>
  int
  line_info<L>::descent() const
  {
    return baseline_ - d_height() + 1;
  }


  template <typename L>
  unsigned
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
  void
  line_info<L>::update_components_type(component::Type type)
  {
    for_all_elements(i, components_)
    {
      unsigned c = components_[i];
      holder_.components_()(c).update_type(type);
    }
  }


  template <typename L>
  void
  line_info<L>::update_type(line::Type type)
  {
    type_ = type;

    // Some line types may involve updating components type as well.
    if (type == line::Punctuation)
      update_components_type(component::Punctuation);
    else if (type == line::Text)
      update_components_type(component::Character);
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
  line_info<L>::has_text() const
  {
    return !text_.empty();
  }

  template <typename L>
  const std::string&
  line_info<L>::text() const
  {
    return text_;
  }


  template <typename L>
  void
  line_info<L>::update_text(const std::string& str)
  {
    text_ = str;
  }


  template <typename L>
  bool
  line_info<L>::is_valid() const
  {
    return id_ != 0u;
  }


  template <typename L>
  bool
  line_info<L>::is_hidden() const
  {
    return hidden_;
  }


  template <typename L>
  void
  line_info<L>::set_hidden(bool b)
  {
    hidden_ = b;
  }


  template <typename L>
  inline
  int
  line_info<L>::delta_of_line() const
  {
    return char_width() + 2 * char_space();
    // FIXME: choose between:
    //   not enough: char_width + char_space
    //   too much:   2 * char_width
    //   looks good: char_width + 2 * char_space
  }


  template <typename L>
  mln::box2d
  line_info<L>::enlarge(const mln::box2d& b, int delta) const
  {
    mln::box2d b_(mln::point2d(b.pmin().row(), b.pmin().col() - delta),
		  mln::point2d(b.pmax().row(), b.pmax().col() + delta));
    return b_;
  }


  template <typename L>
  void
  line_info<L>::update_ebbox()
  {
    int A = a_height_ - x_height_;
    int D = - d_height_;
    if (A <= 2 && D > 2)
      A = D;
    if (D <= 2 && A > 2)
      D = A;

    int delta = delta_of_line();

    ebbox_ = mln::make::box2d(meanline_ - A, bbox().pmin().col() - delta,
			      baseline_ + D, bbox().pmax().col() + delta);

    ebbox_.crop_wrt(holder_.components().labeled_image().domain());
  }


  template <typename L>
  mln::box2d
  line_info<L>::merged_ebbox(const scribo::line_info<L>& info_l,
			     const scribo::line_info<L>& info)
  {
    // line data
    int
      baseline_l = info_l.baseline(),
      d_height = info_l.d_height();
    unsigned
      a_height = info_l.a_height(),
      x_height = info_l.x_height();
    int A_l = a_height - x_height;
    int D_l = - d_height;
    if (A_l <= 2 && D_l > 2)
      A_l = D_l;
    if (D_l <= 2 && A_l > 2)
      D_l = A_l;
    unsigned delta_l = info_l.delta_of_line();
    int meanline_l = info_l.meanline();

    // non-line data
    unsigned delta_ = info.delta_of_line();

    mln::box2d b = mln::make::box2d(// pmin
      meanline_l - A_l,
      std::min(info_l.bbox().pmin().col(), info.bbox().pmin().col()) - std::max(delta_l, delta_),
      // pmax
      baseline_l + D_l,
      std::max(info_l.bbox().pmax().col(), info.bbox().pmax().col()) + std::max(delta_l, delta_));

    return b;
  }


  template <typename L>
  void
  line_info<L>::update_bbox_and_ebox(line_info<L>& other)
  {
    // Merging ebboxes depending on the type of the line.

    if (type() == line::Text) // /this/ IS a text line
    {
      if (other.type() == line::Text) // /other/ IS a text line.
      {
	// Adjusting ebboxes with the highest delta and merging ebboxes.
	int d_delta = other.delta_of_line() - this->delta_of_line();
	if (d_delta < 0) // other.delta_of_line() < this->delta_of_line()
	  ebbox_.merge(enlarge(other.ebbox(), - d_delta));
	else
	{
	  mln::box2d b = ebbox_;
	  ebbox_ = other.bbox();
	  ebbox_.merge(enlarge(b, d_delta));
	}

	ebbox_.crop_wrt(holder_.components().labeled_image().domain());
      }
      else // /other/ IS NOT a text line.
      {
	ebbox_.merge(other.ebbox());
	ebbox_.merge(merged_ebbox(*this, other));
      }
    }
    else // /this/ is NOT a text line
    {
      if (other.type() != line::Text)
      {
	std::cerr << "error in 'line_info::update_bbox_and_ebox':"
		  << "Merging two non text lines." << std::endl;
	std::abort();
      }

      update_type(line::Text);
      ebbox_.merge(other.ebbox());
      ebbox_.merge(merged_ebbox(other, *this));
    }

    // Merging bboxes.
    bbox_.merge(other.bbox());

    // Make sure the ebbox is included in the image domain.
    ebbox_.crop_wrt(holder_.components().labeled_image().domain());
  }


  template <typename L>
  void
  line_info<L>::fast_merge(line_info<L>& other, bool hide)
  {
    tag_ = line::Needs_Precise_Stats_Update;
    other.update_tag(line::Merged);
    other.set_hidden(hide);

    // Update bbox and ebbox
    update_bbox_and_ebox(other);

    components_.append(other.components());
  }


  template <typename L>
  void
  line_info<L>::precise_merge(line_info<L>& other, bool hide)
  {
    fast_merge(other, hide);
    force_stats_update();
  }

  template <typename L>
  void
  line_info<L>::force_stats_update()
  {
    typedef mln_site(L) P;
    const component_set<L>& comp_set = holder_.components();

    // FIXME: int_u<12> may not be enought but we can't use unsigned
    // or any other larger types since there is no median
    // implementation for high quantification types...

    // Init.
    typedef mln::value::int_u<12> median_data_t;
    typedef mln::accu::stat::median_h<median_data_t> median_t;
    median_t
      absolute_meanline,
      absolute_baseline,
      char_space,
      char_width;

    mln::accu::shape::bbox<P> bbox;

    for_all_elements(i, components_)
    {
      unsigned c = components_(i);

      const mln::box2d& bb = comp_set(c).bbox();

      // Bounding box.
      bbox.take(bb);

      // Ignore punctuation for stats computation but not for bbox
      // computation.
      if (holder_.components()(c).type() == component::Punctuation)
	continue;



      // Space between characters.
      int space = bb.pmin().col()
	- comp_set(holder_.links()(c)).bbox().pmax().col() + 1;

      // -- Ignore overlapped characters.
      if (space > 0)
	char_space.take(space);

      // Character width
      // -- Ignore too large components.
      if (bb.width() <= 1000)
	char_width.take(bb.width());

      // Meanline (compute an absolute value, from the top left
      // corner of the image).
      absolute_meanline.take(bb.pmin().row());

      // Baseline (compute an absolute value, from the top left
      // corner of the image).
      absolute_baseline.take(bb.pmax().row());
    }

    // Finalization
    {
      tag_ = line::None;
      bbox_ = bbox.to_result();

      // Char space
      if (char_space.card() < 2)
	char_space_ = 0;
      else
	char_space_ = char_space.to_result();

      // Char width
      if (card() == 2)
	char_width_ = (comp_set(components_[0]).bbox().width()
		       + comp_set(components_[1]).bbox().width()) / 2;
      else
	char_width_ = char_width.to_result();

      // FIXME: There is a bug here when the input document is too
      // large. The baselines indexes are too high for the type used
      // in the median accumulator!
      baseline_ = absolute_baseline.to_result();
      meanline_ = absolute_meanline.to_result();
      x_height_ = absolute_baseline - absolute_meanline + 1;
      d_height_ = absolute_baseline - bbox.to_result().pmax().row() + 1;
      a_height_ = absolute_baseline - bbox.to_result().pmin().row() + 1;

      //FIXME
      //
      //word_space_ = ...;
      //reading_direction_ = ...;
      //reverse_video_ = ...;
      //orientation_ = ...;
      //reading_orientation_ = ...;
      //indented_ = ...;

      update_ebbox();
    }

  }


  template <typename L>
  const line_set<L>&
  line_info<L>::holder() const
  {
    return holder_;
  }


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_info<L>& info)
  {
    return ostr << "line_info("
		<< "id=" << info.id()
		<< ", tag=" << info.tag()
		<< ", type=" << info.type()
		<< ", bbox=" << info.bbox()
		<< ", ebbox=" << info.ebbox()
		<< ", components=" << info.components()
		<< ", baseline=" << info.baseline()
		<< ", meanline=" << info.meanline()
		<< ", ascent=" << info.ascent()
		<< ", descent=" << info.descent()
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
		<< ", hidden=" << info.is_hidden()
		<< ", text=" << info.text()
		<< ")" << std::endl;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_LINE_INFO_HH
