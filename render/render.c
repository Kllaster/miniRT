#include "mini_rt.h"

void 	inter_plane(t_plane *s_plane, t_ray *s_ray)
{
    double			x;
    t_ray			s_ray_inter;

	x = vector_scalar_mul(&s_ray->s_vector_start_normal, s_plane->s_angle);
    if (ft_dabs(x) <= 0.0001)
        return ;
    s_ray_inter.s_vector_inter = vector_sub(s_plane->s_coordinates, &s_ray->s_vector_start);
	x = vector_scalar_mul(&s_ray_inter.s_vector_inter, s_plane->s_angle) / x;
	if (x <= 0.0001 || s_ray->length <= x)
		return ;
	s_ray_inter = *s_ray;
	s_ray_inter.length = x;
	s_ray_inter.s_color_obj = *s_plane->s_color;
	s_ray_inter.s_vector_inter = vector_mul(&s_ray->s_vector_start_normal, x);
	s_ray_inter.s_vector_inter = vector_sum(&s_ray->s_vector_start, &s_ray_inter.s_vector_inter);
	s_ray_inter.s_vector_inter_normal = *s_plane->s_angle;
	*s_ray = s_ray_inter;
}

void 	inter_sphere(t_sphere *s_sphere, t_ray *s_ray)
{
	double			b;
	double			c;
	double			d_x;
	t_ray			s_ray_inter;

	s_ray_inter.s_vector_inter = vector_sub(&s_ray->s_vector_start, s_sphere->s_coordinates);
	b = 2 * vector_scalar_mul(&s_ray_inter.s_vector_inter, &s_ray->s_vector_start_normal);
	c = vector_scalar_mul(&s_ray_inter.s_vector_inter, &s_ray_inter.s_vector_inter) -
		(s_sphere->radius * s_sphere->radius);
	d_x = b * b - (4 * c);
	if (d_x < 0)
		return ;
	d_x = (-b - sqrt(d_x)) / 2;
	if (d_x >= 0.0001 && d_x < s_ray->length)
	{
		s_ray_inter = *s_ray;
		s_ray_inter.length = d_x;
		s_ray_inter.s_color_obj = *s_sphere->s_color;
		s_ray_inter.s_vector_inter = vector_mul(&s_ray->s_vector_start_normal, d_x);
		s_ray_inter.s_vector_inter = vector_sum(&s_ray->s_vector_start, &s_ray_inter.s_vector_inter);
        s_ray_inter.s_vector_inter_normal = vector_sub(&s_ray_inter.s_vector_inter, s_sphere->s_coordinates);
        *s_ray = s_ray_inter;
	}
}

void 	check_inter_objs(t_stage *s_stage, t_ray *s_ray, double distance)
{
	t_list_objs	*s_list_obj;

	s_ray->length = distance;
	s_list_obj = s_stage->s_list_objs;
	while (s_list_obj)
	{
		if (s_list_obj->type & (unsigned)OBJ_SPHERE)
			inter_sphere(s_list_obj->content, s_ray);
		else if (s_list_obj->type & (unsigned)OBJ_PLANE)
			inter_plane(s_list_obj->content, s_ray);
//		else if (s_list_obj->type & (unsigned)OBJ_SQUARE)
//			inter_square(s_list_obj->content);
//		else if (s_list_obj->type & (unsigned)OBJ_CYLINDER)
//			inter_cylinder(s_list_obj->content);
//		else if (s_list_obj->type & (unsigned)OBJ_TRIANGLE)
//			inter_triangle(s_list_obj->content);
		s_list_obj = s_list_obj->next;
	}
	if (s_ray->length < distance)
        s_ray->s_vector_inter_normal = vector_norm(&s_ray->s_vector_inter_normal);
}

int		get_color_pixel(t_stage *s_stage, t_ray *s_ray)
{
	t_ray		    s_light_ray;
	t_rgb           s_color_res;
	t_rgb           s_color_obj;
	t_rgb           s_color_light;
    t_list          *s_list_light;
    double		    light_length;
    double		    angel_incidence;

	s_color_res = (t_rgb){0,0,0};
	s_list_light = s_stage->s_list_lights;
    if (vector_scalar_mul(&s_ray->s_vector_inter_normal, &s_ray->s_vector_start_normal) > 0)
		s_ray->s_vector_inter_normal = vector_mul(&s_ray->s_vector_inter_normal, -1);
	while (s_list_light)
    {
		s_light_ray.s_vector_start = s_ray->s_vector_inter;
        s_light_ray.s_vector_start_normal = vector_sub(((t_light *)s_list_light->content)->s_coordinates, &s_ray->s_vector_inter);
        light_length = vector_len(&s_light_ray.s_vector_start_normal);
        s_light_ray.s_vector_start_normal = vector_norm(&s_light_ray.s_vector_start_normal);
        check_inter_objs(s_stage, &s_light_ray, light_length);
		if (s_light_ray.length == light_length)
        {
			angel_incidence = vector_scalar_mul(&s_light_ray.s_vector_start_normal,
												&s_ray->s_vector_inter_normal);
			if (angel_incidence > 0)
            {
                s_color_light = rgb_mul(((t_light *)s_list_light->content)->s_color,
                                angel_incidence *
                                ((t_light *)s_list_light->content)->brightness);
				s_color_obj = rgb_mul_arr(&s_ray->s_color_obj, &s_color_light);
				s_color_res = rgb_sum(&s_color_res, &s_color_obj);
            }
        }
        s_list_light = s_list_light->next;
    }
	s_color_light = rgb_mul(s_stage->s_g_light->s_color,
							s_stage->s_g_light->brightness);
	s_color_obj = rgb_mul_arr(&s_ray->s_color_obj, &s_color_light);
	s_color_res = rgb_sum(&s_color_res, &s_color_obj);
	return (rgb_get_int(&s_color_res));
}

int		start_ray(t_stage *s_stage, t_ray *s_ray)
{
	check_inter_objs(s_stage, s_ray, MAX_DISTANCE);
	if (s_ray->length < MAX_DISTANCE)
		return (get_color_pixel(s_stage, s_ray));
	return (DEFAULT_COLOR);
}

void	init_vscreen(t_screen *s_screen, t_camera *s_camera, t_vscreen *s_vscreen)
{
	s_vscreen->width = (double)s_screen->width / 2.0;
	s_vscreen->height = (double)s_screen->height / 2.0;
	s_vscreen->z = (double)s_screen->width / (2 * tan((double)s_camera->fov / 2 * (M_PI / 180)));
}

void	get_another_camera(t_stage *s_stage)
{
	s_stage->s_selected_camera = s_stage->s_list_cameras->content;
	if ((s_stage->s_selected_camera->s_mlx_img = malloc(sizeof(t_mlx_img))) == NULL)
		error_end("Ошибка выделения памяти s_stage->s_selected_camera->s_mlx_img", 1);
	s_stage->s_selected_camera->s_mlx_img->img = mlx_new_image(s_stage->mlx_p,
												s_stage->s_screen.width,
												s_stage->s_screen.height);
	s_stage->s_selected_camera->s_mlx_img->addr = mlx_get_data_addr(s_stage->s_selected_camera->s_mlx_img->img,
								&(s_stage->s_selected_camera->s_mlx_img->bits_per_pixel),
								&(s_stage->s_selected_camera->s_mlx_img->line_length),
								&(s_stage->s_selected_camera->s_mlx_img->endian));
	if ((s_stage->s_selected_camera->s_vscreen = malloc(sizeof(t_vscreen))) == NULL)
		error_end("Ошибка выделения памяти s_stage->s_selected_camera->s_vscreen", 1);
	init_vscreen(&s_stage->s_screen, s_stage->s_selected_camera, s_stage->s_selected_camera->s_vscreen);
}

int		render(t_stage *s_stage)
{
	int				x;
	int				y;
	t_ray 			s_ray;
	t_screen		s_screen;
	t_matrix		s_matrix;

	s_stage->mlx_p = NULL;
	if ((s_stage->mlx_p = mlx_init()) == NULL)
		return (-1);
	mlx_get_screen_size(s_stage->mlx_p, &(s_screen.width), &(s_screen.height));
	if (s_stage->s_screen.width > s_screen.width)
		s_stage->s_screen.width = s_screen.width;
	if (s_stage->s_screen.height > s_screen.height)
		s_stage->s_screen.height = s_screen.height;
	s_stage->mlx_window = mlx_new_window(s_stage->mlx_p, s_stage->s_screen.width,
										 s_stage->s_screen.height, "miniRT");
	get_another_camera(s_stage);
	x = 0;
	while (x < s_stage->s_screen.width)
	{
		y = 0;
		while (y < s_stage->s_screen.height)
		{
			s_ray.s_vector_start = *s_stage->s_selected_camera->s_coordinates;
			s_ray.s_vector_start_normal.x = (x + 0.5) - s_stage->s_selected_camera->s_vscreen->width;
			s_ray.s_vector_start_normal.y = -(y + 0.5) + s_stage->s_selected_camera->s_vscreen->height;
			s_ray.s_vector_start_normal.z = s_stage->s_selected_camera->s_vscreen->z;
			s_matrix = get_matrix_rotate(s_stage->s_selected_camera->s_coordinates, s_stage->s_selected_camera->s_angle);
			s_ray.s_vector_start_normal = matrix_mul(&s_ray.s_vector_start_normal, &s_matrix);
			s_ray.s_vector_start_normal = vector_sub(&s_ray.s_vector_start_normal, s_stage->s_selected_camera->s_coordinates);
			s_ray.s_vector_start_normal = vector_norm(&s_ray.s_vector_start_normal);
			my_mlx_pixel_put(s_stage->s_selected_camera->s_mlx_img, x, y, start_ray(s_stage, &s_ray));
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(s_stage->mlx_p, s_stage->mlx_window, s_stage->s_selected_camera->s_mlx_img->img, 0, 0);
	mlx_loop(s_stage->mlx_p);
	return (0);
}