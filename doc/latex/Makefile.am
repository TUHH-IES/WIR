#
# Target to build
#

.SILENT:


#
# Source files of the target
#

TEXFILES		= wir.tex
TEXSOURCES		= wir.tex \
			  0-PREAMBLE/acronyms.tex 0-PREAMBLE/commands.tex \
			  0-PREAMBLE/glossary.tex 0-PREAMBLE/packages.tex \
			  1-INTRODUCTION/chapter.tex \
			  2-PREREQUISITES/chapter.tex \
			  3-DIRECTORY/chapter.tex \
			  4-BOOTSTRAP/chapter.tex \
			  5-BUILD/chapter.tex \
			  6-LISTS/chapter.tex \
			  7-CONSTRUCTION/chapter.tex \
			  8-SWMODEL/chapter.tex \
			  9-HWMODEL/chapter.tex \
			  10-SYSMODEL/chapter.tex \
			  11-SETSMAPS/chapter.tex \
			  12-CODEGEN/chapter.tex \
			  13-ANALYSES/chapter.tex \
			  14-OPTIMIZATIONS/chapter.tex \
			  15-DEBUGGING/chapter.tex \
			  16-PROCRETARGET/chapter.tex \
			  17-ANARETARGET/chapter.tex \
			  18-OPTRETARGET/chapter.tex
BIBFILES		= wir.bib
FIGURES			= PIC/acyclicregions.dot PIC/acyclicregions.png \
			  PIC/arm.png PIC/arm.pptx \
			  PIC/bitdfg.dot PIC/bitdfg.png \
			  PIC/busarbitration.png PIC/busarbitration.pptx \
			  PIC/cfg.dot PIC/cfg.png \
			  PIC/cfgs.dot PIC/cfgs.png \
			  PIC/containerAPI.png PIC/containerAPI.pptx \
			  PIC/controltreenodes.png PIC/controltreenodes.pptx \
			  PIC/cyclicregions.dot PIC/cyclicregions.png \
			  PIC/flowfacts.png PIC/flowfacts.pptx \
			  PIC/hardwaremodel1.png PIC/hardwaremodel2.png \
			  PIC/igraph1.dot PIC/igraph1.png \
			  PIC/igraph2.dot PIC/igraph2.png \
			  PIC/immediates.png \
			  PIC/l4.png PIC/l4.pptx \
			  PIC/logo_es_text.png \
			  PIC/memlayout.png PIC/memlayout.pptx \
			  PIC/parameters.png \
			  PIC/processors.png PIC/processors.pptx \
			  PIC/riscv.png PIC/riscv.pptx \
			  PIC/schedulingprios.png PIC/schedulingprios.pptx \
			  PIC/softwaremodel.png \
			  PIC/tricore.png PIC/tricore.pptx \
			  PIC/unstructured.dot PIC/unstructured.png \
			  PIC/wirsystem.png
LOGFILES		= $(wildcard *.log)
COMMON_DEPS		= $(TEXSOURCES) $(BIBFILES) $(FIGURES)

# Derived values
BASENAMES		= $(basename $(TEXFILES))

PDFS			= $(addsuffix .pdf, $(BASENAMES)) \
			  authors.tex version.tex

UTIL_FILES		= $(addsuffix .aux, $(basename $(BIBFILES))) \
			  $(addsuffix .bbl, $(basename $(BIBFILES))) \
			  $(addsuffix .blg, $(basename $(BIBFILES))) \
			  $(addsuffix .bcf, $(BASENAMES))            \
			  $(addsuffix .aux, $(BASENAMES))            \
			  $(addsuffix .dvi, $(BASENAMES))            \
			  $(addsuffix .out, $(BASENAMES))            \
			  $(addsuffix .acn, $(BASENAMES))            \
			  $(addsuffix .acr, $(BASENAMES))            \
			  $(addsuffix .alg, $(BASENAMES))            \
			  $(addsuffix .bbl, $(BASENAMES))            \
			  $(addsuffix .blg, $(BASENAMES))            \
			  $(addsuffix .glg, $(BASENAMES))            \
			  $(addsuffix .glo, $(BASENAMES))            \
			  $(addsuffix .gls, $(BASENAMES))            \
			  $(addsuffix .idx, $(BASENAMES))            \
			  $(addsuffix .ist, $(BASENAMES))            \
			  $(addsuffix .run.xml, $(BASENAMES))        \
			  $(addsuffix .tdn, $(BASENAMES))            \
			  $(addsuffix .tld, $(BASENAMES))            \
			  $(addsuffix .tlg, $(BASENAMES))            \
			  $(addsuffix .toc, $(BASENAMES))            \
			  $(addsuffix .out, $(BASENAMES))            \
			  $(addsuffix .log, $(BASENAMES))            \
			  $(addsuffix .lol, $(BASENAMES))            \
			  $(addsuffix .lof, $(BASENAMES))            \
			  $(addsuffix .lot, $(BASENAMES))            \
			  $(addsuffix .xdy, $(BASENAMES))            \
			  $(LOGFILES)

BUILT_SOURCES		=
EXTRA_DIST		= $(COMMON_DEPS)

if WITH_LATEX_DOC

  BUILT_SOURCES	       += authors.tex version.tex pdf

endif

CLEANFILES		= $(PDFS) $(UTIL_FILES)


#
# Specific rules
#

if WITH_LATEX_DOC

pdf: $(PDFS)
	@exit 0; # To make 'gmake' happy

wir.pdf: $(COMMON_DEPS)
	@TEXINPUTS=.:@srcdir@:@srcdir@/../..: @PDFLATEX@ wir; \
	if [ $$? != 0 ]; then exit 1; fi;                     \
	@MAKEGLOSSARIES@ wir;                                 \
	BIBINPUTS=@srcdir@ @BIBER@ wir;                       \
	TEXINPUTS=.:@srcdir@:@srcdir@/../..: @PDFLATEX@ wir;  \
	if [ $$? != 0 ]; then exit 1; fi;                     \
	TEXINPUTS=.:@srcdir@:@srcdir@/../..: @PDFLATEX@ wir;  \
	if [ $$? != 0 ]; then exit 1; fi;                     \
	TEXINPUTS=.:@srcdir@:@srcdir@/../..: @PDFLATEX@ wir;  \
	if [ $$? != 0 ]; then exit 1; fi;

authors.tex: ../../AUTHORS
	@CAT@ $< | @CUT@ -d '<' -f1 | @AWK@ '{print "\\item " $$0;}' - > $@

version.tex: ../../Releases
	echo "\newcommand{\WIRVERSION}{@WIR_RELEASE@}" > $@

endif
