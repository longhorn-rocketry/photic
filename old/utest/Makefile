make utest:
	g++ -std=c++11 utest_main.cpp -o utest -lCppUTest \
	-I../src/ \
	-I../src/gnc \
	-I../src/math \
	-I../src/model \
	-I../src/platform \
	-I../src/util \
	../src/gnc/kfilter.cpp \
	../src/gnc/kfilter.h \
	../src/math/matrix.cpp \
	../src/math/matrix.h \
	../src/math/rocketmath.cpp \
	../src/math/rocketmath.h \
	../src/util/history.cpp \
	../src/util/history.h \
	../src/util/metronome.cpp \
	../src/util/metronome.h \
	../src/util/float16.cpp \
	../src/util/float16.h \
	../src/platform/imu.cpp \
	../src/platform/imu.h \
	../src/platform/barometer.cpp \
	../src/platform/barometer.h \
	../src/platform/device.h \
	../src/model/stdatmos.cpp \
	../src/model/stdatmos.h \
	../src/util/structutil.h \
	testutil.h \
	utest_matrix.cpp \
	utest_kalman.cpp \
	utest_history.cpp \
	utest_metronome.cpp \
	utest_float16.cpp \
	utest_stdatmos.cpp

make clean:
	rm utest
