// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_PARAGRAPH_SET_HH
# define SCRIBO_CORE_PARAGRAPH_SET_HH

# include <mln/util/array.hh>
# include <mln/make/relabelfun.hh>
# include <mln/value/int_u16.hh>
# include <mln/core/concept/function.hh>
# include <scribo/core/line_links.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/paragraph_info.hh>
# include <scribo/core/stats.hh>

# include <scribo/core/concept/serializable.hh>
# include <scribo/core/tag/paragraph.hh>

namespace scribo
{

  typedef mln::util::object_id<scribo::ParagraphId, unsigned> paragraph_id_t;

  namespace internal
  {

    /// Data structure for \c scribo::paragraph_set<I>.
    template <typename L>
    struct paragraph_set_data
    {
      paragraph_set_data();
      paragraph_set_data(const line_links<L>& llines, unsigned npars);

      mln::util::array<paragraph_info<L> > pars_;
      line_set<L> lines_;
      line_links<L> links_;
    };

  } // end of namespace scribo::internal


  /*! \brief Paragraph container.

    Paragraph ids start from 1.

    \ingroup grpstruct
   */
  template <typename L>
  class paragraph_set : public Serializable<paragraph_set<L> >
  {
    typedef internal::paragraph_set_data<L> data_t;

  public:
    paragraph_set();
    paragraph_set(internal::paragraph_set_data<L>* data);
    paragraph_set(const line_links<L>& llinks, unsigned npars);

    unsigned nelements() const;

    paragraph_info<L>& operator()(const paragraph_id_t& i);
    const paragraph_info<L>& operator()(const paragraph_id_t& i) const;

    bool is_valid() const;

    // Massively invalidate paragraphs.
    template <typename F>
    void invalidate(const Function_v2b<F>& f);

    const line_set<L>& lines() const;

    const line_links<L>& links() const;

    paragraph_set<L> duplicate() const;

  private:
    mln::util::tracked_ptr< internal::paragraph_set_data<L> > data_;
  };


  template <typename L>
  bool operator==(const paragraph_set<L>& lhs, const paragraph_set<L>& rhs);

  namespace make
  {

    /// \brief Construct a paragraph set from line links information.
    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const line_links<L>& llinks,
	      const line_links<L>& rlinks);

    /// \brief Construct a paragraph set from line set information.
    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const scribo::line_set<L>& lines);


    /// \brief Construct a paragraph set from line links information.
    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const line_links<L>& llinks);


  } // end of namespace scribo::make


# ifndef MLN_INCLUDE_ONLY

  // paragraph_set_data<L> >

  namespace internal
  {

    // data< paragraph_set<L> >


    template <typename L>
    inline
    paragraph_set_data<L>::paragraph_set_data()
    {
    }


    template <typename L>
    inline
    paragraph_set_data<L>::paragraph_set_data(const line_links<L>& llinks, unsigned npars)
      : pars_(npars + 1, paragraph_info<L>(llinks)), links_(llinks)
    {
      lines_ = llinks.lines();
    }

  } // end of namespace mln::internal


  template <typename L>
  paragraph_set<L>::paragraph_set()
    : data_(0)
  {
  }

  template <typename L>
  paragraph_set<L>::paragraph_set(internal::paragraph_set_data<L>* data)
  {
    data_ = data;
  }

  template <typename L>
  paragraph_set<L>::paragraph_set(const line_links<L>& llinks, unsigned npars)
  {
    data_ = new internal::paragraph_set_data<L>(llinks, npars);
  }

  template <typename L>
  unsigned
  paragraph_set<L>::nelements() const
  {
    mln_precondition(data_ != 0);
    return data_->pars_.nelements() - 1;
  }

  template <typename L>
  paragraph_info<L>&
  paragraph_set<L>::operator()(const paragraph_id_t& i)
  {
    mln_precondition(data_ != 0);
    return data_->pars_[i];
  }

  template <typename L>
  const paragraph_info<L>&
  paragraph_set<L>::operator()(const paragraph_id_t& i) const
  {
    mln_precondition(data_ != 0);
    return data_->pars_[i];
  }


  template <typename L>
  bool
  paragraph_set<L>::is_valid() const
  {
    return data_ && !data_->pars_.is_empty();
  }

  template <typename L>
  template <typename F>
  void
  paragraph_set<L>::invalidate(const Function_v2b<F>& f_)
  {
    const F& f = exact(f_);

    for_all_paragraphs(p, (*this))
      if (!f(p))
	(*this)(p).invalidate();
  }

  template <typename L>
  const line_set<L>&
  paragraph_set<L>::lines() const
  {
    mln_precondition(data_ != 0);
    return data_->lines_;
  }


  template <typename L>
  const line_links<L>&
  paragraph_set<L>::links() const
  {
    mln_precondition(data_ != 0);
    return data_->links_;
  }


  template <typename L>
  inline
  paragraph_set<L>
  paragraph_set<L>::duplicate() const
  {
    paragraph_set<L> output;
    output.data_ = new data_t();

    *(output.data_.ptr_) = *(data_.ptr_);
    return output;
  }


  template <typename L>
  bool operator==(const paragraph_set<L>& lhs, const paragraph_set<L>& rhs)
  {
    if (! (lhs.lines() == rhs.lines() && lhs.nelements() == rhs.nelements()))
    {
      return false;
    }

    for_all_paragraphs(p, lhs)
      if (!(lhs(p) == rhs(p)))
      {
	return false;
      }

    return true;
  }


  namespace make
  {

    namespace internal
    {

      template <typename L>
      inline
      unsigned
      find_root(line_links<L>& parent, unsigned x)
      {
	unsigned tmp_x = x;

	while (parent(tmp_x) != tmp_x)
	  tmp_x = parent(tmp_x);

	while (parent(x) != x)
	{
	  const unsigned tmp = parent(x);
	  x = parent(x);
	  parent(tmp) = tmp_x;
	}

	return x;
      }

      template <typename L>
      inline
      void
      set_root(line_links<L>& parent, unsigned x, const unsigned root)
      {
	while (parent(x) != x && parent(x) != root)
	{
	  const unsigned tmp = parent(x);
	  x = parent(x);
	  parent(tmp) = root;
	}

	parent(x) = root;
      }


    } // end of namespace scribo::make::internal


    // FIXME: move that code into paragraph_set constructor?
    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const line_links<L>& llinks,
	      const line_links<L>& rlinks)
    {
      line_links<L> links = llinks.duplicate();

      for_all_links(l, links)
      {
	const line_id_t current_neighbor = llinks(l);
	links(l) = internal::find_root(links, l);
	const line_id_t root_index = links(l);

	for (unsigned j = 0; j < rlinks.nelements(); ++j)
	{
	  if (l != j &&
	      current_neighbor != l &&
	      rlinks(j) == l)
	    internal::set_root(links, j, root_index);
	}
      }

      unsigned npars;
      mln::fun::i2v::array<unsigned>
	par_ids = mln::make::relabelfun(links.line_to_link(),
					links.nelements() - 1, npars);
      paragraph_set<L> parset(links, npars);

      const scribo::line_set<L>& lines = links.lines();
      for_all_links(l, links)
	if (links(l))
	{
	  value::int_u16 par_id = par_ids(l);
	  parset(par_id).add_line(lines(l));
	}

      for_all_paragraphs(p, parset)
      {
	paragraph_info<L>& current_par = parset(p);
	stats< float > delta(current_par.nlines());

	// Update stats
	current_par.force_stats_update();

	// Compute paragraph's delta baseline
	const mln::util::array<line_id_t>& line_ids = current_par.line_ids();
	const unsigned nelements = line_ids.nelements();

	for (unsigned i = 0; i < nelements; ++i)
	{
	  const line_id_t& current_id = line_ids(i);

	  if (llinks(current_id) != current_id)
	  {
	    const line_info<L>& current_line = lines(current_id);
	    const line_info<L>& left_line = lines(llinks(current_id));

	    delta.take(left_line.baseline() - current_line.baseline());
	  }
	}

	int median = delta.median();

	if (!median)
	  median = lines(current_par.line_ids()(0)).x_height();

	current_par.set_delta_baseline(median);
      }

      return parset;
    }


    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const line_links<L>& llinks)
    {
      line_links<L> links = llinks.duplicate();

      for_all_links(l, links)
	links(l) = internal::find_root(links, l);

      unsigned npars;
      mln::fun::i2v::array<unsigned>
	par_ids = mln::make::relabelfun(links.line_to_link(),
					links.nelements() - 1, npars);
      paragraph_set<L> parset(links, npars);

      const scribo::line_set<L>& lines = links.lines();
      for_all_links(l, links)
	if (links(l))
	{
	  value::int_u16 par_id = par_ids(l);
	  parset(par_id).add_line(lines(l));
	}

      for_all_paragraphs(p, parset)
	parset(p).force_stats_update();

      return parset;
    }

    // FIXME: move that code into paragraph_set constructor?
    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const scribo::line_set<L>& lines)
    {
      line_links<L> links(lines);
      links.init();

      unsigned npars;
      mln::fun::i2v::array<unsigned>
	par_ids = mln::make::relabelfun(links.line_to_link(),
					links.nelements() - 1, npars);
      paragraph_set<L> parset(links, npars);

      for_all_links(l, links)
	if (links(l))
	{
	  value::int_u16 par_id = par_ids(l);
	  parset(par_id).add_line(lines(l));
	}

      for_all_paragraphs(p, parset)
	parset(p).force_stats_update();

      return parset;
    }



  } // end of namespace scribo::make


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_PARAGRAPH_SET_HH
