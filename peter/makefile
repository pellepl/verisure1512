BINARY = flir

############
#
# STM Device settings
#
############

STARTUP = startup_stm32f746xx.s
FLAGS += -DSTM32F746xx
FLAGS += -DUSE_HAL_DRIVER
FLAGS += -DHSE_VALUE=25000000

############
#
# Paths
#
############

sourcedir = src
builddir = build

basetoolsdir = ~/toolchain/arm-none-eabi-toolchain-gcc-5.2.0-hardfloat

stmlibdir = ../ext-libs/STM32Cube_FW_F7_V1.2.0
stmdriverdir = ${stmlibdir}/Drivers/STM32F7xx_HAL_Driver
stmcmsisdir = ${stmlibdir}/Drivers/CMSIS/Device/ST/STM32F7xx
stmcmsisdircore = ${stmlibdir}/Drivers/CMSIS/Include

tools = ${basetoolsdir}/bin

CPATH =
SPATH =
INC =
SFILES =
CFILES =
RFILES = 

#############
#
# Build tools
#
#############

CROSS_COMPILE=${tools}/arm-none-eabi-
CC = $(CROSS_COMPILE)gcc $(COMPILEROPTIONS)
AS = $(CROSS_COMPILE)gcc $(ASSEMBLEROPTIONS)
LD = $(CROSS_COMPILE)ld
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size
MKDIR = mkdir -p
RM = rm -f

###############
#
# Build configs
#
###############

INCLUDE_DIRECTIVES = 
COMPILEROPTIONS = $(INCLUDE_DIRECTIVES) $(FLAGS) -mcpu=cortex-m4 -mfloat-abi=hard -mno-thumb-interwork -mthumb -Wall -gdwarf-2 -ffunction-sections -fdata-sections
COMPILEROPTIONS += -Os -nostartfiles -nostdlib 
ASSEMBLEROPTION = $(COMPILEROPTIONS)
LINKERSCRIPT = arm.ld
LINKEROPTIONS = --gc-sections -cref
OBJCOPYOPTIONS_HEX = -O ihex ${builddir}/$(BINARY).elf
OBJCOPYOPTIONS_BIN = -O binary ${builddir}/$(BINARY).elf

BUILD_NUMBER_FILE=build-number.txt

###############
#
# Files and libs
#
###############

# app files
CPATH 		+= ${sourcedir}
CPATH 		+= ${sourcedir}/fromcube
SPATH 		+= ${sourcedir}
INC				+= -I./${sourcedir}
INC				+= -I./${sourcedir}/fromcube
SFILES		+= memset.s
SFILES		+= memcpy.s
SFILES		+= variadic.s
CFILES		+= main.c
CFILES		+= miniutils.c
CFILES		+= uart_driver.c

CFILES		+= stm32f7xx_hal_msp.c

# stm32 lib files
SPATH 		+= ${stmcmsisdir}/Source/Templates/gcc_ride7
SFILES 		+= $(STARTUP)

CPATH			+= ${stmdriverdir}/Src 
CPATH			+= ${stmcmsisdir}/Source/Templates 
CPATH			+= ${stmcmsisdircore}
INC				+= -I./${stmdriverdir}/Inc 
INC				+= -I./${stmcmsisdir}/Include 
INC				+= -I./${stmcmsisdircore}

CFILES		+= stm32f7xx_hal.c
#CFILES		+= stm32f7xx_hal_adc.c
#CFILES		+= stm32f7xx_hal_adc_ex.c
#CFILES		+= stm32f7xx_hal_can.c
#CFILES		+= stm32f7xx_hal_cec.c
CFILES		+= stm32f7xx_hal_cortex.c
#CFILES		+= stm32f7xx_hal_crc.c
#CFILES		+= stm32f7xx_hal_crc_ex.c
#CFILES		+= stm32f7xx_hal_cryp_ex.c
#CFILES		+= stm32f7xx_hal_cryp.c
#CFILES		+= stm32f7xx_hal_dac.c
#CFILES		+= stm32f7xx_hal_dac_ex.c
#CFILES		+= stm32f7xx_hal_dcmi.c
#CFILES		+= stm32f7xx_hal_dcmi_ex.c
#CFILES		+= stm32f7xx_hal_dma2d.c
CFILES		+= stm32f7xx_hal_dma.c
CFILES		+= stm32f7xx_hal_dma_ex.c
#CFILES		+= stm32f7xx_hal_eth.c
CFILES		+= stm32f7xx_hal_flash.c
CFILES		+= stm32f7xx_hal_flash_ex.c
CFILES		+= stm32f7xx_hal_gpio.c
#CFILES		+= stm32f7xx_hal_hash.c
#CFILES		+= stm32f7xx_hal_hash_ex.c
#CFILES		+= stm32f7xx_hal_hcd.c
#CFILES		+= stm32f7xx_hal_i2c.c
#CFILES		+= stm32f7xx_hal_i2c_ex.c
#CFILES		+= stm32f7xx_hal_i2s.c
#CFILES		+= stm32f7xx_hal_irda.c
#CFILES		+= stm32f7xx_hal_iwdg.c
#CFILES		+= stm32f7xx_hal_lptim.c
CFILES		+= stm32f7xx_hal_ltdc.c
#CFILES		+= stm32f7xx_hal_msp_template.c
#CFILES		+= stm32f7xx_hal_nand.c
#CFILES		+= stm32f7xx_hal_nor.c
#CFILES		+= stm32f7xx_hal_pcd.c
#CFILES		+= stm32f7xx_hal_pcd_ex.c
CFILES		+= stm32f7xx_hal_pwr.c
CFILES		+= stm32f7xx_hal_pwr_ex.c
#CFILES		+= stm32f7xx_hal_qspi.c
CFILES		+= stm32f7xx_hal_rcc.c
CFILES		+= stm32f7xx_hal_rcc_ex.c
#CFILES		+= stm32f7xx_hal_rng.c
#CFILES		+= stm32f7xx_hal_rtc.c
#CFILES		+= stm32f7xx_hal_rtc_ex.c
#CFILES		+= stm32f7xx_hal_sai.c
#CFILES		+= stm32f7xx_hal_sai_ex.c
#CFILES		+= stm32f7xx_hal_sd.c
#CFILES		+= stm32f7xx_hal_smartcard.c
#CFILES		+= stm32f7xx_hal_smartcard_ex.c
#CFILES		+= stm32f7xx_hal_spdifrx.c
#CFILES		+= stm32f7xx_hal_spi.c
CFILES		+= stm32f7xx_hal_sdram.c
CFILES		+= stm32f7xx_hal_sram.c
#CFILES		+= stm32f7xx_hal_tim.c
#CFILES		+= stm32f7xx_hal_tim_ex.c
#CFILES		+= stm32f7xx_hal_uart.c
#CFILES		+= stm32f7xx_hal_usart.c
#CFILES		+= stm32f7xx_hal_wwdg.c
CFILES		+= stm32f7xx_ll_fmc.c
#CFILES		+= stm32f7xx_ll_sdmmc.c
#CFILES		+= stm32f7xx_ll_usb.c

# cmsis files
CFILES += 	system_stm32f7xx.c

LIBS = 

BINARYEXT = .hex

############
#
# Tasks
#
############


vpath %.c $(CPATH)
vpath %.s $(SPATH)
INCLUDE_DIRECTIVES += $(INC)

SOBJFILES = $(SFILES:%.s=${builddir}/%.o)
OBJFILES = $(CFILES:%.c=${builddir}/%.o)
ROBJFILES = $(RFILES:%.c=${builddir}/%.o)

DEPFILES = $(CFILES:%.c=${builddir}/%.d)
DEPFILES += $(RFILES:%.c=${builddir}/%.d)

ALLOBJFILES  = $(SOBJFILES)
ALLOBJFILES += $(OBJFILES)
ALLOBJFILES += $(ROBJFILES)

DEPENDENCIES = $(DEPFILES) 

# link object files, create binary for flashing
$(BINARY): $(ALLOBJFILES)
	@echo "... build info"
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(($$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)	
	@echo "... linking"
	@${LD} $(LINKEROPTIONS) $(BUILD_NUMBER_LDFLAGS) -T $(LINKERSCRIPT) -Map ${builddir}/$(BINARY).map -o ${builddir}/$(BINARY).elf $(ALLOBJFILES) $(LIBS)
	@echo "... objcopy"
	@${OBJCOPY} $(OBJCOPYOPTIONS_BIN) ${builddir}/$(BINARY).out
	@${OBJCOPY} $(OBJCOPYOPTIONS_HEX) ${builddir}/$(BINARY)$(BINARYEXT) 
	@echo "... disasm"
	@${OBJDUMP} -hd -j .text -j.data -j .bss -j .bootloader_text -j .bootloader_data -d -S ${builddir}/$(BINARY).elf > ${builddir}/$(BINARY)_disasm.s
	@echo "${BINARY}.out is `stat -c%s ${builddir}/${BINARY}.out` bytes on flash"

-include $(DEPENDENCIES)	   	

# compile assembly files, arm
$(SOBJFILES) : ${builddir}/%.o:%.s
		@echo "... assembly $@"
		@${AS} -c -o $@ $<
		
# compile c files
$(OBJFILES) : ${builddir}/%.o:%.c
		@echo "... compile $@"
		@${CC} -c -o $@ $<

# compile c files designated for ram
$(ROBJFILES) : ${builddir}/%.o:%.c
		@echo "... ram compile $@"
		@${CC} -c -o $@ $< 

# make dependencies
$(DEPFILES) : ${builddir}/%.d:%.c
		@echo "... depend $@"; \
		${RM} $@; \
		${CC} $(COMPILEROPTIONS) -M $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*, ${builddir}/\1.o $@ : ,g' < $@.$$$$ > $@; \
		${RM} $@.$$$$

all: info mkdirs $(BINARY)

info:
	@echo "* Building to ${builddir}"
	@echo "* Compiler options:  $(COMPILEROPTIONS)" 
	@echo "* Assembler options: $(ASSEMBLEROPTIONS)" 
	@echo "* Linker options:    $(LINKEROPTIONS)" 
	@echo "* Linker script:     ${LINKERSCRIPT}"
	
mkdirs:
	-@${MKDIR} ${builddir}
	
clean:
	@echo ... removing build files in ${builddir}
	@${RM} ${builddir}/*.o
	@${RM} ${builddir}/*.d
	@${RM} ${builddir}/*.out
	@${RM} ${builddir}/*.hex
	@${RM} ${builddir}/*.elf
	@${RM} ${builddir}/*.map
	@${RM} ${builddir}/*_disasm.s
	@${RM} _stm32flash.script

install: binlen = $(shell stat -c%s ${builddir}/${BINARY}.out)
install: $(BINARY)
	@echo "binary length of install is ${binlen} bytes.."
	@echo ${PWD}
	@sed 's|BUILDFILE|${PWD}/${builddir}/${BINARY}.out|;s|LENGTH|${binlen}|' stm32flash.script > _stm32flash.script
	@echo "script ${PWD}/_stm32flash.script" | nc localhost 4444
	@${RM} _stm32flash.script
	

debug: $(BINARY)
	@${GDB} ${builddir}/${BINARY}.elf -x debug.gdb

build-info:
	@echo "*** INCLUDE PATHS"
	@echo "${INC}"
	@echo "*** SOURCE PATHS"
	@echo "${CPATH}"
	@echo "*** ASSEMBLY PATHS"
	@echo "${SPATH}"
	@echo "*** SOURCE FILES"
	@echo "${CFILES}"
	@echo "*** ASSEMBLY FILES"
	@echo "${SFILES}"
	@echo "*** FLAGS"
	@echo "${FLAGS}"
	
############
#
# Build info
#
############

BUILD_NUMBER_LDFLAGS  = --defsym __BUILD_DATE=$$(date +'%Y%m%d')
BUILD_NUMBER_LDFLAGS += --defsym __BUILD_NUMBER=$$(cat $(BUILD_NUMBER_FILE))
