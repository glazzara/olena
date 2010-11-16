// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_DOCUMENT_HH
# define SCRIBO_CORE_DOCUMENT_HH

/// \file
///
/// \brief Describes document content.

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/magick/load.hh>

# include <scribo/core/component_set.hh>
# include <scribo/core/line_set.hh>

namespace scribo
{

  template <typename L>
  struct document
  {
  public:

    document();
    document(const char *filename);

    void open();

    const char * filename() const;
    void set_filename(const char*name);

    bool is_valid() const;

    const line_set<L>& text() const;
    bool has_text() const;
    void set_text(const line_set<L>& line);

    const component_set<L>& elements() const;
    bool has_elements() const;
    void set_elements(const component_set<L>& elements);

    const mln::image2d<value::rgb8>& image() const;

  private:
    const char *filename_;
    mln::image2d<mln::value::rgb8> image_;

    line_set<L> lines_;
    component_set<L> elements_;
  };



# ifndef MLN_INCLUDE_ONLY


  template <typename L>
  document<L>::document()
  {
  }


  template <typename L>
  document<L>::document(const char *filename)
    : filename_(filename)
  {
    open();
  }


  template <typename L>
  const char *
  document<L>::filename() const
  {
    return filename_;
  }


  template <typename L>
  void
  document<L>::set_filename(const char *filename)
  {
    filename_ = filename;
  }


  template <typename L>
  void
  document<L>::open()
  {
    mln::io::magick::load(image_, filename_);
  }


  template <typename L>
  bool
  document<L>::is_valid() const
  {
    return image_.is_valid();
  }


  template <typename L>
  const line_set<L>&
  document<L>::text() const
  {
    return lines_;
  }


  template <typename L>
  bool
  document<L>::has_text() const
  {
    return lines_.is_valid();
  }


  template <typename L>
  void
  document<L>::set_text(const line_set<L>& line)
  {
    lines_ = line;
  }


  template <typename L>
  const component_set<L>&
  document<L>::elements() const
  {
    return elements_;
  }


  template <typename L>
  bool
  document<L>::has_elements() const
  {
    return elements_.is_valid();
  }


  template <typename L>
  void
  document<L>::set_elements(const component_set<L>& elements)
  {
    elements_ = elements;
  }


  template <typename L>
  const mln::image2d<value::rgb8>&
  document<L>::image() const
  {
    return image_;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_DOCUMENT_HH
