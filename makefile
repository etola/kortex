#automatically generated makefile
packagename := kortex
major_version := 0
minor_version := 1
tiny_version := 0
author := Engin Tola
description := base components of the kortex vision library developed by Aurvis R&D
licence := see licence.txt
#........................................
installdir := /home/tola/usr/local/
external_sources :=
external_libraries := libjpeg lapack blas libpng
libdir := lib
srcdir := src
includedir:= include
define_flags := -DWITH_LIBPNG -DWITH_LIBJPEG -DWITH_LAPACK -DWITH_LAPACK -DWITH_SSE
#........................................
optimize := true
parallelize := true
f77 := false
sse := true
multi-threading := false
profile := false
#........................................
specialize := true
platform := native
#........................................
sources := log_manager.cc check.cc filter.cc mem_manager.cc mem_unit.cc image.cc image_processing.cc image_conversion.cc image_io.cc image_io_pnm.cc image_io_png.cc image_io_jpg.cc image_paint.cc sse_extensions.cc string.cc fileio.cc message.cc color.cc minmax.cc math.cc progress_bar.cc random.cc rect2.cc linear_algebra.cc matrix.cc kmatrix.cc rotation.cc svd.cc sorting.cc timer.cc eigen_conversion.cc option_parser.cc object_cache.cc color_map.cc sparse_array_t.cc indexed_array.cc histogram.cc pair_indexed_array.cc sorted_pair_map.cc geometry.cc random_generator.cc bit_operations.cc

#........................................

define_flags := -DWITH_LIBPNG -DWITH_LIBJPEG -DWITH_LAPACK -DWITH_LAPACK -DWITH_SSE
custom_ld_flags := -lstdc++fs
custom_cflags := -std=c++0x
#........................................
################################################################################
################# - MAKEFILE STATIC VARIABLES - ################################
################################################################################

ifeq ($(major_version),)
 major_version := 0
endif
ifeq ($(minor_version),)
 minor_version := 1
endif
ifeq ($(tiny_version),)
 tiny_version := 0
endif

ifeq ($(version),)
 version := $(major_version)"."$(minor_version)"."$(tiny_version)
endif

# used when exporting standalone makefile
packagename_o  := $(packagename)
sources_list_o := ${sources}
libdir_o       := ${libdir}
srcdir_o       := ${srcdir}

#
path_after_home = $(shell echo ${PWD} | cut -d'/' -f4-)
ifneq ($(main_build_dir),)
  build_dir := ${main_build_dir}${path_after_home}
else
  build_dir := ./
endif


ifeq ($(optimize),false)
  packagename := $(packagename)d
  outdir := ${build_dir}/debug/
  l_outdir := debug
else
  outdir := ${build_dir}/release/
  l_outdir := release
endif

ifneq (${build_dir},./)
RM_STATE    := $(shell rm ${l_outdir} )
_LINK_STATE := $(shell ln -sf ${outdir} ${l_outdir})
endif


srcdir := ${srcdir}/

sources_list = $(addprefix $(srcdir), $(sources))

objectfiles       := $(filter %.o,$(subst   .c,.o,$(sources)))
objectfiles       += $(filter %.o,$(subst  .cc,.o,$(sources)))
objectfiles       += $(filter %.o,$(subst .cpp,.o,$(sources)))

objects = $(addprefix $(outdir), $(objectfiles))

dependencies  := $(subst .o,.d,$(objects))

depdir := $(dir ${dependencies})

bindir        := ${outdir}bin/
libdir        := ${outdir}${libdir}
libname       := lib$(packagename)
libtarget     := $(libdir)/$(libname).a
libsoname     := $(libname).so.$(major_version)
librealname   := $(libdir)/$(libname).so.$(version)
exetarget     := ${bindir}$(packagename)
pkgconfigfile := $(packagename).pc

automakefile := make.auto
commentfile  := makefile.comment

tag_file:=TAGS
tag_generator:='$(MAKEFILE_HEAVEN)/tags.sh'
tag_depends:=${external_libraries}
tag_src := ${includedir}/*.h ${includedir}/${packagename}/*.h		\
${includedir}/*.tcc ${includedir}/${packagename}/*.tcc ${srcdir}*.cpp	\
${srcdir}*.cc $(srcdir)*.c

ifeq ($(compiler),)
 compiler := g++
endif
CXX := ${compiler}

curpath=`pwd -P`

REQUIRED_DIRS = ${outdir} ${libdir} ${depdir} ${bindir}

################################################################################
########################### - MAKEFILE FLAGS - #################################
################################################################################

ARFLAGS = ruv
CTAGFLAGS := -e -R --languages=c++,c

#
#
#
ifneq (,$(findstring libpng,$(external_libraries)))
  define_flags += -DWITH_LIBPNG
endif
ifneq (,$(findstring libjpeg,$(external_libraries)))
  define_flags += -DWITH_LIBJPEG
endif
ifneq (,$(findstring opencv,$(external_libraries)))
  define_flags += -DWITH_OPENCV
endif
ifneq (,$(findstring blas,$(external_libraries)))
  define_flags += -DWITH_LAPACK
endif
ifneq (,$(findstring lapack,$(external_libraries)))
  define_flags += -DWITH_LAPACK
endif
ifneq (,$(findstring eigen3,$(external_libraries)))
  define_flags += -DWITH_EIGEN
endif

ifeq ($(optimize),false)
  external_libraries := $(patsubst argus,argusd,$(external_libraries))
  external_libraries := $(patsubst cosmos,cosmosd,$(external_libraries))
  external_libraries := $(patsubst kutility,kutilityd,$(external_libraries))
  external_libraries := $(patsubst karpet,karpetd,$(external_libraries))
  external_libraries := $(patsubst daisy,daisyd,$(external_libraries))
  external_libraries := $(patsubst kortex,kortexd,$(external_libraries))
  external_libraries := $(patsubst mrf,mrfd,$(external_libraries))
  external_libraries := $(patsubst kortex-ext-advanced,kortex-ext-advancedd,$(external_libraries))
  external_libraries := $(patsubst kortex-ext-opencv,kortex-ext-opencvd,$(external_libraries))
  external_libraries := $(patsubst kortex-ext-dronedeploy,kortex-ext-dronedeployd,$(external_libraries))
  external_libraries := $(patsubst kortex-ext-fusion,kortex-ext-fusiond,$(external_libraries))
  external_libraries := $(patsubst kortex-shared,kortex-sharedd,$(external_libraries))
external_libraries := $(patsubst strekker,strekkerd,$(external_libraries))
  external_libraries := $(patsubst ceres,ceres,$(external_libraries))
  external_libraries := $(patsubst coldet,coldetd,$(external_libraries))
endif

# makes gcc stop at the first error
# CXXFLAGS += -Wfatal-errors
CXXFLAGS += -fmax-errors=5

CXXFLAGS += ${define_flags} -I$(includedir)

ifneq ($(external_sources),)
 CXXFLAGS += `pkg-config --cflags ${external_sources}`
endif

ifneq ($(external_libraries),)
 CXXFLAGS += `pkg-config --cflags ${external_libraries}`
 LDFLAGS  += `pkg-config --cflags-only-other --libs ${external_libraries}`
endif

ifneq ($(external_libraries_static),)
 CXXFLAGS += `pkg-config --cflags ${external_libraries_static}`
 LDFLAGS  += `pkg-config --static --cflags-only-other --libs ${external_libraries_static}`
endif


# rdynamic lets to have backtraces
CXXFLAGS += -rdynamic

CXXFLAGS += ${custom_cflags}
LDFLAGS  += ${custom_ld_flags}

ifneq (,$(findstring libpng,$(external_libraries)))
  LDFLAGS += -lz
endif

ifeq ($(f77),true)
 LDFLAGS += -lg2c
endif

ifeq ($(boost-thread),true)
 LDFLAGS += -lboost_thread-mt
endif

LDFLAGS += -Wl,-rpath=./lib/

ifeq ($(sse),true)
    define_flags += -DWITH_SSE
    CXXFLAGS += -msse -msse2 -msse4.2
    CPPFLAGS += -msse -msse2 -msse4.2
endif

CXXFLAGS += -fno-strict-aliasing -Wall -fPIC

ifeq ($(multi-threading),true)
    CXXFLAGS += -lpthread
endif

ifeq ($(profile),true)
  CXXFLAGS+= -pg
endif


ifeq ($(optimize),true)
  CXXFLAGS += -O3 -DNDEBUG -DHAVE_INLINE
  spc_flags = -march=$(platform) -mfpmath=sse
  ifeq ($(specialize),true)
     CXXFLAGS += $(spc_flags)
  endif
else
  CXXFLAGS += -g
  parallelize = false
  profile = true
endif

ifeq ($(parallelize),true)
   CXXFLAGS += -fopenmp
endif

is_debug := $(wildcard .debug)

ifeq ($(strip $(is_debug)),)
  is_debug = true
else
  is_debug = false
endif

ifeq ($(optimize),true)
  state_file = .release
else
  state_file = .debug
endif

#
## -rdynamic: lets meaningful backtrace messagas.
#
# CXXFLAGS += -ffast-math -rdynamic  ${define_flags} -I$(includedir) ${custom_cflags}
################################################################################
################################ - MAKEFILE RULES - ############################
################################################################################

_MKDIRS := $(shell mkdir -p ${REQUIRED_DIRS})


.PHONY       : $(exetarget)
$(exetarget) : ${objects}
	@echo compiler path = ${compiler}
	@echo
	@echo ------------------ making executable
	@echo
	$(compiler) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

.PHONY : flags
flags :
	@echo
	@echo ------------------ build flags
	@echo
	@echo ldflags  = $(LDFLAGS)
	@echo
	@echo cxxflags = $(CXXFLAGS)
	@echo
	@echo source_list = ${sources_list}
	@echo
	@echo objects = ${objects}
	@echo
	@echo dependencies = ${dependencies}
	@echo depdir = ${depdir}
	@echo curpath = ${curpath}
	@echo build_dir = ${build_dir}
	@echo l_out = ${_LINK_STATE}

.PHONY : internal_var
internal_var :
	@echo {curpath}       ${curpath}
	@echo {compiler}      ${compiler}
	@echo {CXX}           ${CXX}
	@echo {libname}       ${libname}
	@echo {libtarget}     ${libtarget}
	@echo {libsoname}     ${libsoname}
	@echo {librealname}   ${librealname}
	@echo {exetarget}     ${exetarget}
	@echo {pkgconfigfile} ${pkgconfigfile}
	@echo {automakefile}  ${automakefile}
	@echo {commentfile}   ${commentfile}
	@echo {tag_file}      ${tag_file}
	@echo {tag_generator} ${tag_generator}
	@echo {tag_depends}   ${tag_depends}
	@echo {tag_src}       ${tag_src}
	@echo
	@echo ldflags  = $(LDFLAGS)
	@echo cxxflags = $(CXXFLAGS)
	@echo source_list = ${sources_list}
	@echo objects = ${objects}
	@echo dependencies = ${dependencies}
	@echo

.PHONY : compilation
compilation:
ifeq ($(is_debug),true)
	@echo debug
else
	@echo release
endif

.PHONY  : slib
slib   : $(objects)
	g++ -shared -Wl,-soname,$(libsoname) -o $(librealname)  $^
	ldconfig -v -n $(libdir)
	ln -sf $(libsoname) $(libdir)/$(libname).so

.PHONY  : library
library : $(libtarget) tags
	@echo
	@echo ------------------ library $(libtarget) is built.
	@echo

$(libtarget): $(objects)
	@echo
	@echo ------------------ creating library
	@echo
	$(AR) $(ARFLAGS) $@ $^

.PHONY : tags
tags   :
#	@echo
#	@echo ------------------ creating tag entries
#	@echo
# 	@echo tin=$(tag_incl)
# 	@echo tag_depdends:${tag_depends}
# 	@echo tag_src:${tag_src}
#	@${tag_generator} -l ${tag_depends} -f ${tag_src} -o ${tag_file}

.PHONY : dox
dox    : Doxyfile
	@echo
	@echo ------------------ creating documentation
	@echo
	@doxygen Doxyfile

.PHONY : clean
clean  :
	@echo
	@echo ------------------ cleaning *.o exe lib
	@echo
	@echo rm -f $(objects) $(libtarget) ${exetarget} $(tag_file) gmon.out $(librealname) $(libdir)/$(libname).so $(libdir)/$(libsoname)
	@rm -f $(objects) $(libtarget) ${libtarget} ${exetarget} $(tag_file) gmon.out $(librealname) $(libdir)/$(libname).so $(libdir)/$(libsoname)


.PHONY : cleanaux
cleanaux  :
	@echo
	@echo ------------------ cleaning *.o *.d
	@echo
	@echo rm -f $(objects) ${dependencies} $(tag_file) gmon.out
	@rm -f $(objects) ${dependencies} $(tag_file) gmon.out

.PHONY   : cleandox
cleandox :
	@echo
	@echo ------------------ removing documentation
	@echo
	@rm -rf doc

.PHONY : cleandist
cleandist  :
	@echo
	@echo ------------------ cleaning everything
	@echo
	@rm -rf $(outdir)
## @rm -f $(pkgconfigfile) $(libtarget) $(objects) ${exetarget} $(dependencies) $(tag_file) gmon.out  $(librealname) $(libdir)/$(libname).so $(libdir)/$(libsoname)

.PHONY   : cleandep
cleandep :
	@echo ------------------ cleaning dependencies
	@rm -rf ${dependencies}

.PHONY : clear
clear :
	@rm -rf \#* ${dependencies} *~

.PHONY: install-lib
install-lib: $(libtarget) tags pkgfile
	@echo
	@echo ------------------ installing library and header files
	@echo
	@echo ------------------ installing at $(installdir)
	@echo
	@mkdir -p $(installdir)/include
	@rsync -rvu --exclude=.svn $(includedir)/* $(installdir)/include/
	@mkdir -p $(installdir)/lib/pkgconfig
	@cp -vfr $(libtarget)  $(installdir)/lib
	@echo
	@echo ------------------ installing the pkg-config file to $(installdir)/lib/pkgconfig. \
		Remember to add this path to your PKG_CONFIG_PATH variable
	@echo
	@cp $(pkgconfigfile) $(installdir)/lib/pkgconfig/

.PHONY: install-slib
install-slib: $(slib) tags pkgfile
	@echo
	@echo ------------------ installing library and header files
	@echo
	@echo ------------------ installing at $(installdir)
	@echo
	@mkdir -p $(installdir)/include
	@rsync -rvu --exclude=.svn $(includedir)/* $(installdir)/include/
	@mkdir -p $(installdir)/lib/pkgconfig
	@cp -vfr $(libdir)/*  $(installdir)/lib
	@echo
	@echo ------------------ installing the pkg-config file to $(installdir)/lib/pkgconfig. \
		Remember to add this path to your PKG_CONFIG_PATH variable
	@echo
	@cp $(pkgconfigfile) $(installdir)/lib/pkgconfig/


.PHONY: install
install: $(exetarget) tags
	@echo $(installdir)/bin
	@mkdir -p $(installdir)/bin
	@cp -f $(exetarget) $(installdir)/bin/

.PHONY: install-dev
install-dev : $(libtarget) pkgfile uninstall
	@echo
	@echo ------------------ installing library and development files
	@echo
	@echo ------------------ installing at $(installdir)
	@echo
	@mkdir -p $(installdir)/include/$(packagename)
	@echo ------------------ copying .h $(installdir)/include/
	@rsync -rvu --exclude=.svn $(includedir)/* $(installdir)/include/
	@mkdir -p $(installdir)/lib/pkgconfig
	@cp -vfR $(libtarget)  $(installdir)/lib                 # copy the static library
	@mkdir -p $(installdir)/src/$(packagename)                 # create the source directory
	@rsync -rvu --exclude=.svn $(srcdir)/* $(installdir)/src/$(packagename)
	@cp -vf makefile $(installdir)/src/$(packagename)
	@cp $(pkgconfigfile) $(installdir)/lib/pkgconfig/

.PHONY: uninstall
uninstall:
	@echo
	@echo ------------------ uninstalling if-installed
	@echo
	@rm -rf $(installdir)/include/$(packagename)
	@rm -f   $(installdir)/lib/$(libtarget)
	@rm -rf $(installdir)/src/$(packagename)
	@rm -f   $(installdir)/lib/pkgconfig/$(pkgconfigfile)
	@rm -f   $(installdir)/bin/$(exetarget)
	@rm -f $(installdir)/lib/$(libsoname)
	@rm -f $(installdir)/$(librealname)
	@rm -f $(installdir)/lib/$(libname).so

.PHONY : pkgfile
pkgfile:
	@echo
	@echo ------------------ creating pkg-config file
	@echo
	@echo "# Package Information for pkg-config"    >  $(pkgconfigfile)
	@echo "# Author= $(author)" 			>> $(pkgconfigfile)
	@echo "# Created= `date`"			>> $(pkgconfigfile)
	@echo "# Licence= $(licence)"			>> $(pkgconfigfile)
	@echo 						>> $(pkgconfigfile)
	@echo prefix=$(installdir)       		>> $(pkgconfigfile)
	@echo exec_prefix=$$\{prefix\}     		>> $(pkgconfigfile)
	@echo libdir=$$\{exec_prefix\}/lib 		>> $(pkgconfigfile)
	@echo includedir=$$\{prefix\}/include   	>> $(pkgconfigfile)
	@echo 						>> $(pkgconfigfile)
	@echo Name: "$(packagename)" 			>> $(pkgconfigfile)
	@echo Description: "$(description)" 		>> $(pkgconfigfile)
	@echo Version: "$(version)"                     >> $(pkgconfigfile)
	@echo Libs: -L$$\{libdir} -l$(packagename) ${LDFLAGS}	>> $(pkgconfigfile)
	@echo Cflags: -I$$\{includedir\} ${define_flags} ${custom_cflags}  >> $(pkgconfigfile)
	@echo Requires: ${external_libraries}           >> $(pkgconfigfile)
	@echo Path=$(curpath)                           >> $(pkgconfigfile)
	@echo tagfile=$$\{Path\}/$(tag_file)            >> $(pkgconfigfile)
	@echo 						>> $(pkgconfigfile)

.PHONY : revert_makefile
revert :
	@mv -f makefile.in makefile

.PHONY : export_makefile
export_makefile :
	@echo "#automatically generated makefile"         >  $(automakefile)
	@echo packagename := ${packagename_o}             >> ${automakefile}
	@echo major_version := ${major_version}           >> ${automakefile}
	@echo minor_version := ${minor_version}           >> ${automakefile}
	@echo tiny_version  := ${tiny_version}            >> ${automakefile}
	@echo author := ${author}                         >> ${automakefile}
	@echo description := "${description}"             >> ${automakefile}
	@echo licence := ${licence}                       >> ${automakefile}
	@echo "#........................................" >> ${automakefile}
	@echo installdir := ${installdir}                 >> $(automakefile)
	@echo external_sources := ${external_sources}     >> ${automakefile}
	@echo external_libraries := ${external_libraries} >> ${automakefile}
	@echo libdir := ${libdir_o}                       >> ${automakefile}
	@echo srcdir := ${srcdir_o}                       >> ${automakefile}
	@echo includedir:= ${includedir}                  >> ${automakefile}
	@echo define_flags := ${define_flags}             >> ${automakefile}
	@echo "#........................................" >> ${automakefile}
	@echo optimize := ${optimize}                     >> ${automakefile}
	@echo parallelize := ${parallelize}               >> ${automakefile}
	@echo f77 := ${f77}                               >> ${automakefile}
	@echo sse := ${sse}                               >> ${automakefile}
	@echo multi-threading := ${multi-threading}       >> ${automakefile}
	@echo profile := ${profile}                       >> ${automakefile}
	@echo "#........................................" >> ${automakefile}
	@echo specialize := ${specialize}                 >> ${automakefile}
	@echo platform := ${platform}                     >> ${automakefile}
	@echo "#........................................" >> ${automakefile}
	@echo sources := ${sources_list_o}                >> ${automakefile}
	@echo                                             >> ${automakefile}
	@echo "#........................................" >> ${automakefile}
	@echo                                             >> ${automakefile}
	@echo define_flags    := ${define_flags}          >> ${automakefile}
	@echo custom_ld_flags := ${custom_ld_flags}       >> ${automakefile}
	@echo custom_cflags   := ${custom_cflags}         >> ${automakefile}
	@echo "#........................................" >> ${automakefile}
	@cat ${MAKEFILE_HEAVEN}/static-variables.makefile >> ${automakefile}
	@cat ${MAKEFILE_HEAVEN}/flags.makefile            >> ${automakefile}
	@cat ${MAKEFILE_HEAVEN}/rules.makefile            >> ${automakefile}
	@echo >> ${automakefile}
	@mv makefile makefile.in
	@mv ${automakefile} makefile

.PHONY : gflat
gflat :
	@echo "gprof $(exetarget) gmon.out -p | more"
	@gprof $(exetarget) gmon.out -p | more

.PHONY : gcall
gcall :
	@echo "gprof $(exetarget) gmon.out -q | more"
	@gprof $(exetarget) gmon.out -q | more

.PHONY : state
state  :
	@echo
	@echo "package name      : ${packagename} v${version} by ${author}"
	@echo "                   (${description}) "
	@echo "------------------------------------------------------------------------"
	@echo "install directory : ${installdir}"
	@echo "external sources  : ${external_sources}"
	@echo "external libs     : ${external_libraries}"
	@echo "fortran support   : ${f77}"
	@echo "------------------------------------------------------------------------"
	@echo "optimize          : ${optimize}"
	@echo "parallelize       : ${parallelize}"
	@echo "profile           : ${profile}"
	@echo "sse               : ${sse}"
	@echo "multi-threading   : ${multi-threading}"
	@echo "------------------------------------------------------------------------"
	@echo "specialize        : ${specialize} for ${platform}"
	@echo "------------------------------------------------------------------------"
	@echo "sources           : ${sources_list}"
	@echo "------------------------------------------------------------------------"
	@echo ldflags  = $(LDFLAGS)
	@echo cxxflags = $(CXXFLAGS)
	@echo sources = ${sources_list}
	@echo objects = ${objects}

.PHONY : rules
rules :
	@echo
	@echo ------------------ legitimate rules
	@echo
	@echo "(nothing)	: makes the executable : "
	@echo "library		: generates the library"
	@echo "slib		: generates shared library"
	@echo "install-slib	: installs shared library"
	@echo "tags		: generates etags files"
	@echo "dox		: generates the doxygen documentation if Doxyfile exists"
	@echo "clear		: cleans up *~ #* and dependencies"
	@echo "clean		: cleans up .o lib and exe files"
	@echo "cleanaux		: cleans auxilary files: *.o *.d"
	@echo "cleandep		: cleans up the dependency files"
	@echo "cleandox		: cleans up the documentation"
	@echo "cleandist	: cleans everything except source+headers"
	@echo "install		: installs the executable"
	@echo "install-lib	: installs the library"
	@echo "install-dev	: installs the library along with documentation files"
	@echo "uninstall	: uninstalls the library"
	@echo "pkgfile		: generates the pkg-config file"
	@echo "flags		: shows the flags that will be used"
	@echo "gflat		: shows gprof profiler flat view result"
	@echo "gcall		: shows gprof profiler call graph view result"
	@echo "rules		: shows this text"
	@echo "state		: show the configuration state of the package"
	@echo "revert		: moves makefile.in to makefile"
	@echo "export_makefile	: export the makefile"
	@echo

${outdir}%.d : ${srcdir}%.c
	@echo
	@echo ------------------ creating dependencies for $@
	@echo
	$(compiler) $(CXXFLAGS) $(TARGET_ARCH) -MM $< | sed 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	mv -f $@.tmp $@
	@echo

${outdir}%.d : ${srcdir}%.cc
	@echo
	@echo ------------------ creating dependencies for $@
	@echo
	$(compiler) $(CXXFLAGS) $(TARGET_ARCH) -MM $< | sed 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	mv -f $@.tmp $@
	@echo

${outdir}%.d : ${srcdir}%.cpp
	@echo
	@echo ------------------ creating dependencies for $@
	@echo
	$(compiler) $(CXXFLAGS) $(TARGET_ARCH) -MM $< | sed 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	mv -f $@.tmp $@
	@echo

${outdir}%.o : ${srcdir}%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${outdir}%.o : ${srcdir}%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

${outdir}%.o : ${srcdir}%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

