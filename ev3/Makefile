CC=arm-linux-gnueabi-gcc
CXX=arm-linux-gnueabi-g++
CXXCFLAGS=-DEV3DEV_PLATFORM_EV3 -isystem ev3dev-lang-cpp -pthread
CXXFLAGS=-std=gnu++17 -O3 ${CXXCFLAGS}

lift_test: lift_test.cpp Makefile 
	$(CXX) $(CXXFLAGS) -o $@ $<
