REMOTE_HOST=mtl

SCHEDULE=./sched.sh

NAME=msp
PID_FILE=batch.pid
BUILD_DIR=out
BUILD_PATH=$(BUILD_DIR)/$(NAME)
DIST_NAME=acceler8
DIST_ARCH=$(DIST_NAME).tgz
SOLUTION_NAME=Acceler8-Alexander_Solovets

SOURCE=src/$(NAME).c

# 'debug' is default target
debug : CFLAGS = -ggdb -mssse3 -DDEBUG
debug : $(BUILD_PATH)_dbg

optimized : CFLAGS = -O2
optimized : $(BUILD_PATH)_opt

simd : CFLAGS = -mssse3
simd : $(BUILD_PATH)_sse

openmp : CFLAGS = -fopenmp
openmp : $(BUILD_PATH)_omp

retail : CFLAGS = -fopenmp -O2
retail : $(BUILD_PATH)_rtl

extreme : CFLAGS = -fopenmp -O3 -mssse3
extreme : $(BUILD_PATH)_xtm

vector : CFLAGS = -fopenmp -O2 -mssse3
vector : $(BUILD_PATH)_vec

default : $(BUILD_PATH)_nop

# Compile sources into object file and save asm listing.
$(BUILD_PATH)%.o : $(SOURCE)
	test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) -g -c -Wa,-a,-ad $< > $(@:.o=.lst) -o $@

# Link object file and create executable.
$(BUILD_PATH)% : $(BUILD_PATH)%.o
	LD_LIBRARY_PATH=$(LIBRARY_PATH) $(CC) $(CFLAGS) $< -o $@

all : debug optimized openmp retail extreme simd default vector

all-mtl : CC = /opt/gcc/4.5.1/bin/gcc
all-mtl : LIBRARY_PATH = /opt/mpc/lib:/opt/mpfr/lib/:/opt/gmp/lib
all-mtl : all

clean: doc-clean
	$(RM) $(BUILD_DIR)/*

dist:
	git archive --prefix=$(DIST_NAME)/ HEAD | gzip > $(DIST_ARCH)

dist-copy: dist
	scp $(DIST_ARCH) $(REMOTE_HOST):~mbait
	ssh $(REMOTE_HOST) 'tar zxf $(DIST_ARCH) && rm $(DIST_ARCH)'

solution: doc
	$(TAR) --transform 's,^,${SOLUTION_NAME}/,S' -zcf ${SOLUTION_NAME}.tgz \
		Makefile \
		src/msp.c \
		doc/out/report.html

batch: all-mtl
	$(RM) -r ${PID_FILE}
	$(SCHEDULE) 1 >> ${PID_FILE}
	$(SCHEDULE) 4 >> ${PID_FILE}
	$(SCHEDULE) 8 >> ${PID_FILE}
	$(SCHEDULE) 16 >> ${PID_FILE}
	$(SCHEDULE) 35 >> ${PID_FILE}
	$(SCHEDULE) 40 >> ${PID_FILE}
	cat ${PID_FILE}

doc:
	$(MAKE) -C doc

doc-clean:
	$(MAKE) -C doc clean


.SUFFIXES :

.PHONY : doc
