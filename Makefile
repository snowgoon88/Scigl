# Makefile 
# Make according to Peter Miller's "Recursive Make Considered Harmful"
#                http://www.canb.auug.org.au/~millerp
# and to Emile van Bergen, http://www.xs4all.nl/~evbergen/nonrecursive-make.html
#

VERYCLEAN	:= *~

PROJET		:= SciGL_gamma
DIR_PROJET	:= essai_glfw_scigl

GLFW_HOME 	:= ../glfw-2.7
EIGEN_ROOT      := ../NeuralNetwork/eigen-eigen-65ee2328342f
DSOM_ROOT	:= ../NeuralNetwork/DSOM
### Build flags for all targets
#
PLATFORM	= $(shell uname)
CF_ALL          = -g -Wall
LF_ALL          = 
LL_ALL          =
MK_DYN_LIB	= -fPIC -shared

### Build tools
# 
CC              = ./build/ccd-g++
INST		= ./build/install
COMP            = $(CC) $(CF_ALL) $(CF_TGT) -o $@ -c $<
LINK            = $(CC) $(LF_ALL) $(LF_TGT) -o $@ $^ $(LL_TGT) $(LL_ALL)
COMPLINK        = $(CC) $(CF_ALL) $(CF_TGT) $(LF_ALL) $(LF_TGT) -o $@ $< $(LL_TGT) $(LL_ALL)
ARCH		= ar rcs $@ $^
DYN_LIB		= $(CC) $(MK_DYN_LIB) $(LF_ALL) $(LF_TGT) -o $@ $^ $(LL_TGT) $(LL_ALL)
GENERATE_DOC	= doxygen $^
TAR		= tar
### Standard parts
#
include rules.mk

