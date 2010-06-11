GCC =   gcc
RM  =   rm -f
LN	=	ln -s
STRIP	= strip
GCCFLAG	= -Wall -ansi -pedantic -fPIC
LDFLAG	= -shared -Wl,-soname,$(FINAME) -lc -L../../base/linux -lmmp_base
INCLUDE	= -I../../base/
SRCDIR	= ..
OBJDIR	= .
BINDIR	= .
FINAME	= libdisml.so.1
LNNAME	= libdisml.so

OBJS   = dis_ops.o dis_parse.o dis_print.o
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
