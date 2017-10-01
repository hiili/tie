include ../Makefile.common


# Dirs ------------------------------------------
bindir           = .
srcdir           = .
objdir           = .
incdirs          = $(incdirs_common) $(LIFESPACE_BASE_DIR)/include \
                   $(VJ_BASE_DIR)/include \
                   $(FLUID_ROOT)/include \
                   $(MUSTAJUURI_PATH)/include


# Defines ---------------------------------------
DEFS             = $(DEFS_common) $(DEFS_glow)


# Source files ----------------------------------
sources          = \
    UserInterface.cpp \
    GestureHandler.cpp \
    MotionHandler.cpp \
    constants.cpp \
    TIEMaterials.cpp \
    CylinderVisual.cpp \
    Torso.cpp \
    Head.cpp \
    Hand.cpp \
    ODEHand.cpp \
    Arm.cpp \
    Finger.cpp \
    ODEFinger.cpp \
    FluidSkeleton.cpp \
    ODESkeleton.cpp \
    SensorController.cpp \
    GloveController.cpp \
    Avatar.cpp \
    BowlingAlley.cpp \
    BowlingPin.cpp \
    Spawner.cpp \
    BallSpawner.cpp \
    CubeSpawner.cpp \
    BVDSpawner.cpp \
    Seesaw.cpp \
#    <source1.cpp> \
#    <source2.cpp> \
#    <sourceN.cpp> \

# Main target -----------------------------------
MAINTARGET       = $(bindir)/libtie.a








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
