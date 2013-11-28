CXX		 = mpic++
CXXFLAGS = -I../header -Wall -O3 
LINKFLAGS= 
LIBFLAGS = 
LIBDIR   = 


DIRS	 = src

MAKEFLAG = CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" LINKFLAGS="$(LINKFLAGS)" LIBFLAGS="$(LIBFLAGS)" LIBDIR="$(LIBDIR)"

all:
	@echo ""
	@echo ""
	@echo "======================================== compiling ========================================"
	@for x in $(DIRS); \
	do \
		echo "      enter: $$x"; \
		(cd $$x; make $(MAKEFLAG) all ; cd .. ); \
		echo "      exit: $$x"; \
		echo " "; \
	done
	@echo "======================================== compiling ok ========================================"

clean:
	@echo "======================================== clear ========================================"
	-rm -f *.o
	@for x in $(DIRS); \
	do \
		echo "      enter: $$x"; \
		(cd $$x; make clean ; cd .. ); \
		echo "      exit: $$x"; \
		echo " "; \
	done
	@echo "======================================== clear ok ========================================"

