#include "multithreading.h"
#include <math.h>

/**
 * blur_portion - Apply Gaussian blur to a portion of an image.
 * @portion: Pointer to the portion structure containing the image,
 *           kernel, and region details.
 */
void blur_portion(blur_portion_t const *portion)
{
    size_t i, j, ki, kj;
    int x, y;
    float r, g, b;
    size_t kernel_offset = portion->kernel->size / 2;

    for (y = portion->y; y < portion->y + portion->h; y++)
    {
        for (x = portion->x; x < portion->x + portion->w; x++)
        {
            r = g = b = 0.0;

            /* Apply kernel */
            for (ki = 0; ki < portion->kernel->size; ki++)
            {
                for (kj = 0; kj < portion->kernel->size; kj++)
                {
                    int img_x = x + kj - kernel_offset;
                    int img_y = y + ki - kernel_offset;

                    /* Boundary check */
                    if (img_x >= 0 && img_x < (int)portion->img->w &&
                        img_y >= 0 && img_y < (int)portion->img->h)
                    {
                        pixel_t *pixel = &portion->img->pixels[img_y * portion->img->w + img_x];
                        float kernel_value = portion->kernel->matrix[ki][kj];

                        r += pixel->r * kernel_value;
                        g += pixel->g * kernel_value;
                        b += pixel->b * kernel_value;
                    }
                }
            }

            /* Update blurred image */
            pixel_t *blur_pixel = &portion->img_blur->pixels[y * portion->img->w + x];
            blur_pixel->r = (unsigned char)fmin(fmax(r, 0), 255);
            blur_pixel->g = (unsigned char)fmin(fmax(g, 0), 255);
            blur_pixel->b = (unsigned char)fmin(fmax(b, 0), 255);
        }
    }
}