#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "multithreading.h"

#define MAX_THREADS 16

// Structure to pass data to threads
typedef struct thread_data {
    img_t *img_blur;
    img_t const *img;
    kernel_t const *kernel;
    size_t start_row;
    size_t end_row;
} thread_data_t;

/**
 * blur_pixel - Apply the kernel to a pixel in the image
 * @img: Image to process
 * @kernel: Convolution kernel
 * @x: X coordinate of the pixel to blur
 * @y: Y coordinate of the pixel to blur
 * Return: The blurred pixel
 */
pixel_t blur_pixel(img_t const *img, kernel_t const *kernel, size_t x, size_t y) {
    int kernel_offset, pixel_x, pixel_y;
    float red = 0, green = 0, blue = 0;
    int kernel_half = kernel->size / 2;

    // Apply kernel
    for (int ky = 0; ky < kernel->size; ky++) {
        for (int kx = 0; kx < kernel->size; kx++) {
            pixel_x = x + kx - kernel_half;
            pixel_y = y + ky - kernel_half;

            // Check if the pixel is within bounds
            if (pixel_x >= 0 && pixel_x < img->w && pixel_y >= 0 && pixel_y < img->h) {
                kernel_offset = ky * kernel->size + kx;
                red += img->pixels[pixel_y * img->w + pixel_x].r * kernel->matrix[ky][kx];
                green += img->pixels[pixel_y * img->w + pixel_x].g * kernel->matrix[ky][kx];
                blue += img->pixels[pixel_y * img->w + pixel_x].b * kernel->matrix[ky][kx];
            }
        }
    }

    pixel_t blurred_pixel = {red, green, blue};
    return blurred_pixel;
}

/**
 * blur_portion - Process a portion of the image and blur it
 * @arg: The thread's data (portion of the image to blur)
 */
void *blur_portion(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    img_t *img_blur = data->img_blur;
    img_t const *img = data->img;
    kernel_t const *kernel = data->kernel;

    for (size_t y = data->start_row; y < data->end_row; y++) {
        for (size_t x = 0; x < img->w; x++) {
            img_blur->pixels[y * img->w + x] = blur_pixel(img, kernel, x, y);
        }
    }

    return NULL;
}

/**
 * blur_image - Blur an entire image using multiple threads
 * @img_blur: The resulting blurred image
 * @img: The original image
 * @kernel: The kernel to apply
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel) {
    pthread_t threads[MAX_THREADS];
    thread_data_t thread_data[MAX_THREADS];
    size_t rows_per_thread = img->h / MAX_THREADS;
    size_t remainder = img->h % MAX_THREADS;

    // Divide the work among threads
    for (size_t i = 0; i < MAX_THREADS; i++) {
        thread_data[i].img_blur = img_blur;
        thread_data[i].img = img;
        thread_data[i].kernel = kernel;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i + 1) * rows_per_thread;

        // Add the remainder to the last thread
        if (i == MAX_THREADS - 1) {
            thread_data[i].end_row += remainder;
        }

        pthread_create(&threads[i], NULL, blur_portion, &thread_data[i]);
    }

    /*Wait for all threads to complete*/
    for (size_t i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}