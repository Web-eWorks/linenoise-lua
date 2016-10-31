CC = gcc
CFLAGS += -O2 -fPIC $(CWARN)
CWARN = -Wall -Werror
Q=@

LNDIR = linenoise
SO = linenoise.so

LUAVER = 5.3
LUALIBDIR = /usr/local/lib/lua/$(LUAVER)

.PHONY: all, install, clean, debug

all: $(SO)

debug: CFLAGS += -g -O1
debug: Q =
debug: $(SO)

$(LNDIR)/linenoise.c:
	@ git submodule update $(LNDIR)

$(SO): linenoise-lua.c $(LNDIR)/linenoise.c
	@ echo Compiling {$^} to [$@]
	$(Q) $(CC) $(CFLAGS) -shared -o $@ $^ -I$(LNDIR)

install:
	@ cp -u $(SO) $(LUALIBDIR)/$(SO)

uninstall:
	@ rm $(LUALIBDIR)/$(SO)

clean:
	@ rm -f *.o *.so
