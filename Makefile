COMP=g++
DEBUG=gdb
COMPILER_OPTIONS=-lstdc++ -x c++
DEBUG_COMPILER_BUILD_OPTIONS=-lstdc++ -x c++ -g
FILES=EMU/main.cpp EMU/CPU1618.cpp

Build/JD1618: $(FILES)
	mkdir -pv Build
	$(COMP) -o $@ $(FILES) $(COMPILER_OPTIONS)

DebugBuild: $(FILES)
	mkdir -pv Build/Debug
	$(COMP) -o Build/Debug/JD1618 $(FILES) $(DEBUG_COMPILER_BUILD_OPTIONS)
	objcopy --only-keep-debug Build/Debug/JD1618 JD1618.elf
	mv JD1618.elf Build/Debug

debug: DebugBuild
	$(DEBUG) -s Build/Debug/JD1618.elf -e Build/Debug/JD1618

run: Build/JD1618
	cp Test.rom Build
	exec $<

clean:
	rm -rf Build