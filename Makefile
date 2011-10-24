NAME=msp
BUILD_DIR=out
BUILD_PATH=$(BUILD_DIR)/$(NAME)

SOURCE=src/$(NAME).c

# 'debug' is default target
debug:
	CFLAGS="-ggdb" $(MAKE) $(BUILD_PATH)_dbg

opt:
	CFLAGS="-O2" $(MAKE) $(BUILD_PATH)_opt

openmp:
	CFLAGS="-fopenmp" $(MAKE) $(BUILD_PATH)_omp

retail:
	CFLAGS="-O2 -fopenmp" $(MAKE) $(BUILD_PATH)_full

extreme:
	CFLAGS="-O3 -fopenmp" $(MAKE) $(BUILD_PATH)_xtrm

$(BUILD_PATH)%: $(SOURCE)
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) -g -c -Wa,-a,-ad $< > $(@:=.lst)

all: debug opt openmp retail extreme

clean:
	$(RM) $(BUILD_DIR)/*

.SUFFIXES :

.PHONY : clean

