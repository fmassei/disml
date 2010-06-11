GCC =   gcc
LD	=	gcc
RM  =   rm -f
AR	=	ar rcs
GCCFLAG	= -Wall -ansi -pedantic -fPIC
LDFLAG	= -L../../disml/linux -ldisml `pkg-config --libs gtk+-2.0`
INCLUDE	= -I../../deps/include/mmp/ -I../../disml `pkg-config --cflags gtk+-2.0`
SRCDIR	= ..
OBJDIR	= .
BINDIR	= .
FINAME	= disgtk

OBJS   = disgtk.o disgtk_treeview.o
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
