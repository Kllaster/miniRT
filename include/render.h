#ifndef RENDER_H
# define RENDER_H

# include "mini_rt.h"

typedef	struct	s_ray {
	t_vec	s_vec_inter_normal;
	t_vec	s_vec_inter;
	t_vec	s_vec_start_normal;
	t_vec	s_vec_start;
	t_rgb	s_color_obj;
	double	length;
}				t_ray;

void			start_render(t_rt *s_rt);
void			create_threads_data(t_rt *s_rt);
void			get_another_camera(t_rt *s_rt);
void			init_render(t_rt *s_rt);
void			*render(void *data);
void			inter_sphere(t_sphere *s_sphere, t_ray *s_ray);
void			inter_plane(t_plane *s_plane, t_ray *s_ray);
void			my_mlx_pixel_put(t_mlx_img *s_mlx_img, int x, int y, int color);
void			get_aa_sample(t_aa_sample *s_aa_sample);

#endif