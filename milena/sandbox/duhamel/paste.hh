#include "mem.hh"

# include <mln/core/concept/image.hh>
# include <mln/core/concept/fast_image.hh>
# include <mln/core/sparse_image.hh>
# include <mln/core/sparse_encode.hh>
# include <mln/core/rle_image.hh>
# include <mln/core/rle_encode.hh>

namespace mln
{
  namespace level
  {

    template <typename I>
    void paste(const I& data_, I& destination_)
    {
      imacpy_ (destination_, data_, destination_.ncells());
    }

    template <typename I, typename J>
    void paste(const sparse_image<I, J>& data_, sparse_image<I, J>& destination_)
    {
      imacpy_ (destination_, data_, data_.size ());
    }

    template <typename I, typename J>
    void paste(const rle_image<I, J>& data_, rle_image<I, J>& destination_)
    {
      // TO FIX
      imacpy_ (destination_, data_, 42);
    }


  }
}
