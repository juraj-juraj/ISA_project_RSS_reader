chmod +x ./tests/integration_tests.sh
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON -S . -B ./build
make -j

