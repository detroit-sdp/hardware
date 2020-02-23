CC=arm-linux-gnueabi-gcc
CXX=arm-linux-gnueabi-g++
CXXCFLAGS=-DEV3DEV_PLATFORM_EV3 -isystem ev3dev-lang-cpp -pthread
CXXFLAGS=-std=gnu++17 -O3 ${CXXCFLAGS}

lift_test: lift_test.cpp Makefile 
	$(CXX) $(CXXFLAGS) -o $@ $<

quant: lift_quant_analysis.cpp Makefile 
	$(CXX) $(CXXFLAGS) -o $@ $<

speed: lift_speed_analysis.cpp Makefile 
	$(CXX) $(CXXFLAGS) -o $@ $<

