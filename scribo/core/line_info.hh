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
# include <mln/util/object_id.hh>

# include <scribo/core/tag/component.hh>
# include <scribo/core/tag/line.hh>

namespace scribo
{

  typedef mln::util::object_id<scribo::LineId, unsigned> line_id_t;

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

    void merge(const line_info& line) const;

    bool is_valid() const;

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
  };


  std::ostream&
  operator<<(std::ostream& ostr, const line_info& info);


# ifndef MLN_INCLUDE_ONLY


  line_info::line_info()
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

    The baseline is computed relatively to the top left corner of the
    line bounding box.

  */
  line_info::line_info(const line_id_t& id,
		       const mln::box2d& bbox,
		       const mln::util::array<component_id_t>& comps,
		       unsigned absolute_median,
		       unsigned absolute_baseline,
		       unsigned char_space,
		       unsigned char_width)
    : id_(id), tag_(line::None), bbox_(bbox), components_(comps),
      char_space_(char_space), char_width_(char_width)
  {
    baseline_ = absolute_baseline - bbox.pmin().row();
    median_   = absolute_median - bbox.pmin().row();
    x_height_ = absolute_baseline - absolute_median + 1;
    d_height_ = bbox.pmax().row() - absolute_baseline;
    a_height_ = absolute_baseline - bbox.pmin().row() + 1;
  }


  line_info::line_id_t
  line_info::id() const
  {
    return id_;
  }

  line::Tag
  line_info::tag() const
  {
    return tag_;
  }


  void
  line_info::update_tag(line::Tag tag)
  {
    tag_ = tag;
  }

  const mln::box2d&
  line_info::bbox() const
  {
    return bbox_;
  }


  const mln::util::array<line_info::component_id_t>&
  line_info::components() const
  {
    return components_;
  }

  unsigned
  line_info::card() const
  {
    return components_.size();
  }


  unsigned
  line_info::baseline() const
  {
    return baseline_;
  }


  unsigned
  line_info::median() const
  {
    return median_;
  }

  int
  line_info::x_height() const
  {
    return x_height_;
  }


  int
  line_info::d_height() const
  {
    return d_height_;
  }


  int
  line_info::a_height() const
  {
    return a_height_;
  }


  unsigned
  line_info::char_space() const
  {
    return char_space_;
  }


  unsigned
  line_info::char_width() const
  {
    return char_width_;
  }


  unsigned
  line_info::word_space() const
  {
    return word_space_;
  }


  void
  line_info::merge(const line_info& line) const
  {
    (void) line;
    std::cout << "merge not implemented!" << std::endl;
    abort();
  }


  bool
  line_info::is_valid() const
  {
    return id_ != 0u;
  }



  std::ostream&
  operator<<(std::ostream& ostr, const line_info& info)
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
		<< ")" << std::endl;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_LINE_INFO_HH
