#include <mln/geom/ncols.hh>
#include <mln/geom/nrows.hh>
#include <mln/util/couple.hh>
#include <scribo/core/component_set.hh>
#include <scribo/core/macros.hh>

namespace scribo
{

  namespace primitive
  {

    namespace group
    {

      template <typename L>
      object_groups<L>
      regroup_left(const component_set<L>& components,
		   const object_groups<L>& groups,
		   unsigned dmax)
      {
	trace::entering("scribo::primitive::group::regroup_left");

	mln_precondition(groups.is_valid());

	object_groups<L>
	  new_groups(components, static_cast<unsigned>(components.nlabels()) + 1, 0);

	unsigned ngroups = 0;
	for_all_comps(i, components)
	{
	  if (groups[i] == 0)
	    continue;

	  // We MUST set a group id here since the most left group
	  // component won't have any id otherwise.
	  if (new_groups[i] == 0)
	    new_groups[i] = ++ngroups;

	  const mln_site(L) c = components.mass_center(i);

	  int
	    midcol = (components.bbox(i).pmax().col()
		      - components.bbox(i).pmin().col()) / 2;
	  int
	    nleftima = geom::ncols(components),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Left
	  {
	    const mln_value(L)
	      *p = &components(c),
	      *pstop = p - nleft - 1;

	    for (; p != pstop; --p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
//		  && new_groups[*p] != ngroups
		  && groups[*p] != 0)
	      {
		if (new_groups[*p] == 0)
		  new_groups[*p] = ngroups;
		else
		{
		  new_groups[i] = new_groups[*p];
		  --ngroups;
		}

		break;
	      }
	    }
	  }
	}

	return new_groups;
      }

    } // end of namespace scribo::primitive::group

  } // end of namespace scribo::primitive

} // end of namespace scribo
