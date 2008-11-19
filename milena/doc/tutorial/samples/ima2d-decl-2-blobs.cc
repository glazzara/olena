int_u8 nlabels;
image2d<int_u8> lbl = labeling::blobs(ima, c4(), nlabels);
