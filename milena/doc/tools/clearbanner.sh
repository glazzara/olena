#!/bin/sh

sed -i -e 's,<HR SIZE=2><BLOCKQUOTE CLASS="quote"><EM>This document was translated from L<sup>A</sup>T<sub>E</sub>X by,,g' $1
sed -i -e 's,<A HREF="http://hevea.inria.fr/index.html">H<FONT SIZE=2><sup>E</sup></FONT>V<FONT SIZE=2><sup>E</sup></FONT>A</A>.</EM></BLOCKQUOTE></BODY>,,g' $1

