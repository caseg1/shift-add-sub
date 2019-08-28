CROSS_CC ?= aarch64-linux-gnu-gcc
CFLAGS ?= --std=c99 -Wall -O0 -g
LDFLAGS ?= -lrt
CROSS_AS ?= aarch64-linux-gnu-as
ASFLAGS ?= -g
bin_PROGRAMS = hw3
C_SOURCES = hw3.c
OBJS = hw3.o hw3_asm.o

$(bin_PROGRAMS): $(OBJS)
	$(CROSS_CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CROSS_CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CROSS_AS) $(ASFLAGS) -o $@ $<

indent:
	for f in $(C_SOURCES); do indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0 $$f; done

clean:
	-rm -f $(bin_PROGRAMS) $(OBJS)

.PHONY: indent clean
