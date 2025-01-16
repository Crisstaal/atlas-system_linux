#include "multithreading.h"
#include <stdint.h>

/**
 * blurPixel - applies a Gaussian Blur filter to a single RGB pixel in an img_t image.
 * @img: pointer to struct containing the original image pixel array.
 * @img_blur: pointer to struct containing the blurred image pixel array.
 * @kernel: pointer to struct containing convolution kernel.
 * @px_x: target pixel x coordinate (left to right).
 * @px_y: target pixel y coordinate (top to bottom).
 */
void blurPixel(const img_t *img, img_t *img_blur, const kernel_t *kernel,
               size_t px_x, size_t px_y)
{
    size_t i, j, k_radius, px_i;
    size_t x, y;
    float r_total = 0, g_total = 0, b_total = 0, weight = 0;

    /** Ensure valid input */
    if (!img || !img_blur || !kernel)
        return;

    /** Ensure the pixel coordinates are valid */
    if (px_x >= img->w || px_y >= img->h)  
        return;

    k_radius = kernel->size / 2;

    /** Iterate over the kernel to apply it to the pixel */
    for (i = 0, y = px_y - k_radius; i < kernel->size; i++, y++)
    {
        for (j = 0, x = px_x - k_radius; j < kernel->size; j++, x++)
        {
            if (x < img->w && y < img->h)  // No need to check x >= 0 and y >= 0
            {
                weight += kernel->matrix[i][j];
                px_i = (y * img->w) + x;

                r_total += (img->pixels + px_i)->r *
                           kernel->matrix[i][j];
                g_total += (img->pixels + px_i)->g *
                           kernel->matrix[i][j];
                b_total += (img->pixels + px_i)->b *
                           kernel->matrix[i][j];
            }
        }
    }

    px_i = (px_y * img->w) + px_x;
    (img_blur->pixels + px_i)->r = (uint8_t)(r_total / weight);
    (img_blur->pixels + px_i)->g = (uint8_t)(g_total / weight);
    (img_blur->pixels + px_i)->b = (uint8_t)(b_total / weight);
}

/**
 * blur_portion - blurs a portion of an image using a Gaussian Blur
 *
 * @portion: pointer to struct containing information needed to perform a blur
 *   operation on a portion of an image file
 */
void blur_portion(blur_portion_t const *portion)
{
    size_t x, y, kx, ky;
    float sum_r, sum_g, sum_b;
    size_t kernel_half = portion->kernel->size / 2;
    
    if (!portion || !portion->img || !portion->img_blur || !portion->kernel || !portion->kernel->matrix)
        return;

    /*Iterate*/
    for (y = portion->y; y < portion->y + portion->h; y++) {
        for (x = portion->x; x < portion->x + portion->w; x++) {
            sum_r = sum_g = sum_b = 0.0f;

            /*Apply the kernel to the current pixel*/
            for (ky = 0; ky < portion->kernel->size; ky++) {
                for (kx = 0; kx < portion->kernel->size; kx++) {
                    /*Get the corresponding pixel in the image*/
                    size_t px = x + kx - kernel_half;
                    size_t py = y + ky - kernel_half;

                    /*Ensure pixel is within bounds*/
                    if (px < portion->img->w && py < portion->img->h) {
                        size_t idx = py * portion->img->w + px;
                        float weight = portion->kernel->matrix[ky][kx];

                        /*Accumulate the weighted color values*/
                        sum_r += portion->img->pixels[idx].r * weight;
                        sum_g += portion->img->pixels[idx].g * weight;
                        sum_b += portion->img->pixels[idx].b * weight;
                    }
                }
            }

            /*Set the blurred pixel value in the blurred image*/
            size_t blurred_idx = y * portion->img_blur->w + x;
            portion->img_blur->pixels[blurred_idx].r = (unsigned char)sum_r;
            portion->img_blur->pixels[blurred_idx].g = (unsigned char)sum_g;
            portion->img_blur->pixels[blurred_idx].b = (unsigned char)sum_b;
        }
    }
}
