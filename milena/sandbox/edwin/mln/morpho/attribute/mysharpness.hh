#include <mln/core/routine/initialize.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/volume.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/math/diff_abs.hh>

namespace mln {
  namespace morpho {
    namespace attribute {

      template <typename T>
      mln_ch_value(typename T::function, double)
	mysharpness(const T& tree,
		    unsigned lambda)
      {
	typedef typename T::function F;

	typedef morpho::attribute::volume<F> v_attr;
	typedef mln_ch_value(F, v_attr) V;

	F base = duplicate(tree.f());
	V v_accu_img;
	morpho::tree::compute_attribute_image(v_attr (), tree, &v_accu_img);

	typedef mln_ch_value(F, double) S;
	S s_img;
	initialize(s_img, tree.f());


	unsigned h, delta;
	mln_up_node_piter(T) n(tree);
	for_all(n)
	{
	  mln_psite(T) p = tree.parent(n);

	  if (math::diff_abs(tree.f(p), base(p)) < math::diff_abs(tree.f(p), base(n)))
	    base(p) = base(n);

	  h = math::diff_abs(tree.f(p), base(n));

	  if (h < lambda)
	    {
	      s_img(n) = 0;
	      continue;
	    }

	  delta = math::diff_abs(tree.f(p), tree.f(n));
	  s_img(n) = (double) v_accu_img(n).to_result() / (double) (v_accu_img(n).area() * (h + 1))
	    + (double) (delta) / (double) (h + 1);
	}

	return s_img;
      }
    }
  }
}

