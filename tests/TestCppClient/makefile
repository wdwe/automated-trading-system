CXX=g++
CFLAGS=-pthread -Wall -Wno-switch -Wpedantic -Wno-unused-function -std=c++20
ROOT_DIR=..
BASE_SRC_DIR=${ROOT_DIR}/client
INCLUDES=-I${BASE_SRC_DIR} -I${ROOT_DIR}
LDFLAGS=-L${BASE_SRC_DIR}/lib
LDLIBS=-lbid
SHARED_LIB_DIRS=${BASE_SRC_DIR}
SHARD_LIBS=libTwsSocketClient.so
TARGET=TestCppClient

$(TARGET)Static:
	$(CXX) $(CFLAGS) $(INCLUDES) $(BASE_SRC_DIR)/*.cpp ./*.cpp $(LDFLAGS) $(LDLIBS) -o$(TARGET)Static

$(TARGET):
	$(CXX) $(CFLAGS) $(INCLUDES) ./*.cpp $(BASE_SRC_DIR)/lib/libbid.so $(SHARED_LIB_DIRS)/$(SHARD_LIBS) -o$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET)Static *.o

