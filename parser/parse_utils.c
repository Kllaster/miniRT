#include "mini_rt.h"

void	check_next_arg(char **str)
{
	if (**str == ',')
		++(*str);
	else
	{
		skip_between_param(str, ',');
		if (**str == ',')
			++(*str);
		else
		{
			printf("\n ---> \"%s\"", *str);
			error_end("Error checking the next argument", PARSE_ERROR, 0, NULL);
		}
	}
}

void	skip_between_param(char **str, char symb)
{
	++(*str);
	while (**str)
	{
		if (*(*str) == '\t' || *(*str) == ' ')
			++(*str);
		else if ((*(*str) >= '0' && *(*str) <= '9') ||
			*(*str) == '-' || *(*str) == '+' || *(*str) == symb)
			return ;
		else
		{
			printf("\n ---> \"%s\"", *str);
			error_end("Error checking the next parameter",
						PARSE_ERROR, 0, NULL);
		}
	}
}

float	parse_float(char **str)
{
	float	sign;
	float	num;

	sign = 1;
	if (**str == '-')
		sign = -1;
	num = (float)ft_atoi_pos(str);
	if (num < 0)
		num *= sign = -1;
	if (*(*str) == '.' && (++(*str)))
		num += ft_atoi_pos_mantissa(str);
	num *= sign;
	return (num);
}

t_rgb	parse_rgb(char **str)
{
	t_rgb	s_rgb;

	s_rgb.red = ft_atoi_pos(str);
	if (s_rgb.red < 0 || s_rgb.red > 255)
		error_end("Error checking red color", PARSE_ERROR, 0, NULL);
	check_next_arg(str);
	s_rgb.green = ft_atoi_pos(str);
	if (s_rgb.green < 0 || s_rgb.green > 255)
		error_end("Error checking green color", PARSE_ERROR, 0, NULL);
	check_next_arg(str);
	s_rgb.blue = ft_atoi_pos(str);
	if (s_rgb.blue < 0 || s_rgb.blue > 255)
		error_end("Error checking blue color", PARSE_ERROR, 0, NULL);
	return (s_rgb);
}

t_vec	parse_coordinates(char **str)
{
	t_vec	s_vec;

	s_vec.x = parse_float(str);
	check_next_arg(str);
	s_vec.y = parse_float(str);
	check_next_arg(str);
	s_vec.z = parse_float(str);
	return (s_vec);
}
