p_array2d<bool> arr;

// We add two points in the array.
arr.append(point2d(0, 1));
arr.append(point2d(4, 0));

// We restrict the image to the sites
// contained in arr and fill these ones
// with 0.
// We must call "rw()" here.
fill((ima | arr).rw(), 0);


mln_VAR(ima2, ima | arr);
// We do not need to call "rw()" here.
fill(ima2, 0);
