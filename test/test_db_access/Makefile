#
# Copyright (c) 2001-2012 by PDFTron Systems Inc. All Rights Reserved.
#
ROOT := .

LINK	= g++
CC	= g++ -c -g -DDEBUG
AR	= ar -cr
RM	= rm -rf

# Target
TARGETS	= $(TARGETS1)
TARGETS1= test_db_access

# OBJECT
OBJS	= $(OBJ_T1)
OBJ_T1	= main.o

# DIR
BINDIR	= ../../bin/

# CFLAGS
INCS	= -I ../../include/
#LIBS	= -L ../../lib  -lsystools -ldb_access  -L/usr/lib64/mysql/ -lmysqlclient #depend error
LIBS	= -L ../../lib  -ldb_access -lsystools -L/usr/lib64/mysql/ -lmysqlclient
OPTIONS	= -ftemplate-depth-100 -O2 -finline-functions -Wno-inline -Wall 
CFLAGS	= $(INCS) $(LIBS) $(OPTIONS)

# libs depend
LIBDIRS	= ../../systools\
	  ../../db_access
all: $(TARGETS)

$(TARGETS1): libs $(OBJ_T1) 
	$(LINK) -o $(BINDIR)$(TARGETS1)  $(OBJ_T1) $(CFLAGS)
#	$(AR)  $(BINDIR)$(TARGETS1)  $(OBJ_T1)

libs:
	@$(foreach BIND, $(LIBDIRS), \
            $(MAKE) -C $(BIND) --no-print-directory;)
.cpp.o	:
	$(CC)  $(INCS) $(LIBS) $< 

clean:
	@$(foreach BIND, $(LIBDIRS), \
            $(MAKE) -C $(BIND) clean --no-print-directory;)
	-$(RM) $(OBJS) $(TARGETS) *.o *core

.PHONY: all clean dependents

