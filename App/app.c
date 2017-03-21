#include <sobel.h>
#include <stdio.h>
#include <stdlib.h>

ESobelStatusCode
loadLenaSample(PSImage pimg_lena)
{
    if (pimg_lena == NULL)
    { return STATUS_ERR_INVALID; }

    const size_t li_img_size = 512 * 512;
    FILE* pf_raw_image = fopen("./lena_gray.raw", "rb");
    if (pf_raw_image == NULL)
    { return STATUS_ERR_NODATA; }

    unsigned char* pc_data = malloc(li_img_size);
    if (pc_data == NULL)
    { return STATUS_ERR_NOMEM; }

    (void) fread(pc_data, li_img_size, 1, pf_raw_image);

    pimg_lena->pc_data  = pc_data;
    pimg_lena->n_step   = 512;
    pimg_lena->n_width  = 512;
    pimg_lena->n_height = 512;

    (void) fclose(pf_raw_image);

    return STATUS_OK;
}

int
main(int argc, char* argv[])
{
    SImage img = {0};
    int n_gradient;

    (void) loadLenaSample(&img);
    (void) calcAverageGradient(&img, &n_gradient);

    printf("Average gradient: %d\n", n_gradient);

    if (img.pc_data)
    {
        free(img.pc_data);
    }
}

