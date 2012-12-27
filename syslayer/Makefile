#
# Copyright (c) 2001-2012 by PDFTron Systems Inc. All Rights Reserved.
#
ROOT := .

LINK	= g++ -o
CC	= g++ -c 
AR	= ar -cr
RM	= rm -rf

# Target
TARGETS	= $(TARGETS1) $(TARGETS2)
TARGETS1= libsyslayer.a
TARGETS2= test_syslayer

# OBJECT
OBJS	= $(OBJ_T1)
OBJ_T1	= sysinfod.o
OBJ_T2	= test.o

# DIR
BINDIR	= ../lib/

# CFLAGS
INCS	= -I ../include/
LIBS	= -L ../lib  -lsyslayer
OPTIONS	= -ftemplate-depth-100 -O2 -finline-functions -Wno-inline -Wall 
CFLAGS	= $(INCS) $(LIBS) $(OPTIONS)

# libs depend
LIBDIRS	=

all: $(TARGETS)

$(TARGETS1): libs $(OBJ_T1)
	$(AR)  $(BINDIR)$(TARGETS1)  $(OBJ_T1)

$(TARGETS2): libs $(OBJ_T2)
	$(LINK)  $(TARGETS2)  $(OBJ_T2) $(CFLAGS)

libs:
	@$(foreach BIND, $(LIBDIRS), \
            $(MAKE) -C $(BIND) --no-print-directory;)
.cpp.o	:
	$(CC)  $(INCS) $< 

clean:
	@$(foreach BIND, $(LIBDIRS), \
            $(MAKE) -C $(BIND) clean --no-print-directory;)
	@$(foreach T, $(TARGETS), \
            $(RM)  $(BINDIR)$(T);)
	-$(RM) $(OBJS) *.o *core


.PHONY: all clean dependents

