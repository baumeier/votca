#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/715944016/kmc_run.o \
	${OBJECTDIR}/_ext/715944016/kmc_test.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/kmc_run

dist/Release/GNU-Linux-x86/kmc_run: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kmc_run ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/715944016/kmc_run.o: ../../src/tools/kmc_run.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/715944016
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/715944016/kmc_run.o ../../src/tools/kmc_run.cc

${OBJECTDIR}/_ext/715944016/kmc_test.o: ../../src/tools/kmc_test.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/715944016
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/715944016/kmc_test.o ../../src/tools/kmc_test.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/kmc_run

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
