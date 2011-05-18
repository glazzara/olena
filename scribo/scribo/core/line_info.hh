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

#ifndef SCRIBO_CORE_LINE_INFO_HH
# define SCRIBO_CORE_LINE_INFO_HH

/// \file
///
/// \brief Line information data structure.
///
/// \fixme The meanline should not be stored! The user can deduce it
/// from the x_height and the baseline.
///
/// \fixme The way the meanline and the baseline are computed is not
/// optimal and does not work if the components are too high (because
/// of the median accumulator and int_u12 overflows).

# include <map>

# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/accu/stat/median_h.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/math/min.hh>
# include <mln/util/object_id.hh>
# include <mln/value/int_u.hh>
# include <mln/math/sqr.hh>
# include <mln/value/rgb8.hh>

# include <scribo/core/tag/component.hh>
# include <scribo/core/tag/line.hh>

# include <scribo/core/object_groups.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/component_set.hh>

# include <scribo/core/internal/sort_comp_ids.hh>
# include <scribo/core/concept/serializable.hh>

// DEBUG

# include <scribo/core/stats.hh>

namespace scribo
{

  // Forward declarations.
  template <typename L> class line_set;
  template <typename L> class line_info;

  typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;


  namespace internal
  {

    /// Data structure for \c scribo::line_info<I>.
    template <typename L>
    struct line_info_data
    {
      line_info_data();
      line_info_data(const line_set<L>& holder,
		     const group_info& group);
      // Used for incremental construction (xml loading)
      line_info_data(const line_set<L>& holder,
		     const mln::util::array<component_id_t>& component_ids);

      bool hidden_;
      line::Tag tag_;
      mln::box2d bbox_;
      mln::box2d ebbox_;
      mln::util::array<component_id_t> component_ids_;

      // The number of pixels used for line characters.
      unsigned pixel_area_;

      // Values relative to the line bbox.
      int baseline_;
      int meanline_;

      // Values relative to the baseline.
      unsigned x_height_;
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

      float boldness_;
      float boldness_reliability_;

      mln::value::rgb8 color_;

      // sqrt(Max(VAR(R), VAR(G), VAR(B)))
      float color_reliability_;

      std::string text_;
      std::string html_text_;

      // Line set holding this element.
      line_set<L> holder_;

      // DEBUG
      stats< float > meanline_clusters_;
      stats< float > baseline_clusters_;

    private:
      void init_();
    };

  } // end of namespace scribo::internal



  template <typename L>
  class line_info : public Serializable<line_info<L> >
  {
    typedef Serializable<line_info<L> > parent_t;
    typedef internal::line_info_data<L> data_t;
    typedef mln::util::object_id<scribo::ComponentId, unsigned> component_id_t;
    typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;

  public:

    /// Constructors
    /// @{

    line_info();

    line_info(const line_id_t& id, data_t* data);

    line_info(const line_set<L>& holder,
	      const line_id_t& id,
	      const group_info& group);

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

    const mln::util::array<component_id_t>& component_ids() const;
    unsigned card() const;

    unsigned pixel_area() const;

    float boldness() const;
    float boldness_reliability() const;

    const mln::value::rgb8& color() const;
    float color_reliability() const;

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
    const std::string& html_text() const;
    void update_text(const std::string& str);

    bool is_valid() const;

    bool is_textline() const;

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

    bool chars_same_width() const;
    unsigned get_first_char_height() const;
    /// Force a new computation of statistics.
    void force_stats_update();


    /// Returns the line set holding this element.
    const line_set<L>& holder() const;

    /// Returns the delta used to compute the extended bbox.
    int delta_of_line() const;

    /// Update the extended bbox.
    void update_ebbox();

  private: // Members
    void copy_data(const line_info<L>& other);

    /// Enlarge the width of a given bbox \p b with a \p delta.
    mln::box2d enlarge(const mln::box2d& b, int delta) const;

    /// Update bbox and ebbox_ attributes.
    void update_bbox_and_ebox(line_info<L>& other);

    mln::box2d merged_ebbox(const scribo::line_info<L>& info_l,
			    const scribo::line_info<L>& info);

    void update_components_type(component::Type type);

    int compute_baseline();
    int compute_meanline();

  private: // Attributes
    line_id_t id_;
    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_info<L>& info);

  template <typename L>
  bool
  operator==(const line_info<L>& lhs, const line_info<L>& rhs);


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // INTERNAL TOOLS

    static inline std::map<char, std::string> init_map()
    {
      std::map<char, std::string> html_map;
      html_map['\"'] = "&quot;";
      html_map['<'] = "&lt;";
      html_map['>'] = "&gt;";
      html_map['&'] = "&amp;";
      return html_map;
    }


    inline
    std::string
    html_markups_replace(const std::string& input)
    {
      static std::map<char, std::string> map = init_map();

      std::string output = input;
      for (unsigned i = 0; i < output.size(); ++i)
      {
	std::map<char, std::string>::iterator it = map.find(output.at(i));
	if (it != map.end())
	{
	  output.replace(i, 1, it->second);
	  i += it->second.size() - 1;
	}
      }

      return output;
    }



    // LINE INFO DATA

    template <typename L>
    line_info_data<L>::line_info_data()
    {
      hidden_ = false;
    }

    template <typename L>
    line_info_data<L>::line_info_data(const line_set<L>& holder,
				      const group_info& group)
      : hidden_(false), tag_(line::None), component_ids_(group.component_ids()),
	type_(line::Undefined), holder_(holder)
    {
      init_();
    }

    template <typename L>
    line_info_data<L>::line_info_data(const line_set<L>& holder,
				      const mln::util::array<component_id_t>& component_ids)
      : hidden_(false), tag_(line::None), component_ids_(component_ids),
	type_(line::Undefined), holder_(holder)
    {
      init_();
    }

    template <typename L>
    void
    line_info_data<L>::init_()
    {
      // FIXME: set valid information for these attributes in
      // force_stats_update.
      word_space_ = 0;
      reading_direction_ = line::LeftToRight;
      reverse_video_ = false;

      orientation_ = 0.;
      reading_orientation_ = 0.;

      indented_ = false;
    }

  } // end of namespace scribo::internal


  template <typename L>
  line_info<L>::line_info()
    : parent_t(), id_(0)
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

    data_ = other.data_;
  }


  template <typename L>
  inline
  line_info<L>::line_info(const line_id_t& id, data_t *data)
    : id_(id), data_(data)
  {
  }


  template <typename L>
  inline
  line_info<L>::line_info(const line_info<L>& other)
    : parent_t(other), id_(0)
  {
    //data_->hidden_ = false;
    copy_data(other);
  }


  /*!

    Exemple:

    \verbatim

      0 1 2 3 4 5 6 7 8 9
     ---------------------
   0 | | | |x| | | | | | |   ----> a_height = 5
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

    The baseline and the meanline are defined as an absolute row
    index.

    All other metrics, such as a_height, are computed relatively to
    the Baseline.

  */

  template <typename L>
  line_info<L>::line_info(const line_set<L>& holder,
			  const line_id_t& id,
			  const group_info& group)
    : id_(id)
  {
    data_ = new data_t(holder, group);
    force_stats_update();
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
    return data_->tag_;
  }


  template <typename L>
  void
  line_info<L>::update_tag(line::Tag tag)
  {
    data_->tag_ = tag;
  }


  template <typename L>
  const mln::box2d&
  line_info<L>::bbox() const
  {
    return data_->bbox_;
  }

  template <typename L>
  const mln::box2d&
  line_info<L>::ebbox() const
  {
    return data_->ebbox_;
  }


  template <typename L>
  const mln::util::array<typename line_info<L>::component_id_t>&
  line_info<L>::component_ids() const
  {
    return data_->component_ids_;
  }


  template <typename L>
  unsigned
  line_info<L>::card() const
  {
    return data_->component_ids_.size();
  }


  template <typename L>
  unsigned
  line_info<L>::pixel_area() const
  {
    return data_->pixel_area_;
  }


  template <typename L>
  float
  line_info<L>::boldness() const
  {
    return data_->boldness_;
  }


  template <typename L>
  float
  line_info<L>::boldness_reliability() const
  {
    return data_->boldness_reliability_;
  }


  template <typename L>
  const mln::value::rgb8&
  line_info<L>::color() const
  {
    return data_->color_;
  }


  template <typename L>
  float
  line_info<L>::color_reliability() const
  {
    return data_->color_reliability_;
  }


  template <typename L>
  int
  line_info<L>::baseline() const
  {
    return data_->baseline_;
  }


  template <typename L>
  int
  line_info<L>::meanline() const
  {
    return data_->meanline_;
  }


  template <typename L>
  int
  line_info<L>::ascent() const
  {
    return data_->baseline_ - a_height() + 1;
  }


  template <typename L>
  int
  line_info<L>::descent() const
  {
    return data_->baseline_ - d_height();
  }


  template <typename L>
  unsigned
  line_info<L>::x_height() const
  {
    return data_->x_height_;
  }


  template <typename L>
  int
  line_info<L>::d_height() const
  {
    return data_->d_height_;
  }


  template <typename L>
  int
  line_info<L>::a_height() const
  {
    return data_->a_height_;
  }


  template <typename L>
  unsigned
  line_info<L>::char_space() const
  {
    return data_->char_space_;
  }


  template <typename L>
  unsigned
  line_info<L>::char_width() const
  {
    return data_->char_width_;
  }


  template <typename L>
  unsigned
  line_info<L>::word_space() const
  {
    return data_->word_space_;
  }


  template <typename L>
  line::ReadingDirection
  line_info<L>::reading_direction() const
  {
    return data_->reading_direction_;
  }

  template <typename L>
  line::Type
  line_info<L>::type() const
  {
    return data_->type_;
  }


  template <typename L>
  void
  line_info<L>::update_components_type(component::Type type)
  {
    for_all_elements(i, data_->component_ids_)
    {
      unsigned c = data_->component_ids_[i];
      data_->holder_.components_()(c).update_type(type);
    }
  }


  template <typename L>
  void
  line_info<L>::update_type(line::Type type)
  {
    data_->type_ = type;

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
    return data_->reverse_video_;
  }


  template <typename L>
  float
  line_info<L>::orientation() const
  {
    return data_->orientation_;
  }


  template <typename L>
  float
  line_info<L>::reading_orientation() const
  {
    return data_->reading_orientation_;
  }


  template <typename L>
  bool
  line_info<L>::indented() const
  {
    return data_->indented_;
  }


  template <typename L>
  bool
  line_info<L>::has_text() const
  {
    return !data_->text_.empty();
  }


  template <typename L>
  const std::string&
  line_info<L>::text() const
  {
    return data_->text_;
  }


  template <typename L>
  const std::string&
  line_info<L>::html_text() const
  {
    return data_->html_text_;
  }


  template <typename L>
  void
  line_info<L>::update_text(const std::string& str)
  {
    data_->text_ = str;
    data_->html_text_ = scribo::internal::html_markups_replace(str);
  }


  template <typename L>
  bool
  line_info<L>::is_valid() const
  {
    return id_ != 0u;
  }


  template <typename L>
  bool
  line_info<L>::is_textline() const
  {
    return is_valid()
      && !is_hidden()
      && type() == line::Text;
  }


  template <typename L>
  bool
  line_info<L>::is_hidden() const
  {
    return data_->hidden_;
  }


  template <typename L>
  void
  line_info<L>::set_hidden(bool b)
  {
    data_->hidden_ = b;
  }


  template <typename L>
  inline
  int
  line_info<L>::delta_of_line() const
  {
    return 2 * char_width() + 2 * char_space();
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
    int A = data_->a_height_ - data_->x_height_;
    int D = - data_->d_height_;
    if (A <= 2 && D > 2)
      A = D;
    if (D <= 2 && A > 2)
      D = A;

    int delta = delta_of_line();

    data_->ebbox_ = mln::make::box2d(data_->meanline_ - A,
				     bbox().pmin().col() - delta,
				     data_->baseline_ + D,
				     bbox().pmax().col() + delta);

    data_->ebbox_.crop_wrt(data_->holder_.components().labeled_image().domain());
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
	  data_->ebbox_.merge(enlarge(other.ebbox(), - d_delta));
	else
	{
	  mln::box2d b = data_->ebbox_;
	  data_->ebbox_ = other.bbox();
	  data_->ebbox_.merge(enlarge(b, d_delta));
	}

	data_->ebbox_.crop_wrt(data_->holder_.components().labeled_image().domain());
      }
      else // /other/ IS NOT a text line.
      {
	data_->ebbox_.merge(other.ebbox());
	data_->ebbox_.merge(merged_ebbox(*this, other));
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
      data_->ebbox_.merge(other.ebbox());
      data_->ebbox_.merge(merged_ebbox(other, *this));
    }

    // Merging bboxes.
    data_->bbox_.merge(other.bbox());

    // Make sure the ebbox is included in the image domain.
    data_->ebbox_.crop_wrt(data_->holder_.components().labeled_image().domain());
  }


  template <typename L>
  void
  line_info<L>::fast_merge(line_info<L>& other, bool hide)
  {
    data_->tag_ = line::Needs_Precise_Stats_Update;
    other.update_tag(line::Merged);
    other.set_hidden(hide);

    // Update bbox and ebbox
    update_bbox_and_ebox(other);

    data_->component_ids_.append(other.component_ids());
  }


  template <typename L>
  void
  line_info<L>::precise_merge(line_info<L>& other, bool hide)
  {
    fast_merge(other, hide);
    force_stats_update();
  }

  template <typename L>
  bool
  line_info<L>::chars_same_width() const
  {
    // Only for the case of two-character words
    if (card() == 2)
    {
      const component_set<L>& comp_set = data_->holder_.components();

      const unsigned c1 = data_->component_ids_(0);
      const unsigned c2 = data_->component_ids_(1);

      if (data_->holder_.components()(c1).type() == component::Punctuation
	  || data_->holder_.components()(c2).type() == component::Punctuation)
	return false;

      const mln::box2d& bb1 = comp_set(c1).bbox();
      const mln::box2d& bb2 = comp_set(c2).bbox();

      const float w1 = bb1.width();
      const float h1 = bb1.height();
      const float w2 = bb2.width();
      const float h2 = bb2.height();

      const float space = std::max(bb1.pmin().col(), bb2.pmin().col()) -
      	std::min(bb1.pmax().col(), bb2.pmax().col());

      const int dy = bb1.pmax().row() - bb2.pmax().row();

      // The two characters must be distinct
      // if (space < 0)
      // 	return false;

      if (// Approximately the same width
	((std::max(w1, w2) / std::min(w1, w2)) > 1.3 ||
	 // One character must not be smaller than the space between
	 // the two characters
	   (w1 < space || w2 < space))
	// If the two characters have a different width they must also
	// have a different height
	  && not (std::max(h1, h2) / std::min(h1, h2) <= 1.7f))
	return false;

      // Approximately aligned on baseline
      if (std::abs(dy) > 10 &&
	  not (std::max(h1, h2) / std::min(h1, h2) <= 1.7f))
	return false;

      return true;
    }

    return false;
  }

  template< typename L >
  unsigned
  line_info<L>::get_first_char_height() const
  {
    const component_set<L>& comp_set = data_->holder_.components();
    const unsigned c1 = data_->components_(0);
    const mln::box2d& bb1 = comp_set(c1).bbox();

    return bb1.height();
  }

  template <typename L>
  int
  line_info<L>::compute_baseline()
  {
    const unsigned nelements = data_->baseline_clusters_.nelements();

    if (nelements == 2)
      return data_->baseline_clusters_.mean();

    mln::util::array< cluster_stats< float > >& clusters_b = data_->baseline_clusters_.clusters();

    unsigned index = 0;
    float min_base = 0.0f;
    const unsigned clusters_b_nelements = clusters_b.nelements();

    if (clusters_b_nelements >= 3)
      return data_->baseline_clusters_.mean();


    for (unsigned i = 0; i < clusters_b_nelements; ++i)
    {
      const unsigned clusters_b_i_nelements = clusters_b[i].nelements();

      if (clusters_b_i_nelements >= min_base * 2.0f)
      {
	min_base = clusters_b_i_nelements;
	index = i;
      }
      else if (clusters_b_i_nelements >= 0.5f * min_base)
      {
	if (clusters_b_i_nelements > 1 &&
	    clusters_b[index].median() > clusters_b[i].median())
	{
	  if (clusters_b_i_nelements > min_base)
	    min_base = clusters_b_i_nelements;
	  index = i;
	}
      }
    }

    if (clusters_b[index].nelements() <= 2 && nelements <= 5)
      return data_->baseline_clusters_.mean();

    return clusters_b[index].median();
  }

  template <typename L>
  int
  line_info<L>::compute_meanline()
  {
    mln::util::array< cluster_stats< float > >& clusters_m = data_->meanline_clusters_.clusters();

    unsigned index = 0;
    float max_mean = 0.0f;
    const unsigned clusters_m_nelements = clusters_m.nelements();

    if (clusters_m_nelements >= 3)
      return data_->meanline_clusters_.mean();

    for (unsigned i = 0; i < clusters_m_nelements; ++i)
    {
      const unsigned clusters_m_i_nelements = clusters_m[i].nelements();

      if (clusters_m_i_nelements >= max_mean * 2.0f)
      {
	max_mean = clusters_m_i_nelements;
	index = i;
      }
      else if (clusters_m_i_nelements >= 0.5f * max_mean)
      {
	if (clusters_m[index].median() < clusters_m[i].median())
	{
	  if (clusters_m_i_nelements > max_mean)
	    max_mean = clusters_m_i_nelements;
	  index = i;
	}
      }
    }

    return clusters_m[index].median();
  }

  template <typename L>
  void
  line_info<L>::force_stats_update()
  {
    typedef mln_site(L) P;
    const component_set<L>& comp_set = data_->holder_.components();

    // Init.
    typedef mln::value::int_u<12> median_data_t;
    typedef mln::accu::stat::median_h<median_data_t> median_t;
    median_t
      // meanline,
      // baseline,
      char_space,
      char_width;

    // FIXME: we should use the median for computing the line
    // color. Means should be kept for variance computation.
    mln::accu::stat::mean<mln::value::int_u<8> >
      color_red,
      color_green,
      color_blue,
      boldness;

    float
      sum2_red = 0,
      sum2_green = 0,
      sum2_blue = 0,
      sum2_boldness = 0;

    unsigned pixel_area = 0;

    mln::accu::shape::bbox<P> bbox;

    mln::def::coord ref_line = mln_max(mln::def::coord);

    // DEBUG
    data_->baseline_clusters_.reset();
    data_->meanline_clusters_.reset();

    // Find a reference line to compute baselines and other attributes.
    // Workaround to avoid overflow with int_u<12> in median accumulators.
    //
    // FIXME: not optimal...
    for_all_elements(i, data_->component_ids_)
    {
      unsigned c = data_->component_ids_(i);

      // Ignore punctuation for stats computation but not for bbox
      // computation.
      if (comp_set(c).type() == component::Punctuation)
	continue;

      ref_line = mln::math::min(comp_set(c).bbox().pmin().row(), ref_line);
    }


    unsigned used_comps = 0;
    for_all_elements(i, data_->component_ids_)
    {
      unsigned c = data_->component_ids_(i);

      pixel_area += comp_set(c).card();

      const mln::box2d& bb = comp_set(c).bbox();

      // Bounding box.
      bbox.take(bb);

      // Ignore punctuation for stats computation but not for bbox
      // computation.
      if (comp_set(c).type() == component::Punctuation)
	continue;

      // This component will be used for stats, the counter is
      // incremented.
      ++used_comps;


      // COMPUTE FEATURES DATA
      if (comp_set(c).has_features())
      {
	// Compute boldness
	boldness.take(comp_set(c).features().boldness);
	sum2_boldness += mln::math::sqr<float>(comp_set(c).features().boldness);

	// Compute color
	color_red.take(comp_set(c).features().color.red());
	color_green.take(comp_set(c).features().color.green());
	color_blue.take(comp_set(c).features().color.blue());

	sum2_red += mln::math::sqr<unsigned>(comp_set(c).features().color.red());
	sum2_green += mln::math::sqr<unsigned>(comp_set(c).features().color.green());
	sum2_blue += mln::math::sqr<unsigned>(comp_set(c).features().color.blue());
      }

      // FIXME: we must guaranty here that the relationship is from
      // right to left, otherwise, the space size computed between
      // boxes might be wrong...
      //
      // x------|  |-|     !=     |------|  x-|
      // |------|  x-|            |------x  |-|
      //
      //  (incorrect)              (correct)
      //  (right link)            (left link)

      // Space between characters.
      if (data_->holder_.links()(c) != c)
      {
	int
	  space = bb.pmin().col()
	  - comp_set(data_->holder_.links()(c)).bbox().pmax().col() - 1;

	// -- Ignore overlapped characters.
	if (space > 0)
	  char_space.take(space);
      }

      // Character width
      // -- Ignore too large components.
      //
      // FIXME: should not be a constant?
      if (bb.width() <= 1000)
	char_width.take(bb.width());

      // Meanline (compute an absolute value, from the top left corner
      // of the highest character bounding box, excluding
      // punctuation).
      // meanline.take(bb.pmin().row() - ref_line);
      data_->meanline_clusters_.take(bb.pmin().row());

      // Baseline (compute an absolute value, from the top left corner
      // of the highest character bounding box, excluding
      // punctuation).
      // baseline.take(bb.pmax().row() - ref_line);
      data_->baseline_clusters_.take(bb.pmax().row());
    }

    // Finalization
    {
      // Tag
      data_->tag_ = line::None;

      // Bbox
      data_->bbox_ = bbox.to_result();

      // Pixel area
      data_->pixel_area_ = pixel_area;

      // Order component ids according to component localization (left
      // to right).
      std::sort(data_->component_ids_.hook_std_vector_().begin(),
		data_->component_ids_.hook_std_vector_().end(),
		internal::sort_comp_ids<L>(comp_set));

      // Boldness
      data_->boldness_ = boldness.to_result();
      data_->boldness_reliability_ = std::sqrt(sum2_boldness);

      // Color
      data_->color_ = mln::value::rgb8(color_red, color_green, color_blue);

      // FIXME: we may prefer using the non-biased variance.
      float
	var_red = sum2_red / (float)(used_comps)
	- mln::math::sqr<float>(color_red.to_result()),
	var_green = sum2_green / (float)(used_comps)
	- mln::math::sqr<float>(color_green.to_result()),
	var_blue = sum2_blue / (float)(used_comps)
	- mln::math::sqr<float>(color_blue.to_result());

      data_->color_reliability_ = std::sqrt(std::max(var_red,
						     std::max(var_green, var_blue)));

      // Char space
      if (char_space.card() < 2)
	data_->char_space_ = 0;
      else
	data_->char_space_ = char_space.to_result();

      // Char width
      if (card() == 2)
	data_->char_width_ = (comp_set(data_->component_ids_[0]).bbox().width()
			      + comp_set(data_->component_ids_[1]).bbox().width()) / 2;
      else
	data_->char_width_ = char_width.to_result();

      // // mln::def::coord
      // // 	absolute_baseline_r = baseline.to_result() + ref_line,
      // // 	absolute_meanline_r = meanline.to_result() + ref_line;

      // data_->baseline_ = absolute_baseline_r;
      // data_->meanline_ = absolute_meanline_r;
      data_->baseline_ = compute_baseline();
      data_->meanline_ = compute_meanline();
      data_->x_height_ = data_->baseline_ - data_->meanline_ + 1;
      data_->d_height_ = data_->baseline_ - bbox.to_result().pmax().row();
      data_->a_height_ = data_->baseline_ - bbox.to_result().pmin().row() + 1;

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
    return data_->holder_;
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
		<< ", boldness=" << info.boldness()
		<< ", boldness_reliability=" << info.boldness_reliability()
		<< ", color=" << info.color()
		<< ", color_reliability=" << info.color_reliability()
		<< ", components=" << info.component_ids()
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
		<< ", html_text=" << info.html_text()
		<< ")" << std::endl;
  }


  template <typename L>
  bool
  operator==(const line_info<L>& lhs, const line_info<L>& rhs)
  {
    if (! lhs.is_valid() && ! rhs.is_valid())
      return true;

    return
      lhs.is_valid() == rhs.is_valid()
      && lhs.id() == rhs.id()
      && lhs.pixel_area() == rhs.pixel_area()
      && lhs.tag() == rhs.tag()
      && lhs.type() == rhs.type()
      && lhs.bbox() == rhs.bbox()
      && lhs.ebbox() == rhs.ebbox()
      && lhs.boldness() == rhs.boldness()
      && lhs.boldness_reliability() == rhs.boldness_reliability()
      && lhs.color() == rhs.color()
      && lhs.color_reliability() == rhs.color_reliability()
      && lhs.component_ids() == rhs.component_ids()
      && lhs.baseline() == rhs.baseline()
      && lhs.meanline() == rhs.meanline()
      && lhs.ascent() == rhs.ascent()
      && lhs.descent() == rhs.descent()
      && lhs.x_height() == rhs.x_height()
      && lhs.d_height() == rhs.d_height()
      && lhs.a_height() == rhs.a_height()
      && lhs.char_space() == rhs.char_space()
      && lhs.char_width() == rhs.char_width()
      && lhs.word_space() == rhs.word_space()
      && lhs.reading_orientation() == rhs.reading_orientation()
      && lhs.type() == rhs.type()
      && lhs.reverse_video() == rhs.reverse_video()
      && lhs.orientation() == rhs.orientation()
      && lhs.reading_orientation() == rhs.reading_orientation()
      && lhs.indented() == rhs.indented()
      && lhs.is_hidden() == rhs.is_hidden()
      && lhs.text() == rhs.text()
      && lhs.html_text() == rhs.html_text();
  }

# endif// ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_LINE_INFO_HH
