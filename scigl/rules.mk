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

# Local rules and target
CORE_HDR_$(d)	:= $(d)/axis-ranged.h $(d)/axis.h $(d)/basis-cube.h $(d)/cloud.h \
                   $(d)/colormap.h $(d)/convert.h $(d)/cube.h $(d)/curve.h \
                   $(d)/data.h $(d)/font.h $(d)/font_12.h $(d)/font_16.h \
                   $(d)/font_24.h $(d)/font_32.h $(d)/frame.h $(d)/line.h \
                   $(d)/object.h $(d)/plane-coord.h $(d)/scene.h $(d)/scene-graph.h $(d)/segment.h \
                   $(d)/shapes.h $(d)/terminal.h $(d)/textbox.h $(d)/trackball.h \
                   $(d)/vec4f.h $(d)/widget.h

CORE_SRC_$(d)	:= $(d)/axis-ranged.cc $(d)/axis.cc $(d)/basis-cube.cc \
                   $(d)/cloud.cc $(d)/colormap.cc $(d)/cube.cc $(d)/curve.cc \
                   $(d)/data.cc $(d)/font.cc \
                   $(d)/frame.cc $(d)/line.cc \
                   $(d)/object.cc $(d)/plane-coord.cc $(d)/scene.cc $(d)/scene-graph.cc \
                   $(d)/segment.cc $(d)/shapes.cc $(d)/terminal.cc $(d)/textbox.cc \
                   $(d)/trackball.cc $(d)/widget.cc

CORE_OBJS_$(d)	:= $(CORE_SRC_$(d):%.cc=%.o)

CORE_DEPS_$(d)	:= $(CORE_OBJS_$(d):%=%.d)

TGTS_$(d)	:= $(d)/libscigl.a

DEPS_$(d)	:= $(TGTS_$(d):%=%.d)

#TGT_BIN		:= $(TGT_BIN) verbose_$(d) $(TGTS_$(d))
#TGT_BIN		:= $(TGT_BIN) verbose_$(d) $(CORE_OBJS_$(d))

TAR_SRC		:= $(TAR_SRC) $(CORE_SRC_$(d)) $(CORE_HDR_$(d)) $(d)/rules.mk

CLEAN		:= $(CLEAN) $(CORE_OBJS_$(d)) $(CORE_DEPS_$(d)) \
                   $(DEPS_$(d))
VERYCLEAN	:= $(VERYCLEAN) $(TGTS_$(d)) $(d)/*~

TAG_FILES	:= $(TAG_FILES) $(CORE_SRC_$(d)) $(CORE_SRC_$(d)) $(CORE_HDR_$(d))

# target local
TGT_LIB		:= $(TGT_LIB) verbose_$(d) $(TGTS_$(d))

# Now we list the dependency relations relevant to the files in this subdirectory.
# Most importantly, while generating the objects listed here, we want to set 
# the target-specific compiler flags $(CF_TGT) to include a flag that adds 
# this directory to the include path, so that local headers may be included 
# using #include <localfile.h>, which is, as said, more portable 
# than local includes when the source directory is not the current directory.)


# Local CFLAGS
# Force remake if rules are changed
#$(CORE_OBJS_$(d)):	$(d)/rules.mk

ifeq ($(PLATFORM), Darwin)
$(CORE_OBJS_$(d)): 	CF_TGT := -I$(d) -I/opt/local/include -Wno-deprecated
else
$(CORE_OBJS_$(d)): 	CF_TGT := -I$(d)  -Wno-deprecated
endif
# Local libs
# Force remake if rules are changed
#$(TGTS_$(d)): $(d)/rules.mk
#$(TGTS_$(d)):	LF_TGT := 

# $(TGTS_$(d)):	LL_TGT := $(GDK_LIBS) $(GTK_LIBS) \
# 			  $(UTILS_LIBS) \
# 	                  $(VID_LIBS) $(GST_LIBS) $(OPENCV_LIBS) \
# 	                  -lgthread-2.0 -lboost_thread



#$(CORE_OBJS_$(d)):	CF_TGT := -I. -I$(d) $(shell pkg-config --cflags gtkmm-2.4)
#$(OBJS_$(d)):	LF_TGT := -lgsl -lgslcblas -lm 
#$(OBJS_$(d)):	LL_TGT := $(S_LL_INET) cpp/core.a dana-cpp/dana.a

$(d)/libscigl.a:	$(CORE_OBJS_$(d))
	$(ARCH)

# $(d)/test_neurone:	$(d)/test_neurone.cc $(CORE_OBJS_$(d)) 
#			$(COMPLINK)

.PHONY : verbose_$(d)
verbose_$(d): 
	@echo "======== Compiling in scigl --> libscigl.a"

# As a last step, we restore the value of the current directory variable $(d) 
# by taking it from the current location on the stack, and we "decrement" 
# the stack pointer by stripping away the .x suffix 
# we added at the top of this file.

# Standard things

-include	$(CORE_DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

