#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	$(LIB)(Math.o) \
	$(LIB)(Matrixd.o) \
	$(LIB)(MatrixDecomposition.o) \
	$(LIB)(Matrixf.o) \
	$(LIB)(Quat.o)

all: ${OBJS}

clean:
	-rm -f *.o
