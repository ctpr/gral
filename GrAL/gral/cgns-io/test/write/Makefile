# gral/base/test/gmv-output

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
TARGETS = test-cgns-write
EXEC =  $(TARGETS)

include $(DEPS)

EXTRA_DEPMODULES+= gral/triang2d  geometry

LIBS = -lgral-triang2d -lgral-base -lgeometry


new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-cgns-write: $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 


