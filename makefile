#
# package & author info
#
packagename := kortex
description := base components of the kortex vision library developed by Aurvis R&D
major_version := 0
minor_version := 1
tiny_version  := 0
# version := major_version . minor_version # depracated
author := Engin Tola
licence := see licence.txt
#
# add your cpp cc files here
#
sources := \
log_manager.cc \
check.cc \
mem_manager.cc \
mem_unit.cc \
image.cc \
image_processing.cc \
image_conversion.cc \
image_io.cc \
image_io_pnm.cc \
image_io_png.cc \
image_io_jpg.cc \
string.cc \
fileio.cc \
message.cc \
math.cc

headers := \
log_manager.h \
check.h \
defs.h \
types.h \
mem_manager.h \
mem_unit.h \
image.h \
image_processing.h \
image_conversion.h \
image_io.h \
image_io_pnm.h \
image_io_png.h \
image_io_jpg.h \
string.h \
fileio.h \
message.h \
math.h


#
# output info
#
installdir := /home/tola/usr/local/kortex/
external_sources :=
external_libraries := libjpeg libpng
libdir := lib
srcdir := src
includedir:= include
#
# custom flags
#
define_flags :=
custom_ld_flags :=
custom_cflags := -std=c++0x
# -std=c++0x : to include file cstdint in types.h

#
# optimization & parallelization ?
#
optimize ?= false
parallelize ?= true
boost-thread ?= false
f77 ?= false
sse ?= true
multi-threading ?= false
profile ?= false
#........................................
specialize := true
platform := native
#........................................
compiler := g++
#........................................
include $(MAKEFILE_HEAVEN)/static-variables.makefile
include $(MAKEFILE_HEAVEN)/flags.makefile
include $(MAKEFILE_HEAVEN)/rules.makefile
