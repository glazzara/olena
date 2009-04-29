// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_UTIL_SOFT_HEAP_HH
# define MLN_UTIL_SOFT_HEAP_HH

/// \file mln/util/soft_heap.hh
///
/// Define a generic soft heap.
/*
    This implementation is not an usual heap, it allows to set an error
    rate so that some nodes may be "corrupted". A "corrupted node" means
    that its correct order is not totally preserved for performance reasons.
    Of course, it will have an impact on the returned values.
    As a result, be ware of not using this data structure if the element
    order is relevant for to you.

    A corruption threshold can be passed to the constructor.
    This threshold means that if nodes have a rank higher than this
    threshold they can be "corrupted" and therefore their rank can be
    reduced. Tuning this threshold may have an impact on the structure
    entropy thus on the returned values order. It may also have an impact
    on the performance.


    More implementation details are available in:
    "The soft heap: an approximate priority queue with optimal error rate",
    Bernard Chazelle, JACM, 2000.

    URL: http://www.cs.princeton.edu/~chazelle/pubs/sheap.pdf

    \todo try not to use tracked_ptr.
    \todo try not to call T::plus_infty(). This is not generic enough but
	  it works with mln::point<>.
*/

# include <mln/core/concept/object.hh>
# include <mln/trait/value_.hh>
# include <mln/util/tracked_ptr.hh>
# include <climits>
# include <stack>



namespace mln
{

  namespace util
  {


    /// Element of an item list. Store the data (key) used in soft_heap.
    template <typename T>
    struct ilcell
    {
      typedef util::tracked_ptr< ilcell<T> > ilcell_t;

      ilcell();
      ilcell(const T& key, ilcell_t next = 0);

      ilcell_t next() const;
      const T& key() const;

      void set_next(ilcell_t next);
      void set_key(const T& key);

      private:
      T key_;
      ilcell_t next_;
    };


    /// Meta-data of an element in the heap.
    template <typename T, typename R>
    class node
    {

      typedef util::tracked_ptr< ilcell<T> > ilcell_t;

      public:
      node();
      node(const T& ckey, const R& rank,
	  node<T,R> *next = 0, node<T,R> *child = 0,
	  ilcell_t il = 0, ilcell_t il_tail = 0);
      ~node();

      const T& ckey() const;
      const R& rank() const;
      node<T,R> * next() const;
      node<T,R> * child() const;
      ilcell_t il() const;
      ilcell_t il_tail() const;

      void set_il(ilcell_t il);
      void set_il_tail(ilcell_t il_tail);
      void set_ckey(const T& ckey);
      void set_rank(const R& rank);
      void set_next(node<T,R> * next);
      void set_child(node<T,R> *child);

      private:
      T ckey_;
      R rank_;
      node<T,R> *next_;
      node<T,R> *child_;
      ilcell_t il_;
      ilcell_t il_tail_;

    };




    /// Top structure of the soft heap.
    template <typename T, typename R>
    class head
    {
      public:

	head();
	head(const R& rank, node<T,R> *queue = 0, head<T,R> *next = 0,
	    head<T,R> *prev = 0, head<T,R> *suffix_min = 0);
	~head();

	node<T,R> *queue() const;
	head<T,R> *next() const;
	head<T,R> *prev() const;
	head<T,R> *suffix_min() const;
	const R& rank() const;

	void set_queue(node<T,R> *queue);
	void set_next(head<T,R> *next);
	void set_prev(head<T,R> *prev);
	void set_suffix_min(head<T,R> *suffix_min);
	void set_rank(const R& rank);

      private:
	node<T,R> *queue_;
	head<T,R> *next_;
	head<T,R> *prev_;
	head<T,R> *suffix_min_;
	R rank_;

    };




    /// \brief Soft heap.
    /// T key, the data to store in the heap. For instance a point 2d.
    /// R rank, for instance int_u8
    ///
    /// \ingroup modutil
    //
    template <typename T, typename R>
    class soft_heap : public Object< soft_heap<T,R> >
    {
      typedef util::tracked_ptr< ilcell<T> > ilcell_t;

    public:

      /// Element associated type.
      typedef T element;


      /// Default constructor.
      /// A corruption threshold \p r can be specified.
      /// This threshold means that if nodes have a rank higher than this
      /// threshold they can be "corrupted" and therefore their rank can be
      /// reduced.
      ///
      // FIXME: is it a correct value for r?
      soft_heap(unsigned r = 20);
      /// Destructor
      ~soft_heap();


      /// Add a new element \p element.
      void push(const T& element);

      /// Merge \p sh with this heap.
      /// Be ware that after this call, \p sh will be empty. This heap will
      /// hold the elements which were part of \p sh.
      void push(soft_heap<T,R>& sh);

      /// Returns the element with the lowest priority and remove it from the
      /// heap.
      T pop_front();

      /// Return true if there is at least one element.
      bool is_valid() const;

      /// Return true if there is at least one element.
      bool is_empty() const;

      /// Return the number of element in the heap.
      int nelements() const;

      /// Clear the heap.
      void clear();

      /// Internal use only. Return a pointer to the first header struct
      /// of this heap.
      head<T,R> *head_hook_() const;

      /// Reset the heap to an empty heap. Do *NOT* delete element which may
      /// have been inserted.
      /// \sa push
      void soft_clear_();



    private:
      /// Merge a node \p q to this heap.
      /// \sa push
      void meld(node<T,R> *q);

      /// Update suffix_min pointer according to the new values inserted in
      /// the item lists.
      void fix_minlist(head<T,R> *h);

      /// After the deletion of an element, this function move/update item lists
      /// in the proper nodes.
      node<T,R> * sift(node<T,R> *v);

      /// Delete all element in a C-list from \p begin to \p end. \p end is not
      /// deleted.
      /// The type L must provide 'L *next()'.
      ///
      ///		next
      ///
      ///	  |x| -> |x| -> |x|
      ///
      template <typename L>
      void clear_list(L *begin, L *end = 0);

      /// Delete all element in a 2 dimensional C-list.
      /// The type L must provide 'L *next()' and 'L *child()'.
      ///
      ///		next
      ///
      ///	  |x| -> |x| -> |x|
      ///	   |
      /// child    v
      ///	  |x| -> |x|
      ///
      template <typename L>
      void deep_clear_list(L *n);

      /// Print which node is part of which head.
      void debug_head_list() const;

      /// Display the heap, preserving the hierarchy of the elements.
      void println_() const;

      head<T,R> *header_;
      head<T,R> *tail_;

      /// Corruption threshold. Must be set once through the constructor.
      unsigned r_;

      int nelements_;
    };



# ifndef MLN_INCLUDE_ONLY


    /*-------`
    | ilcell |
    \-------*/


    template <typename T>
    inline
    ilcell<T>::ilcell()
    {
    }


    template <typename T>
    inline
    ilcell<T>::ilcell(const T& key, ilcell_t next)
    : key_(key), next_(next)
    {
    }


    template <typename T>
    inline
    typename ilcell<T>::ilcell_t
    ilcell<T>::next() const
    {
      return next_;
    }


    template <typename T>
    inline
    const T&
    ilcell<T>::key() const
    {
      return key_;
    }


    template <typename T>
    inline
    void
    ilcell<T>::set_next(ilcell_t next)
    {
      next_ = next;
    }


    template <typename T>
    inline
    void
    ilcell<T>::set_key(const T& key)
    {
      key_ = key;
    }




    /*-----`
    | node |
    \-----*/


    template <typename T, typename R>
    inline
    node<T,R>::node()
    : il_(0), il_tail_(0)
    {
    }


    template <typename T, typename R>
    inline
    node<T,R>::node(const T& ckey, const R& rank,
		    node<T,R> *next, node<T,R> *child,
		    ilcell_t il, ilcell_t il_tail)
      : ckey_(ckey), rank_(rank), next_(next), child_(child),
	il_(il), il_tail_(il_tail)
    {
    }


    template <typename T, typename R>
    inline
    node<T,R>::~node()
    {
    }


    template <typename T, typename R>
    inline
    const T&
    node<T,R>::ckey() const
    {
      return ckey_;
    }


    template <typename T, typename R>
    inline
    const R&
    node<T,R>::rank() const
    {
      return rank_;
    }


    template <typename T, typename R>
    inline
    node<T,R> *
    node<T,R>::next() const
    {
      return next_;
    }


    template <typename T, typename R>
    inline
    node<T,R> *
    node<T,R>::child() const
    {
      return child_;
    }


    template <typename T, typename R>
    inline
    typename node<T,R>::ilcell_t
    node<T,R>::il() const
    {
      return il_;
    }


    template <typename T, typename R>
    inline
    typename node<T,R>::ilcell_t
    node<T,R>::il_tail() const
    {
      return il_tail_;
    }


    template <typename T, typename R>
    inline
    void
    node<T,R>::set_il(ilcell_t il)
    {
      il_ = il;
    }


    template <typename T, typename R>
    inline
    void
    node<T,R>::set_il_tail(ilcell_t il_tail)
    {
      il_tail_ = il_tail;
    }


    template <typename T, typename R>
    inline
    void
    node<T,R>::set_ckey(const T& ckey)
    {
      ckey_ = ckey;
    }


    template <typename T, typename R>
    inline
    void
    node<T,R>::set_rank(const R& rank)
    {
      rank_ = rank;
    }


    template <typename T, typename R>
    inline
    void
    node<T,R>::set_next(node<T,R> * next)
    {
      next_ = next;
    }


    template <typename T, typename R>
    inline
    void
    node<T,R>::set_child(node<T,R> *child)
    {
      child_ = child;
    }




    /*-----`
    | head |
    \-----*/


    template <typename T, typename R>
    inline
    head<T,R>::head()
    {
    }


    template <typename T, typename R>
    inline
    head<T,R>::head(const R& rank, node<T,R> *queue, head<T,R> *next,
		    head<T,R> *prev, head<T,R> *suffix_min)
      : queue_(queue), next_(next), prev_(prev),
	suffix_min_(suffix_min), rank_(rank)
    {
    }


    template <typename T, typename R>
    inline
    head<T,R>::~head()
    {
    }


    template <typename T, typename R>
    inline
    node<T,R> *
    head<T,R>::queue() const
    {
      return queue_;
    }


    template <typename T, typename R>
    inline
    head<T,R> *
    head<T,R>::next() const
    {
      return next_;
    }


    template <typename T, typename R>
    inline
    head<T,R> *
    head<T,R>::prev() const
    {
      return prev_;
    }


    template <typename T, typename R>
    inline
    head<T,R> *
    head<T,R>::suffix_min() const
    {
      return suffix_min_;
    }


    template <typename T, typename R>
    inline
    const R&
    head<T,R>::rank() const
    {
      return rank_;
    }


    template <typename T, typename R>
    inline
    void
    head<T,R>::set_queue(node<T,R> *queue)
    {
      queue_ = queue;
    }


    template <typename T, typename R>
    inline
    void
    head<T,R>::set_next(head<T,R> *next)
    {
      next_ = next;
    }


    template <typename T, typename R>
    inline
    void
    head<T,R>::set_prev(head<T,R> *prev)
    {
      prev_ = prev;
    }


    template <typename T, typename R>
    inline
    void
    head<T,R>::set_suffix_min(head<T,R> *suffix_min)
    {
      suffix_min_ = suffix_min;
    }


    template <typename T, typename R>
    inline
    void
    head<T,R>::set_rank(const R& rank)
    {
      rank_ = rank;
    }




    /*------------`
    | soft_heap |
    \------------*/


    template <typename T, typename R>
    inline
    soft_heap<T,R>::soft_heap(unsigned r)
    {
      nelements_ = 0;
      r_ = r;
      header_ = new head<T,R>(mln_max(R));
      tail_ = new head<T,R>(mln_max(R), 0, 0, header_);
      header_->set_next(tail_);
    }


    template <typename T, typename R>
    inline
    soft_heap<T,R>::~soft_heap()
    {
      head<T,R> *tmp = header_;
      while (tmp != 0)
      {
	deep_clear_list(tmp->queue());
	tmp = tmp->next();
      }
      clear_list(header_, tail_->next());
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::push(const T& element)
    {
      ilcell_t l(new ilcell<T>(element));
      node<T,R> *q = new node<T,R>(element, 0, 0, 0, l, l);
      meld(q);
      ++nelements_;
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::push(soft_heap<T,R>& psh)
    {
      head<T,R> *head = psh.head_hook_();
      while (head != 0)
      {
	if (head->queue() != 0)
	  meld(head->queue());
	head = head->next();
      }
      nelements_ += psh.nelements();
      psh.soft_clear_();
    }


    template <typename T, typename R>
    inline
    T
    soft_heap<T,R>::pop_front()
    {
      mln_precondition(is_valid());

      node<T,R> *tmp;

      T min;
      int childcount;
      head<T,R> *h = header_->next()->suffix_min();
      while (h->queue()->il() == 0)
      {
	tmp = h->queue();
	childcount = 0;
	while (tmp->next() != 0)
	{
	  tmp = tmp->next();
	  ++childcount;
	}

	if (childcount < h->rank() / 2)
	{
	  h->prev()->set_next(h->next());
	  h->next()->set_prev(h->prev());
	  fix_minlist(h->prev());
	  tmp = h->queue();
	  while (tmp->next() != 0)
	  {
	    meld(tmp->child());
	    tmp = tmp->next();
	  }
	  //FIXME: is it right?
	  deep_clear_list(h->queue());
	  delete h;
	}
	else
	{
	  h->set_queue(sift(h->queue()));
	  // FIXME: not generic enough.
	  if (h->queue()->ckey() == T::plus_infty())
	  {
	    h->prev()->set_next(h->next());
	    h->next()->set_prev(h->prev());

	    // Remove h and switch to h->prev.
	    head<T,R> *h_bak = h;
	    h = h->prev();
	    deep_clear_list(h_bak->queue());
	    delete h_bak;
	  }
	  fix_minlist(h);
	}
	h = header_->next()->suffix_min();
      }

      min = h->queue()->il()->key();

      //Don't need to delete h->queue()->il(). May be used in another node.
      h->queue()->set_il(h->queue()->il()->next());
      if (h->queue()->il() == 0)
	h->queue()->set_il_tail(0);

      --nelements_;
      return min;
    }


    template <typename T, typename R>
    inline
    bool
    soft_heap<T,R>::is_valid() const
    {
      return header_ != 0 && tail_ != 0;
    }


    template <typename T, typename R>
    inline
    bool
    soft_heap<T,R>::is_empty() const
    {
      return nelements_ == 0 ;
    }


    template <typename T, typename R>
    inline
    int
    soft_heap<T,R>::nelements() const
    {
      return nelements_;
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::clear()
    {
      if (header_->next() == tail_)
	return;

      head<T,R> *tohead = header_->next();
      head<T,R> *prevtail = tail_->prev();
      prevtail->set_next(0);
      tohead->set_prev(0);
      header_->set_next(tail_);
      tail_->set_prev(header_);

      head<T,R> *tmp = tohead;
      while (tmp != 0)
      {
	deep_clear_list(tmp->queue());
	tmp = tmp->next();
      }
      clear_list(tohead);

      nelements_ = 0;
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::soft_clear_()
    {
      clear_list(header_->next(), tail_);
      header_->set_next(tail_);
      header_->set_prev(0);
      tail_->set_next(0);
      tail_->set_prev(header_);
      nelements_ = 0;
    }


    template <typename T, typename R>
    inline
    head<T,R> *
    soft_heap<T,R>::head_hook_() const
    {
      return header_;
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::meld(node<T,R>* q)
    {
      head<T,R> *tohead = header_->next();
      while (q->rank() > tohead->rank())
	tohead = tohead->next();
      head<T,R> *prevhead = tohead->prev();

      node<T,R> *top, *bottom;

      while (q->rank() == tohead->rank())
      {
	if (util::ord_strict(q->ckey(), tohead->queue()->ckey()))
	{
	  top = q;
	  bottom = tohead->queue();
	}
	else
	{
	  top = tohead->queue();
	  bottom = q;
	}

	q = new node<T,R>(top->ckey(), top->rank() + 1, top, bottom,
	    top->il(), top->il_tail());

	tohead = tohead->next();
      }

      head<T,R> *h;
      if (prevhead == tohead->prev())
      {
	h = new head<T,R>(q->rank(), q, tohead, prevhead);
      }
      else
      {
	// Do not create a new head.
	h = prevhead->next();

	//Discard/delete everything between h and tohead.
	head<T,R> *head_del = h->next(), *tmp_del;
	while (head_del != tohead)
	{
	  tmp_del = head_del->next();
	  delete head_del;
	  head_del = tmp_del;
	}

      }
      h->set_queue(q);
      h->set_rank(q->rank());
      h->set_prev(prevhead);
      h->set_next(tohead);

      prevhead->set_next(h);
      tohead->set_prev(h);

      fix_minlist(h);
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::fix_minlist(head<T,R> *h)
    {
      head<T,R> *tmpmin;
      if (h->next() == tail_)
	tmpmin = h;
      else
	tmpmin = h->next()->suffix_min();

      while (h != header_)
      {
	if (util::ord_strict(h->queue()->ckey(), tmpmin->queue()->ckey()))
	  tmpmin = h;
	h->set_suffix_min(tmpmin);
	h = h->prev();
      }
    }


    template <typename T, typename R>
    inline
    node<T,R> *
    soft_heap<T,R>::sift(node<T,R> *v)
    {
      node<T,R> *tmp;
      // We do not need to free the list since these object are also pointed by
      // other structs.
      v->set_il(0);
      v->set_il_tail(0);

      if (v->next() == 0 && v->child() == 0)
      {
	//FIXME: not generic enough...
	v->set_ckey(T::plus_infty());
	return v;
      }
      node<T,R> *v_next_bak = v->next();
      v->set_next(sift(v->next()));

      // Cleanup possibly removed nodes.
      while (v_next_bak != v->next())
      {
	deep_clear_list(v_next_bak->next());
	deep_clear_list(v_next_bak->child());
	tmp = v_next_bak->next();
	delete v_next_bak;
	v_next_bak = tmp;
      }

      if (util::ord_strict(v->child()->ckey(), v->next()->ckey()))
      {
	// Swap child and next.
	tmp = v->child();
	v->set_child(v->next());
	v->set_next(tmp);
      }

      v->set_il(v->next()->il());
      v->set_il_tail(v->next()->il_tail());
      v->set_ckey(v->next()->ckey());

      if (v->rank() > r_ && (v->rank() % 2 == 1
	    || v->child()->rank() < static_cast<unsigned>((v->rank() - 1))))
      {
	node<T,R> *v_next_bak = v->next();
	v->set_next(sift(v->next()));

	// Cleanup possibly removed nodes.
	while (v_next_bak != v->next())
	{
	  deep_clear_list(v_next_bak->next());
	  deep_clear_list(v_next_bak->child());
	  tmp = v_next_bak->next();
	  delete v_next_bak;
	  v_next_bak = tmp;
	}

	if (util::ord_strict(v->child()->ckey(), v->next()->ckey()))
	{
	  tmp = v->child();
	  v->set_child(v->next());
	  v->set_next(tmp);
	}

	if (v->next()->ckey() != T::plus_infty() && v->next()->il() != 0)
	{
	  v->next()->il_tail()->set_next(v->il());
	  v->set_il(v->next()->il());
	  if (v->il_tail() == 0)
	    v->set_il_tail(v->next()->il_tail());
	  v->set_ckey(v->next()->ckey());
	}
      }

      if (v->child()->ckey() == T::plus_infty())
      {
	if (v->next()->ckey() == T::plus_infty())
	{
	  // Clear child and next lists.
	  deep_clear_list(v->child());
	  deep_clear_list(v->next());

	  v->set_child(0);
	  v->set_next(0);
	}
	else
	{
	  node<T,R> *next_bak = v->next();

	  // There may be several children, we must clear all of them.
	  deep_clear_list(v->child());

	  v->set_child(v->next()->child());
	  v->set_next(v->next()->next());
	  delete next_bak;
	}
      }

      return v;
    }


    template <typename T, typename R>
    template <typename L>
    inline
    void
    soft_heap<T,R>::clear_list(L *begin, L *end)
    {
      L *tmp;
      while (begin != end)
      {
	tmp = begin->next();
	delete begin;
	begin = tmp;
      }
    }


    template <typename T, typename R>
    template <typename L>
    inline
    void
    soft_heap<T,R>::deep_clear_list(L *n)
    {
      L *current_node, *tmp;
      std::stack<L *> st;

      st.push(n);
      while (!st.empty())
      {
	current_node = st.top();
	st.pop();
	while (current_node != 0)
	{
	  if (current_node->child() != 0)
	    st.push(current_node->child());

	  tmp = current_node->next();
	  delete current_node;

	  current_node = tmp;
	}
      }
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::debug_head_list() const
    {
      head<T,R> *n = header_;
      std::cout << "Head list = " << std::endl;
      while (n != 0)
      {
	std::cout << n->id << "(";

	node<T,R> *current_node;
	std::stack< node<T,R> *> st;
	st.push(n->queue());
	while (!st.empty())
	{
	  current_node = st.top();
	  st.pop();
	  while (current_node != 0)
	  {
	    if (current_node->child() != 0)
	      st.push(current_node->child());
	    std::cout << current_node->id << ",";
	    current_node = current_node->next();
	  }
	}

	std::cout << ") - ";
	n = n->next();
      }
      std::cout << std::endl;
    }


    template <typename T, typename R>
    inline
    void
    soft_heap<T,R>::println_() const
    {

      std::cout << "===============" << std::endl;
      head<T,R> *head = header_;
      while (head != 0)
      {
	std::cout << "<Head>" << std::endl;
	node<T,R> *node = head->queue(), *child;
	while (node != 0)
	{
	  std::cout << "  <node>" << std::endl;

	  ilcell_t il(node->il());
	  while (il != 0)
	  {
	    std::cout << il->item() << std::endl;
	    il = il->next();
	  }

	  child = node->child();
	  while (child != 0)
	  {
	    std::cout << "    <child>" << std::endl;
	    ilcell_t il(child->il());
	    while (il != 0)
	    {
	      std::cout << il->item() << std::endl;
	      il = il->next();
	    }
	    child = child->child();
	    std::cout << "    </child>" << std::endl;
	  }
	  node = node->next();

	  std::cout << "  </node>" << std::endl;
	}
	std::cout << "</Head>" << std::endl;
	head = head->next();
      }
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_SOFT_HEAP_HH
