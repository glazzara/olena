// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_CORE_DOCUMENT_HH
# define SCRIBO_CORE_DOCUMENT_HH

/// \file
///
/// \brief Describes document content.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/magick/load.hh>

# include <scribo/core/component_set.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/paragraph_set.hh>

# include <scribo/core/concept/serializable.hh>

# include <scribo/primitive/extract/components.hh>

namespace scribo
{

  template <typename L>
  struct document : public Serializable<document<L> >
  {
  public:

    document();
    document(const char *filename);
    document(const char *filename,
	     const mln::image2d<mln::value::rgb8>& input);

    void open();
    bool is_open() const;

    const char * filename() const;
    void set_filename(const char*name);

    bool is_valid() const;

    /*! \brief Check whether this document contains text.

      If it returns true, that document contains paragraphs, lines and
      text components.

     */
    bool has_text() const;

    mln::def::coord height() const;
    mln::def::coord width() const;

    const line_set<L>& lines() const;

    const paragraph_set<L>& paragraphs() const;
    void set_paragraphs(const paragraph_set<L>& parset);

    bool has_elements() const;
    const component_set<L>& elements() const;
    void set_elements(const component_set<L>& elements);

    bool has_whitespace_seps() const;
    const mln::image2d<bool>& whitespace_seps() const;
    const component_set<L>& whitespace_seps_comps() const;
    void set_whitespace_separators(const image2d<bool>& whitespace_seps);

    // Horizontal separators
    bool has_hline_seps() const;
    const mln::image2d<bool>& hline_seps() const;
    const component_set<L>& hline_seps_comps() const;
    void set_hline_separators(const image2d<bool>& line_seps);

    // Vertical separators
    bool has_vline_seps() const;
    const mln::image2d<bool>& vline_seps() const;
    const component_set<L>& vline_seps_comps() const;
    void set_vline_separators(const image2d<bool>& line_seps);

    const mln::image2d<value::rgb8>& image() const;
    void set_image(const mln::image2d<value::rgb8>& image);

    const mln::image2d<bool>& binary_image() const;
    void set_binary_image(const mln::image2d<bool>& binary_image);


  private:
    std::string filename_;
    mln::image2d<mln::value::rgb8> image_;
    mln::image2d<bool> binary_image_;

    paragraph_set<L> parset_;
    component_set<L> elements_;

    mln::image2d<bool> whitespace_seps_;
    component_set<L> whitespace_seps_comps_;

    mln::image2d<bool> hline_seps_;
    component_set<L> hline_seps_comps_;

    mln::image2d<bool> vline_seps_;
    component_set<L> vline_seps_comps_;
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
  }


  template <typename L>
  document<L>::document(const char *filename,
			const mln::image2d<mln::value::rgb8>& input)
    : filename_(filename),
      image_(input)
  {
  }


  template <typename L>
  const char *
  document<L>::filename() const
  {
    return filename_.c_str();
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
    Magick::InitializeMagick(0);
    mln::io::magick::load(image_, filename_);
  }


  template <typename L>
  bool
  document<L>::is_open() const
  {
    return image_.is_valid();
  }


  template <typename L>
  bool
  document<L>::is_valid() const
  {
    return image_.is_valid();
  }


  template <typename L>
  mln::def::coord
  document<L>::width() const
  {
    return image_.ncols();
  }


  template <typename L>
  mln::def::coord
  document<L>::height() const
  {
    return image_.nrows();
  }


  template <typename L>
  bool
  document<L>::has_text() const
  {
    return parset_.is_valid();
  }


  template <typename L>
  const line_set<L>&
  document<L>::lines() const
  {
    return parset_.lines();
  }

  template <typename L>
  const paragraph_set<L>&
  document<L>::paragraphs() const
  {
    return parset_;
  }


  template <typename L>
  void
  document<L>::set_paragraphs(const paragraph_set<L>& parset)
  {
    parset_ = parset;
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
  bool
  document<L>::has_whitespace_seps() const
  {
    return whitespace_seps_.is_valid();
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::whitespace_seps() const
  {
    return whitespace_seps_;
  }


  template <typename L>
  const component_set<L>&
  document<L>::whitespace_seps_comps() const
  {
    return whitespace_seps_comps_;
  }


  template <typename L>
  void
  document<L>::set_whitespace_separators(const image2d<bool>& whitespace_seps)
  {
    whitespace_seps_ = whitespace_seps;

    mln_value(L) ncomps;
    whitespace_seps_comps_ = primitive::extract::components(whitespace_seps,
							    mln::c8(), ncomps,
							    component::WhitespaceSeparator);
  }


  template <typename L>
  bool
  document<L>::has_hline_seps() const
  {
    return hline_seps_.is_valid();
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::hline_seps() const
  {
    return hline_seps_;
  }


  template <typename L>
  const component_set<L>&
  document<L>::hline_seps_comps() const
  {
    return hline_seps_comps_;
  }


  template <typename L>
  void
  document<L>::set_hline_separators(const image2d<bool>& hline_seps)
  {
    hline_seps_ = hline_seps;

    mln_value(L) ncomps;
    hline_seps_comps_ = primitive::extract::components(hline_seps,
						       mln::c8(), ncomps,
						       component::LineSeparator);
  }


  template <typename L>
  bool
  document<L>::has_vline_seps() const
  {
    return vline_seps_.is_valid();
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::vline_seps() const
  {
    return vline_seps_;
  }


  template <typename L>
  const component_set<L>&
  document<L>::vline_seps_comps() const
  {
    return vline_seps_comps_;
  }


  template <typename L>
  void
  document<L>::set_vline_separators(const image2d<bool>& vline_seps)
  {
    vline_seps_ = vline_seps;

    mln_value(L) ncomps;
    vline_seps_comps_ = primitive::extract::components(vline_seps,
						       mln::c8(), ncomps,
						       component::LineSeparator);
  }


  template <typename L>
  const mln::image2d<value::rgb8>&
  document<L>::image() const
  {
    return image_;
  }


  template <typename L>
  void
  document<L>::set_image(const mln::image2d<value::rgb8>& image)
  {
    image_ = image;
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::binary_image() const
  {
    return binary_image_;
  }


  template <typename L>
  void
  document<L>::set_binary_image(const mln::image2d<bool>& binary_image)
  {
    binary_image_ = binary_image;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_DOCUMENT_HH
