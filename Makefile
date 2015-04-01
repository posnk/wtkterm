#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#
TARGET=i386-pc-posnk

# define the C compiler to use
CC = @echo " [  CC  ]	" $< ; $(TARGET)-gcc
LD = @echo " [  LD  ]	" $@ ; $(TARGET)-gcc

# define the program name and path
PROGNAME=wtkterm
PROGPATH=/usr/bin/wtkterm

# define any compile-time flags
CFLAGS = -Wall -g `pkg-config --cflags --libs cairo`

# define any directories containing header files other than /usr/include
#
INCLUDES = -Iinclude

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = 

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lwtk -lwtkmurrine -lbmp -lclara `pkg-config --cflags --libs cairo` -lpixman-1 -lm -lfontconfig -lfreetype -lexpat -lpng -lz

$(BUILDDIR)vterm:
	mkdir $(BUILDDIR)vterm

# define the C source files
OBJS = $(BUILDDIR)vterm/vterm.o \
	$(BUILDDIR)vterm/vterm_colors.o \
	$(BUILDDIR)vterm/vterm_csi.o \
	$(BUILDDIR)vterm/vterm_csi_CUP.o \
	$(BUILDDIR)vterm/vterm_csi_CUx.o \
	$(BUILDDIR)vterm/vterm_csi_DCH.o \
	$(BUILDDIR)vterm/vterm_csi_DECSTBM.o \
	$(BUILDDIR)vterm/vterm_csi_DL.o \
	$(BUILDDIR)vterm/vterm_csi_ECH.o \
	$(BUILDDIR)vterm/vterm_csi_ED.o \
	$(BUILDDIR)vterm/vterm_csi_EL.o \
	$(BUILDDIR)vterm/vterm_csi_ICH.o \
	$(BUILDDIR)vterm/vterm_csi_IL.o \
	$(BUILDDIR)vterm/vterm_csi_RESTORECUR.o \
	$(BUILDDIR)vterm/vterm_csi_SAVECUR.o \
	$(BUILDDIR)vterm/vterm_csi_SGR.o \
	$(BUILDDIR)vterm/vterm_dec_RM.o \
	$(BUILDDIR)vterm/vterm_dec_SM.o \
	$(BUILDDIR)vterm/vterm_erase.o \
	$(BUILDDIR)vterm/vterm_escape.o \
	$(BUILDDIR)vterm/vterm_misc.o \
	$(BUILDDIR)vterm/vterm_render.o \
	$(BUILDDIR)vterm/vterm_resize.o \
	$(BUILDDIR)vterm/vterm_scroll.o \
	$(BUILDDIR)vterm/vterm_tty.o \
	$(BUILDDIR)vterm/vterm_write.o \
	$(BUILDDIR)wtkterm.o 

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
all:	$(BUILDDIR)$(PROGNAME)

$(BUILDDIR)$(PROGNAME): $(BUILDDIR)vterm $(OBJS)
	$(LD) $(LFLAGS) $(LIBS) -o $(BUILDDIR)$(PROGNAME) $(OBJS)

install: $(BUILDDIR)$(PROGNAME)
	install $(BUILDDIR)$(PROGNAME) $(DESTDIR)$(PROGPATH)

.PHONY: depend clean

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
$(BUILDDIR)%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) $(BUILDDIR)*.o $(BUILDDIR)*~ $(BUILDDIR)$(PROGNAME)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

