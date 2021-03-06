# project configuration
LIB_GLUT_LINUX = -lGL -lGLU -lglut
LIB_GLUT_MAC   = -framework OpenGL -framework GLUT
CC             = g++
CC_FLAGS       = -Wall -Wno-deprecated-declarations -std=c++11 -Ofast -funroll-loops
EXEC           = fftocean

# project structure
BUILD_DIR = build
BIN_DIR   = bin
SRC_DIR   = src
MODULES   = ./ ocean fft rendering parameters cross_platform
SRC_DIRS  = $(addprefix $(SRC_DIR)/, $(MODULES))

# libs and headers subfolders lookup
INCLUDE = -I$(SRC_DIR)
SRC     = $(foreach sdir, $(SRC_DIRS), $(wildcard $(sdir)/*.cpp))
OBJ     = $(foreach sdir, $(SRC_DIRS), $(patsubst $(sdir)/%.cpp, $(BUILD_DIR)/%.o, $(wildcard $(sdir)/*.cpp)))

# sourcefile subfolders lookup
VPATH = $(SRC_DIRS)

# entry point
default:
	@echo "You need to specify the system you are building on. Possibilities:"
	@echo "  'make linux'"
	@echo "  'make mac'"

linux: lib_linux make_dir $(BIN_DIR)/$(EXEC)

mac: lib_mac make_dir $(BIN_DIR)/$(EXEC)

lib_linux:
	$(eval LD_FLAGS = $(LIB_GLUT_LINUX))

lib_mac:
	$(eval LD_FLAGS = $(LIB_GLUT_MAC))

make_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# create binary
$(BIN_DIR)/$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LD_FLAGS)

# objects
$(BUILD_DIR)/main.o: main.cpp Window.hpp Ocean.hpp Height.hpp Philipps.hpp Parameters.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

$(BUILD_DIR)/Camera.o: Camera.cpp Camera.hpp GLUT.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

$(BUILD_DIR)/Window.o: Window.cpp Window.hpp Camera.hpp GLUT.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

$(BUILD_DIR)/FFT.o: FFT.cpp FFT.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

$(BUILD_DIR)/Height.o: Height.cpp Height.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

$(BUILD_DIR)/Ocean.o: Ocean.cpp Ocean.hpp Height.hpp GLUT.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

$(BUILD_DIR)/Philipps.o: Philipps.cpp Philipps.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

$(BUILD_DIR)/Parameters.o: Parameters.cpp Parameters.hpp
	$(CC) $(INCLUDE) $(CC_FLAGS) -o $@ -c $<

clean:
	@rm -f  $(BUILD_DIR)/*.o
	@rm -rf $(BUILD_DIR)
