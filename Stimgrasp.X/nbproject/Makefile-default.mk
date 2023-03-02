#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Libs/adc.c Libs/i2c1.c Libs/i2c2.c Libs/ic.c Libs/pwm.c Libs/spi1.c Libs/spi2.c Libs/system.c Libs/timers.c Libs/uart1.c Libs/uart2.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Libs/adc.o ${OBJECTDIR}/Libs/i2c1.o ${OBJECTDIR}/Libs/i2c2.o ${OBJECTDIR}/Libs/ic.o ${OBJECTDIR}/Libs/pwm.o ${OBJECTDIR}/Libs/spi1.o ${OBJECTDIR}/Libs/spi2.o ${OBJECTDIR}/Libs/system.o ${OBJECTDIR}/Libs/timers.o ${OBJECTDIR}/Libs/uart1.o ${OBJECTDIR}/Libs/uart2.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Libs/adc.o.d ${OBJECTDIR}/Libs/i2c1.o.d ${OBJECTDIR}/Libs/i2c2.o.d ${OBJECTDIR}/Libs/ic.o.d ${OBJECTDIR}/Libs/pwm.o.d ${OBJECTDIR}/Libs/spi1.o.d ${OBJECTDIR}/Libs/spi2.o.d ${OBJECTDIR}/Libs/system.o.d ${OBJECTDIR}/Libs/timers.o.d ${OBJECTDIR}/Libs/uart1.o.d ${OBJECTDIR}/Libs/uart2.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Libs/adc.o ${OBJECTDIR}/Libs/i2c1.o ${OBJECTDIR}/Libs/i2c2.o ${OBJECTDIR}/Libs/ic.o ${OBJECTDIR}/Libs/pwm.o ${OBJECTDIR}/Libs/spi1.o ${OBJECTDIR}/Libs/spi2.o ${OBJECTDIR}/Libs/system.o ${OBJECTDIR}/Libs/timers.o ${OBJECTDIR}/Libs/uart1.o ${OBJECTDIR}/Libs/uart2.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=Libs/adc.c Libs/i2c1.c Libs/i2c2.c Libs/ic.c Libs/pwm.c Libs/spi1.c Libs/spi2.c Libs/system.c Libs/timers.c Libs/uart1.c Libs/uart2.c main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GA004
MP_LINKER_FILE_OPTION=,--script=p24FJ64GA004.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Libs/adc.o: Libs/adc.c  .generated_files/flags/default/5a84d0caecea3607003f82945ad0bce43001abf .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/adc.o.d 
	@${RM} ${OBJECTDIR}/Libs/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/adc.c  -o ${OBJECTDIR}/Libs/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/adc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/i2c1.o: Libs/i2c1.c  .generated_files/flags/default/bd82911dda5356eb58f488a4fbba22cbbdcfb4aa .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/i2c1.o.d 
	@${RM} ${OBJECTDIR}/Libs/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/i2c1.c  -o ${OBJECTDIR}/Libs/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/i2c1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/i2c2.o: Libs/i2c2.c  .generated_files/flags/default/1c0b7cad145ca5facbba3971ca957bfb401ab926 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/i2c2.o.d 
	@${RM} ${OBJECTDIR}/Libs/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/i2c2.c  -o ${OBJECTDIR}/Libs/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/i2c2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/ic.o: Libs/ic.c  .generated_files/flags/default/dc31113d08cf2c2e391aa954d867b3bb4d687bdc .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/ic.o.d 
	@${RM} ${OBJECTDIR}/Libs/ic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/ic.c  -o ${OBJECTDIR}/Libs/ic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/ic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/pwm.o: Libs/pwm.c  .generated_files/flags/default/71ac809f48bbe7459d359b3b0d1d52784e3bb275 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/pwm.o.d 
	@${RM} ${OBJECTDIR}/Libs/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/pwm.c  -o ${OBJECTDIR}/Libs/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/spi1.o: Libs/spi1.c  .generated_files/flags/default/eb394f9ca7b0a1edcdf78afe986603311cd57e78 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/spi1.o.d 
	@${RM} ${OBJECTDIR}/Libs/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/spi1.c  -o ${OBJECTDIR}/Libs/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/spi1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/spi2.o: Libs/spi2.c  .generated_files/flags/default/99f4461d056fd0259dc7eb9d90930da21e37cc90 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/spi2.o.d 
	@${RM} ${OBJECTDIR}/Libs/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/spi2.c  -o ${OBJECTDIR}/Libs/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/spi2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/system.o: Libs/system.c  .generated_files/flags/default/dbee2414dad951e081f9e10e498ccbf03ef48205 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/system.o.d 
	@${RM} ${OBJECTDIR}/Libs/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/system.c  -o ${OBJECTDIR}/Libs/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/timers.o: Libs/timers.c  .generated_files/flags/default/1da38341bdac8e42f5dd15bbf905fde39b64cc29 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/timers.o.d 
	@${RM} ${OBJECTDIR}/Libs/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/timers.c  -o ${OBJECTDIR}/Libs/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/timers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/uart1.o: Libs/uart1.c  .generated_files/flags/default/cf24b03e0adf74757613c407d14a69844f943afd .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/uart1.o.d 
	@${RM} ${OBJECTDIR}/Libs/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/uart1.c  -o ${OBJECTDIR}/Libs/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/uart1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/uart2.o: Libs/uart2.c  .generated_files/flags/default/4482f9ba55542d64241c8b63f35d58810f1e2090 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/uart2.o.d 
	@${RM} ${OBJECTDIR}/Libs/uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/uart2.c  -o ${OBJECTDIR}/Libs/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/uart2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/5c804d1e524954a35f2bb8786e54c2c8cc4a526d .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/Libs/adc.o: Libs/adc.c  .generated_files/flags/default/38778332a816e88fdcb494da63ef08c9b54e5cd1 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/adc.o.d 
	@${RM} ${OBJECTDIR}/Libs/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/adc.c  -o ${OBJECTDIR}/Libs/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/adc.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/i2c1.o: Libs/i2c1.c  .generated_files/flags/default/f3312614e329ace3f498a2dc938e08f8a8caaf20 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/i2c1.o.d 
	@${RM} ${OBJECTDIR}/Libs/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/i2c1.c  -o ${OBJECTDIR}/Libs/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/i2c1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/i2c2.o: Libs/i2c2.c  .generated_files/flags/default/cc1e955654976755a23ed9502ff8096af0514c42 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/i2c2.o.d 
	@${RM} ${OBJECTDIR}/Libs/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/i2c2.c  -o ${OBJECTDIR}/Libs/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/i2c2.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/ic.o: Libs/ic.c  .generated_files/flags/default/5c7ac74b3dd9b2d3a2309bad04698b86d427b89c .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/ic.o.d 
	@${RM} ${OBJECTDIR}/Libs/ic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/ic.c  -o ${OBJECTDIR}/Libs/ic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/ic.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/pwm.o: Libs/pwm.c  .generated_files/flags/default/15636e2d8f9aa09ceef9e2e1d245e3e14ad9cfd8 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/pwm.o.d 
	@${RM} ${OBJECTDIR}/Libs/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/pwm.c  -o ${OBJECTDIR}/Libs/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/pwm.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/spi1.o: Libs/spi1.c  .generated_files/flags/default/911e3cd8be087c55db87ea0610d3fa0a5a56c2 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/spi1.o.d 
	@${RM} ${OBJECTDIR}/Libs/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/spi1.c  -o ${OBJECTDIR}/Libs/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/spi1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/spi2.o: Libs/spi2.c  .generated_files/flags/default/d981ea7b65c0a6d18edcbff8c9d4d4d24f6ffc6c .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/spi2.o.d 
	@${RM} ${OBJECTDIR}/Libs/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/spi2.c  -o ${OBJECTDIR}/Libs/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/spi2.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/system.o: Libs/system.c  .generated_files/flags/default/e75271230e8e6783b23a861d47a6bc5b96aa95a5 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/system.o.d 
	@${RM} ${OBJECTDIR}/Libs/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/system.c  -o ${OBJECTDIR}/Libs/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/system.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/timers.o: Libs/timers.c  .generated_files/flags/default/a2d065888b7cce633458de34197cdae1ff9cbba2 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/timers.o.d 
	@${RM} ${OBJECTDIR}/Libs/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/timers.c  -o ${OBJECTDIR}/Libs/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/timers.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/uart1.o: Libs/uart1.c  .generated_files/flags/default/b2b9f2fd10267f67354ad3a9785c057deb34ea4c .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/uart1.o.d 
	@${RM} ${OBJECTDIR}/Libs/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/uart1.c  -o ${OBJECTDIR}/Libs/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/uart1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Libs/uart2.o: Libs/uart2.c  .generated_files/flags/default/70426a151fa2d58411aa0380bee94ed9bd55375d .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}/Libs" 
	@${RM} ${OBJECTDIR}/Libs/uart2.o.d 
	@${RM} ${OBJECTDIR}/Libs/uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Libs/uart2.c  -o ${OBJECTDIR}/Libs/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Libs/uart2.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/6d2cdbc7d17db568c1f153540d753d462b3d52d5 .generated_files/flags/default/b59cf5dcb8793cca336e4cb4e040840ef3050593
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Stimgrasp.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
