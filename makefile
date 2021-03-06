### Project name (also used for output file name)
PROJECT	= motor_control

### Processor type and Thumb(-2) mode for CSRC/ASRC files (YES or NO)
CPU      = cortex-m4
PLATFORM = MKV30F128
DEVICE = MKV30F128XXX10
THUMB    = YES
UART_DMA = YES
# Define the motor drive IC DRV8847 or DRV8847S
CONTROL_DEF  = DRV8847
# Enable exite angle I controller
# CONTROL_DEF += ENABLE_EXI_ANGLE_I
# Enable current PI controller
# CONTROL_DEF += ENABLE_CURRENT_PI
# CONTROL_DEF += DEBUG_ITM

### Source files and search directories
CONTROL_CSRC = adj_velocity.c ele_angle.c pid.c i_excite_angle.c pi_current.c sin_cos_val_table.c svpwm.c step_accumulator.c control.c freqdiv.c fir_filter.c
HARDWARE_CSRC = uart.c system.c syscall.c control_board.c mkv30f_it.c as5047d.c drv8847_s.c tick.c dma_uart.c arm_itm.c
HAL_CSRC	= hal_as5047d.c hal_drv8847_s.c hal_tick.c
CSRC   += $(CONTROL_CSRC) $(HARDWARE_CSRC) $(HAL_CSRC)
CSRC   += main.c
# CSRC   += test_encoder.c
# CSRC   += test_adc.c
# CSRC   += test_timeout.c
# CSRC   += test_dma_transmit.c
# CSRC   += test_step_res.c
# CSRC   += test_id_motor_kt.c
# CSRC   += test_itm.c
# CSRC   += test_filter.c
# CSRC   += test_id_filter.c
ASRC	= fpu.S
CSRCARM	=
ASRCARM	=
VPATH   = src platform/$(PLATFORM) bsp control test

CSRC   += startup_$(PLATFORM).c

### Optimization level (0, 1, 2, 3, 4 or s)
OPTIMIZE = 0

### C Standard level (c89, gnu89, c99 or gnu99)
CSTD = c99

### Linker script for the target MCU
LINKSCRIPT = platform/$(PLATFORM)/$(PLATFORM).ld

### Output file type (hex, bin or both) and debugger type
OUTPUT	= hex
HEXFMT  = ihex
DEBUG	= dwarf-2

### Include dirs, library dirs and definitions
MATHLIB	= -lm
LIBS	=
LIBDIRS	=
INCDIRS	= platform/$(PLATFORM) bsp src control
DEFS	= $(CONTROL_DEF)
ADEFS	=

### Warning contorls
WARNINGS = all extra

### Object output directory
OBJDIR = obj


### Programs to build porject
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size
NM      = arm-none-eabi-nm


# Define all object files
COBJ      = $(CSRC:.c=.o)
AOBJ      = $(ASRC:.S=.o)
COBJARM   = $(CSRCARM:.c=.o)
AOBJARM   = $(ASRCARM:.S=.o)
COBJ      := $(addprefix $(OBJDIR)/,$(COBJ))
AOBJ      := $(addprefix $(OBJDIR)/,$(AOBJ))
COBJARM   := $(addprefix $(OBJDIR)/,$(COBJARM))
AOBJARM   := $(addprefix $(OBJDIR)/,$(AOBJARM))
PROJECT   := $(OBJDIR)/$(PROJECT)


### Compiler flags
ifeq ($(THUMB),YES)
THUMBFLAG = -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16
THUMBIW = -mthumb-interwork
else
THUMBFLAG =
THUMBIW =
endif


# Flags for C files
ifeq ($(UART_DMA),YES)
CFLAGS += -DUSE_UART_DMA
endif
CFLAGS += -std=$(CSTD)
CFLAGS += -g$(DEBUG)
CFLAGS += -O$(OPTIMIZE)
CFLAGS += $(addprefix -W,$(WARNINGS))
CFLAGS += $(addprefix -I,$(INCDIRS))
CFLAGS += $(addprefix -D,$(DEFS))
CFLAGS += -ffunction-sections -fdata-sections -fno-builtin
CFLAGS += -Wp,-MM,-MP,-MT,$(OBJDIR)/$(*F).o,-MF,$(OBJDIR)/$(*F).d


# Assembler flags
ASFLAGS += $(addprefix -D,$(ADEFS)) -Wa,-g$(DEBUG)


# Linker flags
LDFLAGS += -nostartfiles -Wl,-Map=$(PROJECT).map,--cref,--gc-sections
LDFLAGS += -lc -lgcc
LDFLAGS += -u __isr_vectors
LDFLAGS += $(patsubst %,-L%,$(LIBDIRS)) $(patsubst %,-l%,$(LIBS))
LDFLAGS += $(MATHLIB)
LDFLAGS += -T$(LINKSCRIPT)
LDFLAGS += -specs=nosys.specs --specs=nano.specs -flto
LDFLAGS += -u _printf_float -u _scanf_float

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS  = -mcpu=$(CPU) $(THUMBIW) -I. $(CFLAGS)
ALL_ASFLAGS = -mcpu=$(CPU) $(THUMBIW) -I. -x assembler-with-cpp $(ASFLAGS)

## JLINK
JLINK_OPT  = -device $(DEVICE)
JLINK_OPT += -if SWD
JLINK_OPT += -speed 4000
JLINK_OPT += -autoconnect 1

DOT_JLINK_FILE  = .jlink
DOT_JLINK_TEXT  = "r\n"
DOT_JLINK_TEXT += "loadfile $(PROJECT).hex\n"
DOT_JLINK_TEXT += "exit"

# Open com port
p := COM6
# Data size receive
sz := 1000
# Save file name marker
mk := "pmt"
# Receive data factor size
len := 13

# Default target.
all: build size

ifeq ($(OUTPUT),hex)
build: elf hex lst sym
hex: $(PROJECT).hex
else
ifeq ($(OUTPUT),bin)
build: elf bin lst sym
bin: $(PROJECT).bin
else
ifeq ($(OUTPUT),both)
build: elf hex bin lst sym
hex: $(PROJECT).hex
bin: $(PROJECT).bin
else
$(error "Invalid format: $(OUTPUT)")
endif
endif
endif

elf: $(PROJECT).elf
lst: $(PROJECT).lst
sym: $(PROJECT).sym


# Display compiler version information.
version :
	@$(CC) --version

# Create final output file (.hex or .bin) from ELF output file.
%.hex: %.elf
	@echo
	$(OBJCOPY) -O $(HEXFMT) $< $@

%.bin: %.elf
	@echo
	$(OBJCOPY) -O binary $< $@

# Create extended listing file from ELF output file.
%.lst: %.elf
	@echo
	$(OBJDUMP) -h -S -C $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	$(NM) -n $< > $@

# Display size of file.
size:
	@echo
	$(SIZE) -A $(PROJECT).elf


# Link: create ELF output file from object files.
%.elf:  $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ)
	@echo
	@echo Linking...
	$(CC) $(THUMBFLAG) $(ALL_CFLAGS) $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) -o $@ $(LDFLAGS)

# Compile: create object files from C source files. ARM or Thumb(-2)
$(COBJ) : $(OBJDIR)/%.o : %.c
	@echo
	@echo $< :
	$(CC) -c $(THUMBFLAG) $(ALL_CFLAGS) $< -o $@

# Compile: create object files from C source files. ARM-only
$(COBJARM) : $(OBJDIR)/%.o : %.c
	@echo
	@echo $< :
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Assemble: create object files from assembler source files. ARM or Thumb(-2)
$(AOBJ) : $(OBJDIR)/%.o : %.S
	@echo
	@echo $< :
	$(CC) -c $(THUMBFLAG) $(ALL_ASFLAGS) $< -o $@

# Assemble: create object files from assembler source files. ARM-only
$(AOBJARM) : $(OBJDIR)/%.o : %.S
	@echo
	@echo $< :
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Show the detail of elf file
detail: $(PROJECT).elf
	readelf -a $(PROJECT).elf > $(PROJECT)elf.txt

# Target: clean project.
clean:
	@echo
	rm -f -r $(OBJDIR) | exit 0

# Downlaod to device
download:
	@echo start download...
	@echo -e $(DOT_JLINK_TEXT) > $(DOT_JLINK_FILE)
	Jlink $(JLINK_OPT) -CommanderScript $(DOT_JLINK_FILE)

# Collect data
clt_raw:
	@echo start collect data
	@python ./tool/rec_data.py -p $(p) -sz $(sz) -mk $(mk) -len $(len)

# Collect data and handle it
clt:
	@echo start collect data
	@python ./tool/rec_data.py -p $(p) -sz $(sz) -mk $(mk) -len 13
hdl: # 對應main.c 程式碼
	@echo Handle collect data
	@python ./tool/handle_data.py -mk $(mk) -len $(len)
	@echo Done !

# Collect encoder test data and handle it
clt_enc:
	@echo start collect encoder data
	@python ./tool/rec_data.py -p $(p) -sz $(sz) -mk $(mk) -len 5
hdl_enc: # 對應test_encoder.c 程式碼
	@echo Handle encoder data
	@python ./tool/handle_enc.py -mk $(mk) -len $(len)
	@echo Done !

hdl_filter: # 對應test_filter.c 程式碼
	@echo Handle encoder data
	@python ./tool/handle_filter.py -mk $(mk) -len 3
	@echo Done !

# Collect step response data
clt_motor_id:
	@echo start collect step response data
	@python ./tool/rec_data.py -p $(p) -sz $(sz) -mk $(mk) -len 5
hdl_step_res: # 對應test_step_rec.c 程式碼，注意取樣頻率
	@echo Handle step response data
	@python ./tool/handle_step_rec.py -mk $(mk)
	@echo Done !
hdl_motor_id: # 對應test_motor_id.c 程式碼，注意取樣頻率
	@echo Handle motor id data
	@python ./tool/handle_id_motor.py -mk $(mk)
	@echo Done !

# Collect data help
clt_help:
	@echo Collect help
	@echo Example below:
	@echo make clt sz={data size} mk={save file mark} len={receive data kinds}

# Include the dependency files.
-include $(shell mkdir $(OBJDIR) 2>/dev/null) $(wildcard $(OBJDIR)/*.d)

