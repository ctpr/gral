# gral/base/test/extend-grid-types

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/cartesian2d gral/complex2d gral/cartesian3d gral/complex3d geometry
LOCALSOURCES =  test-extend-grid-types.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = $(EXEC)
EXEC =  $(LOCALSOURCES:%.C=%.exe)

include $(DEPS)



LIBS = -lgral-base


new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(TARGETS): $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -f oct.complex2d

