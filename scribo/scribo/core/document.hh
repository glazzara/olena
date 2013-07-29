// Copyright (C) 2010, 2011, 2013 EPITA Research and Development
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

#ifndef SCRIBO_CORE_DOCUMENT_HH
# define SCRIBO_CORE_DOCUMENT_HH

/// \file
///
/// \brief Describes document content.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/rgb8.hh>

# ifndef SCRIBO_NMAGICK
#  include <mln/io/magick/load.hh>
# endif // ! SCRIBO_NMAGICK

# include <scribo/core/component_set.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/paragraph_set.hh>

# include <scribo/core/concept/serializable.hh>

# include <scribo/primitive/extract/components.hh>

namespace scribo
{

  namespace internal
  {
    /// Data structure for \c scribo::document<L>.
    template <typename L>
    struct document_data
    {
      document_data();
      document_data(const char *filename);
      document_data(const char *filename,
		    const mln::image2d<mln::value::rgb8>& input);

      std::string filename_;
      mln::image2d<mln::value::rgb8> image_;
      mln::image2d<bool> binary_image_;
      mln::image2d<bool> binary_image_wo_seps_;

      paragraph_set<L> parset_;
      component_set<L> elements_;

      mln::image2d<bool> whitespace_seps_;
      component_set<L> whitespace_seps_comps_;

      mln::image2d<bool> hline_seps_;
      component_set<L> hline_seps_comps_;

      mln::image2d<bool> vline_seps_;
      component_set<L> vline_seps_comps_;
    };

  } // end of namespace scribo::internal


  /// \brief Represent document data and structure.
  ///
  /// \ingroup grpstruct
  //
  template <typename L>
  class document : public Serializable<document<L> >
  {
    typedef internal::document_data<L> data_t;

  public:

    document();
    document(const char *filename);
    document(const char *filename,
	     const mln::image2d<mln::value::rgb8>& input);

    /* FIXME: This is bad, as it changes the interface of the class.
       But this is probably the smallest change to lift the dependency
       of this class on Magick++.  We shall probably get rid of this
       open() method anyway.  */
# ifndef SCRIBO_NMAGICK
    void open();
# endif // ! SCRIBO_NMAGICK
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
    void set_whitespace_separators(const image2d<bool>& whitespace_seps,
				   const component_set<L>& whitespace_seps_comps);

    // Horizontal separators
    bool has_hline_seps() const;
    const mln::image2d<bool>& hline_seps() const;
    const component_set<L>& hline_seps_comps() const;
    // Set vline separators image. The component is automatically computed.
    void set_hline_separators(const image2d<bool>& line_seps);
    void set_hline_separators(const image2d<bool>& line_seps,
			      const component_set<L>& hline_seps_comps);

    // Vertical separators
    bool has_vline_seps() const;
    const mln::image2d<bool>& vline_seps() const;
    const component_set<L>& vline_seps_comps() const;

    // Set vline separators image. The component is automatically computed.
    void set_vline_separators(const image2d<bool>& vline_seps);
    void set_vline_separators(const image2d<bool>& vline_seps,
			      const component_set<L>& vline_seps_comps);

    const mln::image2d<value::rgb8>& image() const;
    void set_image(const mln::image2d<value::rgb8>& image);

    const mln::image2d<bool>& binary_image() const;
    void set_binary_image(const mln::image2d<bool>& binary_image);

    const mln::image2d<bool>& binary_image_wo_seps() const;
    void set_binary_image_wo_seps(const mln::image2d<bool>& binary_image_wo_seps);

  private:
    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  bool operator==(const document<L>& lhs, const document<L>& rhs);


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename L>
    document_data<L>::document_data()
    {
    }

    template <typename L>
    document_data<L>::document_data(const char *filename,
				    const mln::image2d<mln::value::rgb8>& input)
      : filename_(filename),
	image_(input)
    {
    }

    template <typename L>
    document_data<L>::document_data(const char *filename)
      : filename_(filename)
    {
    }


  } // end of namespace scribo::internal



  template <typename L>
  document<L>::document()
    : data_(0)
  {
  }


  template <typename L>
  document<L>::document(const char *filename)
  {
    data_ = new internal::document_data<L>(filename);
  }


  template <typename L>
  document<L>::document(const char *filename,
			const mln::image2d<mln::value::rgb8>& input)
  {
    data_ = new internal::document_data<L>(filename, input);
  }


  template <typename L>
  const char *
  document<L>::filename() const
  {
    mln_precondition(data_ != 0);
    return data_->filename_.c_str();
  }


  template <typename L>
  void
  document<L>::set_filename(const char *filename)
  {
    mln_precondition(is_valid());
    data_->filename_ = filename;
  }


# ifndef SCRIBO_NMAGICK
  template <typename L>
  void
  document<L>::open()
  {
    mln_precondition(data_ != 0);
    mln::io::magick::load(data_->image_, data_->filename_);
  }
# endif // ! SCRIBO_NMAGICK


  template <typename L>
  bool
  document<L>::is_open() const
  {
    mln_precondition(data_ != 0);
    return data_->image_.is_valid();
  }


  template <typename L>
  bool
  document<L>::is_valid() const
  {
    return data_ != 0 && data_->image_.is_valid();
  }


  template <typename L>
  mln::def::coord
  document<L>::width() const
  {
    mln_precondition(is_valid());
    mln_precondition(data_->image_.is_valid());
    return data_->image_.ncols();
  }


  template <typename L>
  mln::def::coord
  document<L>::height() const
  {
    mln_precondition(is_valid());
    mln_precondition(data_->image_.is_valid());
    return data_->image_.nrows();
  }


  template <typename L>
  bool
  document<L>::has_text() const
  {
    mln_precondition(is_valid());
    return data_->parset_.is_valid();
  }


  template <typename L>
  const line_set<L>&
  document<L>::lines() const
  {
    mln_precondition(is_valid());
    mln_precondition(data_->parset_.is_valid());
    return data_->parset_.lines();
  }

  template <typename L>
  const paragraph_set<L>&
  document<L>::paragraphs() const
  {
    mln_precondition(is_valid());
    return data_->parset_;
  }


  template <typename L>
  void
  document<L>::set_paragraphs(const paragraph_set<L>& parset)
  {
    mln_precondition(is_valid());
    mln_precondition(parset.is_valid());
    data_->parset_ = parset;
  }


  template <typename L>
  const component_set<L>&
  document<L>::elements() const
  {
    mln_precondition(is_valid());
    return data_->elements_;
  }


  template <typename L>
  bool
  document<L>::has_elements() const
  {
    mln_precondition(is_valid());
    return data_->elements_.is_valid();
  }


  template <typename L>
  void
  document<L>::set_elements(const component_set<L>& elements)
  {
    mln_precondition(is_valid());
    mln_precondition(elements.is_valid());
    data_->elements_ = elements;
  }

  template <typename L>
  bool
  document<L>::has_whitespace_seps() const
  {
    mln_precondition(is_valid());
    return data_->whitespace_seps_.is_valid();
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::whitespace_seps() const
  {
    mln_precondition(is_valid());
    return data_->whitespace_seps_;
  }


  template <typename L>
  const component_set<L>&
  document<L>::whitespace_seps_comps() const
  {
    mln_precondition(is_valid());
    return data_->whitespace_seps_comps_;
  }


  template <typename L>
  void
  document<L>::set_whitespace_separators(const image2d<bool>& whitespace_seps,
					 const component_set<L>& whitespace_seps_comps)
  {
    mln_precondition(is_valid());
    mln_precondition(whitespace_seps.is_valid());
    mln_precondition(whitespace_seps_comps.is_valid());
    data_->whitespace_seps_ = whitespace_seps;
    data_->whitespace_seps_comps_ = whitespace_seps_comps;
  }


  template <typename L>
  bool
  document<L>::has_hline_seps() const
  {
    mln_precondition(is_valid());
    return data_->hline_seps_.is_valid();
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::hline_seps() const
  {
    mln_precondition(is_valid());
    return data_->hline_seps_;
  }


  template <typename L>
  const component_set<L>&
  document<L>::hline_seps_comps() const
  {
    mln_precondition(is_valid());
    return data_->hline_seps_comps_;
  }


  template <typename L>
  void
  document<L>::set_hline_separators(const image2d<bool>& hline_seps)
  {
    mln_precondition(is_valid());
    mln_precondition(hline_seps.is_valid());
    data_->hline_seps_ = hline_seps;

    mln_value(L) ncomps;
    data_->hline_seps_comps_ = primitive::extract::components(
      hline_seps,
      mln::c8(), ncomps,
      component::HorizontalLineSeparator);
  }


  template <typename L>
  void
  document<L>::set_hline_separators(const image2d<bool>& hline_seps,
				    const component_set<L>& hline_seps_comps)
  {
    mln_precondition(is_valid());
    mln_precondition(hline_seps.is_valid());
    mln_precondition(hline_seps_comps.is_valid());
    data_->hline_seps_ = hline_seps;
    data_->hline_seps_comps_ = hline_seps_comps;
  }


  template <typename L>
  bool
  document<L>::has_vline_seps() const
  {
    mln_precondition(is_valid());
    return data_->vline_seps_.is_valid();
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::vline_seps() const
  {
    mln_precondition(is_valid());
    return data_->vline_seps_;
  }


  template <typename L>
  const component_set<L>&
  document<L>::vline_seps_comps() const
  {
    mln_precondition(is_valid());
    return data_->vline_seps_comps_;
  }


  template <typename L>
  void
  document<L>::set_vline_separators(const image2d<bool>& vline_seps)
  {
    mln_precondition(is_valid());
    mln_precondition(vline_seps.is_valid());
    data_->vline_seps_ = vline_seps;

    mln_value(L) ncomps;
    data_->vline_seps_comps_ = primitive::extract::components(
      vline_seps,
      mln::c8(), ncomps,
      component::VerticalLineSeparator);
  }


  template <typename L>
  void
  document<L>::set_vline_separators(const image2d<bool>& vline_seps,
				    const component_set<L>& vline_seps_comps)
  {
    mln_precondition(is_valid());
    mln_precondition(vline_seps.is_valid());
    mln_precondition(vline_seps_comps.is_valid());
    data_->vline_seps_ = vline_seps;
    data_->vline_seps_comps_ = vline_seps_comps;
  }


  template <typename L>
  const mln::image2d<value::rgb8>&
  document<L>::image() const
  {
    mln_precondition(is_valid());
    return data_->image_;
  }


  template <typename L>
  void
  document<L>::set_image(const mln::image2d<value::rgb8>& image)
  {
    mln_precondition(data_ != 0);
    mln_precondition(image.is_valid());
    data_->image_ = image;
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::binary_image() const
  {
    mln_precondition(is_valid());
    return data_->binary_image_;
  }


  template <typename L>
  void
  document<L>::set_binary_image(const mln::image2d<bool>& binary_image)
  {
    mln_precondition(is_valid());
    mln_precondition(binary_image.is_valid());
    data_->binary_image_ = binary_image;
  }


  template <typename L>
  const mln::image2d<bool>&
  document<L>::binary_image_wo_seps() const
  {
    mln_precondition(is_valid());
    return data_->binary_image_wo_seps_;
  }


  template <typename L>
  void
  document<L>::set_binary_image_wo_seps(
    const mln::image2d<bool>& binary_image_wo_seps)
  {
    mln_precondition(is_valid());
    mln_precondition(binary_image_wo_seps.is_valid());
    data_->binary_image_wo_seps_ = binary_image_wo_seps;
  }


  template <typename L>
  bool operator==(const document<L>& lhs, const document<L>& rhs)
  {


    return
      lhs.filename() == rhs.filename()
      && lhs.image() == rhs.image()
      && lhs.binary_image() == rhs.binary_image()
      && lhs.has_text() == rhs.has_text()
      && lhs.paragraphs() == rhs.paragraphs()
      && lhs.has_elements() == rhs.has_elements()
      && lhs.elements() == rhs.elements()
      && lhs.has_whitespace_seps() == rhs.has_whitespace_seps()
      && lhs.whitespace_seps() == rhs.whitespace_seps()
      && lhs.whitespace_seps_comps() == rhs.whitespace_seps_comps()
      && lhs.has_hline_seps() == rhs.has_hline_seps()
      && lhs.hline_seps() == rhs.hline_seps()
      && lhs.hline_seps_comps() == rhs.hline_seps_comps()
      && lhs.has_vline_seps() == rhs.has_vline_seps()
      && lhs.vline_seps() == rhs.vline_seps()
      && lhs.vline_seps_comps() == rhs.vline_seps_comps();
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_DOCUMENT_HH
