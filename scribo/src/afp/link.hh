#include <mln/geom/ncols.hh>
#include <mln/geom/nrows.hh>
#include <mln/util/couple.hh>
#include <scribo/core/object_image.hh>
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
      left_right(const object_image(L)& objects)
      {
	object_links<L>
	  right(objects, static_cast<unsigned>(objects.nlabels()) + 1);
	primitive::internal::init_link_array(right);

	object_links<L>
	  left(objects, static_cast<unsigned>(objects.nlabels()) + 1);
	primitive::internal::init_link_array(left);

	for_all_components(i, objects.bboxes())
	{
	  float
	    w = (objects.bbox(i).pmax().col()
		 - objects.bbox(i).pmin().col()),
	    h = (objects.bbox(i).pmax().row()
		 - objects.bbox(i).pmin().row());
	  unsigned dmax = (w / 2.0f) + (3 * math::max(w, h));


	  const mln_site(L) c = objects.mass_center(i);

	  int
	    midcol = (objects.bbox(i).pmax().col()
		      - objects.bbox(i).pmin().col()) / 2;
	  int
	    nrightima = geom::ncols(objects) - c.col(),
	    nleftima = c.col(),
	    nright = std::min(static_cast<unsigned>(nrightima), midcol + dmax),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Right
	  {
	    const mln_value(L)
	      *p = &objects(c),
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
	      *p = &objects(c),
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
      left(const object_image(L)& objects, unsigned dmax)
      {
	object_links<L>
	  left(objects, static_cast<unsigned>(objects.nlabels()) + 1);
	primitive::internal::init_link_array(left);

	for_all_components(i, objects.bboxes())
	{
	  const mln_site(L) c = objects.mass_center(i);

	  int
	    midcol = (objects.bbox(i).pmax().col()
		      - objects.bbox(i).pmin().col()) / 2;
	  int
	    nleftima = c.col(),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Left
	  {
	    const mln_value(L)
	      *p = &objects(c),
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
