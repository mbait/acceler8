NAME=report
SOURCE=$(NAME).txt
OUT_DIR=out
REPORT=$(OUT_DIR)/$(NAME).html

all: $(REPORT)

$(REPORT): $(SOURCE)
	test -d $(OUT_DIR) || mkdir $(OUT_DIR)
	asciidoc -a lang=ru -o $@ $<

clean:
	$(RM) $(REPORT)

.PHONY: clean
