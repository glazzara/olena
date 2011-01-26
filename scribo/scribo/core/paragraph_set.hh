// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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
# include <scribo/core/line_links.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/paragraph_info.hh>

namespace scribo
{

  /*! \brief Paragraph container.

    Paragraph ids start from 1.

   */
  template <typename L>
  class paragraph_set
  {
  public:
    paragraph_set();
    paragraph_set(const line_links<L>& llinks, unsigned npars);

    unsigned nelements() const;

    paragraph_info<L>& operator()(unsigned i);
    const paragraph_info<L>& operator()(unsigned i) const;

    bool is_valid() const;

    const line_set<L>& lines() const;

  private:
    mln::util::array<paragraph_info<L> > pars_;
    line_set<L> lines_;
  };



  namespace make
  {

    /// \brief Construct a paragraph set from line links information.
    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const line_links<L>& llinks);

  } // end of namespace scribo::make


# ifndef MLN_INCLUDE_ONLY

  template <typename L>
  paragraph_set<L>::paragraph_set()
  {
  }

  template <typename L>
  paragraph_set<L>::paragraph_set(const line_links<L>& llinks, unsigned npars)
    : pars_(npars + 1, paragraph_info<L>(llinks))
  {
    lines_ = llinks.lines();
  }

  template <typename L>
  unsigned
  paragraph_set<L>::nelements() const
  {
    return pars_.nelements() - 1;
  }

  template <typename L>
  paragraph_info<L>&
  paragraph_set<L>::operator()(unsigned i)
  {
    return pars_[i];
  }

  template <typename L>
  const paragraph_info<L>&
  paragraph_set<L>::operator()(unsigned i) const
  {
    return pars_[i];
  }


  template <typename L>
  bool
  paragraph_set<L>::is_valid() const
  {
    return !pars_.is_empty();
  }


  template <typename L>
  const line_set<L>&
  paragraph_set<L>::lines() const
  {
    return lines_;
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
	if (parent(x) == x)
	  return x;
	else
	  return parent(x) = find_root(parent, parent(x));
      }

    } // end of namespace scribo::make::internal


    template <typename L>
    scribo::paragraph_set<L>
    paragraph(const line_links<L>& llinks)
    {
      line_links<L> links = llinks.duplicate();

      for (unsigned i = 1; i < links.nelements(); ++i)
	links(i) = internal::find_root(links, i);

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

      return parset;
    }

  } // end of namespace scribo::make


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace scribo

#endif // ! SCRIBO_CORE_PARAGRAPH_SET_HH
