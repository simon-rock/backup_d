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
TARGETS1= mysql_test

# OBJECT
OBJS	= $(OBJ_T1)
OBJ_T1	= mysqlplus.o main.o 

# DIR
BINDIR	= ../bin/

# CFLAGS
INCS	= -I ../include/
LIBS	= -L/usr/lib64/mysql/ -lmysqlclient
OPTIONS	= -ftemplate-depth-100 -O2 -finline-functions -Wno-inline -Wall 
CFLAGS	= $(INCS) $(LIBS) $(OPTIONS)

# libs depend
LIBDIRS	=

all: $(TARGETS)

$(TARGETS1): $(OBJ_T1) libs
	$(LINK) -o $(BINDIR)mysql_test  $(OBJ_T1) $(CFLAGS) 

libs:
	@$(foreach BIND, $(LIBDIRS), \
            $(MAKE) -C $(BIND) --no-print-directory;)
.cpp.o	:
	$(CC)  $(INCS) $< 

clean:
	@$(foreach BIND, $(LIBDIRS), \
            $(MAKE) -C $(BIND) clean --no-print-directory;)
	-$(RM) $(OBJS) $(TARGETS) *.o *core

.PHONY: all clean dependents

