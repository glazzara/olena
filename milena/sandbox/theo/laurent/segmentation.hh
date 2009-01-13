

template <typename I, typename N>
mln_ch_value(I, rgb8) segmentation(const I& ima,
				   const N& nbh,
				   unsigned area)
{
  mln_concrete(I) filtered = morpho::closing_area(ima, nbh, area);

  unsigned nbasins;
  mln_ch_value(I, unsigned) wst = morpho::wst_meyer(filtered, nbh, nbasins);

  return level::transform(wst, colorize(nbasins));
}
