NAME=report
SOURCE=$(NAME).txt
OUT_DIR=out
REPORT=$(OUT_DIR)/$(NAME)
REPORT_HTML=$(REPORT).html

all: $(REPORT_HTML)

$(REPORT_HTML): $(SOURCE)
	test -d $(OUT_DIR) || mkdir $(OUT_DIR)
	asciidoc -a lang=ru -a tabsize=4 -o $@ $<

$(REPORT_TEX) : $(SOURCE)
	asciidoc -a lang=ru --unsafe --backend=latex $<

clean: clean-html

clean-html :
	$(RM) $(REPORT_HTML)

.PHONY: clean
