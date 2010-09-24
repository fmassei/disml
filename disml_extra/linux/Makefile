GCC =   gcc
RM  =   rm -f
LN	=	ln -sf
STRIP	= strip
INSTALL = install
GCCFLAG	= -Wall -ansi -pedantic -fPIC
LDFLAG	= -shared -Wl,-soname,$(FINAME) -lc -lmmp_base -ldisml
INCLUDE	= -I.
SRCDIR	= ..
OBJDIR	= .
BINDIR	= .
FINAME	= libdisml_extra.so.0.1
LNNAME	= libdisml_extra.so.0
DVNAME	= libdisml_extra.so

OBJS   = dis_to_xml.o dis_to_json.o
OBJWD  = echo $(OBJDIR)/$(OBJS) | sed 's/\ /\ \.\//g'

base: all

%.o: $(SRCDIR)/%.c
	$(GCC) $(INCLUDE) $(GCCFLAG) -c $< -o $(OBJDIR)/$@

clean:
	-$(RM) $(OBJDIR)/*.o
	-$(RM) $(BINDIR)/$(FINAME)
	-$(RM) $(BINDIR)/$(LNNAME)

main: $(OBJS)
	$(GCC) $(shell $(OBJWD)) $(LDFLAG) -o $(BINDIR)/$(FINAME)
	-$(LN) $(BINDIR)/$(FINAME) $(BINDIR)/$(LNNAME)

all: main
	$(STRIP) $(FINAME)

debug: GCCFLAG += -g
debug: main

install:
	$(INSTALL) -m 755 -d /usr/include/disml
	$(INSTALL) -m 644 $(SRCDIR)/*.h /usr/include/disml
	$(INSTALL) -m 644 $(FINAME) /usr/lib/
	$(LN) /usr/lib/$(FINAME) /usr/lib/$(LNNAME)
	$(LN) /usr/lib/$(LNNAME) /usr/lib/$(DVNAME)
