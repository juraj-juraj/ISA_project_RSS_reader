BUILD_SCRIPT=./scripts/build.sh
NAME=xnovos13
FILES=tests src scripts files doc .github CMakeLists.txt Makefile LICENSE .clangd .clangd .clang-tidy .clang-format documentation.pdf

all: build
	make -C build/

test: build
	make -C build/ test

build:
	chmod +x $(BUILD_SCRIPT)
	$(BUILD_SCRIPT)
	ln -s ./build/feedreader feedreader 

zip:
	tar -cvf $(NAME).tar $(FILES)

clear:
	rm feedreader
	rm -rf build/
