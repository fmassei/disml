GCC =   gcc
RM  =   rm -f
LN	=	ln -s
STRIP	= strip
GCCFLAG	= -Wall -ansi -pedantic -fPIC
LDFLAG	= -shared -Wl,-soname,$(FINAME) -lc -L../../base/linux -lmmp_base -L../../disml/linux -ldisml
INCLUDE	= -I../../base/ -I../../disml
SRCDIR	= ..
OBJDIR	= .
BINDIR	= .
FINAME	= libdisml_extra.so.1
LNNAME	= libdisml_extra.so

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
