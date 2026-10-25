CC      = i686-w64-mingw32-gcc
SDL     = third_party/SDL3-3.4.18/i686-w64-mingw32
OUT     = build/pc

GAME_CFLAGS = -std=gnu89 -O0 -g -fcommon -fno-strict-aliasing -fwrapv -Igame/include -include port/port.h \
              -w
PORT_CFLAGS = -std=gnu11 -O2 -g -fno-omit-frame-pointer -Wall -Wno-unused -I$(SDL)/include -Iport
LDFLAGS     = -L$(SDL)/lib -lSDL3 -lwinmm -mconsole -Wl,--disable-dynamicbase

VERSIONS = en fr de nl
EXE_en   = zelda.exe
EXE_fr   = zelda_fr.exe
EXE_de   = zelda_de.exe
EXE_nl   = zelda_nl.exe
DEF_en   =
DEF_fr   = -DFRENCH
DEF_de   = -DGERMAN
DEF_nl   = -DDUTCH

GAME_SRC = $(wildcard game/src/*.c)
PORT_SRC = $(wildcard port/*.c)
PORT_OBJ = $(patsubst port/%.c,$(OUT)/port/%.o,$(PORT_SRC)) $(OUT)/port/zelda_res.o

all: $(foreach v,$(VERSIONS),$(OUT)/$(EXE_$(v))) $(OUT)/SDL3.dll

define VERSION_RULES
build/gen/$(1)/a5globals.s build/gen/$(1)/a5swap.c: tools/gen_globals.py config/symbols.txt $(wildcard config/$(1)/symbols.txt) $(wildcard game/include/*.h) $(GAME_SRC)
	python3 tools/gen_globals.py --version $(1)

$(OUT)/$(1)/game/%.o: game/src/%.c game/include/game.h game/include/version.h port/port.h
	@mkdir -p $$(dir $$@)
	$(CC) $(GAME_CFLAGS) $(DEF_$(1)) -c $$< -o $$@

$(OUT)/$(1)/gen/a5globals.o: build/gen/$(1)/a5globals.s
	@mkdir -p $$(dir $$@)
	$(CC) -c $$< -o $$@

$(OUT)/$(1)/gen/a5swap.o: build/gen/$(1)/a5swap.c
	@mkdir -p $$(dir $$@)
	$(CC) -O2 -c $$< -o $$@

$(OUT)/$(EXE_$(1)): $(patsubst game/src/%.c,$(OUT)/$(1)/game/%.o,$(GAME_SRC)) $(OUT)/$(1)/gen/a5globals.o $(OUT)/$(1)/gen/a5swap.o $(PORT_OBJ)
	$(CC) -o $$@ $$^ $(LDFLAGS)
endef
$(foreach v,$(VERSIONS),$(eval $(call VERSION_RULES,$(v))))

$(OUT)/port/%.o: port/%.c $(wildcard port/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(PORT_CFLAGS) -c $< -o $@

$(OUT)/port/zelda_res.o: port/zelda.rc port/zelda.ico
	@mkdir -p $(dir $@)
	i686-w64-mingw32-windres -I port $< -o $@

$(OUT)/SDL3.dll: $(SDL)/bin/SDL3.dll
	cp $< $@

REL := build/release/zelda-wand-of-gamelon-pc

release: all
	rm -rf build/release
	mkdir -p $(REL)
	for v in $(VERSIONS); do \
		case $$v in en) e=zelda.exe;; *) e=zelda_$$v.exe;; esac; \
		i686-w64-mingw32-strip -o $(REL)/$$e $(OUT)/$$e; \
	done
	cp $(OUT)/SDL3.dll README.md $(REL)/
	cp $(SDL)/../LICENSE.txt $(REL)/SDL3-LICENSE.txt
	cd build/release && python3 -c "import shutil; shutil.make_archive('zelda-wand-of-gamelon-pc', 'zip', '.', 'zelda-wand-of-gamelon-pc')"

clean:
	rm -rf build/pc build/gen build/release

.PHONY: all clean release
