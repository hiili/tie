include ../../../Makefile.common


# Check the environment variables ---------------
ifneq ($(MAKECMDGOALS),clean)
ifndef LIFESPACE_BASE_DIR
$(error Environment variable LIFESPACE_BASE_DIR must be set!)
endif
ifndef VJ_BASE_DIR
$(error Environment variable VJ_BASE_DIR must be set!)
endif
endif


# Dirs ------------------------------------------
bindir           = .
srcdir           = .
objdir           = .
incdirs          = $(incdirs_common) \
                   $(LIFESPACE_BASE_DIR)/include $(VJ_BASE_DIR)/include

# Defines ---------------------------------------
DEFS             = $(DEFS_common)

# Flags -----------------------------------------
CPPFLAGS         = $(CPPFLAGS_common)


# Source files ----------------------------------
sources          = \
    VRJDevice.cpp \
    VRJViewport.cpp \


# Main target -----------------------------------
MAINTARGET       = $(bindir)/liblifespacevrjuggler.a








### ------------------------------------------------------------- ###
### --- No changes from here on!
### ---   (contains: standard targets, linking,
### ---              compiling and dependency automation)
### ------------------------------------------------------------- ###




### variable reformatting
### ------------------------------------------------------------- ###
objects          = $(sources:%.cpp=$(objdir)/%.o)
deps             = $(objects:.o=.d)


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
	# --------   Creating the final target $@   --------
	rm -f $@
	$(AR_CMD) $@ $(objects)

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
