COMP=g++
DEBUG=gdb
COMPILER_OPTIONS=-lstdc++ -x c++
DEBUG_COMPILER_BUILD_OPTIONS=-lstdc++ -x c++ -g
FILES=EMU/main.cpp EMU/CPU1618.cpp
.RECIPEPREFIX=^

Build/JD1618: $(FILES)
^mkdir -pv Build
^$(COMP) -o $@ $(FILES) $(COMPILER_OPTIONS) && cp Test.rom Build

DebugBuild: $(FILES)
^mkdir -pv Build/Debug
^$(COMP) -o Build/Debug/JD1618 $(FILES) $(DEBUG_COMPILER_BUILD_OPTIONS)
^objcopy --only-keep-debug Build/Debug/JD1618 JD1618.elf
^mv JD1618.elf Build/Debug

debug: DebugBuild
^$(DEBUG) -s Build/Debug/JD1618.elf -e Build/Debug/JD1618

run: Build/JD1618
^cp Test.rom Build
^exec $<

clean:
^rm -rf Build

install: Build/JD1618
^sudo install Build/JD1618 /usr/bin/

Cross_Compile_AArch64:
^sudo apt-get install \
    ca-certificates \
    curl \
    gnupg \
    lsb-release
^curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
^echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
^sudo apt-get update
^sudo apt-get install docker-ce docker-ce-cli containerd.io
^docker pull dockcross/android-arm64
^docker run --rm dockcross/android-arm64 > ./dockcross
^sudo chmod +x ./dockcross
^sudo mv ./dockcross /bin/
^dockcross bash -c 'mkdir -pv Build && $(COMP) -o Build/JD1618_aarch64-android $(FILES) $(COMPILER_OPTIONS) && cp Test.rom Build'

 
