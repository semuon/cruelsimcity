# Create you own Makefile.inc in order to define
# machine specific options:
#    - Location of libraries (OPENBLAS_DIR)
include Makefile.inc

# Compiler setup

# Determine compilation mode: debug or release
# By default: release
DEBUG_LEVEL ?= 0

CC		?=	gcc
CPP		?=	g++
LD		?=	g++

###############################################################################
# Apps dir
APPS_DIR	=	./apps
# Source dirs
SRC_DIRS	=	./src
# Objects dir
OBJ_DIR		=	./obj
# Binaries dir
BIN_DIR		= 	./bin

INC		=	-I./include $(LIB_INC)

# Compiler flags

CPPFLAGS	?= -Wall -fopenmp
CCFLAGS		?= -std=c99 -Wall -fopenmp

# Optimization and debugging flags
ifeq ($(DEBUG_LEVEL), 0)
CPPFLAGS += -O2
CCFLAGS  += -O2
else ifeq ($(DEBUG_LEVEL), 1)
CPPFLAGS += -O2 -DENABLE_DEBUG_ASSERT
CCFLAGS  += -O2 -DENABLE_DEBUG_ASSERT
else ifeq ($(DEBUG_LEVEL), 2)
CPPFLAGS += -O0 -g -DDEBUG -DENABLE_DEBUG_ASSERT
CCFLAGS  += -O0 -g -DDEBUG -DENABLE_DEBUG_ASSERT
endif

# Linker flags
LIBS		=	-lm
LDFLAGS		=	-L$(OBJ_DIR) $(LIB_LD)

SRC_CPP	 	:= 	$(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
SRC_CC	 	:= 	$(wildcard $(addsuffix /*.c,   $(SRC_DIRS)))	

OBJS 		:= 	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC_CPP)))
OBJS		+=	$(patsubst %.c,   $(OBJ_DIR)/%.o, $(notdir $(SRC_CC)))

vpath %.c   $(SRC_DIRS)
vpath %.cpp $(SRC_DIRS)


###############################################################################
# targets
###############################################################################

all: simcity

mkdirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean: mkdirs
	@( \
		cd $(OBJ_DIR); \
		rm -f -v *.o; \
		cd ..; \
		cd $(BIN_DIR); \
		rm -f -v *; \
		cd ..; \
		rm -f -v ./tests/runners/*.cpp; \
	)

# Applications
define app_compile_template
 $(1)_DIR  = ./apps/$(1)
 $(1)_SRC  = $$(wildcard $$($(1)_DIR)/*.cpp)
 $(1)_INC  = $$(wildcard $$($(1)_DIR)/*.h)
$(1): mkdirs $$(OBJS) $$($(1)_SRC) $$($(1)_INC)
	@echo "\nBuilding $$@"
	$$(LD) $$(INC) $$(CPPFLAGS) -I$$($(1)_DIR) -o $$(BIN_DIR)/$$@ $$($(1)_SRC) $$(LDFLAGS) $$(OBJS) $$(LIBS)
endef

APPS          = $(notdir $(shell find $(APPS_DIR)/* -type d))

$(foreach app, $(APPS), $(eval $(call app_compile_template,$(app))))

.SUFFIXES:
.SUFFIXES: .cpp .c .o


$(OBJ_DIR)/%.o: %.cpp
	$(CPP) $(INC) $(CPPFLAGS) -c -o $@  $<

$(OBJ_DIR)/%.o: %.c
	$(CC) $(INC) $(CCFLAGS) -c -o $@  $<
