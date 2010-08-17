// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_FIBONACCI_HEAP_HH
# define MLN_UTIL_FIBONACCI_HEAP_HH


# include <iostream>
# include <mln/core/concept/object.hh>
# include <mln/util/ord.hh>



namespace mln
{

  namespace util
  {


    namespace internal
    {

      /*---------------------------.
      | Fibonacci Heap node Class. |
      `---------------------------*/

      template <typename P, typename T>
      class fibonacci_heap_node
      {

      public:
	/// Default constructor.
	fibonacci_heap_node();

	/// Construct a new node with \p value as value.
	fibonacci_heap_node(const P& priority, const T& value);

	~fibonacci_heap_node();

	/// Return the node value.
	const T& value() const;

	const P& priority() const;

	fibonacci_heap_node<P,T> *left() const;
	fibonacci_heap_node<P,T> *right() const;
	fibonacci_heap_node<P,T> *parent() const;
	fibonacci_heap_node<P,T> *child() const;

	short degree() const;

	short mark() const;


	void set_value(const T&);
	void set_left(fibonacci_heap_node<P,T> *node);
	void set_right(fibonacci_heap_node<P,T> *node);
	void set_parent(fibonacci_heap_node<P,T> *node);
	void set_child(fibonacci_heap_node<P,T> *node);
	void set_degree(short degree);
	void set_mark(short mark);

	void operator=(fibonacci_heap_node<P,T>& rhs);
	bool operator==(fibonacci_heap_node<P,T>& rhs);
	bool operator<(fibonacci_heap_node<P,T>& rhs);

	void print_(std::ostream& ostr) const;


      private:

	fibonacci_heap_node<P,T> *left_;
	fibonacci_heap_node<P,T> *right_;
	fibonacci_heap_node<P,T> *parent_;
	fibonacci_heap_node<P,T> *child_;
	short degree_;
	short mark_;
	P priority_;
	T value_;
      };

    } // end of namespace mln::util::internal



    /*----------------------.
    | Fibonacci Heap Class. |
    `----------------------*/

    /// \brief Fibonacci heap.
    ///
    /// \ingroup modutil
    template <typename P, typename T>
    class fibonacci_heap : public Object< fibonacci_heap<P,T> >
    {
    public:

      typedef T element;

      /// Default constructor.
      fibonacci_heap();

      /// Copy constructor.
      ///
      /// Beware that once this heap is constructed, the argument \a node
      /// is cleared and all its elements are part of this new heap.
      fibonacci_heap(const fibonacci_heap<P,T>& node);

      ~fibonacci_heap();

      /// Push a new element in the heap.
      /// \sa insert
      void push(const P& priority, const T& value);

      /// Take \p other_heap's elements and insert them in this heap.
      /// After this call \p other_heap is cleared.
      void push(fibonacci_heap<P,T>& other_heap);

      /// Return the minimum value in the heap.
      const T& front() const;

      /// Return and remove the minimum value in the heap.
      T pop_front();

      /// Is it empty?
      bool is_empty() const;

      /// return false if it is empty.
      bool is_valid() const;

      /// Return the number of elements.
      unsigned nelements() const;

      /// Clear all elements in the heap and make the heap empty.
      void clear();

      /// Assignment operator.
      ///
      /// Beware that this operator does <em>not</em> copy data from
      /// \a rhs to this heap.  It moves all elements which means that
      /// afterwards, \p rhs is is cleared and all its elements are
      /// part of this new heap.
      fibonacci_heap<P,T>& operator=(fibonacci_heap<P,T>& rhs);



      std::ostream& print_(std::ostream& ostr,
			   internal::fibonacci_heap_node<P,T> *tree   = 0,
			   internal::fibonacci_heap_node<P,T> *parent = 0) const;

    private:

      // Internal functions that help to implement the Standard Operations


      /// Allow to change a node value.
      // FIXME: Cannot use that function efficiently except by passing the
      // node pointer.  Any idea why?
      // FIXME: May be part of the public interface.
      int decrease_key(internal::fibonacci_heap_node<P,T> *node,
		       internal::fibonacci_heap_node<P,T>& key);

      /// Remove node \p node from the child list of its parent node.
      // FIXME: Cannot use that function efficiently except by passing the
      // node pointer.  Any idea why?
      // FIXME: May be part of the public interface.
      int remove(internal::fibonacci_heap_node<P,T> *node);

      /// Insert a new node \p node in the heap.
      void insert(internal::fibonacci_heap_node<P,T> *node);

      /// Swap the two given nodes.
      void exchange(internal::fibonacci_heap_node<P,T>*& lhs,
		    internal::fibonacci_heap_node<P,T>*& rhs);

      /// Internal function that reorganizes heap as part of an pop_front().
      /// We must find new minimum in heap, which could be anywhere along the
      /// root list.  The search could be O(n) since all nodes could be on
      /// the root list.  So, we reorganize the tree into more of a binomial
      /// forest structure, and then find the new minimum on the consolidated
      /// O(lg n) sized root list, and in the process set each Parent pointer
      /// to 0, and count the number of resulting subtrees.
      ///
      /// Note that after a list of n inserts, there will be n nodes on the
      /// root list, so the first pop_front() will be O(n) regardless of
      /// whether or not we consolidate.  However, the consolidation causes
      /// subsequent pop_front() operations to be O(lg n).  Furthermore,
      /// the extra cost of the first pop_front() is covered by the higher
      /// amortized cost of insert(), which is the real governing factor in
      /// how costly the first pop_front() will be.
      void consolidate();

      /// The node \p lhs is removed from the root list and becomes a subtree
      /// of node \p rhs.
      void link(internal::fibonacci_heap_node<P,T> *lhs,
		internal::fibonacci_heap_node<P,T> *rhs);

      void add_to_root_list(internal::fibonacci_heap_node<P,T> *);

      /// Remove node \p lhs from the child list of its parent node \p rhs.
      void cut(internal::fibonacci_heap_node<P,T> *lhs,
	       internal::fibonacci_heap_node<P,T> *rhs);

      /// Cuts each node in parent list, putting successive ancestor nodes on
      /// the root list until we either arrive at the root list or until we
      /// find an ancestor that is unmarked.  When a mark is set (which only
      /// happens during a cascading cut), it means that one child subtree has
      /// been lost; if a second subtree is lost later during another
      /// cascading cut, then we move the node to the root list so that it
      /// can be re-balanced on the next consolidate.
      void cascading_cut(internal::fibonacci_heap_node<P,T> *);

      /// Clear the heap but do *NOT* delete elements.
      void soft_clear_();

      /// FIXME: Ugly but we need to be able to soft_clear() the heap in the
      /// copy constructor...  Any idea how to do it without that?
      mutable internal::fibonacci_heap_node<P,T> *min_root;
      mutable long num_nodes;
      mutable long num_trees;
      mutable long num_marked_nodes;

    };


    template <typename P, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const fibonacci_heap<P,T>& heap);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      /*---------------------.
      | fibonacci_heap_node. |
      `---------------------*/


      template <typename P, typename T>
      inline
      fibonacci_heap_node<P,T>::fibonacci_heap_node()
	: left_(0), right_(0), parent_(0), child_(0),
	  degree_(0), mark_(0), priority_(0)
      // FIXME: Don't we want to initialize priority with literal::zero?
      {
      }


      template <typename P, typename T>
      inline
      fibonacci_heap_node<P,T>::fibonacci_heap_node(const P& priority,
						    const T& new_value)
        : left_(0), right_(0), parent_(0), child_(0),
	  degree_(0), mark_(0), priority_(priority), value_(new_value)
      {
      }


      template <typename P, typename T>
      inline
      fibonacci_heap_node<P,T>::~fibonacci_heap_node()
      {
      }


      template <typename P, typename T>
      inline
      const T&
      fibonacci_heap_node<P,T>::value() const
      {
        return value_;
      }


      template <typename P, typename T>
      inline
      const P&
      fibonacci_heap_node<P,T>::priority() const
      {
	return priority_;
      }


      template <typename P, typename T>
      inline
      fibonacci_heap_node<P,T> *
      fibonacci_heap_node<P,T>::left() const
      {
        return left_;
      }


      template <typename P, typename T>
      inline
      fibonacci_heap_node<P,T> *
      fibonacci_heap_node<P,T>::right() const
      {
        return right_;
      }


      template <typename P, typename T>
      inline
      fibonacci_heap_node<P,T> *
      fibonacci_heap_node<P,T>::parent() const
      {
	return parent_;
      }


      template <typename P, typename T>
      inline
      fibonacci_heap_node<P,T> *
      fibonacci_heap_node<P,T>::child() const
      {
	return child_;
      }


      template <typename P, typename T>
      inline
      short
      fibonacci_heap_node<P,T>::degree() const
      {
	return degree_;
      }


      template <typename P, typename T>
      inline
      short
      fibonacci_heap_node<P,T>::mark() const
      {
	return mark_;
      }


      template <typename P, typename T>
      inline
      void
      fibonacci_heap_node<P,T>::set_value(const T& value)
      {
	value_ = value;
      }


      template <typename P, typename T>
      inline
      void
      fibonacci_heap_node<P,T>::set_left(fibonacci_heap_node<P,T> *node)
      {
	left_ = node;
      }


      template <typename P, typename T>
      inline
      void
      fibonacci_heap_node<P,T>::set_right(fibonacci_heap_node<P,T> *node)
      {
	right_ = node;
      }


      template <typename P, typename T>
      inline
      void
      fibonacci_heap_node<P,T>::set_parent(fibonacci_heap_node<P,T> *node)
      {
	parent_ = node;
      }


      template <typename P, typename T>
      inline
      void
      fibonacci_heap_node<P,T>::set_child(fibonacci_heap_node<P,T> *node)
      {
	child_ = node;
      }


      template <typename P, typename T>
      inline
      void
      fibonacci_heap_node<P,T>::set_degree(short degree)
      {
	degree_ = degree;
      }


      template <typename P, typename T>
      inline
      void
      fibonacci_heap_node<P,T>::set_mark(short mark)
      {
	mark_ = mark;
      }


      template <typename P, typename T>
      inline
      void fibonacci_heap_node<P,T>::operator=(fibonacci_heap_node<P,T>& rhs)
      {
	priority_ = rhs.priority();
	value_ = rhs.value();
      }


      template <typename P, typename T>
      inline
      bool
      fibonacci_heap_node<P,T>::operator==(fibonacci_heap_node<P,T>& rhs)
      {
	return priority_ == rhs.priority() && value_ == rhs.value();
      }


      template <typename P, typename T>
      inline
      bool
      fibonacci_heap_node<P,T>::operator<(fibonacci_heap_node<P,T>& rhs)
      {
	return util::ord_strict(priority_, rhs.priority())
	    || (priority_ == rhs.priority() && util::ord_strict(value_, rhs.value()));
      }


      template <typename P, typename T>
      inline
      void fibonacci_heap_node<P,T>::print_(std::ostream& ostr) const
      {
	ostr << value_ << " (" << priority_ << ")";
      }


    } // end of namespace mln::util::internal



    /*---------------\
    | fibonacci_heap |
    `---------------*/

    template <typename P, typename T>
    inline
    fibonacci_heap<P,T>::fibonacci_heap()
    {
      soft_clear_();
    }


    template <typename P, typename T>
    inline
    fibonacci_heap<P,T>::fibonacci_heap(const fibonacci_heap<P,T>& rhs)
      : Object< fibonacci_heap<P,T> >()
    {
      min_root = rhs.min_root;
      num_nodes = rhs.num_nodes;
      num_trees = rhs.num_trees;
      num_marked_nodes = rhs.num_marked_nodes;

//    rhs is const, we cannot call that method.
//    rhs.soft_clear_();
      rhs.min_root  = 0;
      rhs.num_nodes = 0;
      rhs.num_trees = 0;
      rhs.num_marked_nodes = 0;
    }


    template <typename P, typename T>
    inline
    fibonacci_heap<P,T>::~fibonacci_heap()
    {
      clear();
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::push(const P& priority, const T& value)
    {
      typedef internal::fibonacci_heap_node<P,T> node_t;
      node_t *new_node = new node_t(priority, value);

      insert(new_node);
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::push(fibonacci_heap<P,T>& other_heap)
    {
      if (other_heap.is_empty() || &other_heap == this)
	return;

      if (min_root != 0)
      {
	internal::fibonacci_heap_node<P,T> *min1, *min2, *next1, *next2;

	// We join the two circular lists by cutting each list between its
	// min node and the node after the min.  This code just pulls those
	// nodes into temporary variables so we don't get lost as changes
	// are made.
	min1 = min_root;
	min2 = other_heap.min_root;
	next1 = min1->right();
	next2 = min2->right();

	// To join the two circles, we join the minimum nodes to the next
	// nodes on the opposite chains.  Conceptually, it looks like the way
	// two bubbles join to form one larger bubble.  They meet at one point
	// of contact, then expand out to make the bigger circle.
	min1->set_right(next2);
	next2->set_left(min1);
	min2->set_right(next1);
	next1->set_left(min2);

	// Choose the new minimum for the heap.
	if (*min2 < *min1)
	  min_root = min2;
      }
      else
	min_root = other_heap.min_root;

      // Set the amortized analysis statistics and size of the new heap.
      num_nodes += other_heap.num_nodes;
      num_marked_nodes += other_heap.num_marked_nodes;
      num_trees += other_heap.num_trees;

      // Complete the union by setting the other heap to emptiness.
      other_heap.soft_clear_();

      mln_postcondition(other_heap.is_empty());
    }


    template <typename P, typename T>
    inline
    const T&
    fibonacci_heap<P,T>::front() const
    {
      return min_root->value();
    }


    template <typename P, typename T>
    inline
    T
    fibonacci_heap<P,T>::pop_front()
    {
      mln_precondition(is_valid());
      mln_precondition(!is_empty());

      internal::fibonacci_heap_node<P,T> *result = min_root;
      fibonacci_heap<P,T> *child_heap = 0;

      // Remove minimum node and set min_root to next node.
      min_root = result->right();
      result->right()->set_left(result->left());
      result->left()->set_right(result->right());
      result->set_left(0);
      result->set_right(0);

      --num_nodes;
      if (result->mark())
      {
	--num_marked_nodes;
	result->set_mark(0);
      }
      result->set_degree(0);

      // Attach child list of minimum node to the root list of the heap
      // If there is no child list, then do no work.
      if (result->child() == 0)
      {
	if (min_root == result)
	  min_root = 0;
      }

      // If min_root==result then there was only one root tree, so the
      // root list is simply the child list of that node (which is
      // 0 if this is the last node in the list).
      else if (min_root == result)
	min_root = result->child();

      // If min_root is different, then the child list is pushed into a
      // new temporary heap, which is then merged by union() onto the
      // root list of this heap.
      else
      {
	child_heap = new fibonacci_heap<P,T>();
	child_heap->min_root = result->child();
      }

      // Complete the disassociation of the result node from the heap.
      if (result->child() != 0)
	result->child()->set_parent(0);
      result->set_child(0);
      result->set_parent(0);

      // If there was a child list, then we now merge it with the
      // rest of the root list.
      if (child_heap)
      {
	push(*child_heap);
	delete child_heap;
      }

      // Consolidate heap to find new minimum and do reorganize work.
      if (min_root != 0)
	consolidate();

      // Return the minimum node, which is now disassociated with the heap
      // It has left, right, parent, child, mark and degree cleared.
      T val = result->value();
      delete result;

      return val;
    }


    template <typename P, typename T>
    inline
    int
    fibonacci_heap<P,T>::decrease_key(internal::fibonacci_heap_node<P,T> *node,
				      internal::fibonacci_heap_node<P,T>& key)
    {
      internal::fibonacci_heap_node<P,T> *parent;

      if (node == 0 || *node < key)
	return -1;

      *node = key;

      parent = node->parent();
      if (parent != 0 && *node < *parent)
      {
	cut(node, parent);
	cascading_cut(parent);
      }

      if (*node < *min_root)
	min_root = node;

      return 0;
    }


    template <typename P, typename T>
    inline
    int
    fibonacci_heap<P,T>::remove(internal::fibonacci_heap_node<P,T> *node)
    {
      internal::fibonacci_heap_node<P,T> temp;
      int result;

      if (node == 0)
	return -1;

      result = decrease_key(node, temp);

      if (result == 0)
	if (pop_front() == 0)
	  result = -1;

      if (result == 0)
	delete node;

      return result;
    }


    template <typename P, typename T>
    inline
    bool
    fibonacci_heap<P,T>::is_empty() const
    {
      return min_root == 0;
    }


    template <typename P, typename T>
    inline
    bool
    fibonacci_heap<P,T>::is_valid() const
    {
      return min_root != 0;
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::clear()
    {
      while (min_root != 0)
	pop_front();
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::soft_clear_()
    {
      min_root  = 0;
      num_nodes = 0;
      num_trees = 0;
      num_marked_nodes = 0;
    }


    template <typename P, typename T>
    inline
    unsigned
    fibonacci_heap<P,T>::nelements() const
    {
      return num_nodes;
    };


    template <typename P, typename T>
    inline
    fibonacci_heap<P,T>&
    fibonacci_heap<P,T>::operator=(fibonacci_heap<P,T>& rhs)
    {
      if (&rhs != this)
      {
        min_root = rhs.min_root;
        num_nodes = rhs.num_nodes;
        num_trees = rhs.num_trees;
	num_marked_nodes = rhs.num_marked_nodes;
	rhs.soft_clear_();
      }
      return *this;
    }


    template <typename P, typename T>
    std::ostream&
    fibonacci_heap<P,T>::print_(std::ostream& ostr,
				internal::fibonacci_heap_node<P,T> *tree,
				internal::fibonacci_heap_node<P,T> *parent) const
    {
      internal::fibonacci_heap_node<P,T>* temp = 0;

      if (tree == 0)
	tree = min_root;

      temp = tree;
      if (temp != 0)
      {
	do {
	  if (temp->left() == 0)
	    ostr << "(left is 0)";
	  temp->print_();
	  if (temp->parent() != parent)
	    ostr << "(parent is incorrect)";
	  if (temp->right() == 0)
	    ostr << "(right is 0)";
	  else if (temp->right()->left() != temp)
	    ostr << "(Error in left link left) ->";
	  else
	    ostr << " <-> ";

	  temp = temp->right();

	} while (temp != 0 && temp != tree);
      }
      else
	ostr << "  <empty>" << std::endl;
      ostr << std::endl;

      temp = tree;
      if (temp != 0)
      {
	do {
	  ostr << "children of " << temp->value() << ": ";
	  if (temp->child() == 0)
	    ostr << "NONE" << std::endl;
	  else print_(ostr, temp->child(), temp);
	  temp = temp->right();
	} while (temp!=0 && temp != tree);
      }

      return ostr;
    }


    template <typename P, typename T>
    inline
    void fibonacci_heap<P,T>::consolidate()
    {
      internal::fibonacci_heap_node<P,T> *x, *y, *w;
      internal::fibonacci_heap_node<P,T> *a[1 + 8 * sizeof (long)]; // 1+lg(n)
      short dn = 1 + 8 * sizeof (long);

      // Initialize the consolidation detection array.
      for (int i = 0; i < dn; ++i)
	a[i] = 0;

      // We need to loop through all elements on root list.
      // When a collision of degree is found, the two trees
      // are consolidated in favor of the one with the lesser
      // element key value.  We first need to break the circle
      // so that we can have a stopping condition (we can't go
      // around until we reach the tree we started with
      // because all root trees are subject to becoming a
      // child during the consolidation).
      min_root->left()->set_right(0);
      min_root->set_left(0);
      w = min_root;

      short d;
      do {
	x = w;
	d = x->degree();
	w = w->right();

	// We need another loop here because the consolidated result
	// may collide with another large tree on the root list.
	while (a[d] != 0)
	{
	  y = a[d];
	  if (*y < *x)
	    exchange(x, y);
	  if (w == y) w = y->right();
	  link(y, x);
	  a[d] = 0;
	  ++d;
	}
	a[d] = x;

      } while (w != 0);

      // Now we rebuild the root list, find the new minimum,
      // set all root list nodes' parent pointers to 0 and
      // count the number of subtrees.
      min_root = 0;
      num_trees = 0;
      for (int i = 0; i < dn; ++i)
	if (a[i] != 0)
	  add_to_root_list(a[i]);
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::link(internal::fibonacci_heap_node<P,T> *y,
			      internal::fibonacci_heap_node<P,T> *x)
    {
      // Remove node y from root list.
      if (y->right() != 0)
	y->right()->set_left(y->left());
      if (y->left() != 0)
	y->left()->set_right(y->right());
      --num_trees;

      // Make node y a singleton circular list with a parent of x.
      y->set_left(y);
      y->set_right(y);
      y->set_parent(x);

      // If node x has no children, then list y is its new child list.
      if (x->child() == 0)
	x->set_child(y);

      // Otherwise, node y must be added to node x's child list.
      else
      {
	y->set_left(x->child());
	y->set_right(x->child()->right());
	x->child()->set_right(y);
	y->right()->set_left(y);
      }

      // Increase the degree of node x because it's now a bigger tree.
      x->set_degree(x->degree() + 1);

      // Node y has just been made a child, so clear its mark.
      if (y->mark())
	--num_marked_nodes;
      y->set_mark(0);
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::add_to_root_list(internal::fibonacci_heap_node<P,T> *x)
    {
      if (x->mark())
	--num_marked_nodes;
      x->set_mark(0);

      --num_nodes;
      insert(x);
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::cut(internal::fibonacci_heap_node<P,T> *x,
			   internal::fibonacci_heap_node<P,T> *y)
    {
      if (y->child() == x)
	y->child() = x->right();
      if (y->child() == x)
	y->child() = 0;

      y->set_degree(y->degree() - 1);

      x->left()->right() = x->right();
      x->right()->left() = x->left();

      add_to_root_list(x);
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::cascading_cut(internal::fibonacci_heap_node<P,T> *y)
    {
      internal::fibonacci_heap_node<P,T> *z = y->parent();

      while (z != 0)
      {
	if (y->mark() == 0)
	{
	  y->mark() = 1;
	  ++num_marked_nodes;
	  z = 0;
	}
	else
	{
	  cut(y, z);
	  y = z;
	  z = y->parent();
	}
      }
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::insert(internal::fibonacci_heap_node<P,T> *node)
    {
      if (node == 0)
	return;

      // If the heap is currently empty, then new node becomes singleton
      // circular root list.
      if (min_root == 0)
      {
	min_root = node;
	node->set_left(node);
	node->set_right(node);
      }
      else
      {
	// Pointers from node set to insert between min_root and
	// min_root->right().
	node->set_right(min_root->right());
	node->set_left(min_root);

	// Set pointers to node.
	node->left()->set_right(node);
	node->right()->set_left(node);

	// The new node becomes new min_root if it is less than current
	// min_root.
	if (*node < *min_root)
	  min_root = node;
      }

      // We have one more node in the heap, and it is a tree on the root list.
      ++num_nodes;
      ++num_trees;
      node->set_parent(0);
    }


    template <typename P, typename T>
    inline
    void
    fibonacci_heap<P,T>::exchange(internal::fibonacci_heap_node<P,T>*& n1,
				  internal::fibonacci_heap_node<P,T>*& n2)
    {
      internal::fibonacci_heap_node<P,T> *temp;

      temp = n1;
      n1 = n2;
      n2 = temp;
    }


    template <typename P, typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const fibonacci_heap<P,T>& heap)
    {
      return heap.print_(ostr);
    }

# endif // ! MLN_INCLUDE_ONLY



  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_FIBONACCI_HEAP_HH
