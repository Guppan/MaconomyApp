
# The path to the curl directory has to be specified on the command line.
# make CURL_PATH=<path to CURL directory>
#
# To build unit tests, define TEST=1 on the command line.
# make CURL_PATH=... TEST=1

TARGET_EXEC := maconomyLog

# Build directory
BUILD_DIR := ./build

# Source file directory.
SRC_DIRS := ./src

# Test file directory.
TEST_DIR := $(BUILD_DIR)/src/UnitTesting

# Curl include directory.
INC_CURL := $(CURL_PATH)/include

# Curl library directory.
LIB_CURL := $(CURL_PATH)/lib

# Curl bin directory.
BIN_CURL := $(CURL_PATH)/bin


# Recursively find all C++ files to compile.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
INC_CURL_FLAGS := $(addprefix -I,$(INC_CURL))

CPPFLAGS := $(INC_FLAGS) $(INC_CURL_FLAGS) -MMD -MP
CXXFLAGS := -std=c++14

ifdef UNIT_TEST

# Path to test files.
TEST_FILES := $(shell find $(SRC_DIRS)/UnitTesting -name '*.txt')

# Compiler flag for running unit tests.
CPPFLAGS += -DUNIT_TEST

endif

LDFLAGS += -L$(LIB_CURL) -lcurl.dll

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	rm -r $(BUILD_DIR)/src
ifdef UNIT_TEST
	mkdir -p $(TEST_DIR)
	cp $(TEST_FILES) $(TEST_DIR)
endif
	
	
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	cp $(BIN_CURL)/libcurl.dll $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	
-include $(DEPS)