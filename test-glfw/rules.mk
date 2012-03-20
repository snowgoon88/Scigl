# Standard things
# saves the variable $(d) that holds the current directory on the stack,
# and sets it to the current directory given in $(dir), 
# which was passed as a parameter by the parent rules.mk

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

# Subdirectories, if any, in random order

#dir	:= $(d)/test
#include		$(dir)/Rules.mk

# Next, we create an immediately evaluated variable $(OBJS_common) 
# that holds all the objects in that directory.
# We also create $(DEPS_common), which lists any automatic dependency files 
# generated later on.
# To the global variable $(CLEAN), we add the files that the rules present here 
# may create, i.e. the ones we want deleted by a make clean command.)

# Local variables
#EIGEN_ROOT	:= ../eigen-eigen-65ee2328342f
#EIGEN_FLAGS	:= -I$(EIGEN_ROOT)

#SRC_FLAGS	:= -Isrc

# Local rules and target
CORE_HDR_$(d)	:= 

CORE_SRC_$(d)	:= $(d)/glfw-frame.cc \
                   $(d)/glfw-coord-points.cc \
                   $(d)/glfw-terminal.cc \
                   $(d)/glfw-scene-graph.cc \
                   $(d)/glfw-curve.cc \
                   $(d)/glfw-sliding.cc \
                   $(d)/glfw-plane-coord.cc \
                   $(d)/glfw-basis-cube.cc

#CORE_OBJS_$(d)	:= $(CORE_SRC_$(d):%.cc=%.o)

#CORE_DEPS_$(d)	:= $(CORE_OBJS_$(d):%=%.d)

TGTS_$(d)	:= $(CORE_SRC_$(d):%.cc=%)

DEPS_$(d)	:= $(TGTS_$(d):%=%.d)

TGT_BIN		:= $(TGT_BIN) verbose_$(d) $(TGTS_$(d))

TAR_SRC		:= $(TAR_SRC) $(CORE_SRC_$(d)) $(d)/rules.mk

CLEAN		:= $(CLEAN) $(TGTS_$(d)) $(DEPS_$(d))
VERYCLEAN	:= $(VERYCLEAN) $(d)/*~

# target local
# TGT_LIB		:= $(TGT_LIB) verbose_$(d) $(TGTS_$(d))

# Now we list the dependency relations relevant to the files in this subdirectory.
# Most importantly, while generating the objects listed here, we want to set 
# the target-specific compiler flags $(CF_TGT) to include a flag that adds 
# this directory to the include path, so that local headers may be included 
# using #include <localfile.h>, which is, as said, more portable 
# than local includes when the source directory is not the current directory.)


# Local CFLAGS
# Force remake if rules are changed
#$(CORE_OBJS_$(d)):	$(d)/rules.mk

# $(CORE_OBJS_$(d)): 	CF_TGT := -I$(d) $(EIGEN_FLAGS) -Wno-deprecated

# Local libs
# Force remake if rules are changed
#$(TGTS_$(d)): $(d)/rules.mk

$(TGTS_$(d)):	CF_TGT := -Iscigl -I/opt/local/include \
                          -I$(GLFW_HOME)/include

$(TGTS_$(d)):	LF_TGT := -framework OpenGL -framework GLUT -framework Cocoa \
                          -L/opt/local/lib \
                          -lGLEW
#                         $(GDK_LIBS) $(GTK_LIBS) \
# 			  $(UTILS_LIBS) \
# 	                  $(VID_LIBS) $(GST_LIBS) $(OPENCV_LIBS) \
# 	                  -lgthread-2.0 -lboost_thread

$(TGTS_$(d)):	LL_TGT := $(S_LL_INET) scigl/libscigl.a \
                          $(GLFW_HOME)/lib/cocoa/libglfw.a


#$(CORE_OBJS_$(d)):	CF_TGT := -I. -I$(d) $(shell pkg-config --cflags gtkmm-2.4)
#$(OBJS_$(d)):	LF_TGT := -lgsl -lgslcblas -lm 
#$(OBJS_$(d)):	LL_TGT := $(S_LL_INET) cpp/core.a dana-cpp/dana.a

$(d)/glfw-frame: $(d)/glfw-frame.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	

$(d)/glfw-coord-points: $(d)/glfw-coord-points.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	

$(d)/glfw-terminal: $(d)/glfw-terminal.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	

$(d)/glfw-scene-graph: $(d)/glfw-scene-graph.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	

$(d)/glfw-curve: $(d)/glfw-curve.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	

$(d)/glfw-sliding: $(d)/glfw-sliding.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	

$(d)/glfw-plane-coord: $(d)/glfw-plane-coord.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	

$(d)/glfw-basis-cube: $(d)/glfw-basis-cube.cc scigl/libscigl.a
	@echo "===== Compiling and Linking $@"
	$(COMPLINK)	


.PHONY : verbose_$(d)
verbose_$(d): $(TGTS_$(d))
	@echo "**** Generating $^"

# As a last step, we restore the value of the current directory variable $(d) 
# by taking it from the current location on the stack, and we "decrement" 
# the stack pointer by stripping away the .x suffix 
# we added at the top of this file.

# Standard things

-include	$(CORE_DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

