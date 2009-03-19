/*
** We do NOT want to pass images as reference.
**
** Assigning an image to an image concept will compile but
** will not perform anything. This is ERROR PRONE.
**
** Thus, if the image passed as reference is not initialized it will remain
** uninitialized and an assertion will raised. The problem is if the image is
** already initialized. It will FAIL SILENTLY and give wrong image values.
**
** Always prefer returning images.
*/

#include <mln/essential/2d.hh>

namespace mln
{

  template <typename I>
  mln_ch_value(I,value::label_8)
  bar(const Image<I>& ima)
  {
    value::label_8 n;

    mln_ch_value(I,value::label_8) lbl;
    initialize(lbl, ima);

    lbl = labeling::blobs(ima, c8(),n);
    mln_assertion(lbl.is_valid());

    return lbl;
  }

  // DO NOT WORK!
  template <typename I>
  void foo_template_and_concept(Image<I>& lbl)
  {
    mln_ch_value(I,bool) ima(3,4);

    /// The difference is here!
    lbl = bar(ima);

    mln_assertion(exact(lbl).is_valid());
  }

  // WORK
  template <typename I>
  void foo_template_concept_and_exact(Image<I>& lbl)
  {
    mln_ch_value(I,bool) ima(3,4);

    /// The difference is here!
    exact(lbl) = bar(ima);

    mln_assertion(exact(lbl).is_valid());
  }

}



int main(int, char*argv[])
{
  using namespace mln;

  image2d<bool> ima(3,4);

  // WORK
  {
    std::cout << "template with I + concept + exact" << std::endl;
    image2d<value::label_8> lbl;
    foo_template_concept_and_exact(lbl);
    mln_assertion(lbl.is_valid());
  }

  // DO NOT WORK
  {
    std::cout << "template with I + concept" << std::endl;
    image2d<value::label_8> lbl;
    foo_template_and_concept(lbl);
    mln_assertion(lbl.is_valid());
  }

}
