#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeDynamics.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

SUBDIRS = 

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(JSBSimModel.o) \
	$(LIB)(LaeroModel.o)  \
	$(LIB)(RacModel.o) \
	$(LIB)(dynamicsFF.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
