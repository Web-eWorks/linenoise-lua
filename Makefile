CC = clang
CFLAGS = -O2 $(CWARN)
CWARN = -Wall -Werror

LNDIR = linenoise
SO = linenoise.so

LUAVER = 5.3
LUALIBDIR = /usr/local/lib/lua/$(LUAVER)

.PHONY: all, install, clean

all: $(SO)

$(LNDIR)/linenoise.c:
	git submodule update $(LNDIR)

$(SO): CFLAGS += -fPIC
$(SO): linenoise-lua.c $(LNDIR)/linenoise.c
	$(CC) $(CFLAGS) -shared -o $@ $^ -I$(LNDIR)

install:
	cp -u $(SO) $(LUALIBDIR)/$(SO)

uninstall:
	rm $(LUALIBDIR)/$(SO)

clean:
	rm -f *.o *.so
