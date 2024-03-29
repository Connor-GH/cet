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
COMMON_FLAGS = -O2 -pipe $(IVARS) -DLANGNAME_STRING=\"$(LANG_NAME)\"
the_CFLAGS = $(COMMON_FLAGS) -pedantic $(_CFLAGS)
the_CXXFLAGS = $(COMMON_FLAGS) -Wno-c++98-compat -fno-rtti $(_CXXFLAGS)
the_LFLAGS = $(LDFLAGS) $(IVARS) $(_LFLAGS)
the_CXX_LFLAGS = $(the_LFLAGS)

.PHONY: all clean remove

all:
	$(MAKE) remove
	$(MAKE) $(TARGET)


$(TARGET): $(OBJECTS_C) $(OBJECTS_CXX) | $(BINDIR)
	$(CXX) -o $(BINDIR)/$@ $(the_CXX_LFLAGS) $^

$(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -o $@ $(the_CFLAGS) $^ -c

$(OBJDIR)/%.o : $(SRCDIR_FRONTEND)/%.c | $(OBJDIR)
	$(CC) -o $@ $(the_CFLAGS) $^ -c

$(OBJDIR)/%.o : $(SRCDIR_BACKEND)/%.c | $(OBJDIR)
	$(CC) -o $@ $(the_CFLAGS) $^ -c

$(OBJDIR)/%.o : $(SRCDIR_FRONTEND)/%.cpp | $(OBJDIR)
	$(CXX) -o $@ $(the_CXXFLAGS) $^ -c

$(OBJDIR)/%.o : $(SRCDIR_BACKEND)/%.cpp | $(OBJDIR)
	$(CXX) -o $@ $(the_CXXFLAGS) $^ -c


$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJECTS_C) $(OBJECTS_CXX)

remove: clean
	rm -rf $(BINDIR)/$(TARGET)

