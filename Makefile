CUDA_PATH = /usr/local/cuda
CXX = g++
NVCC = nvcc
OPENCV_FLAGS = `pkg-config --cflags --libs opencv4`

TARGET = image_processor
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(NVCC) -o $@ $^ -lopencv_core -lopencv_highgui -lopencv_imgcodecs $(OPENCV_FLAGS)

clean:
	rm -f $(TARGET)
