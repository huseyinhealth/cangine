# CMake wrapper

DebugCXXFlags ?= -DCANGINE_DEBUG -g
EXAMPLE ?= sandbox
BUILDEXAMPLES ?= OFF
BUILDDIR ?= build
EXPORTCOMPILECOMMANDS ?= ON

DebugFlag = -DCMAKE_CXX_FLAGS="$(DebugCXXFlags)"
CMakeGenArgs := -S . -B $(BUILDDIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=$(EXPORTCOMPILECOMMANDS)
CMakeBuildCommand := $(CMakeGenArgs) -DBUILD_EXAMPLES=$(BUILDEXAMPLES)
ExampleFileName = unknown
EditorFileName = unknown

.DEFAULT_GOAL := engine

ifeq ($(OS), Windows_NT)
	ExampleFileName=$(EXAMPLE).exe
	EditorFileName=editor.exe
else
	ExampleFileName=$(EXAMPLE)
	EditorFileName=editor
endif

WEBOUTDIR ?= web-build

web:
	emcmake cmake $(CMakeGenArgs) -DBUILD_EXAMPLES=ON \
		-DCMAKE_BUILD_TYPE=Release \
		-B $(WEBOUTDIR)
	cmake --build $(WEBOUTDIR)
	@echo "Build tamam. Test için:"
	@echo "  cd $(WEBOUTDIR) && python3 -m http.server 8080"

webclean:
	rm -rf $(WEBOUTDIR)

engine:
	cmake $(CMakeBuildCommand)
	cmake --build $(BUILDDIR)

engined:
	cmake $(CMakeBuildCommand) $(DebugFlag)
	cmake --build $(BUILDDIR)

example:
	cmake $(CMakeGenArgs) -DBUILD_EXAMPLES=ON
	cmake --build $(BUILDDIR)
	cd $(BUILDDIR)/examples/$(EXAMPLE) && ./$(ExampleFileName)

exampled:
	cmake $(CMakeGenArgs) -DBUILD_EXAMPLES=ON $(DebugFlag)
	cmake --build $(BUILDDIR)
	cd $(BUILDDIR)/examples/$(EXAMPLE) && gdb ./$(ExampleFileName)

editor:
	cmake $(CMakeGenArgs) -DBUILD_EDITOR=ON -DBUILD_EXAMPLES=OFF
	cmake --build $(BUILDDIR)
	cd $(BUILDDIR)/editor && ./$(EditorFileName)

editord:
	cmake $(CMakeGenArgs) -DBUILD_EDITOR=ON -DBUILD_EXAMPLES=OFF $(DebugFlag) 
	cmake --build $(BUILDDIR)
	cd $(BUILDDIR)/editor && gdb ./$(EditorFileName)

gdbed:
	cmake $(CMakeGenArgs) -DBUILD_EDITOR=ON -DBUILD_EXAMPLES=OFF $(DebugFlag) 
	cmake --build $(BUILDDIR)
	cd $(BUILDDIR)/editor && gdb ./$(EditorFileName)

clean:
	rm -rf $(BUILDDIR)

help:
	@echo -------------
	@echo -- cangine --
	@echo -------------
	@echo " "
	@echo Usage:
	@echo "  make engine             	  Builds cangine game engine."
	@echo "  make engined            	  Builds cangine and passes debug macro to compiler."
	@echo "  make example EXAMPLE=X  	  Builds and runs example X."
	@echo "  make exampled EXAMPLE=X 	  Builds and runs example X in debug mode."
	@echo "  make editor             	  Builds and runs editor"
	@echo "  make editord            	  Builds and runs editor in debug mode."
	@echo ""
	@echo "Optional variables:"
	@echo "  EXAMPLE=name"
	@echo "  BUILDDIR=build"
	@echo "  BUILDEXAMPLES=ON/OFF"
	@echo "  EXPORTCOMPILECOMMANDS=ON/OFF"
	@echo ""
	@echo Thanks for downloading cangine.

.PHONY: help example exampled engine engined clean editor gdbed
