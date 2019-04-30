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
filter.cc \
mem_manager.cc \
mem_unit.cc \
image.cc \
image_processing.cc \
image_conversion.cc \
image_io.cc \
image_io_pnm.cc \
image_io_png.cc \
image_io_jpg.cc \
image_paint.cc \
sse_extensions.cc \
string.cc \
fileio.cc \
message.cc \
color.cc \
minmax.cc \
math.cc \
progress_bar.cc \
random.cc \
rect2.cc \
linear_algebra.cc \
matrix.cc \
kmatrix.cc \
rotation.cc \
svd.cc \
sorting.cc \
timer.cc \
eigen_conversion.cc \
option_parser.cc \
object_cache.cc \
color_map.cc \
sparse_array_t.cc \
indexed_array.cc \
histogram.cc \
pair_indexed_array.cc \
sorted_pair_map.cc \
geometry.cc \
random_generator.cc

headers := \
log_manager.h \
bit_operations.h \
check.h \
defs.h \
filter.h \
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
image_paint.h \
sse_extensions.h \
string.h \
fileio.h \
message.h \
color.h \
minmax.h \
math.h \
progress_bar.h \
random.h \
indexed_types.h \
rect2.h \
linear_algebra.h \
matrix.h \
kmatrix.h \
rotation.h \
lapack_externs.h \
svd.h \
sorting.h \
timer.h \
eigen_conversion.h \
option_parser.h \
object_cache.h \
color_map.h \
sparse_array_t.h \
indexed_array.h \
histogram.h \
keyed_value.h \
heap.h \
heap.tcc \
pair_indexed_array.h \
sorted_pair_map.h \
geometry.h \
random_generator.h


#
# output info
#
main_build_dir := ${HOME}/build/
installdir := ${HOME}/usr/local/
external_sources :=
external_libraries := libjpeg lapack blas libpng
# external_libraries := libjpeg libpng12 eigen3
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
optimize ?= true
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
