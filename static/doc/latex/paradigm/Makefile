include share/make/share.mk
include share/make/tex.mk
TEXI2PDF_FLAGS += -I figures
include share/make/pdf-figs.mk

# The article must be named using the last name of the first author
# plus the extension, according to the ICIP Paper Submision Kit.
BASE = scoop
PDF  = slides.pdf

.PHONY: final
final: $(BASE).pdf

$(BASE).pdf: slides.pdf
	cp $< $@

CLEANFILES += $(PDF) $(HTML) $(BASE).pdf

all : pdf
pdf: $(PDF)

DOCUMENT_IMAGES =				\
  figures/design.fig				\
  figures/simple0x.fig				\
  figures/simplegp.fig				\
  figures/simplescoop1.fig			\
  figures/simpleoovt.fig			\
  figures/simplevt.fig				\
  figures/simpledeco.fig			\
  figures/harddeco.fig				\
  figures/vt1.fig				\
  figures/vt2.fig				\
  figures/corn.fig				\
  figures/glance.fig				\
  figures/zoomin.fig				\
  figures/alt.fig				\
  figures/evolution.fig				\
  figures/selectors.fig				\
  figures/harder0x.fig				\
  figures/simpleoo.fig

CLEANFILES +=					\
  figures/*~					\
  figures/*.eps

# Dependencies.
# figures/times.pdf: figures/times-16bit.dat figures/times-float.dat

# FIXME: Generate figures/h16b.pdf (and figures/h16b.ps) from
# figures/h16.dat.gz using grace/xmgrace.
#EXTRA_DIST = figures/h16.dat.gz figures/h16b.ps

DOCUMENT_IMAGES_PDF = $(call share_convert_to_pdf,$(DOCUMENT_IMAGES))

debug:
	@echo "DOCUMENT_IMAGES_PDF = $(DOCUMENT_IMAGES_PDF)"

CLEANFILES += $(DOCUMENT_IMAGES_PDF) figures/*.{pdftex,pdftex_t}

slides.pdf: $(DOCUMENT_IMAGES_PDF)

view pdfview: pdf
	xpdf $(PDF)
