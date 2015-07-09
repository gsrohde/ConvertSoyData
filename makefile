RM := rm -rf

# All of the sources participating in the build are defined here
C_SRCS += \
./convert_soy_data.c 

OBJS += \
./convert_soy_data.o 


# All Target
all: ConvertSoyData

# Tool invocations
ConvertSoyData: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc `xml2-config --libs` -o "ConvertSoyData" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

%.o: %.c
	@echo 'Building file: $<'
	gcc -Wall -c `xml2-config --cflags` -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) ConvertSoyData
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

test:
	xml2-config --cflags
