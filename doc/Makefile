NAME=report
SOURCE=$(NAME).txt
OUT_DIR=out
IMG_DIR=${OUT_DIR}/img
REPORT=$(OUT_DIR)/$(NAME)
REPORT_HTML=$(REPORT).html

SUBDIRS = img

all: $(REPORT_HTML)

$(REPORT_HTML): $(SOURCE)
	$(MAKE) -C img
	test -d ${IMG_DIR} || mkdir -p ${IMG_DIR}
	cp img/*.png ${IMG_DIR}
	cp ~/CV2/out/resume.pdf ${OUT_DIR}
	asciidoc -a lang=ru -a tabsize=4 -o $@ $<
	sed -i $@ -e 's/\(class="title"\)/\1 style="text-align:inherit"/'

$(REPORT_TEX) : $(SOURCE)
	asciidoc -a lang=ru --unsafe --backend=latex $<

clean:
	$(MAKE) -C ${SUBDIRS} clean
	$(RM) -r out

.PHONY: clean
