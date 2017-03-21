#ifndef SOBEL_HEADER_GUARD
#   define SOBEL_HEADER_GUARD

#   include <stddef.h>

/*
 * Hungarian notation: 'pt2', and 'ppt2'.
 * E.g:
 *     SIntPoint2D pt2_speed = { .x = 1, .y = 2 };
 *     PSIntPoint2D pt2_acceliration = &(SIntPoint2D){ .x = -3, .y = 10 };
 */
typedef struct __SIntPoint2D {
    int x;
    int y;
} SIntPoint2D, *PSIntPoint2D;

/*
 * Hungarian notation: 'ps2', and 'pps2'.
 */
typedef struct __SPosition2D {
    size_t x;
    size_t y;
    size_t stride;
} SPosition2D, *PSPosition2D;

/*
 * Hungarian notation: 'img', and 'pimg'.
 */
typedef struct __SImage {
    unsigned char* pc_data;
    size_t         n_step;
    int            n_width;
    int            n_height;
} SImage, *PSImage;

/*
 * Hungarian notatino: 'esc', and 'pesc'.
 */
typedef enum __SobelStatusCode {
    STATUS_OK           = 0,
    STATUS_ERR_INVALID  = -1,
    STATUS_ERR_NOMEM    = -2,
    STATUS_ERR_NODATA   = -3,
    STATUS_ERR_SINGULAR = -0x10,
} ESobelStatusCode, *PESobelStatusCode;

int getAverageGradients(const unsigned char* pc_data,
                        int                  n_width,
                        int                  n_height,
                        size_t               n_step,
                        int*                 pn_avgGrad);

ESobelStatusCode calcAverageGradient(PSImage pimg_img,
                                     int*    pn_avg_grad);

#endif // SOBEL_HEADER_GUARD
