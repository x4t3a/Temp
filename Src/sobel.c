#include <sobel.h>
#include <stdlib.h>
#include <math.h>

static
ESobelStatusCode
calcXGradient(const unsigned char* pc_data,
              const PSPosition2D   pps2_pos,
              int*                 pn_x_grad)
{
    if ((pc_data == NULL) || (pps2_pos == NULL) || (pn_x_grad == NULL))
    { return STATUS_ERR_INVALID; }

    *pn_x_grad = (
            pc_data[ ((pps2_pos->y - 1) * pps2_pos->stride) + (pps2_pos->x - 1) ] +
        2 * pc_data[ ((pps2_pos->y    ) * pps2_pos->stride) + (pps2_pos->x - 1) ] +
            pc_data[ ((pps2_pos->y + 1) * pps2_pos->stride) + (pps2_pos->x - 1) ] -
            pc_data[ ((pps2_pos->y - 1) * pps2_pos->stride) + (pps2_pos->x + 1) ] -
        2 * pc_data[ ((pps2_pos->y    ) * pps2_pos->stride) + (pps2_pos->x + 1) ] -
            pc_data[ ((pps2_pos->y + 1) * pps2_pos->stride) + (pps2_pos->x + 1) ]
    );

    return STATUS_OK;
}

static
ESobelStatusCode
calcYGradient(const unsigned char* pc_data,
              const PSPosition2D   pps2_pos,
              int*                 pn_y_grad)
{
    if ((pc_data == NULL) || (pps2_pos == NULL) || (pn_y_grad == NULL))
    { return STATUS_ERR_INVALID; }

    *pn_y_grad = (
            pc_data[ ((pps2_pos->y - 1) * pps2_pos->stride) + (pps2_pos->x - 1) ] +
        2 * pc_data[ ((pps2_pos->y - 1) * pps2_pos->stride) + (pps2_pos->x    ) ] +
            pc_data[ ((pps2_pos->y - 1) * pps2_pos->stride) + (pps2_pos->x + 1) ] -
            pc_data[ ((pps2_pos->y + 1) * pps2_pos->stride) + (pps2_pos->x - 1) ] -
        2 * pc_data[ ((pps2_pos->y + 1) * pps2_pos->stride) + (pps2_pos->x    ) ] -
            pc_data[ ((pps2_pos->y + 1) * pps2_pos->stride) + (pps2_pos->x + 1) ]
    );

    return STATUS_OK;
}

static
ESobelStatusCode
calcGradient(const PSIntPoint2D ppt2_grad_comp,
             int*               pn_grad)
{
    if ((ppt2_grad_comp == NULL) || (pn_grad == NULL))
    { return STATUS_ERR_INVALID; }

    *pn_grad = (
#if defined(APPROXIMATE_SOBEL)
        abs(ppt2_grad_comp->x) + abs(ppt2_grad_comp->y)
#else
        sqrt(pow(ppt2_grad_comp->x, 2) + pow(ppt2_grad_comp->y, 2))
#endif
    );

    return STATUS_OK;
}

ESobelStatusCode
calcAverageGradient(PSImage pimg_img,
                    int*    pn_avg_grad)
{
    if ((pimg_img == NULL) || (pimg_img->pc_data == NULL) || (pn_avg_grad == NULL))
    { return STATUS_ERR_INVALID; }

    /* If we just sum everything into an int and then divide,
       it is possible that the int will be overflowed on the 1st step.
       We can either sum everything into an double and then divide,
       or divide on each iteration.
       I.e:
            avg = (a + b + c) / n
            avg = (a / n) + (b / n) + (c / n)
       The second option saves us from *unlikly* possible overflow in double,
       but also tends to lose precision (minorly, according to IEEE754).
       But we'll cast the result to an int anyway,
       so losing this much precision is not a problem in this very case.
    */
    SPosition2D ps2_pos = { .x = 0, .y = 0, .stride = pimg_img->n_step };
    SIntPoint2D pt2_img_size = { .x = pimg_img->n_height - 1,
                                 .y = pimg_img->n_width - 1 };
    SIntPoint2D pt2_grad_comp;
    double d_avg_grad = 0.0;
    int n_grad;
    long int li_pixels_number = pimg_img->n_width * pimg_img->n_height;
    ESobelStatusCode esc_status = STATUS_OK;

    *pn_avg_grad = 0;

    for (ps2_pos.y = 1; ps2_pos.y < pt2_img_size.y; ++ps2_pos.y)
    {
        for (ps2_pos.x = 1; ps2_pos.x < pt2_img_size.x; ++ps2_pos.x)
        {
            esc_status = calcXGradient(pimg_img->pc_data, &ps2_pos, &pt2_grad_comp.x);
            if (esc_status != STATUS_OK) { return esc_status; }

            esc_status = calcYGradient(pimg_img->pc_data, &ps2_pos, &pt2_grad_comp.y);
            if (esc_status != STATUS_OK) { return esc_status; }

            esc_status = calcGradient(&pt2_grad_comp, &n_grad);
            if (esc_status != STATUS_OK) { return esc_status; }

            d_avg_grad += ((double) n_grad / li_pixels_number);
        }
    }

    *pn_avg_grad = (int) d_avg_grad;

    return esc_status;
}

int
getAverageGradients(const unsigned char* pc_data,
                    int                  n_width,
                    int                  n_height,
                    size_t               n_step,
                    int*                 pn_avg_grad)
{
    return calcAverageGradient(&(SImage){ .pc_data = (unsigned char*) pc_data,
                                          .n_width = n_width,
                                          .n_height = n_height,
                                          .n_step   = n_step },
                               pn_avg_grad);
}

