#
# Makefile for hexameter
# @author Takahide Matsutsuka <markn@markn.org>
#
# $Id: Makefile,v 1.3 2009/12/15 07:13:14 matsutsuka Exp $
#

#CFLAGS		= -std=c99 -g -mno-cygwin -Wall
CFLAGS		= -g -Wall
SOURCEDIR	= src
SOURCES		= ota_prepare.c
OBJECTDIR	= bin
OBJECTS		= ${addprefix $(OBJECTDIR)/,$(SOURCES:.c=.o)}

vpath %.c $(SOURCEDIR)

ota_prepare: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

clean:
	rm -rf bin
	rm -f *~ *.stackdump
	rm -f *~ ota_prepare

$(OBJECTDIR):
	mkdir $@

$(OBJECTDIR)/%.o: %.c | $(OBJECTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

