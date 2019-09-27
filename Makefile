####################################################################
#
#  Contents:     Makefile for the v1725 frontend for TUCAN
#
#
#####################################################################
#

# Path to gcc 4.8.1 binaries (needed to use new C++ stuff)
PATH := /home/ucn/packages/newgcc/bin:$(PATH)

USE_SYSTEM_BUFFER=1
SIMULATION=0
# Hardware setup (if not simulating)
NBLINKSPERA3818=1 #2 Number of optical links used per A3818
NBLINKSPERFE=1 #2 Number of optical links controlled by each frontend
NB1725PERLINK=1 # Number of daisy-chained v1720s per optical link
NBV1725TOTAL=1 #2 Number of v1720 boards in total

HWFLAGS = -DUSE_SYSTEM_BUFFER=$(USE_SYSTEM_BUFFER) \
-DNBLINKSPERA3818=$(NBLINKSPERA3818) -DNBLINKSPERFE=$(NBLINKSPERFE) \
-DNB1725PERLINK=$(NB1725PERLINK) -DNBV1725TOTAL=$(NBV1725TOTAL)

#--------------------------------------------------------------------
# The MIDASSYS should be defined prior the use of this Makefile
ifndef MIDASSYS
missmidas::
	@echo "...";
	@echo "Missing definition of environment variable 'MIDASSYS' !";
	@echo "...";
endif

#--------------------------------------------------------------------
# The following lines contain specific switches for different UNIX
# systems. Find the one which matches your OS and outcomment the 
# lines below.
#
# get OS type from shell
OSTYPE = $(shell uname)

#-----------------------------------------
# This is for Linux
ifeq ($(OSTYPE),Linux)
OSTYPE = linux
endif

ifeq ($(OSTYPE),linux)
#OS_DIR = linux-m64
OS_DIR = linux
OSFLAGS = -DOS_LINUX -DLINUX
CFLAGS = -g -Wall -pthread -I$(MIDASSYS)/include -DHAVE_LIBUSB -I$(MIDASSYS)/mscb -I$(MIDASSYS)/drivers/divers
LDFLAGS = -g -lm -lutil -lnsl -lpthread -lrt -lc 
endif

#-----------------------------------------
# optimize?

# CFLAGS += -O2

#-----------------------------------------
# ROOT flags and libs
#
ifdef ROOTSYS
ROOTCFLAGS := $(shell  $(ROOTSYS)/bin/root-config --cflags)
ROOTCFLAGS += -DHAVE_ROOT -DUSE_ROOT
ROOTLIBS   := $(shell  $(ROOTSYS)/bin/root-config --libs) -Wl,-rpath,$(ROOTSYS)/lib
ROOTLIBS   += -lThread
else
missroot:
	@echo "...";
	@echo "Missing definition of environment variable 'ROOTSYS' !";
	@echo "...";
endif

# Google perf tools
PERFLIBS=
ifdef GPROFILE
CXXFLAGS += -I/usr/local/include/google/ # -I/home/lindner/tools/gperftools-2.1/src
PERFLIBS += -lprofiler # /home/lindner/tools/gperftools-2.1/libprofiler.la
endif


#-------------------------------------------------------------------
# The following lines define directories. Adjust if necessary
#
CONET2_DIR   = $(HOME)/Andrew/CAENStuff
CAENCOMM_DIR = $(CONET2_DIR)/CAENComm-1.2
CAENCOMM_LIB = $(CAENCOMM_DIR)/lib/x86
CAENDGTZ_DIR = $(CONET2_DIR)/CAENDigitizer_2.7.9
CAENDGTZ_LIB = $(CAENDGTZ_DIR)/lib/x86_64
CAENVME_DIR  = $(CONET2_DIR)/CAENVMELib-2.50
CAENVME_LIB  = $(CAENVME_DIR)/lib/x86
MIDAS_INC    = $(MIDASSYS)/include
MIDAS_LIB    = $(MIDASSYS)/$(OS_DIR)/lib
MIDAS_SRC    = $(MIDASSYS)/src
MIDAS_DRV    = $(MIDASSYS)/drivers/vme
ROOTANA      = $(HOME)/packages/rootana

####################################################################
# Lines below here should not be edited
####################################################################
#
# compiler
CC   = gcc #-std=c99
CXX  = g++ -g -std=c++0x
#
# MIDAS library
LIBMIDAS=-L$(MIDAS_LIB) -lmidas
#
# CAENComm
LIBCAENCOMM=-L$(CAENCOMM_LIB) -lCAENComm
#
# CAENVME
LIBCAENVME=-L$(CAENVME_LIB) -lCAENVME
#
# CAENDigitizer
LIBCAENDGTZ =-L$(CAENDGTZ_LIB) -lCAENDigitizer
#
# All includes
MIDASINCS = -I. -I./include -I$(MIDAS_INC) -I$(MIDAS_DRV)
CAENINCS = -I$(CAENVME_DIR)/include -I$(CAENCOMM_DIR)/include -I$(CAENDGTZ_DIR)/include

DRIVERS         = hv.o multi.o null.o nulldev.o mscbdev.o mscb.o

####################################################################
# General commands
####################################################################

all: fe
	@echo "***** Finished"
	@echo "***** Use 'make doc' to build documentation"

fe : feoV1725.exe 

doc ::
	doxygen
	@echo "***** Use firefox --no-remote doc/html/index.html to view if outside gateway"

####################################################################
# Libraries/shared stuff
####################################################################

ov1725.o : $(MIDAS_DRV)/ov1720.c
	$(CC) -c $(CFLAGS) $(MIDASINCS) $(CAENINCS) $< -o $@ 

####################################################################
# Single-thread frontend
####################################################################

feoV1725.exe: $(MIDAS_LIB)/mfe.o  feoV1725.o ov1725.o v1725CONET2.o
	$(CXX) $(OSFLAGS) feoV1725.o v1725CONET2.o ov1725.o $(MIDAS_LIB)/mfe.o $(LIBMIDAS) $(LIBCAENCOMM) $(LIBCAENVME) $(LIBCAENDGTZ) -o $@ $(LDFLAGS)

feoV1725.o : feoV1725.cxx v1725CONET2.o
	$(CXX) $(CFLAGS) $(OSFLAGS) $(HWFLAGS) $(MIDASINCS) $(CAENINCS) -Ife -c $< -o $@

v1725CONET2.o : v1725CONET2.cxx
	$(CXX) $(CFLAGS) $(OSFLAGS) $(HWFLAGS) $(MIDASINCS) $(CAENINCS) -Ife -c $< -o $@


.c.o:


$(MIDAS_LIB)/mfe.o:
	@cd $(MIDASSYS) && make
####################################################################
# Clean
####################################################################

clean:
	rm -f *.o *.exe
	rm -f *~
	rm -rf html
	rm -rf stress


#end file
