// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
/// \brief Tree node class used in XY-Cut algorithm.


#ifndef SCRIBO_LAYOUT_INTERNAL_NODE_HH
# define SCRIBO_LAYOUT_INTERNAL_NODE_HH

# include <mln/core/alias/box2d.hh>

namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      /// \internal \brief Tree node class used in XY-Cut algorithm.
      template <typename B>
      class node
      {
      public:
	node();
	node(const B& bbox);

	~node();

	const B& get_bbox() const;

	node<B>* get_ls();
	const node<B>* get_ls() const;
	node<B>* get_rs();
	const node<B>* get_rs() const;

	bool is_leaf() const;

	void set_ls(node<B>* n);
	void set_rs(node<B>* n);

      private:
	B bbox_;
	node<B>* ls_;
	node<B>* rs_;
      };

# ifndef MLN_INCLUDE_ONLY

      template <typename B>
      node<B>::node()
	: ls_(0),
	  rs_(0)
      {
      }

      template <typename B>
      node<B>::node(const B& bbox)
	: bbox_(bbox),
	  ls_(0),
	  rs_(0)
      {
      }

      template <typename B>
      node<B>::~node()
      {
	if (ls_)
	  delete ls_;

	if (rs_)
	  delete rs_;
      }

      template <typename B>
      const B& node<B>::get_bbox() const
      {
	return bbox_;
      }

      template <typename B>
      node<B>* node<B>::get_ls()
      {
	return ls_;
      }

      template <typename B>
      const node<B>* node<B>::get_ls() const
      {
	return ls_;
      }

      template <typename B>
      node<B>* node<B>::get_rs()
      {
	return rs_;
      }

      template <typename B>
      const node<B>* node<B>::get_rs() const
      {
	return rs_;
      }

      template <typename B>
      bool node<B>::is_leaf() const
      {
	return ls_ == rs_;
      }

      template <typename B>
      void node<B>::set_ls(node<B>* n)
      {
	ls_ = n;
      }

      template <typename B>
      void node<B>::set_rs(node<B>* n)
      {
	rs_ = n;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_NODE_HH
