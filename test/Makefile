# Usage: gmake [<target>] [cmode=<cmode>]


include ../Makefile.common


# list of tests ---------------------------------
tests            = \
    startup \
    testi \
    multicam \
    avatarbasic \
    avatarcollisiontest \
    bodytest \
	imworld \
    spawnertest \

# list of compilation modes ---------------------
cmodes           = \
    glow \
    vrj \

# default compilation mode ----------------------
cmode_def        = glow








### ------------------------------------------------------------- ###
### --- No changes from here on!
### ------------------------------------------------------------- ###
.PHONY: all clean cleanbin




### check the compilation mode
### ------------------------------------------------------------- ###
ifeq (,$(cmode))
$(warning No compilation mode given, using $(cmode_def)..)
cmode           = $(cmode_def)
endif

ifneq ($(cmode),$(strip $(foreach id,$(cmodes),$(findstring $(id),$(cmode)))))
$(error Unknown compilation mode $(cmode))
endif




### init some variables
### ------------------------------------------------------------- ###
ifeq ($(UNAME),Cygwin)
all_targets      = $(tests:%=%_$(cmode).exe)
else
all_targets      = $(tests:%=%_$(cmode))
endif


### default target
### ------------------------------------------------------------- ###
all: $(all_targets)


### cleanup
### ------------------------------------------------------------- ###
clean: cleanbin
	for test in $(tests:%=%_src) ; do \
	  $(MAKE) -C $$test cmode=$(cmode) clean ; \
	done

cleanbin:
	rm -f $(all_targets)


### compiling
### ------------------------------------------------------------- ###
%:
	$(MAKE) -C $(firstword $(subst _, ,$@))_src cmode=$(cmode)
