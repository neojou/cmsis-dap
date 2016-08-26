
PROJECT=exam3

PROJECT_BASE_PATH = ..
OUTPUT_PATH = ./output

ARM_GCC_TOOLCHAIN=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_NAME)/bin
CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)/arm-none-eabi-

AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
SIZE = $(CROSS_COMPILE)size
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy


OPTIONS = -g -Wall 
OPTIONS += -ffunction-sections -fdata-sections -fno-exceptions -fno-delete-null-pointer-checks
OPTIONS += -fmessage-length=0 -fno-builtin -mthumb
OPTIONS += -mcpu=cortex-m0 -MMD -MP 
OPTIONS += -D__CORTEX_M0 -D__TARGET_ARCH_6S_M 

CFLAGS = $(OPTIONS) -Os -std=gnu99 -fno-strict-aliasing 
CXXFLAGS = $(OPTIONS) -Os -fno-rtti -std=gnu++98 -fno-strict-aliasing 

ASFLAGS = -mcpu=cortex-m0  

LIBS = -lstdc++ -lsupc++ -lm -lgcc -lc -lnosys

#LD_FLAGS = -nostdlib -fno-builtin 
LD_FLAGS += -mcpu=cortex-m0 -mthumb -march=armv6-m
LD_FLAGS += -ffunction-sections -fdata-sections
LD_FLAGS += -Os  
LD_FLAGS += -specs=nano.specs
LD_FLAGS += -Tld/lpc11u35.ld
LD_FLAGS += -Wl,-Map=$(PROJECT).map
LD_FLAGS += -Wl,--gc-sections


#
vpath %.h $(PROJECT_BASE_PATH)/src
vpath %.c $(PROJECT_BASE_PATH)/src
vpath %.h $(PROJECT_BASE_PATH)/src/rtos
vpath %.c $(PROJECT_BASE_PATH)/src/rtos
vpath %.s $(PROJECT_BASE_PATH)/src/rtos
vpath %.c $(PROJECT_BASE_PATH)/src/rtos/cm0
vpath %.s $(PROJECT_BASE_PATH)/src/rtos/cm0

VPATH=$(PROJECT_BASE_PATH)/src
VPATH=$(PROJECT_BASE_PATH)/src/rtos
VPATH=$(PROJECT_BASE_PATH)/src/rtos/cm0

INCLUDES = -I$(PROJECT_BASE_PATH)/src
INCLUDES += -I$(PROJECT_BASE_PATH)/src/rtos

CFLAGS += $(INCLUDES)
ASFLAGS += $(INCLUDES)


C_SRC=$(wildcard $(PROJECT_BASE_PATH)/src/*.c)
C_SRC+=$(wildcard $(PROJECT_BASE_PATH)/src/rtos/*.c)
C_SRC+=$(wildcard $(PROJECT_BASE_PATH)/src/rtos/cm0/*.c)

C_OBJ_TEMP=$(patsubst %.c, %.o, $(notdir $(C_SRC)))
C_OBJ_FILTER=
C_OBJ=$(filter-out $(C_OBJ_FILTER), $(C_OBJ_TEMP))

A_SRC=$(wildcard $(PROJECT_BASE_PATH)/src/*.s)
A_SRC+=$(wildcard $(PROJECT_BASE_PATH)/src/rtos/*.s)
A_SRC+=$(wildcard $(PROJECT_BASE_PATH)/src/rtos/cm0/*.s)

A_OBJ_TEMP=$(patsubst %.s, %.o, $(notdir $(A_SRC)))
A_OBJ_FILTER=
A_OBJ=$(filter-out $(A_OBJ_FILTER), $(A_OBJ_TEMP))

all: $(PROJECT).axf firmware.bin 

$(addprefix $(OUTPUT_PATH)/,$(C_OBJ)): $(OUTPUT_PATH)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(addprefix $(OUTPUT_PATH)/,$(A_OBJ)): $(OUTPUT_PATH)/%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@


$(PROJECT).axf: $(addprefix $(OUTPUT_PATH)/, $(C_OBJ)) $(addprefix $(OUTPUT_PATH)/, $(A_OBJ))
	$(LD) $(LD_FLAGS) -o $@ $^ $(LIBS)
	$(SIZE) $@

firmware.bin: $(PROJECT).axf
	$(OBJDUMP) -S -d $< > $(PROJECT).asm
	$(OBJCOPY) -O binary $< $@
#ifeq ($(HOST), cygwin)
#	./checksum.exe -p lpc11u35 -d $@
#endif


clean :
	rm -f $(PROJECT).* output/* firmware.bin

