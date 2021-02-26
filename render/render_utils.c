#include "mini_rt.h"

void	get_aa_sample(t_aa_sample *s_aa_sample)
{
	int 	i;
	double	aa_sample1[1][2] = {{0,0}};
	double	aa_sample2[2][2] = {{-0.35,0.35}, {0.35,-0.35}};
	double	aa_sample3[3][2] = {{-0.45,0.45}, {-0.4,-0.45}, {-0.45,-0.4}};
	double	aa_sample4[4][2] = {{-0.45,0.4}, {0.4,0.45}, {-0.4,-0.45}, {-0.45,-0.45}};
	double	aa_sample5[5][2] = {{-0.45,0.4}, {0.4,0.45}, {0,0}, {-0.4,-0.45}, {-0.45,-0.45}};

	if ((s_aa_sample->matrix_sample = malloc(sizeof(double) * ANTI_ALIASING)) == NULL)
		error_end("Ошибка выделения памяти s_aa_sample->matrix_sample", MALLOC_ERROR);
	i = -1;
	while (i++ < ANTI_ALIASING)
	{
		if ((s_aa_sample->matrix_sample[i] = malloc(sizeof(double) * 2)) == NULL)
			error_end("Ошибка выделения памяти s_aa_sample->matrix_sample[]", MALLOC_ERROR);
		if (ANTI_ALIASING == 1)
		{
			s_aa_sample->matrix_sample[i][0] = aa_sample1[i][0];
			s_aa_sample->matrix_sample[i][1] = aa_sample1[i][1];
		}
		else if (ANTI_ALIASING == 2)
		{
			s_aa_sample->matrix_sample[i][0] = aa_sample2[i][0];
			s_aa_sample->matrix_sample[i][1] = aa_sample2[i][1];
		}
		else if (ANTI_ALIASING == 3)
		{
			s_aa_sample->matrix_sample[i][0] = aa_sample3[i][0];
			s_aa_sample->matrix_sample[i][1] = aa_sample3[i][1];
		}
		else if (ANTI_ALIASING == 4)
		{
			s_aa_sample->matrix_sample[i][0] = aa_sample4[i][0];
			s_aa_sample->matrix_sample[i][1] = aa_sample4[i][1];
		}
		else if (ANTI_ALIASING == 5)
		{
			s_aa_sample->matrix_sample[i][0] = aa_sample5[i][0];
			s_aa_sample->matrix_sample[i][1] = aa_sample5[i][1];
		}
	}
}

int		rgb_get_int(t_rgb *s_rgb)
{
	if (!s_rgb)
		return (0);
	if (s_rgb->red > 255)
		s_rgb->red = 255;
	if (s_rgb->green > 255)
		s_rgb->green = 255;
	if (s_rgb->blue > 255)
		s_rgb->blue = 255;
	return (s_rgb->red << 16 | s_rgb->green << 8 | s_rgb->blue);
}

void	my_mlx_pixel_put(t_mlx_img *s_mlx_img, int x, int y, int color)
{
	*(unsigned int *)(s_mlx_img->addr + (y * s_mlx_img->line_length + x *
									(s_mlx_img->bits_per_pixel / 8))) = color;
}