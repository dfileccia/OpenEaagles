#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeHla.a

OBJS =  \
	$(LIB)(Ambassador.o) \
	$(LIB)(hlaFF.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(Nib.o)

SUBDIRS = rprFom

#CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

#all: subdirs ${OBJS} 
all: ${OBJS} 

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	for subdir in $(SUBDIRS); do \
	    echo making $@ in $$subdir; \
	    (cd $$subdir && $(MAKE) $@) || exit 1; \
	done
	-rm -f $(LIB)

