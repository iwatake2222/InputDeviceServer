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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Application.o \
	${OBJECTDIR}/DriverGPIO/gpio.o \
	${OBJECTDIR}/DriverI2C/i2c.o \
	${OBJECTDIR}/DriverLcdST7735R/DriverLcdST7735R.o \
	${OBJECTDIR}/DriverLcdST7735R/DriverLcdST7735RText.o \
	${OBJECTDIR}/DriverOledSSD1306/DriverOledSSD1306.o \
	${OBJECTDIR}/DriverOledSSD1306/DriverOledSSD1306Text.o \
	${OBJECTDIR}/DriverSpi/spi.o \
	${OBJECTDIR}/InputDevices/InputDevices.o \
	${OBJECTDIR}/OutputTest.o \
	${OBJECTDIR}/PythonWrapper.o \
	${OBJECTDIR}/font/font.o \
	${OBJECTDIR}/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inputdeviceserver

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inputdeviceserver: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inputdeviceserver ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Application.o: Application.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Application.o Application.cpp

${OBJECTDIR}/DriverGPIO/gpio.o: DriverGPIO/gpio.c 
	${MKDIR} -p ${OBJECTDIR}/DriverGPIO
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DriverGPIO/gpio.o DriverGPIO/gpio.c

${OBJECTDIR}/DriverI2C/i2c.o: DriverI2C/i2c.c 
	${MKDIR} -p ${OBJECTDIR}/DriverI2C
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DriverI2C/i2c.o DriverI2C/i2c.c

${OBJECTDIR}/DriverLcdST7735R/DriverLcdST7735R.o: DriverLcdST7735R/DriverLcdST7735R.cpp 
	${MKDIR} -p ${OBJECTDIR}/DriverLcdST7735R
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DriverLcdST7735R/DriverLcdST7735R.o DriverLcdST7735R/DriverLcdST7735R.cpp

${OBJECTDIR}/DriverLcdST7735R/DriverLcdST7735RText.o: DriverLcdST7735R/DriverLcdST7735RText.cpp 
	${MKDIR} -p ${OBJECTDIR}/DriverLcdST7735R
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DriverLcdST7735R/DriverLcdST7735RText.o DriverLcdST7735R/DriverLcdST7735RText.cpp

${OBJECTDIR}/DriverOledSSD1306/DriverOledSSD1306.o: DriverOledSSD1306/DriverOledSSD1306.cpp 
	${MKDIR} -p ${OBJECTDIR}/DriverOledSSD1306
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DriverOledSSD1306/DriverOledSSD1306.o DriverOledSSD1306/DriverOledSSD1306.cpp

${OBJECTDIR}/DriverOledSSD1306/DriverOledSSD1306Text.o: DriverOledSSD1306/DriverOledSSD1306Text.cpp 
	${MKDIR} -p ${OBJECTDIR}/DriverOledSSD1306
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DriverOledSSD1306/DriverOledSSD1306Text.o DriverOledSSD1306/DriverOledSSD1306Text.cpp

${OBJECTDIR}/DriverSpi/spi.o: DriverSpi/spi.c 
	${MKDIR} -p ${OBJECTDIR}/DriverSpi
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DriverSpi/spi.o DriverSpi/spi.c

${OBJECTDIR}/InputDevices/InputDevices.o: InputDevices/InputDevices.cpp 
	${MKDIR} -p ${OBJECTDIR}/InputDevices
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/InputDevices/InputDevices.o InputDevices/InputDevices.cpp

${OBJECTDIR}/OutputTest.o: OutputTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OutputTest.o OutputTest.cpp

${OBJECTDIR}/PythonWrapper.o: PythonWrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PythonWrapper.o PythonWrapper.cpp

${OBJECTDIR}/font/font.o: font/font.c 
	${MKDIR} -p ${OBJECTDIR}/font
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font/font.o font/font.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inputdeviceserver

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
