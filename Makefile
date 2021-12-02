#
# Makefile for module.
#

CROSS		?= 


AS		:= $(CROSS)gcc -x assembler-with-cpp
CC		:= $(CROSS)gcc
CXX		:= $(CROSS)g++
LD		:= $(CROSS)ld
AR		:= $(CROSS)ar
OC		:= $(CROSS)objcopy
OD		:= $(CROSS)objdump
RM		:= rm -fr


ASFLAGS		:= -g -ggdb -Wall -O3 -ffunction-sections -fdata-sections -ffreestanding -std=gnu99
CFLAGS		:= -g -ggdb -Wall -O3 -ffunction-sections -fdata-sections -ffreestanding -std=gnu99
CXXFLAGS	:= -g -ggdb -Wall -O3 -ffunction-sections -fdata-sections -ffreestanding -std=gnu99
LDFLAGS		:= -Wl,-gc-sections
ARFLAGS		:= -rcs
OCFLAGS		:= -v -O binary
ODFLAGS		:=
MCFLAGS		:=

LIBDIRS		:=
LIBS 		:= -lm

INCDIRS		:= -I .
SRCDIRS		:= .

SFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPPFILES	:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

SDEPS		:= $(patsubst %, %, $(SFILES:.S=.o.d))
CDEPS		:= $(patsubst %, %, $(CFILES:.c=.o.d))
CPPDEPS		:= $(patsubst %, %, $(CPPFILES:.cpp=.o.d))
DEPS		:= $(SDEPS) $(CDEPS) $(CPPDEPS)

SOBJS		:= $(patsubst %, %, $(SFILES:.S=.o))
COBJS		:= $(patsubst %, %, $(CFILES:.c=.o))
CPPOBJS		:= $(patsubst %, %, $(CPPFILES:.cpp=.o)) 
OBJS		:= $(SOBJS) $(COBJS) $(CPPOBJS)

OBJDIRS		:= $(patsubst %, %, $(SRCDIRS))
NAME		:= example
VPATH		:= $(OBJDIRS)

.PHONY:		all clean

all : $(NAME)

$(NAME) : $(OBJS)
	@echo [LD] Linking $@
	@$(CC) $(LDFLAGS) $(LIBDIRS) $^ -o $@ $(LIBS) -static

$(SOBJS) : %.o : %.S
	@echo [AS] $<
	@$(AS) $(ASFLAGS) $(INCDIRS) -c $< -o $@
	@$(AS) $(ASFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(COBJS) : %.o : %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@
	@$(CC) $(CFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(CPPOBJS) : %.o : %.cpp
	@echo [CXX] $<
	@$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@	
	@$(CXX) $(CXXFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

clean:
	@$(RM) $(DEPS) $(OBJS) $(NAME).map $(NAME) *.png *~
	@echo Clean complete.
