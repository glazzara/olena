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

#ifndef SCRIBO_CORE_LINE_LINKS_HH
# define SCRIBO_CORE_LINE_LINKS_HH

/// \file
///
/// \brief Line links representation.


# include <mln/util/array.hh>
# include <mln/util/tracked_ptr.hh>

# include <scribo/core/concept/serializable.hh>
# include <scribo/core/line_set.hh>


namespace scribo
{

  using namespace mln;

  // Forward declaration.
  template <typename L> class line_links;


  namespace internal
  {
    /// Data structure for \c scribo::line_links<I>.
    template <typename L>
    struct line_links_data
    {
      line_links_data();
      line_links_data(const line_set<L>& lines, unsigned size);
      line_links_data(const line_set<L>& lines,
		      unsigned size, line_id_t value);

      mln::util::array<line_id_t> line_to_link_;
      line_set<L> lines_;
    };

  } // end of namespace scribo::internal




  /// \brief Line group representation.
  //
  template <typename L>
  class line_links : public Serializable<line_links<L> >
  {
    typedef internal::line_links_data<L> data_t;

  public:
    line_links();
    line_links(const line_set<L>& lines);
    line_links(const line_set<L>& lines, line_id_t value);

    const line_set<L>& lines() const;

    bool is_valid() const;

    unsigned nelements() const;

    line_id_t& operator()(line_id_t comp_id);
    const line_id_t& operator()(line_id_t comp_id) const;

    const mln::util::array<line_id_t>& line_to_link() const;

    void init();

    line_links<L> duplicate() const;

  private:
    mln::util::tracked_ptr<data_t> data_;
  };


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_links<L>& links);

  template <typename L>
  bool
  operator==(const line_links<L>& lhs, const line_links<L>& rhs);


# ifndef MLN_INCLUDE_ONLY


  namespace internal
  {


    /// Data structure for \c scribo::line_links<I>.
    template <typename L>
    line_links_data<L>::line_links_data()
    {
    }


    template <typename L>
    line_links_data<L>::line_links_data(const line_set<L>& lines,
					unsigned size)
      : line_to_link_(size), lines_(lines)
    {
    };


    template <typename L>
    line_links_data<L>::line_links_data(const line_set<L>& lines,
					unsigned size, line_id_t value)
      : line_to_link_(size, value), lines_(lines)
    {
    };


  } // end of namespace scribo::internal



  template <typename L>
  line_links<L>::line_links()
    : data_(0)
  {
  }


  template <typename L>
  line_links<L>::line_links(const line_set<L>& lines)
  {
    data_ = new data_t(lines, value::next(lines.nelements()));
  }


  template <typename L>
  line_links<L>::line_links(const line_set<L>& lines,
			    line_id_t value)
  {
    data_ = new data_t(lines, value::next(lines.nelements()),
		       value);
  }


  template <typename L>
  const line_set<L>&
  line_links<L>::lines() const
  {
    return data_->lines_;
  }


  template <typename L>
  bool
  line_links<L>::is_valid() const
  {
    return data_->lines_.is_valid()
      && data_->lines_.nelements() == (this->nelements() - 1);
  }


  template <typename L>
  unsigned
  line_links<L>::nelements() const
  {
    return data_->line_to_link_.nelements();
  }


  template <typename L>
  line_id_t&
  line_links<L>::operator()(line_id_t comp_id)
  {
    return data_->line_to_link_(comp_id);
  }


  template <typename L>
  const line_id_t&
  line_links<L>::operator()(line_id_t comp_id) const
  {
    return data_->line_to_link_(comp_id);
  }


  template <typename L>
  const mln::util::array<line_id_t>&
  line_links<L>::line_to_link() const
  {
    return data_->line_to_link_;
  }


  template <typename L>
  void
  line_links<L>::init()
  {
    for (unsigned i = 0; i < nelements(); ++i)
      if (!data_->lines_(i).is_valid()
	  || !data_->lines_(i).is_textline())
      {
	data_->line_to_link_(i) = 0;
      }
      else
      {
	data_->line_to_link_(i) = i;
      }
  }

  template <typename L>
  inline
  line_links<L>
  line_links<L>::duplicate() const
  {
    line_links<L> output;
    output.data_ = new data_t();

    *(output.data_.ptr_) = *(data_.ptr_);
    return output;
  }


  template <typename L>
  std::ostream&
  operator<<(std::ostream& ostr, const line_links<L>& links)
  {
    ostr << "line_links[";

    for_all_links(l, links)
      ostr << l << "->" << links.line_to_link()[l] << ", ";

    ostr << "]";

    return ostr;
  }


  template <typename L>
  bool
  operator==(const line_links<L>& lhs, const line_links<L>& rhs)
  {
    return lhs.lines() == rhs.lines()
      && lhs.line_to_link() == rhs.line_to_link();
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_LINE_LINKS_HH
