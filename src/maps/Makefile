#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeMaps.a

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(mapsFF.o)

SUBDIRS = rpfMap

all: subdirs ${OBJS}

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	-rm -f $(LIB)

