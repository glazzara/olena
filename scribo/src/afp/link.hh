#include <mln/geom/ncols.hh>
#include <mln/geom/nrows.hh>
#include <mln/util/couple.hh>
#include <scribo/core/component_set.hh>
#include <scribo/core/macros.hh>
#include <scribo/primitive/internal/init_link_array.hh>

namespace scribo
{

  namespace primitive
  {

    namespace link
    {


      template <typename L>
      util::couple<object_links<L>, object_links<L> >
      left_right(const component_set<L>& components)
      {
	object_links<L>
	  right(components, static_cast<unsigned>(components.nelements()) + 1);
	primitive::internal::init_link_array(right);

	object_links<L>
	  left(components, static_cast<unsigned>(components.nelements()) + 1);
	primitive::internal::init_link_array(left);

	const L& lbl_ima = components.labeled_image();

	for_all_comps(i, components)
	{
	  float
	    w = (components(i).bbox().pmax().col()
		 - components(i).bbox().pmin().col()),
	    h = (components(i).bbox().pmax().row()
		 - components(i).bbox().pmin().row());
	  unsigned dmax = (w / 2.0f) + (3 * math::max(w, h));


	  const mln_site(L) c = components(i).mass_center();

	  int
	    midcol = (components(i).bbox().pmax().col()
		      - components(i).bbox().pmin().col()) / 2;
	  int
	    nrightima = geom::ncols(lbl_ima) - c.col(),
	    nleftima = c.col(),
	    nright = std::min(static_cast<unsigned>(nrightima), midcol + dmax),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Right
	  {
	    const mln_value(L)
	      *p = &lbl_ima(c),
	      *pstop = p + nright + 1;

	    for (; p != pstop; ++p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
		  && right[*p] != i) // No loops
	      {
		right[i] = *p;
		break;
	      }
	    }
	  }


	  // Left
	  {
	    const mln_value(L)
	      *p = &lbl_ima(c),
	      *pstop = p - nleft - 1;

	    for (; p != pstop; --p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
		  && left[*p] != i) // No loops
	      {
		left[i] = *p;
		break;
	      }
	    }
	  }
	}

	return mln::make::couple(left, right);
      }


      template <typename L>
      object_links<L>
      left(const component_set<L>& components, unsigned dmax)
      {
	object_links<L>
	  left(components, static_cast<unsigned>(components.nelements()) + 1);
	primitive::internal::init_link_array(left);

	const L& lbl_ima = components.labeled_image();

	for_all_comps(i, components)
	{
	  const mln_site(L) c = components(i).mass_center();

	  int
	    midcol = (components(i).bbox().pmax().col()
		      - components(i).bbox().pmin().col()) / 2;
	  int
	    nleftima = c.col(),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Left
	  {
	    const mln_value(L)
	      *p = &lbl_ima(c),
	      *pstop = p - nleft - 1;

	    for (; p != pstop; --p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
		  && left[*p] != i) // No loops
	      {
		left[i] = *p;
		break;
	      }
	    }
	  }
	}

	return left;
      }

    } // end of namespace scribo::primitive::link

  } // end of namespace scribo::primitive

} // end of namespace scribo
