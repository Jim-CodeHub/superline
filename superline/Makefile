#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for superline source file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------


OBJS    = super_base.o 
SUBDIRS = consumer producer
 
 
#-------------------------------------------------------------------------------------------------------
#																									   #
#										  Make rules 									   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------


.PHONY: all clean $(SUBDIRS)

all:$(SUBDIRS) $(OBJS)

$(SUBDIRS):ECHO
	$(MAKE) -C $@

ECHO:

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

.PHONY:clean
clean:
	rm -rf *.o

