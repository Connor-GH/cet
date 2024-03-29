# variables defined for "cc_and_flags" API
PACKAGE_NAME = "C Explicit Types"
TARGET = cet
PACKAGE_VERSION = 0.0.1
C_STD ?= c11
CXX_STD ?= c++17
# setting up sources
SRCDIR = src
OBJDIR = obj
BINDIR = bin
SRCDIR_FRONTEND = $(SRCDIR)/frontend
SRCDIR_BACKEND_ = $(SRCDIR)/backend
INCLUDES = $(wildcard $(SRCDIR)/include/*.h)
LANG_NAME = $(TARGET)
# parsing/frontend
SOURCES_C_FRONTEND = $(wildcard $(SRCDIR_FRONTEND)/*.c)
OBJECTS_C_FRONTEND = $(SOURCES_C_FRONTEND:$(SRCDIR_FRONTEND)/%.c=$(OBJDIR)/%.o)
SOURCES_CXX_FRONTEND = $(wildcard $(SRCDIR_FRONTEND)/*.cpp)
OBJECTS_CXX_FRONTEND = $(SOURCES_CXX_FRONTEND:$(SRCDIR_FRONTEND)/%.cpp=$(OBJDIR)/%.o)

### SELF-HOSTING VARIABLES
CET ?= ../../bin/cet



TARGET_ARCH ?= x86
ifeq ($(TARGET_ARCH), x86)
	MARCH = x86-64
endif
SRCDIR_BACKEND = $(SRCDIR_BACKEND_)/arch/$(TARGET_ARCH)


# codegen/backend
SOURCES_C_BACKEND = $(wildcard $(SRCDIR_BACKEND)/*.c)
OBJECTS_C_BACKEND = $(SOURCES_C_BACKEND:$(SRCDIR_BACKEND)/%.c=$(OBJDIR)/%.o)
SOURCES_CXX_BACKEND = $(wildcard $(SRCDIR_BACKEND)/*.cpp)
OBJECTS_CXX_BACKEND = $(SOURCES_CXX_BACKEND:$(SRCDIR_BACKEND)/%.cpp=$(OBJDIR)/%.o)

SOURCES_C_DRIVER = $(wildcard $(SRCDIR)/*.c)
OBJECTS_C_DRIVER = $(SOURCES_C_DRIVER:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

SOURCES_C = $(SOURCES_C_FRONTEND) $(SOURCES_C_BACKEND) $(SOURCES_C_DRIVER)
OBJECTS_C = $(OBJECTS_C_FRONTEND) $(OBJECTS_C_BACKEND) $(OBJECTS_C_DRIVER)
SOURCES_CXX = $(SOURCES_CXX_FRONTEND) $(SOURCES_CXX_BACKEND)
OBJECTS_CXX = $(OBJECTS_CXX_FRONTEND) $(OBJECTS_CXX_BACKEND)

IVARS = -I$(SRCDIR)/include -I$(SRCDIR_FRONTEND)/include -I$(SRCDIR_BACKEND)/include

include cc_and_flags.mk
COMMON_FLAGS = -O2 -pipe $(IVARS) \
			   -DLANGNAME_STRING=\"$(LANG_NAME)\" -DLANGNAME=$(LANG_NAME)
the_CFLAGS = -pedantic $(COMMON_FLAGS) $(_CFLAGS)
the_CXXFLAGS = $(COMMON_FLAGS) -std=$(CXX_STD) -Wno-c++98-compat $(CXXFLAGS)
the_LFLAGS = $(LDFLAGS) $(IVARS) $(_LFLAGS)
the_CXX_LFLAGS = $(the_LFLAGS)

.PHONY: all clean remove

all: remove $(TARGET)


$(TARGET): $(OBJECTS_C) $(OBJECTS_CXX) | $(BINDIR)
	$(CXX) -o $(BINDIR)/$@ $(the_CXX_LFLAGS) $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
#	$(CC) -o $@ $(the_CFLAGS) $^ -c
	$(CET) -f $^  | $(CC) -xc - -c -o $@ $(the_CFLAGS)

$(OBJDIR)/%.o : $(SRCDIR_FRONTEND)/%.c | $(OBJDIR)
#	$(CC) -o $@ $(the_CFLAGS) $^ -c
	$(CET) -f $^  | $(CC) -xc - -c -o $@ $(the_CFLAGS)

$(OBJDIR)/%.o : $(SRCDIR_BACKEND)/%.c | $(OBJDIR)
#	$(CC) -o $@ $(the_CFLAGS) $^ -c
	$(CET) -f $^  | $(CC) -xc - -c -o $@ $(the_CFLAGS)

$(OBJDIR)/%.o : $(SRCDIR_FRONTEND)/%.cpp | $(OBJDIR)
#	$(CXX) -o $@ $(the_CXXFLAGS) $^ -c
	$(CET) -f $^  | $(CXX) -xc++ - -c -o $@ $(the_CXXFLAGS)

$(OBJDIR)/%.o : $(SRCDIR_BACKEND)/%.cpp | $(OBJDIR)
#	$(CXX) -o $@ $(the_CXXFLAGS) $^ -c
	$(CET) -f $^  | $(CXX) -xc++ - -c -o $@ $(the_CXXFLAGS)


$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJECTS_C) $(OBJECTS_CXX)

remove: clean
ifeq (SELFHOST, 1)
	rm -rf $(BINDIR)/$(TARGET)
endif

