    typedef value::label_8 V;
    V nlabels;
    mln_ch_value(I,V) lbl = labeling::blobs(ima, nbh, nlabels);
    util::array<unsigned>
      count = labeling::compute(accu::meta::count(), lbl, nlabels);
