WGCC   = -Wlogical-op -Wcast-align=strict
WGCC  += -Wsuggest-attribute=format -Wsuggest-attribute=malloc
WGCC  += -Wsuggest-attribute=pure -Wsuggest-attribute=const
WGCC  += -Wsuggest-attribute=noreturn -Wsuggest-attribute=cold
WGCC  += -Wformat-security -Warray-bounds -Wstack-protector
WGCC  += -Wall -Wextra -Wpedantic -Wshadow -Wvla -Wpointer-arith -Wwrite-strings \
		 -Wfloat-equal -Wcast-align -Wcast-qual -Wbad-function-cast \
		 -Wunreachable-code -Wundef -Werror=format-security -Werror=array-bounds

WNOFLAGS=
VISIBILITY ?= -fvisibility=hidden

ifeq ($(RELEASE),true)
	_CFLAGS = -march=$(MARCH) -O2 -D_POSIX_C_SOURCE=200809L
else
	_CFLAGS = -march=native -O2 -D_POSIX_C_SOURCE=200809L
endif
_LFLAGS  =

# detect if the user chose GCC or Clang

ifeq ($(shell $(CC) -v 2>&1 | grep -c "gcc version"), 1)
	include gcc_chosen.mk
	ifeq ($(DEBUG),true)
		# gcc-specific security/debug flags
		WGCC   += -fanalyzer
		_CFLAGS += -ggdb
	endif #debug
	_CFLAGS += $(WGCC)
	LTO = -flto -fno-fat-lto-objects

else ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang version"), 1)
	include clang_chosen.mk
	ifeq ($(DEBUG),true)
	# clang-specific security/debug flags
		_CFLAGS += -fsanitize=undefined,signed-integer-overflow,null,alignment,address,leak,cfi \
				  -fsanitize-undefined-trap-on-error -ftrivial-auto-var-init=pattern \
				  -mspeculative-load-hardening -mretpoline
		_LFLAGS  = -fsanitize=address
endif #debug

	_CFLAGS += -Weverything
	WNOFLAGS += -Wno-disabled-macro-expansion
	LTO = -flto=thin
endif #compiler


ifeq ($(DEBUG),true)
	# generic security/debug flags
	_CFLAGS += -Og -D_DEBUG -fno-builtin
	_LFLAGS += -Wl,-z,relro,-z,noexecstack
endif # DEBUG
ifeq ($(RELEASE),true)
	_CFLAGS += -fstack-clash-protection -D_FORTIFY_SOURCE=2 -fcf-protection \
			  -Werror=format-security
	_LFLAGS += -fPIE -fPIC
endif


# Flags every compile will need
_CFLAGS += -D_PACKAGE_NAME=\"$(PACKAGE_NAME)\" -D_PACKAGE_VERSION=\"$(PACKAGE_VERSION)\" \
		  -std=$(C_STD) $(LTO) $(WNOFLAGS) $(VISIBILITY) $(CFLAGS)
_LFLAGS += $(LTO) -L/usr/local/lib $(LDFLAGS)
