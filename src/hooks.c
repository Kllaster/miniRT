#include "mini_rt.h"

void	change_img_window(t_rt *s_rt)
{
	if (s_rt->s_stage.s_main_camera->render_ready == 1)
	{
		mlx_clear_window(s_rt->mlx_p, s_rt->mlx_window);
		mlx_put_image_to_window(s_rt->mlx_p, s_rt->mlx_window,
							s_rt->s_stage.s_main_camera->s_mlx_img.img_ptr, 0, 0);
	}
	else
		start_render(s_rt);
}

int		change_pos_camera(t_rt *s_rt, t_vec *s_vec)
{
	*s_rt->s_stage.s_main_camera->s_vec_origin =
			vec_sum(s_rt->s_stage.s_main_camera->s_vec_origin, s_vec);
	return (1);
}

int		change_dir_camera(t_rt *s_rt, t_vec *s_vec)
{
	t_vec	s_vec_new;

	if (s_rt->s_stage.s_main_camera->s_vec_dir->z == -1)
		*s_vec = vec_mul(s_vec, -1);
	s_vec_new = vec_sum(s_rt->s_stage.s_main_camera->s_vec_dir, s_vec);
	if (s_vec_new.x > 1)
		return (0);
	if (s_vec_new.x < -1)
		return (0);
	if (s_vec_new.y > 1)
		return (0);
	if (s_vec_new.y < -1)
		return (0);
	if (s_vec_new.z > 1)
		return (0);
	if (s_vec_new.z < -1)
		return (0);
	*s_rt->s_stage.s_main_camera->s_vec_dir = s_vec_new;
	s_rt->s_stage.s_main_camera->s_matrix_rotate =
			get_matrix_rotate(s_rt->s_stage.s_main_camera->s_vec_origin,
								  s_rt->s_stage.s_main_camera->s_vec_dir);
	return (1);
}

int		key_press(int keycode, t_rt *s_rt)
{
	int	i;

	if (keycode == KEY_ESC)
		exit(0);
	else if (keycode == KEY_W)
		s_rt->new_origin = (t_vec){0, 0, SPEED_MOVE};
	else if (keycode == KEY_S)
		s_rt->new_origin = (t_vec){0, 0, -SPEED_MOVE};
	else if (keycode == KEY_A)
		s_rt->new_origin = (t_vec){SPEED_MOVE, 0, 0};
	else if (keycode == KEY_D)
		s_rt->new_origin = (t_vec){-SPEED_MOVE, 0, 0};
	else if (keycode == KEY_Z)
		s_rt->new_origin = (t_vec){0, SPEED_MOVE, 0};
	else if (keycode == KEY_X)
		s_rt->new_origin = (t_vec){0, -SPEED_MOVE, 0};
	else if (keycode == KEY_Q)
		s_rt->new_dir = (t_vec){(double)ANGEL_ROTATION * 0.01, 0, 0};
	else if (keycode == KEY_E)
		s_rt->new_dir = (t_vec){(double)-ANGEL_ROTATION * 0.01, 0, 0};
	else if (keycode == KEY_SPACE)
	{
		get_another_camera(s_rt);
		i = -1;
		while (++i < COUNT_THREADS)
			s_rt->s_thread_data[i].s_main_camera = s_rt->s_stage.s_main_camera;
		change_img_window(s_rt);
	}
	return (0);
}

int		key_release(int keycode, t_rt *s_rt)
{
	if (keycode == KEY_W || keycode == KEY_A || keycode == KEY_D ||
		keycode == KEY_S || keycode == KEY_Z || keycode == KEY_X)
		s_rt->new_origin = (t_vec){0,0,0};
	else if (keycode == KEY_Q || keycode == KEY_E)
		s_rt->new_dir = (t_vec){0,0,0};
	return (0);
}

int		new_frame(t_rt *s_rt)
{
	static int	update = 0;
	void		*img_ptr;

	if (!s_rt->render_now && vec_check_unzero(&s_rt->new_origin))
		update = change_pos_camera(s_rt, &s_rt->new_origin);
	if (!s_rt->render_now && vec_check_unzero(&s_rt->new_dir))
		update = change_dir_camera(s_rt, &s_rt->new_dir);
	if (update && !s_rt->render_now)
	{
		img_ptr = s_rt->s_stage.s_main_camera->s_mlx_img.img_ptr;
		if ((s_rt->s_stage.s_main_camera->s_mlx_img.img_ptr = mlx_new_image(s_rt->mlx_p,
													 s_rt->s_stage.s_screen.width,
													 s_rt->s_stage.s_screen.height)) == NULL)
			error_end("Ошибка при mlx_new_image()", MLX_ERROR);
		if ((s_rt->s_stage.s_main_camera->s_mlx_img.addr =
				mlx_get_data_addr(s_rt->s_stage.s_main_camera->s_mlx_img.img_ptr,
								 &(s_rt->s_stage.s_main_camera->s_mlx_img.bits_per_pixel),
								 &(s_rt->s_stage.s_main_camera->s_mlx_img.line_length),
								 &(s_rt->s_stage.s_main_camera->s_mlx_img.endian))) == NULL)
			error_end("Ошибка при mlx_get_data_addr()", MLX_ERROR);
		mlx_clear_window(s_rt->mlx_p, s_rt->mlx_window);
		start_render(s_rt);
		mlx_destroy_image(s_rt->mlx_p, img_ptr);
		update = 0;
	}
	return (0);
}