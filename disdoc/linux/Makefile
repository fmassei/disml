GCC =   gcc
LD	=	gcc
RM  =   rm -f
AR	=	ar rcs
GCCFLAG	= -Wall -ansi -pedantic -fPIC
LDFLAG	= -ldisml -lmmp_base
INCLUDE	= -I.
SRCDIR	= ..
OBJDIR	= .
BINDIR	= .
FINAME	= p

OBJS   = disdoc_main.o disdoc_ops.o disdoc_to_html.o
OBJWD  = echo $(OBJDIR)/$(OBJS) | sed 's/\ /\ \.\//g'

base: all

%.o: $(SRCDIR)/%.c
	$(GCC) $(INCLUDE) $(GCCFLAG) -c $< -o $(OBJDIR)/$@

clean:
	-$(RM) $(OBJDIR)/*.o
	-$(RM) $(BINDIR)/$(FINAME)

all: $(OBJS)
	$(LD) $(LDFLAG) -o $(BINDIR)/$(FINAME) $(shell $(OBJWD))

debug: GCCFLAG += -g
debug: all
