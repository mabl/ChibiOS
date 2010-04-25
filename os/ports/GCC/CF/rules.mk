# ColdFire makefile scripts and rules.

# Automatic compiler options
OPT = $(USE_OPT)
CPPOPT = $(USE_CPPOPT)
ifeq ($(USE_CURRP_CACHING),yes)
  OPT += -ffixed-a2 -DCH_CURRP_REGISTER_CACHE='"a2"'
endif
ifeq ($(USE_LINK_GC),yes)
  OPT += -ffunction-sections
endif

# Source files groups
SRC	     = $(CSRC)$(CPPSRC)

# Object files groups
COBJS   = $(CSRC:.c=.o)
CPPOBJS = $(CPPSRC:.cpp=.o)
ASMOBJS = $(ASMSRC:.s=.o)
OBJS	= $(ASMOBJS) $(COBJS) $(CPPOBJS)

# Paths
IINCDIR = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))
LLIBDIR = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

# Macros
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)

# Libs
LIBS    = $(DLIBS) $(ULIBS)

MCFLAGS = -m$(MCU)
ODFLAGS	= -x --syms
ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) $(WARN) -Wa,-ald=$(<:.c=.lst) $(DEFS)
ifeq ($(USE_LINK_GC),yes)
  LDFLAGS = $(MCFLAGS) -T$(LDSCRIPT) -nostartfiles -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch,--gc-sections,--relax $(LLIBDIR)
else
  LDFLAGS = $(MCFLAGS) -T$(LDSCRIPT) -nostartfiles -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LLIBDIR)
endif

# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d
# Sibling calls optimizations must be disabled (-fno-optimize-sibling-calls). It is enabled by default in -O2, -O3 or -Os.
# Leaving this option enabled (-foptimize-sibling-calls) the OS don't pass all tests (it hangs on Test Case 2.1).
CPFLAGS += -fno-optimize-sibling-calls
CPFLAGS += --param inline-call-cost=4


#
# Makefile rules
#
all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).dmp

$(CPPOBJS) : %.o : %.cpp
	@echo
	$(CPPC) -c $(CPPFLAGS) -I . $(IINCDIR) $< -o $@

$(COBJS) : %.o : %.c
	@echo
	$(CC) -c $(CPFLAGS) -I . $(IINCDIR) $< -o $@

$(ASMOBJS) : %.o : %.s
	@echo
	$(AS) -c $(ASFLAGS) -I . $(IINCDIR) $< -o $@

%elf: $(OBJS)
	@echo
	$(LD) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN) $< $@

%dmp: %elf
	$(OD) $(ODFLAGS) $< > $@

clean:
	-rm -f $(OBJS)
	-rm -f $(CSRC:.c=.lst) $(CPPSRC:.cpp=.lst) $(ASMSRC:.s=.lst)
	-rm -f $(PROJECT).elf $(PROJECT).dmp $(PROJECT).map $(PROJECT).hex $(PROJECT).bin
	-rm -fR .dep

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
