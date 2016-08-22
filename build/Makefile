
PROJECT=exam1

PROJECT_BASE_PATH = ..
OUTPUT_PATH = ./output

ARM_GCC_TOOLCHAIN=../../../toolchain/gcc-arm-none-eabi-5_4-2016q2/bin
CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)/arm-none-eabi-

AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
SIZE = $(CROSS_COMPILE)size
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy


OPTIONS = -g -Os -Wall -fno-strict-aliasing 
OPTIONS += -ffunction-sections -fdata-sections -fno-exceptions -fno-delete-null-pointer-checks
OPTIONS += -fmessage-length=0 -fno-builtin -mthumb
OPTIONS += -mcpu=cortex-m0 -MMD -MP 

CFLAGS = $(OPTIONS) -std=gnu99
CXXFLAGS = $(OPTIONS) -fno-rtti -std=gnu++98 

ASFLAGS = $(OPTIONS) 

LIBS = -lstdc++ -lsupc++ -lm -lgcc -lc -lnosys

#LD_FLAGS = -nostdlib -fno-builtin 
LD_FLAGS += -mcpu=cortex-m0 -mthumb
LD_FLAGS += -Os  
LD_FLAGS += -specs=nano.specs
LD_FLAGS += -Tld/lpc11u35.ld
LD_FLAGS += -Wl,-Map=$(PROJECT).map,--gc-sections

vpath %.h $(PROJECT_BASE_PATH)/src
vpath %.c $(PROJECT_BASE_PATH)/src

VPATH=$(PROJECT_BASE_PATH)/src

INCLUDES = -I$(PROJECT_BASE_PATH)/src

CFLAGS += $(INCLUDES)
AFLAGS += $(INCLUDES)


C_SRC=$(wildcard $(PROJECT_BASE_PATH)/src/*.c)
C_OBJ_TEMP=$(patsubst %.c, %.o, $(notdir $(C_SRC)))
C_OBJ_FILTER=
C_OBJ=$(filter-out $(C_OBJ_FILTER), $(C_OBJ_TEMP))

A_SRC=$(wildcard $(PROJECT_BASE_PATH)/src/*.s)
A_OBJ_TEMP=$(patsubst %.s, %.o, $(notdir $(A_SRC)))
A_OBJ_FILTER=
A_OBJ=$(filter-out $(A_OBJ_FILTER), $(A_OBJ_TEMP))

all: $(PROJECT)

$(addprefix $(OUTPUT_PATH)/,$(C_OBJ)): $(OUTPUT_PATH)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(addprefix $(OUTPUT_PATH)/,$(A_OBJ)): $(OUTPUT_PATH)/%.o: %.s
	$(AS) -c $(AFLAGS) $< -o $@


$(PROJECT): $(addprefix $(OUTPUT_PATH)/, $(C_OBJ)) $(addprefix $(OUTPUT_PATH)/, $(A_OBJ))
	$(LD) $(LD_FLAGS) -o $(PROJECT).axf $^ $(LIBS)
	$(SIZE) $(PROJECT).axf
	$(OBJDUMP) -S -d $(PROJECT).axf > $(PROJECT).asm
	$(OBJCOPY) -O binary -v $(PROJECT).axf firmware.bin

clean :
	rm -f $(PROJECT).* output/*

