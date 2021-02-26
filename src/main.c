#include "mini_rt.h"

int	main(int argc, char *argv[])
{
	t_rt	s_rt;

	if (argc == 1 || argc > 3)
		error_end("Неверное кол-во аргументов", PARSE_ERROR);
	ft_bzero(&s_rt, sizeof(t_rt));
	parse_file(argv[1], &s_rt.s_stage);
	if (!s_rt.s_stage.s_screen.width && !s_rt.s_stage.s_screen.height)
		error_end("Не задано окно", PARSE_ERROR);
	if (!s_rt.s_stage.s_list_cameras)
		error_end("Не задана ни одна камера", PARSE_ERROR);
	init_render(&s_rt);
	debug_print_s_stage(&s_rt.s_stage);
	start_render(&s_rt);
	return (0);
}