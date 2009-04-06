include makefile.rules
TARGET=test
sharp_SRC=sharp.cc
sharp_OBJS=${sharp_SRC:.cc=.o}

test_SRC=test.cc
test_OBJS=${test_SRC:.cc=.o}

OLENADIR=$(MLN_DIR)/..
MILENADIR=$(OLENADIR)/milena

CXXFLAGS=-I$(MILENADIR) -I./ -W -Wall


CXXFLAGS += $(if $(DEBUG), -g -ggdb, -DNDEBUG\
	$(if $(RELEASE), -O3, -O1))

CXX=g++
LD=g++
LDFLAGS=

all: $(TARGET)


$(TARGET): $($(TARGET)_OBJS) $($(TARGET)_SRC)
	$(LD) $(LDFLAGS) -o $@ $($(TARGET)_OBJS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.hh
	$(CXX) $(CXXFLAGS) -c $<

depend:
	makedepend -- $(CXXFLAGS) -- -v $($(TARGET)_SRC)

clean:
	rm -f *.o $(TARGET)
	#rm -f *.pbm
	#find  -name "*.pgm" \! -regex ".*/affiche2?.pgm" -delete
# DO NOT DELETE

test.o: /work/carlinet/trunk/milena/mln/core/image/image2d.hh
# /work/carlinet/trunk/milena/mln/core/image/image2d.hh includes:
#	mln/core/internal/image_primary.hh
#	mln/core/internal/fixme.hh
#	mln/core/alias/box2d.hh
#	mln/core/routine/init.hh
#	mln/border/thickness.hh
#	mln/value/set.hh
#	mln/fun/i2v/all_to.hh
#	mln/core/trait/pixter.hh
#	mln/core/dpoints_pixter.hh
#	mln/core/pixter2d.hh
#	mln/make/image.hh
#	mln/make/image2d.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/image_primary.hh
# /work/carlinet/trunk/milena/mln/core/internal/image_primary.hh includes:
#	mln/core/internal/image_base.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/image_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/image_base.hh includes:
#	mln/core/concept/image.hh
#	mln/core/grids.hh
#	mln/core/trait/qlf_value.hh
#	mln/core/internal/check/image_all.hh
#	mln/core/internal/data.hh
#	mln/core/internal/morpher_lvalue.hh
#	mln/util/tracked_ptr.hh
#	mln/value/set.hh
#	mln/value/super_value.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/image.hh
# /work/carlinet/trunk/milena/mln/core/concept/image.hh includes:
#	mln/core/concept/site_set.hh
#	mln/core/concept/mesh.hh
#	mln/core/trait/all.hh
#	mln/core/macros.hh
#	mln/core/site_set/box.hh
#	mln/trait/concrete.hh
#	mln/trait/images.hh
#	mln/metal/is_a.hh
#	mln/metal/equal.hh
#	mln/tag/init.hh
#	mln/core/routine/initialize.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/site_set.hh
# /work/carlinet/trunk/milena/mln/core/concept/site_set.hh includes:
#	mln/core/concept/site_iterator.hh
#	mln/trait/site_sets.hh
#	mln/metal/not_equal.hh
#	mln/metal/is_a.hh
#	mln/metal/is_unqualif.hh
#	mln/util/ord.hh
#	mln/core/site_set/operators.hh
#	mln/core/routine/ops.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/site_iterator.hh
# /work/carlinet/trunk/milena/mln/core/concept/site_iterator.hh includes:
#	mln/core/concept/site_proxy.hh
#	mln/core/concept/iterator.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/site_proxy.hh
# /work/carlinet/trunk/milena/mln/core/concept/site_proxy.hh includes:
#	mln/core/concept/proxy.hh
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/proxy.hh
# /work/carlinet/trunk/milena/mln/core/concept/proxy.hh includes:
#	mln/core/concept/object.hh
#	mln/value/ops.hh
#	mln/convert/from_to.hxx
#	mln/core/concept/proxy.hxx
test.o: /work/carlinet/trunk/milena/mln/core/concept/object.hh
# /work/carlinet/trunk/milena/mln/core/concept/object.hh includes:
#	mln/core/macros.hh
#	mln/core/category.hh
#	mln/core/contract.hh
#	mln/core/internal/fixme.hh
#	mln/trace/all.hh
#	mln/metal/abort.hh
#	mln/metal/is_a.hh
#	mln/metal/is_not_a.hh
#	mln/metal/is.hh
#	mln/metal/is_not.hh
#	mln/metal/equal.hh
#	mln/metal/not_equal.hh
#	mln/metal/converts_to.hh
#	mln/metal/ret.hh
#	mln/metal/unqualif.hh
#	mln/metal/math/all.hh
#	mln/core/routine/exact.hh
#	mln/core/routine/ops.hh
test.o: /work/carlinet/trunk/milena/mln/core/macros.hh
test.o: /work/carlinet/trunk/milena/mln/core/category.hh
# /work/carlinet/trunk/milena/mln/core/category.hh includes:
#	mln/metal/equal.hh
test.o: /work/carlinet/trunk/milena/mln/metal/equal.hh
# /work/carlinet/trunk/milena/mln/metal/equal.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/metal/bool.hh
# /work/carlinet/trunk/milena/mln/metal/bool.hh includes:
#	mln/metal/bexpr.hh
test.o: /work/carlinet/trunk/milena/mln/metal/bexpr.hh
# /work/carlinet/trunk/milena/mln/metal/bexpr.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/core/contract.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/fixme.hh
test.o: /work/carlinet/trunk/milena/mln/trace/all.hh
# /work/carlinet/trunk/milena/mln/trace/all.hh includes:
#	mln/trace/quiet.hh
#	mln/trace/entering.hh
#	mln/trace/exiting.hh
#	mln/trace/stop.hh
#	mln/trace/resume.hh
#	mln/trace/warning.hh
test.o: /work/carlinet/trunk/milena/mln/trace/quiet.hh
# /work/carlinet/trunk/milena/mln/trace/quiet.hh includes:
#	sys/time.h
test.o: /usr/include/sys/time.h
# /usr/include/sys/time.h includes:
#	features.h
#	bits/types.h
#	time.h
#	bits/time.h
#	sys/select.h
test.o: /usr/include/features.h
# /usr/include/features.h includes:
#	sys/cdefs.h
#	gnu/stubs.h
test.o: /usr/include/sys/cdefs.h
# /usr/include/sys/cdefs.h includes:
#	bits/wordsize.h
test.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
# /usr/include/gnu/stubs.h includes:
#	bits/wordsize.h
#	gnu/stubs-32.h
test.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
# /usr/include/bits/types.h includes:
#	features.h
#	bits/wordsize.h
#	bits/typesizes.h
test.o: /usr/include/bits/typesizes.h /usr/include/time.h
# /usr/include/time.h includes:
#	bits/types.h
test.o: /usr/include/bits/time.h
# /usr/include/bits/time.h includes:
#	bits/types.h
test.o: /usr/include/sys/select.h
# /usr/include/sys/select.h includes:
#	features.h
#	bits/types.h
#	bits/select.h
#	bits/sigset.h
#	time.h
#	bits/time.h
test.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
test.o: /work/carlinet/trunk/milena/mln/trace/entering.hh
# /work/carlinet/trunk/milena/mln/trace/entering.hh includes:
#	mln/trace/quiet.hh
test.o: /work/carlinet/trunk/milena/mln/trace/exiting.hh
# /work/carlinet/trunk/milena/mln/trace/exiting.hh includes:
#	mln/core/contract.hh
#	mln/trace/quiet.hh
test.o: /work/carlinet/trunk/milena/mln/trace/stop.hh
# /work/carlinet/trunk/milena/mln/trace/stop.hh includes:
#	mln/trace/quiet.hh
test.o: /work/carlinet/trunk/milena/mln/trace/resume.hh
# /work/carlinet/trunk/milena/mln/trace/resume.hh includes:
#	mln/trace/quiet.hh
test.o: /work/carlinet/trunk/milena/mln/trace/warning.hh
# /work/carlinet/trunk/milena/mln/trace/warning.hh includes:
#	mln/trace/quiet.hh
test.o: /work/carlinet/trunk/milena/mln/metal/abort.hh
# /work/carlinet/trunk/milena/mln/metal/abort.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is_a.hh
# /work/carlinet/trunk/milena/mln/metal/is_a.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is_not_a.hh
# /work/carlinet/trunk/milena/mln/metal/is_not_a.hh includes:
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is.hh
# /work/carlinet/trunk/milena/mln/metal/is.hh includes:
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is_not.hh
# /work/carlinet/trunk/milena/mln/metal/is_not.hh includes:
#	mln/metal/is.hh
test.o: /work/carlinet/trunk/milena/mln/metal/not_equal.hh
# /work/carlinet/trunk/milena/mln/metal/not_equal.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/metal/converts_to.hh
# /work/carlinet/trunk/milena/mln/metal/converts_to.hh includes:
#	mln/metal/is_a.hh
#	mln/metal/const.hh
test.o: /work/carlinet/trunk/milena/mln/metal/const.hh
test.o: /work/carlinet/trunk/milena/mln/metal/ret.hh
test.o: /work/carlinet/trunk/milena/mln/metal/unqualif.hh
# /work/carlinet/trunk/milena/mln/metal/unqualif.hh includes:
#	mln/metal/unconst.hh
#	mln/metal/unref.hh
test.o: /work/carlinet/trunk/milena/mln/metal/unconst.hh
test.o: /work/carlinet/trunk/milena/mln/metal/unref.hh
test.o: /work/carlinet/trunk/milena/mln/metal/math/all.hh
# /work/carlinet/trunk/milena/mln/metal/math/all.hh includes:
#	mln/metal/math/pow.hh
#	mln/metal/math/root.hh
#	mln/metal/math/sqrt.hh
#	mln/metal/math/max.hh
test.o: /work/carlinet/trunk/milena/mln/metal/math/pow.hh
# /work/carlinet/trunk/milena/mln/metal/math/pow.hh includes:
#	mln/metal/bool.hh
#	mln/metal/int.hh
test.o: /work/carlinet/trunk/milena/mln/metal/int.hh
test.o: /work/carlinet/trunk/milena/mln/metal/math/root.hh
# /work/carlinet/trunk/milena/mln/metal/math/root.hh includes:
#	mln/metal/math/pow.hh
test.o: /work/carlinet/trunk/milena/mln/metal/math/sqrt.hh
# /work/carlinet/trunk/milena/mln/metal/math/sqrt.hh includes:
#	mln/metal/bool.hh
#	mln/metal/int.hh
test.o: /work/carlinet/trunk/milena/mln/metal/math/max.hh
# /work/carlinet/trunk/milena/mln/metal/math/max.hh includes:
#	mln/metal/bool.hh
#	mln/metal/int.hh
test.o: /work/carlinet/trunk/milena/mln/core/routine/exact.hh
# /work/carlinet/trunk/milena/mln/core/routine/exact.hh includes:
#	mln/core/internal/exact.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/exact.hh
# /work/carlinet/trunk/milena/mln/core/internal/exact.hh includes:
#	mln/core/concept/object.hh
test.o: /work/carlinet/trunk/milena/mln/core/routine/ops.hh
# /work/carlinet/trunk/milena/mln/core/routine/ops.hh includes:
#	mln/trait/op/all.hh
#	mln/core/concept/object.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/all.hh
# /work/carlinet/trunk/milena/mln/trait/op/all.hh includes:
#	mln/trait/op/plus.hh
#	mln/trait/op/minus.hh
#	mln/trait/op/times.hh
#	mln/trait/op/div.hh
#	mln/trait/op/mod.hh
#	mln/trait/op/uplus.hh
#	mln/trait/op/uminus.hh
#	mln/trait/op/preinc.hh
#	mln/trait/op/postinc.hh
#	mln/trait/op/predec.hh
#	mln/trait/op/postdec.hh
#	mln/trait/op/eq.hh
#	mln/trait/op/neq.hh
#	mln/trait/op/less.hh
#	mln/trait/op/leq.hh
#	mln/trait/op/geq.hh
#	mln/trait/op/greater.hh
#	mln/trait/op/and.hh
#	mln/trait/op/or.hh
#	mln/trait/op/xor.hh
#	mln/trait/op/lor.hh
#	mln/trait/op/not.hh
#	mln/trait/op/ord.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/plus.hh
# /work/carlinet/trunk/milena/mln/trait/op/plus.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/promote.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/decl.hh
test.o: /work/carlinet/trunk/milena/mln/trait/promote.hh
# /work/carlinet/trunk/milena/mln/trait/promote.hh includes:
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/solve.hh
# /work/carlinet/trunk/milena/mln/trait/solve.hh includes:
#	mln/core/category.hh
#	mln/metal/equal.hh
#	mln/metal/if.hh
#	mln/metal/ret.hh
#	mln/trait/solve_unary.hh
#	mln/trait/solve_binary.hh
test.o: /work/carlinet/trunk/milena/mln/metal/if.hh
# /work/carlinet/trunk/milena/mln/metal/if.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/trait/solve_unary.hh
# /work/carlinet/trunk/milena/mln/trait/solve_unary.hh includes:
#	mln/core/category.hh
#	mln/core/routine/exact.hh
#	mln/metal/equal.hh
#	mln/metal/if.hh
#	mln/metal/ret.hh
test.o: /work/carlinet/trunk/milena/mln/trait/solve_binary.hh
# /work/carlinet/trunk/milena/mln/trait/solve_binary.hh includes:
#	mln/core/category.hh
#	mln/core/routine/exact.hh
#	mln/metal/equal.hh
#	mln/metal/if.hh
#	mln/metal/ret.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/minus.hh
# /work/carlinet/trunk/milena/mln/trait/op/minus.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/promote.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/times.hh
# /work/carlinet/trunk/milena/mln/trait/op/times.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/div.hh
# /work/carlinet/trunk/milena/mln/trait/op/div.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/promote.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/mod.hh
# /work/carlinet/trunk/milena/mln/trait/op/mod.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/promote.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/uplus.hh
# /work/carlinet/trunk/milena/mln/trait/op/uplus.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/uminus.hh
# /work/carlinet/trunk/milena/mln/trait/op/uminus.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/preinc.hh
# /work/carlinet/trunk/milena/mln/trait/op/preinc.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/postinc.hh
# /work/carlinet/trunk/milena/mln/trait/op/postinc.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/predec.hh
# /work/carlinet/trunk/milena/mln/trait/op/predec.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/postdec.hh
# /work/carlinet/trunk/milena/mln/trait/op/postdec.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/eq.hh
# /work/carlinet/trunk/milena/mln/trait/op/eq.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/neq.hh
# /work/carlinet/trunk/milena/mln/trait/op/neq.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/less.hh
# /work/carlinet/trunk/milena/mln/trait/op/less.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/leq.hh
# /work/carlinet/trunk/milena/mln/trait/op/leq.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/geq.hh
# /work/carlinet/trunk/milena/mln/trait/op/geq.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/greater.hh
# /work/carlinet/trunk/milena/mln/trait/op/greater.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/and.hh
# /work/carlinet/trunk/milena/mln/trait/op/and.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/or.hh
# /work/carlinet/trunk/milena/mln/trait/op/or.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/xor.hh
# /work/carlinet/trunk/milena/mln/trait/op/xor.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/lor.hh
# /work/carlinet/trunk/milena/mln/trait/op/lor.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/not.hh
# /work/carlinet/trunk/milena/mln/trait/op/not.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
test.o: /work/carlinet/trunk/milena/mln/trait/op/ord.hh
# /work/carlinet/trunk/milena/mln/trait/op/ord.hh includes:
#	mln/trait/op/decl.hh
#	mln/trait/solve.hh
#	mln/util/ord.hh
test.o: /work/carlinet/trunk/milena/mln/util/ord.hh
# /work/carlinet/trunk/milena/mln/util/ord.hh includes:
#	mln/core/concept/object.hh
#	mln/trait/op/ord.hh
test.o: /work/carlinet/trunk/milena/mln/value/ops.hh
# /work/carlinet/trunk/milena/mln/value/ops.hh includes:
#	mln/trait/op/all.hh
#	mln/value/builtin/all.hh
#	mln/value/concept/all.hh
#	mln/value/equiv.hh
#	mln/trait/value_.hh
#	mln/literal/zero.hh
#	mln/literal/one.hh
#	mln/literal/ops.hh
#	mln/metal/ret.hh
test.o: /work/carlinet/trunk/milena/mln/value/builtin/all.hh
# /work/carlinet/trunk/milena/mln/value/builtin/all.hh includes:
#	mln/value/builtin/carrays.hh
#	mln/value/builtin/integers.hh
#	mln/value/builtin/floatings.hh
#	mln/value/builtin/symbolics.hh
#	mln/value/builtin/promotions.hh
#	mln/value/builtin/ops.hh
test.o: /work/carlinet/trunk/milena/mln/value/builtin/carrays.hh
# /work/carlinet/trunk/milena/mln/value/builtin/carrays.hh includes:
#	mln/value/concept/built_in.hh
#	mln/value/concept/vectorial.hh
#	mln/trait/value_.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/built_in.hh
# /work/carlinet/trunk/milena/mln/value/concept/built_in.hh includes:
#	mln/core/category.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/vectorial.hh
# /work/carlinet/trunk/milena/mln/value/concept/vectorial.hh includes:
#	mln/core/concept/value.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/value.hh
# /work/carlinet/trunk/milena/mln/core/concept/value.hh includes:
#	mln/core/concept/object.hh
#	mln/trait/value_.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value_.hh
# /work/carlinet/trunk/milena/mln/trait/value_.hh includes:
#	mln/metal/int.hh
#	mln/metal/math/pow.hh
#	mln/metal/if.hh
#	mln/trait/value/all.hh
#	mln/core/def/low_quant_nbits.hh
#	mln/trait/value/print.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value/all.hh
# /work/carlinet/trunk/milena/mln/trait/value/all.hh includes:
#	mln/trait/undef.hh
#	mln/trait/value/nature.hh
#	mln/trait/value/kind.hh
#	mln/trait/value/quant.hh
#	mln/trait/value/internal/all.hh
test.o: /work/carlinet/trunk/milena/mln/trait/undef.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value/nature.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value/kind.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value/quant.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value/internal/all.hh
# /work/carlinet/trunk/milena/mln/trait/value/internal/all.hh includes:
#	mln/trait/value/internal/comp.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value/internal/comp.hh
# /work/carlinet/trunk/milena/mln/trait/value/internal/comp.hh includes:
#	mln/metal/bool.hh
#	mln/metal/if.hh
test.o: /work/carlinet/trunk/milena/mln/core/def/low_quant_nbits.hh
test.o: /work/carlinet/trunk/milena/mln/trait/value/print.hh
# /work/carlinet/trunk/milena/mln/trait/value/print.hh includes:
#	mln/trait/value_.hh
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/value/builtin/integers.hh
# /work/carlinet/trunk/milena/mln/value/builtin/integers.hh includes:
#	mln/value/internal/limits.hh
#	mln/value/concept/built_in.hh
#	mln/value/concept/integer.hh
#	mln/trait/value_.hh
#	mln/metal/int.hh
#	mln/metal/if.hh
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/value/internal/limits.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/integer.hh
# /work/carlinet/trunk/milena/mln/value/concept/integer.hh includes:
#	mln/value/concept/scalar.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/scalar.hh
# /work/carlinet/trunk/milena/mln/value/concept/scalar.hh includes:
#	mln/core/concept/value.hh
#	mln/literal/one.hh
test.o: /work/carlinet/trunk/milena/mln/literal/one.hh
# /work/carlinet/trunk/milena/mln/literal/one.hh includes:
#	mln/core/concept/literal.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/literal.hh
# /work/carlinet/trunk/milena/mln/core/concept/literal.hh includes:
#	mln/core/concept/object.hh
test.o: /work/carlinet/trunk/milena/mln/value/builtin/floatings.hh
# /work/carlinet/trunk/milena/mln/value/builtin/floatings.hh includes:
#	mln/value/internal/limits.hh
#	mln/value/concept/built_in.hh
#	mln/value/concept/floating.hh
#	mln/trait/value_.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/floating.hh
# /work/carlinet/trunk/milena/mln/value/concept/floating.hh includes:
#	mln/value/concept/scalar.hh
test.o: /work/carlinet/trunk/milena/mln/value/builtin/symbolics.hh
# /work/carlinet/trunk/milena/mln/value/builtin/symbolics.hh includes:
#	mln/value/concept/built_in.hh
#	mln/value/concept/symbolic.hh
#	mln/trait/value_.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/symbolic.hh
# /work/carlinet/trunk/milena/mln/value/concept/symbolic.hh includes:
#	mln/core/concept/value.hh
test.o: /work/carlinet/trunk/milena/mln/value/builtin/promotions.hh
# /work/carlinet/trunk/milena/mln/value/builtin/promotions.hh includes:
#	mln/trait/promote.hh
#	mln/metal/ret.hh
test.o: /work/carlinet/trunk/milena/mln/value/builtin/ops.hh
# /work/carlinet/trunk/milena/mln/value/builtin/ops.hh includes:
#	mln/value/scalar.hh
#	mln/trait/op/all.hh
#	mln/value/builtin/promotions.hh
test.o: /work/carlinet/trunk/milena/mln/value/scalar.hh
# /work/carlinet/trunk/milena/mln/value/scalar.hh includes:
#	mln/value/concept/scalar.hh
#	mln/metal/is_a.hh
#	mln/metal/if.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/all.hh
# /work/carlinet/trunk/milena/mln/value/concept/all.hh includes:
#	mln/value/concept/built_in.hh
#	mln/value/concept/data.hh
#	mln/value/concept/floating.hh
#	mln/value/concept/integer.hh
#	mln/value/concept/scalar.hh
#	mln/value/concept/structured.hh
#	mln/value/concept/symbolic.hh
#	mln/value/concept/vectorial.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/data.hh
# /work/carlinet/trunk/milena/mln/value/concept/data.hh includes:
#	mln/core/concept/value.hh
test.o: /work/carlinet/trunk/milena/mln/value/concept/structured.hh
# /work/carlinet/trunk/milena/mln/value/concept/structured.hh includes:
#	mln/core/concept/value.hh
test.o: /work/carlinet/trunk/milena/mln/value/equiv.hh
# /work/carlinet/trunk/milena/mln/value/equiv.hh includes:
#	mln/core/concept/value.hh
#	mln/value/cast.hh
test.o: /work/carlinet/trunk/milena/mln/value/cast.hh
# /work/carlinet/trunk/milena/mln/value/cast.hh includes:
#	mln/core/concept/value.hh
#	mln/value/equiv.hh
test.o: /work/carlinet/trunk/milena/mln/literal/zero.hh
# /work/carlinet/trunk/milena/mln/literal/zero.hh includes:
#	mln/core/concept/literal.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/literal/ops.hh
# /work/carlinet/trunk/milena/mln/literal/ops.hh includes:
#	mln/core/concept/literal.hh
#	mln/trait/all.hh
#	mln/metal/equal.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/trait/all.hh
# /work/carlinet/trunk/milena/mln/trait/all.hh includes:
#	mln/trait/solve.hh
#	mln/trait/promote.hh
#	mln/trait/op/all.hh
test.o: /work/carlinet/trunk/milena/mln/convert/from_to.hxx
# /work/carlinet/trunk/milena/mln/convert/from_to.hxx includes:
#	mln/core/def/all.hh
#	mln/core/grids.hh
test.o: /work/carlinet/trunk/milena/mln/core/def/all.hh
# /work/carlinet/trunk/milena/mln/core/def/all.hh includes:
#	mln/core/def/coord.hh
#	mln/core/def/coordf.hh
#	mln/core/def/low_quant_nbits.hh
test.o: /work/carlinet/trunk/milena/mln/core/def/coord.hh
test.o: /work/carlinet/trunk/milena/mln/core/def/coordf.hh
test.o: /work/carlinet/trunk/milena/mln/core/grids.hh
# /work/carlinet/trunk/milena/mln/core/grids.hh includes:
#	mln/core/concept/regular_grid.hh
#	mln/core/def/coord.hh
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/regular_grid.hh
# /work/carlinet/trunk/milena/mln/core/concept/regular_grid.hh includes:
#	mln/core/concept/mesh.hh
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/mesh.hh
# /work/carlinet/trunk/milena/mln/core/concept/mesh.hh includes:
#	mln/core/concept/object.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/proxy.hxx
# /work/carlinet/trunk/milena/mln/core/concept/proxy.hxx includes:
#	mln/core/internal/force_exact.hh
#	mln/metal/unqualif.hh
#	mln/metal/is_a.hh
#	mln/metal/if.hh
#	mln/metal/is_const.hh
#	mln/metal/const.hh
#	mln/metal/unconst.hh
#	mln/metal/is_not_ref.hh
#	mln/metal/ref.hh
#	mln/metal/fix_return.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/force_exact.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is_const.hh
# /work/carlinet/trunk/milena/mln/metal/is_const.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is_not_ref.hh
# /work/carlinet/trunk/milena/mln/metal/is_not_ref.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/metal/ref.hh
test.o: /work/carlinet/trunk/milena/mln/metal/fix_return.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/iterator.hh
# /work/carlinet/trunk/milena/mln/core/concept/iterator.hh includes:
#	mln/core/concept/object.hh
test.o: /work/carlinet/trunk/milena/mln/trait/site_sets.hh
# /work/carlinet/trunk/milena/mln/trait/site_sets.hh includes:
#	mln/trait/undef.hh
#	mln/trait/site_set/props.hh
#	mln/trait/site_set/print.hh
test.o: /work/carlinet/trunk/milena/mln/trait/site_set/props.hh
# /work/carlinet/trunk/milena/mln/trait/site_set/props.hh includes:
#	mln/trait/undef.hh
test.o: /work/carlinet/trunk/milena/mln/trait/site_set/print.hh
# /work/carlinet/trunk/milena/mln/trait/site_set/print.hh includes:
#	mln/trait/site_sets.hh
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is_unqualif.hh
# /work/carlinet/trunk/milena/mln/metal/is_unqualif.hh includes:
#	mln/metal/equal.hh
#	mln/metal/unqualif.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/operators.hh
# /work/carlinet/trunk/milena/mln/core/site_set/operators.hh includes:
#	mln/core/concept/site_set.hh
test.o: /work/carlinet/trunk/milena/mln/core/trait/all.hh
# /work/carlinet/trunk/milena/mln/core/trait/all.hh includes:
#	mln/core/trait/op_mult.hh
#	mln/core/trait/pixter.hh
#	mln/core/trait/qlf_value.hh
test.o: /work/carlinet/trunk/milena/mln/core/trait/op_mult.hh
test.o: /work/carlinet/trunk/milena/mln/core/trait/pixter.hh
# /work/carlinet/trunk/milena/mln/core/trait/pixter.hh includes:
#	mln/metal/none.hh
test.o: /work/carlinet/trunk/milena/mln/metal/none.hh
test.o: /work/carlinet/trunk/milena/mln/core/trait/qlf_value.hh
# /work/carlinet/trunk/milena/mln/core/trait/qlf_value.hh includes:
#	mln/core/macros.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/box.hh
# /work/carlinet/trunk/milena/mln/core/site_set/box.hh includes:
#	mln/core/concept/box.hh
#	mln/core/internal/box_impl.hh
#	mln/core/point.hh
#	mln/literal/origin.hh
#	mln/core/site_set/box_piter.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/box.hh
# /work/carlinet/trunk/milena/mln/core/concept/box.hh includes:
#	mln/core/concept/site_set.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/box_impl.hh
# /work/carlinet/trunk/milena/mln/core/internal/box_impl.hh includes:
#	mln/core/internal/force_exact.hh
test.o: /work/carlinet/trunk/milena/mln/core/point.hh
# /work/carlinet/trunk/milena/mln/core/point.hh includes:
#	mln/core/def/coord.hh
#	mln/core/concept/proxy.hh
#	mln/core/concept/gpoint.hh
#	mln/core/internal/coord_impl.hh
#	mln/fun/i2v/all_to.hh
#	mln/metal/bool.hh
#	mln/metal/is_not.hh
#	mln/algebra/vec.hh
#	mln/metal/converts_to.hh
#	mln/algebra/h_vec.hh
#	mln/util/yes.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/gpoint.hh
# /work/carlinet/trunk/milena/mln/core/concept/gpoint.hh includes:
#	mln/core/concept/site.hh
#	mln/core/concept/gdpoint.hh
#	mln/value/concept/scalar.hh
#	mln/algebra/vec.hh
#	mln/util/ord.hh
#	mln/debug/format.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/site.hh
# /work/carlinet/trunk/milena/mln/core/concept/site.hh includes:
#	mln/core/concept/object.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/gdpoint.hh
# /work/carlinet/trunk/milena/mln/core/concept/gdpoint.hh includes:
#	mln/core/concept/object.hh
#	mln/core/grids.hh
#	mln/trait/all.hh
#	mln/value/scalar.hh
#	mln/debug/format.hh
test.o: /work/carlinet/trunk/milena/mln/debug/format.hh
test.o: /work/carlinet/trunk/milena/mln/algebra/vec.hh
# /work/carlinet/trunk/milena/mln/algebra/vec.hh includes:
#	mln/core/concept/object.hh
#	mln/literal/zero.hh
#	mln/literal/origin.hh
#	mln/norm/l2.hh
#	mln/trait/all.hh
#	mln/trait/value_.hh
#	mln/fun/i2v/all_to.hh
#	mln/debug/format.hh
#	mln/value/ops.hh
#	mln/make/vec.hh
test.o: /work/carlinet/trunk/milena/mln/literal/origin.hh
# /work/carlinet/trunk/milena/mln/literal/origin.hh includes:
#	mln/core/concept/literal.hh
test.o: /work/carlinet/trunk/milena/mln/norm/l2.hh
# /work/carlinet/trunk/milena/mln/norm/l2.hh includes:
#	mln/math/sqr.hh
#	mln/math/sqrt.hh
#	mln/algebra/vec.hh
#	mln/value/ops.hh
test.o: /work/carlinet/trunk/milena/mln/math/sqr.hh
test.o: /work/carlinet/trunk/milena/mln/math/sqrt.hh
test.o: /work/carlinet/trunk/milena/mln/fun/i2v/all_to.hh
# /work/carlinet/trunk/milena/mln/fun/i2v/all_to.hh includes:
#	mln/core/concept/function.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/function.hh
# /work/carlinet/trunk/milena/mln/core/concept/function.hh includes:
#	mln/core/concept/object.hh
test.o: /work/carlinet/trunk/milena/mln/make/vec.hh
# /work/carlinet/trunk/milena/mln/make/vec.hh includes:
#	mln/algebra/vec.hh
#	mln/core/concept/function.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/coord_impl.hh
# /work/carlinet/trunk/milena/mln/core/internal/coord_impl.hh includes:
#	mln/core/internal/force_exact.hh
test.o: /work/carlinet/trunk/milena/mln/algebra/h_vec.hh
# /work/carlinet/trunk/milena/mln/algebra/h_vec.hh includes:
#	mln/algebra/vec.hh
#	mln/literal/one.hh
test.o: /work/carlinet/trunk/milena/mln/util/yes.hh
# /work/carlinet/trunk/milena/mln/util/yes.hh includes:
#	mln/core/concept/object.hh
#	mln/core/routine/ops.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/box_piter.hh
# /work/carlinet/trunk/milena/mln/core/site_set/box_piter.hh includes:
#	mln/core/internal/site_set_iterator_base.hh
#	mln/core/concept/box.hh
#	mln/core/site_set/box.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/site_set_iterator_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/site_set_iterator_base.hh includes:
#	mln/core/internal/site_iterator_base.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/site_iterator_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/site_iterator_base.hh includes:
#	mln/core/concept/site_iterator.hh
#	mln/core/concept/pseudo_site.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/pseudo_site.hh
# /work/carlinet/trunk/milena/mln/core/concept/pseudo_site.hh includes:
#	mln/core/concept/site_proxy.hh
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/trait/concrete.hh
# /work/carlinet/trunk/milena/mln/trait/concrete.hh includes:
#	mln/trait/ch_value.hh
test.o: /work/carlinet/trunk/milena/mln/trait/ch_value.hh
# /work/carlinet/trunk/milena/mln/trait/ch_value.hh includes:
#	mln/tag/skeleton.hh
#	mln/trait/image_from_grid.hh
#	mln/trait/ch_function_value.hh
test.o: /work/carlinet/trunk/milena/mln/tag/skeleton.hh
test.o: /work/carlinet/trunk/milena/mln/trait/image_from_grid.hh
# /work/carlinet/trunk/milena/mln/trait/image_from_grid.hh includes:
#	mln/core/grids.hh
test.o: /work/carlinet/trunk/milena/mln/trait/ch_function_value.hh
# /work/carlinet/trunk/milena/mln/trait/ch_function_value.hh includes:
#	mln/fun/v2v/ch_function_value.hh
test.o: /work/carlinet/trunk/milena/mln/fun/v2v/ch_function_value.hh
# /work/carlinet/trunk/milena/mln/fun/v2v/ch_function_value.hh includes:
#	mln/core/concept/function.hh
#	mln/fun/internal/ch_function_value_impl.hh
test.o: /work/carlinet/trunk/milena/mln/fun/internal/ch_function_value_impl.hh
test.o: /work/carlinet/trunk/milena/mln/trait/images.hh
# /work/carlinet/trunk/milena/mln/trait/images.hh includes:
#	mln/trait/undef.hh
#	mln/trait/image/props.hh
#	mln/trait/value_.hh
#	mln/metal/bexpr.hh
#	mln/metal/equal.hh
#	mln/metal/if.hh
#	mln/metal/is_const.hh
#	mln/trait/image/print.hh
test.o: /work/carlinet/trunk/milena/mln/trait/image/props.hh
# /work/carlinet/trunk/milena/mln/trait/image/props.hh includes:
#	mln/trait/undef.hh
#	mln/trait/value/kind.hh
#	mln/core/def/coord.hh
test.o: /work/carlinet/trunk/milena/mln/trait/image/print.hh
# /work/carlinet/trunk/milena/mln/trait/image/print.hh includes:
#	mln/trait/images.hh
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/tag/init.hh
test.o: /work/carlinet/trunk/milena/mln/core/routine/initialize.hh
# /work/carlinet/trunk/milena/mln/core/routine/initialize.hh includes:
#	mln/core/concept/image.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/check/image_all.hh
# /work/carlinet/trunk/milena/mln/core/internal/check/image_all.hh includes:
#	mln/trait/images.hh
#	mln/core/internal/check/image_fastest.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/check/image_fastest.hh
# /work/carlinet/trunk/milena/mln/core/internal/check/image_fastest.hh includes:
#	mln/metal/bool.hh
#	mln/core/macros.hh
#	mln/core/trait/pixter.hh
#	mln/core/trait/qlf_value.hh
#	mln/core/internal/force_exact.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/data.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/morpher_lvalue.hh
# /work/carlinet/trunk/milena/mln/core/internal/morpher_lvalue.hh includes:
#	mln/core/macros.hh
test.o: /work/carlinet/trunk/milena/mln/util/tracked_ptr.hh
# /work/carlinet/trunk/milena/mln/util/tracked_ptr.hh includes:
#	mln/core/contract.hh
test.o: /work/carlinet/trunk/milena/mln/value/set.hh
# /work/carlinet/trunk/milena/mln/value/set.hh includes:
#	mln/value/internal/iterable_set.hh
#	mln/trait/value_.hh
test.o: /work/carlinet/trunk/milena/mln/value/internal/iterable_set.hh
# /work/carlinet/trunk/milena/mln/value/internal/iterable_set.hh includes:
#	mln/core/concept/value_set.hh
#	mln/trait/value_.hh
#	mln/value/builtin/all.hh
#	mln/value/internal/convert.hh
#	mln/value/viter.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/value_set.hh
# /work/carlinet/trunk/milena/mln/core/concept/value_set.hh includes:
#	mln/core/concept/value_iterator.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/value_iterator.hh
# /work/carlinet/trunk/milena/mln/core/concept/value_iterator.hh includes:
#	mln/core/concept/iterator.hh
test.o: /work/carlinet/trunk/milena/mln/value/internal/convert.hh
# /work/carlinet/trunk/milena/mln/value/internal/convert.hh includes:
#	mln/core/contract.hh
#	mln/trait/value_.hh
test.o: /work/carlinet/trunk/milena/mln/value/viter.hh
# /work/carlinet/trunk/milena/mln/value/viter.hh includes:
#	mln/core/concept/value_iterator.hh
#	mln/core/concept/value_set.hh
test.o: /work/carlinet/trunk/milena/mln/value/super_value.hh
# /work/carlinet/trunk/milena/mln/value/super_value.hh includes:
#	mln/value/sign.hh
test.o: /work/carlinet/trunk/milena/mln/value/sign.hh
# /work/carlinet/trunk/milena/mln/value/sign.hh includes:
#	mln/value/internal/integer.hh
#	mln/trait/value_.hh
#	mln/literal/zero.hh
#	mln/literal/one.hh
#	mln/debug/format.hh
test.o: /work/carlinet/trunk/milena/mln/value/internal/integer.hh
# /work/carlinet/trunk/milena/mln/value/internal/integer.hh includes:
#	mln/value/concept/scalar.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/box2d.hh
# /work/carlinet/trunk/milena/mln/core/alias/box2d.hh includes:
#	mln/core/site_set/box.hh
#	mln/core/alias/point2d.hh
#	mln/make/box2d.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/point2d.hh
# /work/carlinet/trunk/milena/mln/core/alias/point2d.hh includes:
#	mln/core/point.hh
#	mln/core/concept/site_proxy.hh
#	mln/core/internal/force_exact.hh
#	mln/core/alias/dpoint2d.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/dpoint2d.hh
# /work/carlinet/trunk/milena/mln/core/alias/dpoint2d.hh includes:
#	mln/core/dpoint.hh
#	mln/core/def/coord.hh
#	mln/core/alias/point2d.hh
test.o: /work/carlinet/trunk/milena/mln/core/dpoint.hh
# /work/carlinet/trunk/milena/mln/core/dpoint.hh includes:
#	mln/core/def/coord.hh
#	mln/core/concept/gdpoint.hh
#	mln/core/internal/coord_impl.hh
#	mln/fun/i2v/all.hh
#	mln/algebra/vec.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/fun/i2v/all.hh
# /work/carlinet/trunk/milena/mln/fun/i2v/all.hh includes:
#	mln/fun/i2v/all_to.hh
#	mln/fun/i2v/array.hh
test.o: /work/carlinet/trunk/milena/mln/fun/i2v/array.hh
# /work/carlinet/trunk/milena/mln/fun/i2v/array.hh includes:
#	mln/core/concept/function.hh
#	mln/fun/internal/array_base.hh
#	mln/util/array.hh
#	mln/metal/equal.hh
test.o: /work/carlinet/trunk/milena/mln/fun/internal/array_base.hh
# /work/carlinet/trunk/milena/mln/fun/internal/array_base.hh includes:
#	mln/util/array.hh
#	mln/tag/init.hh
test.o: /work/carlinet/trunk/milena/mln/util/array.hh
# /work/carlinet/trunk/milena/mln/util/array.hh includes:
#	mln/core/concept/proxy.hh
#	mln/core/concept/iterator.hh
test.o: /work/carlinet/trunk/milena/mln/make/box2d.hh
# /work/carlinet/trunk/milena/mln/make/box2d.hh includes:
#	mln/core/alias/box2d.hh
test.o: /work/carlinet/trunk/milena/mln/core/routine/init.hh
# /work/carlinet/trunk/milena/mln/core/routine/init.hh includes:
#	mln/tag/init.hh
#	mln/geom/bbox.hh
#	mln/border/find.hh
#	mln/core/routine/init.hxx
test.o: /work/carlinet/trunk/milena/mln/geom/bbox.hh
# /work/carlinet/trunk/milena/mln/geom/bbox.hh includes:
#	mln/core/site_set/box.hh
#	mln/core/concept/image.hh
#	mln/core/concept/window.hh
#	mln/core/concept/weighted_window.hh
#	mln/literal/zero.hh
#	mln/accu/bbox.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/window.hh
# /work/carlinet/trunk/milena/mln/core/concept/window.hh includes:
#	mln/core/concept/object.hh
#	mln/core/concept/iterator.hh
#	mln/trait/windows.hh
#	mln/core/site_set/p_array.hh
#	mln/core/internal/geom_bbox.hh
#	mln/convert/from_to.hxx
#	mln/util/array.hh
test.o: /work/carlinet/trunk/milena/mln/trait/windows.hh
# /work/carlinet/trunk/milena/mln/trait/windows.hh includes:
#	mln/trait/undef.hh
#	mln/trait/window/props.hh
#	mln/trait/window/print.hh
test.o: /work/carlinet/trunk/milena/mln/trait/window/props.hh
# /work/carlinet/trunk/milena/mln/trait/window/props.hh includes:
#	mln/trait/undef.hh
test.o: /work/carlinet/trunk/milena/mln/trait/window/print.hh
# /work/carlinet/trunk/milena/mln/trait/window/print.hh includes:
#	mln/trait/windows.hh
#	mln/metal/is_a.hh
#	mln/metal/bexpr.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/p_array.hh
# /work/carlinet/trunk/milena/mln/core/site_set/p_array.hh includes:
#	mln/core/internal/site_set_base.hh
#	mln/core/internal/site_set_iterator_base.hh
#	mln/core/internal/pseudo_site_base.hh
#	mln/util/index.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/site_set_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/site_set_base.hh includes:
#	mln/core/concept/site_set.hh
#	mln/core/concept/site_proxy.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/pseudo_site_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/pseudo_site_base.hh includes:
#	mln/core/concept/pseudo_site.hh
test.o: /work/carlinet/trunk/milena/mln/util/index.hh
# /work/carlinet/trunk/milena/mln/util/index.hh includes:
#	mln/core/concept/object.hh
#	mln/util/dindex.hh
test.o: /work/carlinet/trunk/milena/mln/util/dindex.hh
# /work/carlinet/trunk/milena/mln/util/dindex.hh includes:
#	mln/util/index.hh
#	mln/literal/zero.hh
#	mln/literal/one.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/geom_bbox.hh
# /work/carlinet/trunk/milena/mln/core/internal/geom_bbox.hh includes:
#	mln/accu/bbox.hh
#	mln/literal/origin.hh
test.o: /work/carlinet/trunk/milena/mln/accu/bbox.hh
# /work/carlinet/trunk/milena/mln/accu/bbox.hh includes:
#	mln/core/site_set/box.hh
#	mln/core/concept/meta_accumulator.hh
#	mln/accu/internal/base.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/meta_accumulator.hh
# /work/carlinet/trunk/milena/mln/core/concept/meta_accumulator.hh includes:
#	mln/core/concept/object.hh
#	mln/core/concept/accumulator.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/accumulator.hh
# /work/carlinet/trunk/milena/mln/core/concept/accumulator.hh includes:
#	mln/core/concept/proxy.hh
#	mln/metal/fix_return.hh
#	mln/metal/const.hh
#	mln/trait/accumulators.hh
test.o: /work/carlinet/trunk/milena/mln/trait/accumulators.hh
# /work/carlinet/trunk/milena/mln/trait/accumulators.hh includes:
#	mln/trait/accumulator/props.hh
#	mln/trait/undef.hh
#	mln/trait/accumulator/print.hh
test.o: /work/carlinet/trunk/milena/mln/trait/accumulator/props.hh
test.o: /work/carlinet/trunk/milena/mln/trait/accumulator/print.hh
# /work/carlinet/trunk/milena/mln/trait/accumulator/print.hh includes:
#	mln/trait/accumulators.hh
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/accu/internal/base.hh
# /work/carlinet/trunk/milena/mln/accu/internal/base.hh includes:
#	mln/core/concept/accumulator.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/weighted_window.hh
# /work/carlinet/trunk/milena/mln/core/concept/weighted_window.hh includes:
#	mln/core/concept/object.hh
#	mln/core/concept/iterator.hh
#	mln/trait/windows.hh
test.o: /work/carlinet/trunk/milena/mln/border/find.hh
# /work/carlinet/trunk/milena/mln/border/find.hh includes:
#	mln/core/internal/image_morpher.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/image_morpher.hh
# /work/carlinet/trunk/milena/mln/core/internal/image_morpher.hh includes:
#	mln/core/internal/image_base.hh
#	mln/metal/const.hh
#	mln/metal/is_const.hh
#	mln/metal/is_not_const.hh
test.o: /work/carlinet/trunk/milena/mln/metal/is_not_const.hh
# /work/carlinet/trunk/milena/mln/metal/is_not_const.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/core/routine/init.hxx
test.o: /work/carlinet/trunk/milena/mln/border/thickness.hh
test.o: /work/carlinet/trunk/milena/mln/core/dpoints_pixter.hh
# /work/carlinet/trunk/milena/mln/core/dpoints_pixter.hh includes:
#	mln/core/concept/proxy.hh
#	mln/core/concept/pixel_iterator.hh
#	mln/core/internal/pixel_impl.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/pixel_iterator.hh
# /work/carlinet/trunk/milena/mln/core/concept/pixel_iterator.hh includes:
#	mln/core/concept/iterator.hh
#	mln/core/concept/generalized_pixel.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/generalized_pixel.hh
# /work/carlinet/trunk/milena/mln/core/concept/generalized_pixel.hh includes:
#	mln/core/concept/object.hh
#	mln/core/internal/force_exact.hh
#	mln/core/trait/qlf_value.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/pixel_impl.hh
# /work/carlinet/trunk/milena/mln/core/internal/pixel_impl.hh includes:
#	mln/core/concept/image.hh
#	mln/core/internal/force_exact.hh
#	mln/util/pix.hh
test.o: /work/carlinet/trunk/milena/mln/util/pix.hh
# /work/carlinet/trunk/milena/mln/util/pix.hh includes:
#	mln/core/concept/image.hh
#	mln/make/pix.hh
test.o: /work/carlinet/trunk/milena/mln/make/pix.hh
# /work/carlinet/trunk/milena/mln/make/pix.hh includes:
#	mln/util/pix.hh
test.o: /work/carlinet/trunk/milena/mln/core/pixter2d.hh
# /work/carlinet/trunk/milena/mln/core/pixter2d.hh includes:
#	mln/core/internal/pixel_iterator_base.hh
#	mln/core/alias/point2d.hh
#	mln/geom/size2d.hh
#	mln/opt/at.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/pixel_iterator_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/pixel_iterator_base.hh includes:
#	mln/core/concept/pixel_iterator.hh
#	mln/core/internal/pixel_impl.hh
#	mln/core/trait/qlf_value.hh
test.o: /work/carlinet/trunk/milena/mln/geom/size2d.hh
# /work/carlinet/trunk/milena/mln/geom/size2d.hh includes:
#	mln/geom/nrows.hh
#	mln/geom/ncols.hh
test.o: /work/carlinet/trunk/milena/mln/geom/nrows.hh
# /work/carlinet/trunk/milena/mln/geom/nrows.hh includes:
#	mln/geom/min_row.hh
#	mln/geom/max_row.hh
test.o: /work/carlinet/trunk/milena/mln/geom/min_row.hh
# /work/carlinet/trunk/milena/mln/geom/min_row.hh includes:
#	mln/core/concept/image.hh
#	mln/geom/bbox.hh
#	mln/metal/bexpr.hh
#	mln/metal/int.hh
#	mln/metal/equal.hh
test.o: /work/carlinet/trunk/milena/mln/geom/max_row.hh
# /work/carlinet/trunk/milena/mln/geom/max_row.hh includes:
#	mln/core/concept/image.hh
#	mln/geom/bbox.hh
test.o: /work/carlinet/trunk/milena/mln/geom/ncols.hh
# /work/carlinet/trunk/milena/mln/geom/ncols.hh includes:
#	mln/geom/min_col.hh
#	mln/geom/max_col.hh
test.o: /work/carlinet/trunk/milena/mln/geom/min_col.hh
# /work/carlinet/trunk/milena/mln/geom/min_col.hh includes:
#	mln/core/concept/image.hh
#	mln/geom/bbox.hh
test.o: /work/carlinet/trunk/milena/mln/geom/max_col.hh
# /work/carlinet/trunk/milena/mln/geom/max_col.hh includes:
#	mln/core/concept/image.hh
#	mln/geom/bbox.hh
test.o: /work/carlinet/trunk/milena/mln/opt/at.hh
# /work/carlinet/trunk/milena/mln/opt/at.hh includes:
#	mln/core/concept/image.hh
#	mln/trait/images.hh
#	mln/trace/all.hh
#	mln/core/alias/point1d.hh
#	mln/core/alias/point2d.hh
#	mln/core/alias/point3d.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/point1d.hh
# /work/carlinet/trunk/milena/mln/core/alias/point1d.hh includes:
#	mln/core/point.hh
#	mln/core/concept/site_proxy.hh
#	mln/core/internal/force_exact.hh
#	mln/core/alias/dpoint1d.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/dpoint1d.hh
# /work/carlinet/trunk/milena/mln/core/alias/dpoint1d.hh includes:
#	mln/core/dpoint.hh
#	mln/core/grids.hh
#	mln/core/def/coord.hh
#	mln/core/alias/point1d.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/point3d.hh
# /work/carlinet/trunk/milena/mln/core/alias/point3d.hh includes:
#	mln/core/point.hh
#	mln/core/concept/site_proxy.hh
#	mln/core/internal/force_exact.hh
#	mln/core/alias/dpoint3d.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/dpoint3d.hh
# /work/carlinet/trunk/milena/mln/core/alias/dpoint3d.hh includes:
#	mln/core/dpoint.hh
#	mln/core/grids.hh
#	mln/core/def/coord.hh
#	mln/core/alias/point3d.hh
test.o: /work/carlinet/trunk/milena/mln/make/image.hh
# /work/carlinet/trunk/milena/mln/make/image.hh includes:
#	mln/core/image/image1d.hh
#	mln/core/image/image2d.hh
#	mln/core/image/image3d.hh
#	mln/opt/at.hh
test.o: /work/carlinet/trunk/milena/mln/core/image/image1d.hh
# /work/carlinet/trunk/milena/mln/core/image/image1d.hh includes:
#	mln/core/internal/fixme.hh
#	mln/core/internal/image_primary.hh
#	mln/core/alias/box1d.hh
#	mln/border/thickness.hh
#	mln/value/set.hh
#	mln/fun/i2v/all_to.hh
#	mln/core/trait/pixter.hh
#	mln/core/dpoints_pixter.hh
#	mln/core/pixter1d.hh
#	mln/core/w_window.hh
#	mln/make/image.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/box1d.hh
# /work/carlinet/trunk/milena/mln/core/alias/box1d.hh includes:
#	mln/core/site_set/box.hh
#	mln/core/alias/point1d.hh
#	mln/make/box1d.hh
test.o: /work/carlinet/trunk/milena/mln/make/box1d.hh
# /work/carlinet/trunk/milena/mln/make/box1d.hh includes:
#	mln/core/alias/box1d.hh
test.o: /work/carlinet/trunk/milena/mln/core/pixter1d.hh
# /work/carlinet/trunk/milena/mln/core/pixter1d.hh includes:
#	mln/core/internal/pixel_iterator_base.hh
#	mln/core/alias/point1d.hh
#	mln/geom/size1d.hh
test.o: /work/carlinet/trunk/milena/mln/geom/size1d.hh
# /work/carlinet/trunk/milena/mln/geom/size1d.hh includes:
#	mln/geom/ninds.hh
test.o: /work/carlinet/trunk/milena/mln/geom/ninds.hh
# /work/carlinet/trunk/milena/mln/geom/ninds.hh includes:
#	mln/geom/min_ind.hh
#	mln/geom/max_ind.hh
test.o: /work/carlinet/trunk/milena/mln/geom/min_ind.hh
# /work/carlinet/trunk/milena/mln/geom/min_ind.hh includes:
#	mln/core/concept/image.hh
test.o: /work/carlinet/trunk/milena/mln/geom/max_ind.hh
# /work/carlinet/trunk/milena/mln/geom/max_ind.hh includes:
#	mln/core/concept/image.hh
test.o: /work/carlinet/trunk/milena/mln/core/w_window.hh
# /work/carlinet/trunk/milena/mln/core/w_window.hh includes:
#	mln/core/internal/weighted_window_base.hh
#	mln/core/concept/image.hh
#	mln/core/site_set/box.hh
#	mln/core/window.hh
#	mln/core/dpsites_piter.hh
#	mln/value/ops.hh
#	mln/util/ord.hh
#	mln/geom/bbox.hh
#	mln/literal/zero.hh
#	mln/convert/to.hh
#	mln/make/w_window.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/weighted_window_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/weighted_window_base.hh includes:
#	mln/core/concept/weighted_window.hh
test.o: /work/carlinet/trunk/milena/mln/core/window.hh
# /work/carlinet/trunk/milena/mln/core/window.hh includes:
#	mln/core/internal/window_base.hh
#	mln/core/concept/gdpoint.hh
#	mln/metal/is_a.hh
#	mln/util/set.hh
#	mln/fun/i2v/all_to.hh
#	mln/norm/linfty.hh
#	mln/literal/zero.hh
#	mln/core/dpsites_piter.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/window_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/window_base.hh includes:
#	mln/core/concept/window.hh
test.o: /work/carlinet/trunk/milena/mln/util/set.hh
# /work/carlinet/trunk/milena/mln/util/set.hh includes:
#	mln/core/concept/proxy.hh
#	mln/util/ord.hh
test.o: /work/carlinet/trunk/milena/mln/norm/linfty.hh
# /work/carlinet/trunk/milena/mln/norm/linfty.hh includes:
#	mln/math/abs.hh
#	mln/algebra/vec.hh
test.o: /work/carlinet/trunk/milena/mln/math/abs.hh
# /work/carlinet/trunk/milena/mln/math/abs.hh includes:
#	mln/value/int_u.hh
test.o: /work/carlinet/trunk/milena/mln/value/int_u.hh
# /work/carlinet/trunk/milena/mln/value/int_u.hh includes:
#	mln/trait/all.hh
#	mln/value/ops.hh
#	mln/metal/math/pow.hh
#	mln/value/internal/value_like.hh
#	mln/value/internal/encoding.hh
#	mln/value/concept/integer.hh
#	mln/trait/value_.hh
#	mln/debug/format.hh
test.o: /work/carlinet/trunk/milena/mln/value/internal/value_like.hh
# /work/carlinet/trunk/milena/mln/value/internal/value_like.hh includes:
#	mln/core/concept/value.hh
#	mln/core/internal/force_exact.hh
test.o: /work/carlinet/trunk/milena/mln/value/internal/encoding.hh
test.o: /work/carlinet/trunk/milena/mln/core/dpsites_piter.hh
# /work/carlinet/trunk/milena/mln/core/dpsites_piter.hh includes:
#	mln/core/internal/site_relative_iterator_base.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/site_relative_iterator_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/site_relative_iterator_base.hh includes:
#	mln/core/internal/site_iterator_base.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/convert/to.hh
# /work/carlinet/trunk/milena/mln/convert/to.hh includes:
#	mln/core/routine/exact.hh
#	mln/metal/equal.hh
#	mln/trace/all.hh
#	mln/convert/from_to.hh
#	mln/convert/from_to.hxx
test.o: /work/carlinet/trunk/milena/mln/convert/from_to.hh
# /work/carlinet/trunk/milena/mln/convert/from_to.hh includes:
#	mln/convert/impl/all.hh
#	mln/convert/from_to.hxx
#	mln/metal/abort.hh
#	mln/metal/converts_to.hh
#	mln/metal/is.hh
#	mln/metal/is_a.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/all.hh
# /work/carlinet/trunk/milena/mln/convert/impl/all.hh includes:
#	mln/convert/impl/from_double_to_value.hh
#	mln/convert/impl/from_float_to_value.hh
#	mln/convert/impl/from_image_to_site_set.hh
#	mln/convert/impl/from_int_to_value.hh
#	mln/convert/impl/from_site_set_to_image.hh
#	mln/convert/impl/from_unsigned_to_value.hh
#	mln/convert/impl/from_value_to_value.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/from_double_to_value.hh
# /work/carlinet/trunk/milena/mln/convert/impl/from_double_to_value.hh includes:
#	mln/value/concept/integer.hh
#	mln/value/concept/floating.hh
#	mln/core/concept/value.hh
#	mln/math/round.hh
test.o: /work/carlinet/trunk/milena/mln/math/round.hh
# /work/carlinet/trunk/milena/mln/math/round.hh includes:
#	mln/core/concept/function.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/from_float_to_value.hh
# /work/carlinet/trunk/milena/mln/convert/impl/from_float_to_value.hh includes:
#	mln/value/concept/integer.hh
#	mln/value/concept/floating.hh
#	mln/core/concept/value.hh
#	mln/math/round.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/from_image_to_site_set.hh
# /work/carlinet/trunk/milena/mln/convert/impl/from_image_to_site_set.hh includes:
#	mln/core/site_set/p_run.hh
#	mln/core/site_set/p_array.hh
#	mln/metal/converts_to.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/p_run.hh
# /work/carlinet/trunk/milena/mln/core/site_set/p_run.hh includes:
#	mln/core/internal/site_set_base.hh
#	mln/core/site_set/box.hh
#	mln/core/internal/pseudo_site_base.hh
#	mln/util/index.hh
#	mln/core/site_set/p_run_piter.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/p_run_piter.hh
# /work/carlinet/trunk/milena/mln/core/site_set/p_run_piter.hh includes:
#	mln/core/site_set/p_run.hh
#	mln/core/internal/site_set_iterator_base.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/from_int_to_value.hh
# /work/carlinet/trunk/milena/mln/convert/impl/from_int_to_value.hh includes:
#	mln/value/concept/integer.hh
#	mln/core/concept/value.hh
#	mln/math/round.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/from_site_set_to_image.hh
# /work/carlinet/trunk/milena/mln/convert/impl/from_site_set_to_image.hh includes:
#	mln/core/image/sub_image.hh
#	mln/geom/bbox.hh
#	mln/trait/image_from_grid.hh
#	mln/data/fill.hh
test.o: /work/carlinet/trunk/milena/mln/core/image/sub_image.hh
# /work/carlinet/trunk/milena/mln/core/image/sub_image.hh includes:
#	mln/core/internal/image_domain_morpher.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/image_domain_morpher.hh
# /work/carlinet/trunk/milena/mln/core/internal/image_domain_morpher.hh includes:
#	mln/core/internal/image_morpher.hh
test.o: /work/carlinet/trunk/milena/mln/data/fill.hh
# /work/carlinet/trunk/milena/mln/data/fill.hh includes:
#	mln/core/concept/function.hh
#	mln/pw/image.hh
#	mln/convert/to_fun.hh
#	mln/data/fill_with_image.hh
#	mln/data/fill_with_value.hh
test.o: /work/carlinet/trunk/milena/mln/pw/image.hh
# /work/carlinet/trunk/milena/mln/pw/image.hh includes:
#	mln/core/internal/image_primary.hh
#	mln/core/concept/function.hh
#	mln/value/set.hh
#	mln/metal/unqualif.hh
#	mln/metal/not_equal.hh
test.o: /work/carlinet/trunk/milena/mln/convert/to_fun.hh
# /work/carlinet/trunk/milena/mln/convert/to_fun.hh includes:
#	mln/pw/value.hh
#	mln/fun/c.hh
test.o: /work/carlinet/trunk/milena/mln/pw/value.hh
# /work/carlinet/trunk/milena/mln/pw/value.hh includes:
#	mln/fun/internal/selector.hh
#	mln/core/concept/image.hh
test.o: /work/carlinet/trunk/milena/mln/fun/internal/selector.hh
# /work/carlinet/trunk/milena/mln/fun/internal/selector.hh includes:
#	mln/core/concept/function.hh
#	mln/core/concept/site.hh
#	mln/core/concept/pseudo_site.hh
#	mln/metal/unqualif.hh
#	mln/metal/if.hh
#	mln/metal/is_a.hh
#	mln/algebra/vec.hh
test.o: /work/carlinet/trunk/milena/mln/fun/c.hh
# /work/carlinet/trunk/milena/mln/fun/c.hh includes:
#	mln/fun/internal/selector.hh
#	mln/metal/unqualif.hh
test.o: /work/carlinet/trunk/milena/mln/data/fill_with_image.hh
# /work/carlinet/trunk/milena/mln/data/fill_with_image.hh includes:
#	mln/core/concept/image.hh
#	mln/data/fill_with_image.spe.hh
test.o: /work/carlinet/trunk/milena/mln/data/fill_with_image.spe.hh
# /work/carlinet/trunk/milena/mln/data/fill_with_image.spe.hh includes:
#	mln/data/memcpy_.hh
#	mln/data/fill_with_value.hh
#	mln/core/pixel.hh
#	mln/core/box_runstart_piter.hh
#	mln/border/get.hh
#	mln/opt/value.hh
#	mln/opt/element.hh
test.o: /work/carlinet/trunk/milena/mln/data/memcpy_.hh
# /work/carlinet/trunk/milena/mln/data/memcpy_.hh includes:
#	mln/core/concept/image.hh
#	mln/core/pixel.hh
#	mln/metal/is_not_const.hh
#	mln/opt/element.hh
test.o: /work/carlinet/trunk/milena/mln/core/pixel.hh
# /work/carlinet/trunk/milena/mln/core/pixel.hh includes:
#	mln/core/concept/generalized_pixel.hh
#	mln/core/internal/pixel_impl.hh
#	mln/make/pixel.hh
test.o: /work/carlinet/trunk/milena/mln/make/pixel.hh
# /work/carlinet/trunk/milena/mln/make/pixel.hh includes:
#	mln/core/concept/image.hh
#	mln/core/pixel.hh
test.o: /work/carlinet/trunk/milena/mln/opt/element.hh
# /work/carlinet/trunk/milena/mln/opt/element.hh includes:
#	mln/core/concept/image.hh
#	mln/trait/images.hh
test.o: /work/carlinet/trunk/milena/mln/data/fill_with_value.hh
# /work/carlinet/trunk/milena/mln/data/fill_with_value.hh includes:
#	mln/core/concept/image.hh
#	mln/data/fill_with_value.spe.hh
test.o: /work/carlinet/trunk/milena/mln/data/fill_with_value.spe.hh
# /work/carlinet/trunk/milena/mln/data/fill_with_value.spe.hh includes:
#	mln/data/memset_.hh
#	mln/opt/value.hh
#	mln/opt/element.hh
test.o: /work/carlinet/trunk/milena/mln/data/memset_.hh
# /work/carlinet/trunk/milena/mln/data/memset_.hh includes:
#	mln/core/concept/image.hh
#	mln/core/pixel.hh
#	mln/metal/is_not_const.hh
#	mln/opt/element.hh
test.o: /work/carlinet/trunk/milena/mln/opt/value.hh
# /work/carlinet/trunk/milena/mln/opt/value.hh includes:
#	mln/core/concept/image.hh
#	mln/trait/images.hh
test.o: /work/carlinet/trunk/milena/mln/core/box_runstart_piter.hh
# /work/carlinet/trunk/milena/mln/core/box_runstart_piter.hh includes:
#	mln/core/internal/site_iterator_base.hh
#	mln/core/site_set/box.hh
test.o: /work/carlinet/trunk/milena/mln/border/get.hh
# /work/carlinet/trunk/milena/mln/border/get.hh includes:
#	mln/trait/images.hh
#	mln/trace/all.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/from_unsigned_to_value.hh
# /work/carlinet/trunk/milena/mln/convert/impl/from_unsigned_to_value.hh includes:
#	mln/value/concept/integer.hh
#	mln/core/concept/value.hh
#	mln/value/label.hh
#	mln/math/round.hh
test.o: /work/carlinet/trunk/milena/mln/value/label.hh
# /work/carlinet/trunk/milena/mln/value/label.hh includes:
#	mln/debug/format.hh
#	mln/metal/math/pow.hh
#	mln/trait/value_.hh
#	mln/value/concept/symbolic.hh
#	mln/value/internal/value_like.hh
#	mln/value/internal/convert.hh
#	mln/value/internal/encoding.hh
test.o: /work/carlinet/trunk/milena/mln/convert/impl/from_value_to_value.hh
# /work/carlinet/trunk/milena/mln/convert/impl/from_value_to_value.hh includes:
#	mln/core/concept/image.hh
#	mln/core/concept/site_set.hh
#	mln/value/concept/all.hh
#	mln/core/site_set/p_run.hh
#	mln/metal/converts_to.hh
#	mln/convert/from_to.hxx
test.o: /work/carlinet/trunk/milena/mln/make/w_window.hh
# /work/carlinet/trunk/milena/mln/make/w_window.hh includes:
#	mln/core/concept/window.hh
#	mln/core/concept/function.hh
#	mln/core/w_window.hh
test.o: /work/carlinet/trunk/milena/mln/core/image/image3d.hh
# /work/carlinet/trunk/milena/mln/core/image/image3d.hh includes:
#	mln/core/internal/fixme.hh
#	mln/core/internal/image_primary.hh
#	mln/core/alias/box3d.hh
#	mln/border/thickness.hh
#	mln/value/set.hh
#	mln/fun/i2v/all_to.hh
#	mln/core/trait/pixter.hh
#	mln/core/dpoints_pixter.hh
#	mln/core/pixter3d.hh
#	mln/core/w_window.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/box3d.hh
# /work/carlinet/trunk/milena/mln/core/alias/box3d.hh includes:
#	mln/core/site_set/box.hh
#	mln/core/alias/point3d.hh
#	mln/make/box3d.hh
test.o: /work/carlinet/trunk/milena/mln/make/box3d.hh
# /work/carlinet/trunk/milena/mln/make/box3d.hh includes:
#	mln/core/alias/box3d.hh
test.o: /work/carlinet/trunk/milena/mln/core/pixter3d.hh
# /work/carlinet/trunk/milena/mln/core/pixter3d.hh includes:
#	mln/core/internal/pixel_iterator_base.hh
#	mln/core/alias/point3d.hh
#	mln/geom/size3d.hh
#	mln/opt/at.hh
test.o: /work/carlinet/trunk/milena/mln/geom/size3d.hh
# /work/carlinet/trunk/milena/mln/geom/size3d.hh includes:
#	mln/geom/nslis.hh
#	mln/geom/nrows.hh
#	mln/geom/ncols.hh
test.o: /work/carlinet/trunk/milena/mln/geom/nslis.hh
# /work/carlinet/trunk/milena/mln/geom/nslis.hh includes:
#	mln/geom/min_sli.hh
#	mln/geom/max_sli.hh
test.o: /work/carlinet/trunk/milena/mln/geom/min_sli.hh
# /work/carlinet/trunk/milena/mln/geom/min_sli.hh includes:
#	mln/core/concept/image.hh
test.o: /work/carlinet/trunk/milena/mln/geom/max_sli.hh
# /work/carlinet/trunk/milena/mln/geom/max_sli.hh includes:
#	mln/core/concept/image.hh
test.o: /work/carlinet/trunk/milena/mln/make/image2d.hh
# /work/carlinet/trunk/milena/mln/make/image2d.hh includes:
#	mln/core/image/image2d.hh
test.o: /work/carlinet/trunk/milena/mln/core/image/image_if.hh
# /work/carlinet/trunk/milena/mln/core/image/image_if.hh includes:
#	mln/core/internal/image_domain_morpher.hh
#	mln/core/site_set/p_if.hh
#	mln/pw/all.hh
#	mln/convert/to_fun.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/p_if.hh
# /work/carlinet/trunk/milena/mln/core/site_set/p_if.hh includes:
#	mln/core/internal/site_set_base.hh
#	mln/core/concept/function.hh
#	mln/core/site_set/p_if_piter.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/p_if_piter.hh
# /work/carlinet/trunk/milena/mln/core/site_set/p_if_piter.hh includes:
#	mln/core/internal/piter_adaptor.hh
#	mln/core/site_set/p_if.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/piter_adaptor.hh
# /work/carlinet/trunk/milena/mln/core/internal/piter_adaptor.hh includes:
#	mln/core/internal/site_iterator_base.hh
test.o: /work/carlinet/trunk/milena/mln/pw/all.hh
# /work/carlinet/trunk/milena/mln/pw/all.hh includes:
#	mln/pw/cst.hh
#	mln/pw/image.hh
#	mln/pw/value.hh
#	mln/pw/var.hh
#	mln/fun/ops.hh
test.o: /work/carlinet/trunk/milena/mln/pw/cst.hh
# /work/carlinet/trunk/milena/mln/pw/cst.hh includes:
#	mln/fun/internal/selector.hh
test.o: /work/carlinet/trunk/milena/mln/pw/var.hh
# /work/carlinet/trunk/milena/mln/pw/var.hh includes:
#	mln/core/concept/function.hh
test.o: /work/carlinet/trunk/milena/mln/fun/ops.hh
# /work/carlinet/trunk/milena/mln/fun/ops.hh includes:
#	mln/core/concept/function.hh
#	mln/fun/internal/selector.hh
#	mln/trait/all.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/neighb2d.hh
# /work/carlinet/trunk/milena/mln/core/alias/neighb2d.hh includes:
#	mln/core/alias/window2d.hh
#	mln/core/neighb.hh
#	mln/convert/from_to.hh
test.o: /work/carlinet/trunk/milena/mln/core/alias/window2d.hh
# /work/carlinet/trunk/milena/mln/core/alias/window2d.hh includes:
#	mln/core/window.hh
#	mln/core/alias/dpoint2d.hh
#	mln/metal/math/sqrt.hh
#	mln/convert/from_to.hxx
test.o: /work/carlinet/trunk/milena/mln/core/neighb.hh
# /work/carlinet/trunk/milena/mln/core/neighb.hh includes:
#	mln/core/internal/neighborhood_base.hh
#	mln/core/internal/site_relative_iterator_base.hh
#	mln/core/internal/neighb_niter_impl.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/neighborhood_base.hh
# /work/carlinet/trunk/milena/mln/core/internal/neighborhood_base.hh includes:
#	mln/core/concept/neighborhood.hh
#	mln/core/concept/window.hh
test.o: /work/carlinet/trunk/milena/mln/core/concept/neighborhood.hh
# /work/carlinet/trunk/milena/mln/core/concept/neighborhood.hh includes:
#	mln/core/concept/window.hh
#	mln/trait/windows.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/neighb_niter_impl.hh
# /work/carlinet/trunk/milena/mln/core/internal/neighb_niter_impl.hh includes:
#	mln/core/macros.hh
#	mln/core/internal/force_exact.hh
test.o: /work/carlinet/trunk/milena/mln/core/routine/duplicate.hh
# /work/carlinet/trunk/milena/mln/core/routine/duplicate.hh includes:
#	mln/core/concept/image.hh
#	mln/core/routine/init.hh
#	mln/data/fill.hh
test.o: /work/carlinet/trunk/milena/mln/core/var.hh
# /work/carlinet/trunk/milena/mln/core/var.hh includes:
#	mln/core/macros.hh
test.o: /work/carlinet/trunk/milena/mln/level/sort_psites.hh
# /work/carlinet/trunk/milena/mln/level/sort_psites.hh includes:
#	mln/core/concept/image.hh
#	mln/convert/to_p_array.hh
#	mln/histo/compute.hh
#	mln/util/ord.hh
#	mln/geom/nsites.hh
test.o: /work/carlinet/trunk/milena/mln/convert/to_p_array.hh
# /work/carlinet/trunk/milena/mln/convert/to_p_array.hh includes:
#	mln/core/site_set/p_array.hh
#	mln/core/concept/image.hh
#	mln/core/concept/window.hh
test.o: /work/carlinet/trunk/milena/mln/histo/compute.hh
# /work/carlinet/trunk/milena/mln/histo/compute.hh includes:
#	mln/core/concept/image.hh
#	mln/histo/array.hh
#	mln/histo/compute.spe.hh
test.o: /work/carlinet/trunk/milena/mln/histo/array.hh
# /work/carlinet/trunk/milena/mln/histo/array.hh includes:
#	mln/value/set.hh
test.o: /work/carlinet/trunk/milena/mln/histo/compute.spe.hh
test.o: /work/carlinet/trunk/milena/mln/geom/nsites.hh
# /work/carlinet/trunk/milena/mln/geom/nsites.hh includes:
#	mln/core/concept/image.hh
#	mln/set/card.hh
test.o: /work/carlinet/trunk/milena/mln/set/card.hh
# /work/carlinet/trunk/milena/mln/set/card.hh includes:
#	mln/core/concept/site_set.hh
test.o: /work/carlinet/trunk/milena/mln/morpho/tree/data.hh
# /work/carlinet/trunk/milena/mln/morpho/tree/data.hh includes:
#	mln/morpho/tree/compute_parent.hh
#	mln/core/site_set/p_array.hh
#	mln/core/internal/site_set_iterator_base.hh
#	mln/core/internal/piter_identity.hh
test.o: /work/carlinet/trunk/milena/mln/morpho/tree/compute_parent.hh
# /work/carlinet/trunk/milena/mln/morpho/tree/compute_parent.hh includes:
#	mln/core/concept/image.hh
#	mln/core/concept/neighborhood.hh
#	mln/data/fill.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/piter_identity.hh
# /work/carlinet/trunk/milena/mln/core/internal/piter_identity.hh includes:
#	mln/core/internal/piter_adaptor.hh
test.o: /work/carlinet/trunk/milena/mln/morpho/tree/compute_attribute_image.hh
# /work/carlinet/trunk/milena/mln/morpho/tree/compute_attribute_image.hh includes:
#	mln/core/concept/image.hh
#	mln/morpho/tree/data.hh
#	mln/trait/accumulators.hh
#	mln/util/pix.hh
#	mln/data/fill.hh
test.o: propagate.hh
# propagate.hh includes:
#	mln/morpho/tree/data.hh
test.o: run.hh
# run.hh includes:
#	mln/core/concept/accumulator.hh
#	mln/core/concept/image.hh
#	mln/core/concept/function.hh
#	mln/core/site_set/p_array.hh
#	mln/util/pix.hh
#	mln/trace/entering.hh
#	mln/trace/exiting.hh
#	propagate_node.hh
test.o: propagate_node.hh
# propagate_node.hh includes:
#	mln/morpho/tree/data.hh
#	mln/core/site_set/p_array.hh
test.o: accumulator/arg_max.hh
# accumulator/arg_max.hh includes:
#	mln/core/concept/image.hh
#	mln/accu/internal/base.hh
#	mln/util/pix.hh
test.o: /work/carlinet/trunk/milena/mln/transform/distance_geodesic.hh
# /work/carlinet/trunk/milena/mln/transform/distance_geodesic.hh includes:
#	mln/canvas/distance_geodesic.hh
#	mln/transform/internal/distance_functor.hh
test.o: /work/carlinet/trunk/milena/mln/canvas/distance_geodesic.hh
# /work/carlinet/trunk/milena/mln/canvas/distance_geodesic.hh includes:
#	mln/core/concept/image.hh
#	mln/core/concept/neighborhood.hh
#	mln/core/routine/duplicate.hh
#	mln/core/site_set/p_queue_fast.hh
#	mln/data/fill.hh
#	mln/extension/adjust_fill.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/p_queue_fast.hh
# /work/carlinet/trunk/milena/mln/core/site_set/p_queue_fast.hh includes:
#	mln/core/site_set/p_array.hh
test.o: /work/carlinet/trunk/milena/mln/extension/adjust_fill.hh
# /work/carlinet/trunk/milena/mln/extension/adjust_fill.hh includes:
#	mln/extension/adjust.hh
#	mln/extension/fill.hh
test.o: /work/carlinet/trunk/milena/mln/extension/adjust.hh
# /work/carlinet/trunk/milena/mln/extension/adjust.hh includes:
#	mln/border/adjust.hh
#	mln/core/concept/window.hh
#	mln/core/concept/weighted_window.hh
#	mln/core/concept/neighborhood.hh
#	mln/geom/delta.hh
test.o: /work/carlinet/trunk/milena/mln/border/adjust.hh
# /work/carlinet/trunk/milena/mln/border/adjust.hh includes:
#	mln/border/resize.hh
test.o: /work/carlinet/trunk/milena/mln/border/resize.hh
# /work/carlinet/trunk/milena/mln/border/resize.hh includes:
#	mln/core/concept/image.hh
#	mln/core/routine/duplicate.hh
#	mln/core/routine/primary.hh
#	mln/border/get.hh
#	mln/data/fill.hh
test.o: /work/carlinet/trunk/milena/mln/core/routine/primary.hh
# /work/carlinet/trunk/milena/mln/core/routine/primary.hh includes:
#	mln/core/concept/image.hh
test.o: /work/carlinet/trunk/milena/mln/geom/delta.hh
# /work/carlinet/trunk/milena/mln/geom/delta.hh includes:
#	mln/core/concept/window.hh
#	mln/core/concept/weighted_window.hh
#	mln/core/concept/neighborhood.hh
test.o: /work/carlinet/trunk/milena/mln/extension/fill.hh
# /work/carlinet/trunk/milena/mln/extension/fill.hh includes:
#	mln/core/concept/image.hh
#	mln/trait/image/props.hh
#	mln/border/fill.hh
#	mln/data/fill_with_value.hh
test.o: /work/carlinet/trunk/milena/mln/border/fill.hh
# /work/carlinet/trunk/milena/mln/border/fill.hh includes:
#	mln/core/concept/image.hh
#	mln/core/box_runstart_piter.hh
#	mln/opt/element.hh
test.o: /work/carlinet/trunk/milena/mln/transform/internal/distance_functor.hh
# /work/carlinet/trunk/milena/mln/transform/internal/distance_functor.hh includes:
#	mln/core/macros.hh
test.o: /work/carlinet/trunk/milena/mln/morpho/attribute/card.hh
# /work/carlinet/trunk/milena/mln/morpho/attribute/card.hh includes:
#	mln/accu/internal/base.hh
#	mln/util/pix.hh
test.o: ../attributes/bbox.hh
# ../attributes/bbox.hh includes:
#	mln/core/concept/box.hh
test.o: /work/carlinet/trunk/milena/mln/io/pbm/load.hh
# /work/carlinet/trunk/milena/mln/io/pbm/load.hh includes:
#	mln/core/image/image2d.hh
#	mln/core/image/image3d.hh
#	mln/io/pnm/load_header.hh
#	mln/make/image3d.hh
test.o: /work/carlinet/trunk/milena/mln/io/pnm/load_header.hh
test.o: /work/carlinet/trunk/milena/mln/make/image3d.hh
# /work/carlinet/trunk/milena/mln/make/image3d.hh includes:
#	mln/core/image/image3d.hh
#	mln/core/image/image2d.hh
#	mln/core/image/slice_image.hh
#	mln/data/paste.hh
#	mln/util/array.hh
test.o: /work/carlinet/trunk/milena/mln/core/image/slice_image.hh
# /work/carlinet/trunk/milena/mln/core/image/slice_image.hh includes:
#	mln/core/internal/image_domain_morpher.hh
#	mln/core/alias/box3d.hh
#	mln/core/alias/box2d.hh
test.o: /work/carlinet/trunk/milena/mln/data/paste.hh
# /work/carlinet/trunk/milena/mln/data/paste.hh includes:
#	mln/core/concept/image.hh
#	mln/data/paste.spe.hh
test.o: /work/carlinet/trunk/milena/mln/data/paste.spe.hh
# /work/carlinet/trunk/milena/mln/data/paste.spe.hh includes:
#	mln/core/pixel.hh
#	mln/data/fill_with_value.hh
#	mln/data/memcpy_.hh
#	mln/core/box_runstart_piter.hh
#	mln/border/get.hh
#	mln/opt/value.hh
#	mln/opt/element.hh
test.o: /work/carlinet/trunk/milena/mln/io/pgm/save.hh
# /work/carlinet/trunk/milena/mln/io/pgm/save.hh includes:
#	mln/io/pnm/save.hh
#	mln/geom/size2d.hh
#	mln/metal/bexpr.hh
#	mln/metal/is_not_a.hh
#	mln/value/concept/vectorial.hh
test.o: /work/carlinet/trunk/milena/mln/io/pnm/save.hh
# /work/carlinet/trunk/milena/mln/io/pnm/save.hh includes:
#	mln/core/concept/image.hh
#	mln/core/alias/point2d.hh
#	mln/value/concept/scalar.hh
#	mln/value/rgb.hh
#	mln/value/rgb8.hh
#	mln/value/int_u8.hh
#	mln/metal/templated_by.hh
#	mln/metal/not_equal.hh
#	mln/io/pnm/save_header.hh
#	mln/io/pnm/macros.hh
#	mln/geom/size2d.hh
test.o: /work/carlinet/trunk/milena/mln/value/rgb.hh
# /work/carlinet/trunk/milena/mln/value/rgb.hh includes:
#	mln/value/ops.hh
#	mln/value/concept/vectorial.hh
#	mln/value/int_u.hh
#	mln/algebra/vec.hh
#	mln/fun/v2v/rgb_to_hsl.hh
test.o: /work/carlinet/trunk/milena/mln/fun/v2v/rgb_to_hsl.hh
# /work/carlinet/trunk/milena/mln/fun/v2v/rgb_to_hsl.hh includes:
#	mln/math/round.hh
#	mln/math/max.hh
#	mln/math/min.hh
#	mln/trait/value_.hh
#	mln/value/rgb.hh
test.o: /work/carlinet/trunk/milena/mln/math/max.hh
test.o: /work/carlinet/trunk/milena/mln/math/min.hh
test.o: /work/carlinet/trunk/milena/mln/value/rgb8.hh
# /work/carlinet/trunk/milena/mln/value/rgb8.hh includes:
#	mln/value/rgb.hh
test.o: /work/carlinet/trunk/milena/mln/value/int_u8.hh
# /work/carlinet/trunk/milena/mln/value/int_u8.hh includes:
#	mln/value/int_u.hh
test.o: /work/carlinet/trunk/milena/mln/metal/templated_by.hh
# /work/carlinet/trunk/milena/mln/metal/templated_by.hh includes:
#	mln/metal/bool.hh
test.o: /work/carlinet/trunk/milena/mln/io/pnm/save_header.hh
# /work/carlinet/trunk/milena/mln/io/pnm/save_header.hh includes:
#	mln/io/pnm/max_component.hh
#	mln/value/rgb.hh
#	mln/geom/nrows.hh
#	mln/geom/ncols.hh
test.o: /work/carlinet/trunk/milena/mln/io/pnm/max_component.hh
# /work/carlinet/trunk/milena/mln/io/pnm/max_component.hh includes:
#	mln/value/rgb.hh
#	mln/value/int_u.hh
test.o: /work/carlinet/trunk/milena/mln/io/pnm/macros.hh
test.o: /work/carlinet/trunk/milena/mln/io/ppm/save.hh
# /work/carlinet/trunk/milena/mln/io/ppm/save.hh includes:
#	mln/core/concept/image.hh
#	mln/metal/templated_by.hh
#	mln/io/pnm/save.hh
test.o: ./../../theo/color/change_attributes.hh
test.o: /work/carlinet/trunk/milena/mln/fun/p2v/ternary.hh
# /work/carlinet/trunk/milena/mln/fun/p2v/ternary.hh includes:
#	mln/fun/internal/selector.hh
test.o: /work/carlinet/trunk/milena/mln/labeling/blobs.hh
# /work/carlinet/trunk/milena/mln/labeling/blobs.hh includes:
#	mln/core/concept/image.hh
#	mln/core/concept/neighborhood.hh
#	mln/data/fill.hh
#	mln/core/site_set/p_queue_fast.hh
test.o: /work/carlinet/trunk/milena/mln/debug/colorize.hh
# /work/carlinet/trunk/milena/mln/debug/colorize.hh includes:
#	mln/core/concept/image.hh
#	mln/fun/i2v/array.hh
#	mln/value/rgb8.hh
#	mln/literal/black.hh
#	mln/level/transform.hh
test.o: /work/carlinet/trunk/milena/mln/literal/black.hh
# /work/carlinet/trunk/milena/mln/literal/black.hh includes:
#	mln/core/concept/literal.hh
test.o: /work/carlinet/trunk/milena/mln/level/transform.hh
# /work/carlinet/trunk/milena/mln/level/transform.hh includes:
#	mln/core/concept/image.hh
#	mln/core/concept/function.hh
#	mln/value/set.hh
#	mln/level/transform.spe.hh
test.o: /work/carlinet/trunk/milena/mln/level/transform.spe.hh
# /work/carlinet/trunk/milena/mln/level/transform.spe.hh includes:
#	mln/core/concept/image.hh
#	mln/core/concept/function.hh
#	mln/data/fill_with_value.hh
#	mln/value/set.hh
#	mln/value/lut_vec.hh
#	mln/opt/value.hh
test.o: /work/carlinet/trunk/milena/mln/value/lut_vec.hh
# /work/carlinet/trunk/milena/mln/value/lut_vec.hh includes:
#	mln/core/concept/value_set.hh
#	mln/core/concept/function.hh
#	mln/trait/value_.hh
#	mln/value/viter.hh
test.o: /work/carlinet/trunk/milena/mln/draw/box.hh
# /work/carlinet/trunk/milena/mln/draw/box.hh includes:
#	mln/core/concept/image.hh
#	mln/core/alias/box2d.hh
#	mln/data/paste.hh
#	mln/draw/line.hh
#	mln/pw/image.hh
#	mln/pw/cst.hh
test.o: /work/carlinet/trunk/milena/mln/draw/line.hh
# /work/carlinet/trunk/milena/mln/draw/line.hh includes:
#	mln/core/concept/image.hh
#	mln/core/site_set/p_line2d.hh
#	mln/core/image/safe.hh
#	mln/data/paste.hh
#	mln/pw/image.hh
#	mln/pw/cst.hh
test.o: /work/carlinet/trunk/milena/mln/core/site_set/p_line2d.hh
# /work/carlinet/trunk/milena/mln/core/site_set/p_line2d.hh includes:
#	mln/core/site_set/p_array.hh
#	mln/core/alias/box2d.hh
#	mln/math/sign.hh
#	mln/math/abs.hh
#	mln/math/min.hh
#	mln/math/max.hh
test.o: /work/carlinet/trunk/milena/mln/math/sign.hh
test.o: /work/carlinet/trunk/milena/mln/core/image/safe.hh
# /work/carlinet/trunk/milena/mln/core/image/safe.hh includes:
#	mln/core/internal/image_identity.hh
test.o: /work/carlinet/trunk/milena/mln/core/internal/image_identity.hh
# /work/carlinet/trunk/milena/mln/core/internal/image_identity.hh includes:
#	mln/core/internal/image_morpher.hh
