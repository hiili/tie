# Usage: gmake cmode=<cmode>


include ../../Makefile.common


# Dirs ------------------------------------------
bindir           = ..
srcdir           = .
objdir           = .
incdirs          = ../../include $(incdirs_common) \
    $(LIFESPACE_BASE_DIR)/include
libdirs          = ../../lib $(libdirs_common) \
    $(LIFESPACE_BASE_DIR)/lib


# Source files ----------------------------------
sources_common   = \
    TestWorld.cpp \
    Body.cpp \

sources_glow     = \
    main_glow.cpp \

sources_vrj      = \
    main_vrj.cpp \


# Main target -----------------------------------
MAINTARGET       = $(bindir)/bodytest_$(cmode)








### ------------------------------------------------------------- ###
### --- No changes from here on!
### ---   (contains: standard targets, linking,
### ---              compiling and dependency automation)
### ------------------------------------------------------------- ###




### mode selection
### ------------------------------------------------------------- ###
libs            := $(libs_common_begin) $(libs_$(cmode)) $(libs_common_end)
DEFS            := $(DEFS_common) $(DEFS_$(cmode))
sources         := $(sources_common) $(sources_$(cmode))


### variable reformatting
### ------------------------------------------------------------- ###
objects          = $(sources:%.cpp=$(objdir)/%.o)
deps             = $(objects:.o=.d)
ifeq ($(UNAME),Cygwin)
MAINTARGET      := $(MAINTARGET).exe
endif


### standard targets
### ------------------------------------------------------------- ###
.PHONY: all cleanbin cleandeps cleanobj clean

all: $(MAINTARGET)
cleanbin:
	rm -f $(MAINTARGET)
cleandeps:
	rm -f $(deps)
cleanobj:
	rm -f $(objects)
clean: cleanbin cleandeps cleanobj




### linking, compiling and dependency automation
### ------------------------------------------------------------- ###
$(MAINTARGET): $(objects)
	#
	# --------   Linking the final target $@   --------
	$(CXX) $(LINKFLAGS) -o $@ $(objects) $(libdirs:%=-L%) $(libs:%=-l%)

ifneq ($(MAKECMDGOALS),clean)
include $(deps)
endif

$(objdir)/%.o: $(srcdir)/%.cpp
	#
	# --------   Compiling object $@   --------
	$(CXX) $(CPPFLAGS) $(DEFS:%=-D%) $(incdirs:%=-I%) -c -o $@ $<

$(objdir)/%.d: $(srcdir)/%.cpp
	#
	# --------   Generating dependencies for $@   --------
	$(DEPCC) $(DEFS:%=-D%) $(incdirs:%=-I%) $< \
	  -MM -MT $@ -MT $(basename $@).o >$@
