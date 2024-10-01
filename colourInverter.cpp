#include <iostream>
#include <cuda.h>
#include <opencv2/opencv.hpp>

#define N 256 // Size of the image (N x N)

// CUDA kernel to apply a simple filter (for demonstration, we just invert the colors)
__global__ void processCatImage(unsigned char* img, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x; // Column index
    int y = blockIdx.y * blockDim.y + threadIdx.y; // Row index

    if (x < width && y < height) {
        int idx = (y * width + x) * 3;
        img[idx + 0] = 255 - img[idx + 0]; // Invert Red channel
        img[idx + 1] = 255 - img[idx + 1]; // Invert Green channel
        img[idx + 2] = 255 - img[idx + 2]; // Invert Blue channel
    }
}

int main() {
    // Load the cat image using OpenCV
    cv::Mat image = cv::imread("cat.jpg");
    
    if (image.empty()) {
        std::cerr << "Error: Could not load image." << std::endl;
        return -1;
    }

    // Resize image to N x N for simplicity
    cv::resize(image, image, cv::Size(N, N));

    // Allocate memory for the image on the host
    unsigned char* h_img = image.data;

    // Allocate memory on the device
    unsigned char* d_img;
    cudaMalloc(&d_img, N * N * 3 * sizeof(unsigned char));

    // Copy the image to the device
    cudaMemcpy(d_img, h_img, N * N * 3 * sizeof(unsigned char), cudaMemcpyHostToDevice);

    // Define block and grid sizes
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((N + threadsPerBlock.x - 1) / threadsPerBlock.x,
                   (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // Launch the kernel to process the cat image
    processCatImage<<<numBlocks, threadsPerBlock>>>(d_img, N, N);

    // Copy the result back to the host
    cudaMemcpy(h_img, d_img, N * N * 3 * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    // Display the processed image
    cv::imshow("Processed Cat Image", image);
    cv::waitKey(0);

    // Cleanup
    cudaFree(d_img);

    return 0;
}