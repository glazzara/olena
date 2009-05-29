
#include <mln/core/concept/function.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/data/sort_psites.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>



namespace mln
{

  namespace internal
  {

    template <typename T>
    struct node_pred : Function_p2b< node_pred<T> >
    {
      typedef bool result;
      
      template <typename P>
      bool operator()(const P& p) const
      {
	return t->is_a_node(p);
      }
      
      const T* t;
    };
  
    template <typename T, typename I, typename M>
    mln_value(I)  rec(const T& t, // tree
		      I& a,       // attribute image
		      M& mark,
		      const mln_psite(I)& p)
    {
      mln_invariant(mark(p) == false);
      mark(p) = true;
      if (t.parent(p) == p || mark(t.parent(p)) == true) // Stop.
	return a(p);
      return a(p) = rec(t, a, mark, t.parent(p));
    }

  } // internal


  template <typename T, typename A>
  void
  extinct_attributes(const T& t, // Tree.
		     A& a) // Attribute image.
  {
    mln_ch_value(A, bool) mark;
    initialize(mark, a);
    data::fill(mark, false);
    
    internal::node_pred<T> node_only;
    node_only.t = &t;

    typedef p_array<mln_site(A)> S;
    S s = data::sort_psites_increasing(a | node_only);

    mln_fwd_piter(S) p(s);
    for_all(p)
      {
	if (mark(p) == true)
	  continue;
	internal::rec(t, a, mark, p);
      }
  }

} // end of namespace mln
