#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeHla.a

#CPPFLAGS += -I$(HLA_INC_DIR)
CPPFLAGS += -DRTI_USES_STD_FSTREAM
#CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

OBJS =  \
	$(LIB)(MunitionDetonation.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(Nib.o) \
	$(LIB)(Ntm.o) \
	$(LIB)(Platforms.o) \
	$(LIB)(RprFom.o) \
	$(LIB)(rprFomFF.o) \
	$(LIB)(WeaponFire.o)

all: ${OBJS} 

clean:
	-rm -f *.o
	-rm -f $(LIB)

