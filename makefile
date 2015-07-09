RM := rm -rf


OBJS += \
./convert_soy_data.o

OBJS2 += \
./barebones_convert_soy_data.o


# All Target
all: ConvertSoyData BarebonesConvertSoyData

# Tool invocations
ConvertSoyData: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc `xml2-config --libs` -o "ConvertSoyData" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

BarebonesConvertSoyData: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc `xml2-config --libs` -o "BarebonesConvertSoyData" $(OBJS2)
	@echo 'Finished building target: $@'
	@echo ' '

%.o: %.c
	@echo 'Building file: $<'
	gcc -Wall -c `xml2-config --cflags` -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(OBJS2) ConvertSoyData BarebonesConvertSoyData
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

test:
	xml2-config --cflags
