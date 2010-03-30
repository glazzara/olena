FILE = design.tex
FILEGEN = $(FILE:.tex=.pdf)
SOURCE =
PDFLATEX = pdflatex

FIND=find .
RM=rm
RMOPT=-rf

all: pdf

light: $(FILE) $(SOURCE)
	$(PDFLATEX) $(FILE)

pdf: $(FILE) $(SOURCE)
	$(PDFLATEX) $(FILE)
	$(PDFLATEX) $(FILE)
	$(PDFLATEX) $(FILE)

clean:
	$(FIND) \( -name '*.toc' -o -name '*.aux' -o -name '*.bat' \) \
	-exec $(RM) $(RMOPT) {} \;
	$(FIND) \( -name '*.glo' -o -name '*.log' -o -name '*.out' \) \
	-exec $(RM) $(RMOPT) {} \;
	$(FIND) \( -name '#*' -o -name '*.aux' -o -name '*~' \
	-o -name '*.idx' \) -exec $(RM) $(RMOPT) {} \;

distclean: clean
	$(RM) $(RMOPT) $(FILEGEN)
