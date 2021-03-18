#include "mini_rt.h"

void	free_rt(t_rt *s_rt)
{
	t_lst	*s_lst_cams;

	if (s_rt->mlx_p)
	{
		s_lst_cams = s_rt->s_stage.s_lst_cams;
		while (s_lst_cams)
		{
			if (((t_cam *)s_lst_cams->content)->s_mlx_img.img_ptr)
				mlx_destroy_image(s_rt->mlx_p,
						((t_cam *)s_lst_cams->content)->s_mlx_img.img_ptr);
			s_lst_cams = s_lst_cams->next;
		}
		if (s_rt->mlx_window)
			mlx_destroy_window(s_rt->mlx_p, s_rt->mlx_window);
	}
	if (s_rt->s_stage.s_ambient_color)
		free(s_rt->s_stage.s_ambient_color);
	if (s_rt->s_stage.s_lst_lights)
		ft_lst_func(s_rt->s_stage.s_lst_lights, free);
	if (s_rt->s_stage.s_lst_cams)
		ft_lst_func(s_rt->s_stage.s_lst_cams, free);
	if (s_rt->s_stage.s_lst_objs)
		ft_lst_obj_func(s_rt->s_stage.s_lst_objs, free);
}

int		end_rt(t_rt *s_rt)
{
	free_rt(s_rt);
	exit(0);
}

void	change_frame(t_rt *s_rt, void *img_old_p)
{
	if (s_rt->s_stage.s_main_cam->render_ready == 0)
		start_render(s_rt);
	if (mlx_clear_window(s_rt->mlx_p, s_rt->mlx_window) == 0)
		error_end("Error at mlx_clear_window()", MLX_ERROR, 0, NULL);
	mlx_put_image_to_window(s_rt->mlx_p, s_rt->mlx_window,
							s_rt->s_stage.s_main_cam->s_mlx_img.img_ptr,
							0, 0);
	debug_window_put_cam(s_rt);
	if (img_old_p != NULL)
		mlx_destroy_image(s_rt->mlx_p, img_old_p);
}

int		check_hooks(t_rt *s_rt)
{
	int						update;
	static long long int	last_hook_time = 0;

	update = 0;
	if (!s_rt->render_now && (last_hook_time + TIME2HOOK) < time_unix_ms())
	{
		if (vec_check_unzero(&s_rt->s_vec_dir))
		{
			update = change_dir_cam(s_rt, &s_rt->s_vec_dir);
			last_hook_time = time_unix_ms();
		}
		else if (vec_check_unzero(&s_rt->s_vec_pos))
		{
			update = change_pos_cam(s_rt, &s_rt->s_vec_pos);
			last_hook_time = time_unix_ms();
		}
		else if (s_rt->change_obj)
		{
			update = 1;
			last_hook_time = time_unix_ms();
			s_rt->change_obj = 0;
			render_all_images(s_rt);
		}
	}
	return (update);
}

int		new_frame(t_rt *s_rt)
{
	static int	update = 0;
	void		*img_old_p;

	update = check_hooks(s_rt);
	if (update && !s_rt->render_now)
	{
		img_old_p = s_rt->s_stage.s_main_cam->s_mlx_img.img_ptr;
		if ((s_rt->s_stage.s_main_cam->s_mlx_img.img_ptr =
				mlx_new_image(s_rt->mlx_p, s_rt->s_stage.width,
								s_rt->s_stage.height)) == NULL)
			error_end("Error at mlx_new_image()", MLX_ERROR, 0, NULL);
		if ((s_rt->s_stage.s_main_cam->s_mlx_img.addr =
			mlx_get_data_addr(
				s_rt->s_stage.s_main_cam->s_mlx_img.img_ptr,
				&(s_rt->s_stage.s_main_cam->s_mlx_img.bits_per_pixel),
				&(s_rt->s_stage.s_main_cam->s_mlx_img.line_length),
				&(s_rt->s_stage.s_main_cam->s_mlx_img.endian))) == NULL)
			error_end("Error at mlx_get_data_addr()", MLX_ERROR, 0, NULL);
		s_rt->s_stage.s_main_cam->render_ready = 0;
		change_frame(s_rt, img_old_p);
		update = 0;
	}
	return (0);
}
