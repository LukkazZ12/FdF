/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucade-s <lucade-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 11:46:36 by lucade-s          #+#    #+#             */
/*   Updated: 2023/03/01 11:46:36 by lucade-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <mlx.h>
#include <stdio.h>
#include <math.h>
#include "libft/libft.h"

#define WIN_HEIGHT 900
#define WIN_WIDTH 1800

typedef struct s_point {
	double	x;
	double	y;
	double	z;
	double	color;
}			t_point;

typedef struct s_data {
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*addr_ptr;
	int		bits_per_pixel;
	int		endian;
	int		line_length;
	char	*map_name;
	t_point	**map;
}			t_data;

typedef struct s_line {
	int	aux_x1;
	int	aux_y1;
	int	aux_x2;
	int	aux_y2;
	int	dx;
	int	dy;
	int	e;
	int	step;
	int	temp;
}		t_line;

typedef struct s_transformation {
	int		scale;
	double	alpha;
	double	beta;
	double	sin_a_mtx;
	double	cos_a_mtx;
	double	sin_b_mtx;
	double	cos_b_mtx;
	double	x;
	double	y;
	double	z;
	double	center[2];
	char	*res;
	char	**split;
	char	**z_color;
}			t_transformation;

static void	mlx_put_pixel_img(char *addr_ptr, int line_length, int bits_per_pixel, int x, int y, double color)
{
	char	*dst;

	if (x > WIN_WIDTH || x < 0 || y > WIN_HEIGHT || y < 0)
		return ;
	dst = addr_ptr + (y * line_length + x * bits_per_pixel / 8);
	*(unsigned int *)dst = color;
}

int	main(int argc, char	*argv[])
{
	t_data	data;
	t_line	draw_line;
	int		rows;
	int		cols;
	int		aux_rows;
	int		aux_cols;
	int		aux_free;
	int		fd;

	if (argc != 2)
	{
		ft_putstr_fd("Error: Invalid parameters.\n", 1);
		return (1);
	}
	data.mlx_ptr = mlx_init();
	data.win_ptr = mlx_new_window(data.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "FdF_lucade-s");
	data.img_ptr = mlx_new_image(data.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	data.addr_ptr = mlx_get_data_addr(data.img_ptr, &data.bits_per_pixel, &data.line_length, &data.endian);
	data.map_name = argv[1];
	rows = 1;
	cols = 1;
	aux_rows = 0;
	aux_cols = 0;
	fd = open(data.map_name, O_RDONLY);
	res = get_next_line(fd);
	if (res[0] == ' ')
		cols--;
	while (res[aux_cols])
	{
		if (res[aux_cols] == ' ')
		{
			while (res[aux_cols] == ' ')
				aux_cols++;
			if (res[aux_cols] != '\n')
				cols++;
		}
		aux_cols++;
	}
	while (res)
	{
		res = get_next_line(fd);
		if (res)
			rows++;
		free(res);
	}
	close(fd);
	data.map = malloc(sizeof(t_point) * (rows + 1));
	data.map[rows] = NULL;
	aux_rows = rows - 1;
	while (aux_rows >= 0)
	{
		data.map[aux_rows] = malloc(sizeof(t_point) * (cols + 1));
		data.map[aux_rows][cols] = (t_point){0};
		aux_rows--;
	}
	aux_rows = 0;
	aux_cols = 0;
	scale = WIN_HEIGHT / hypot(rows, cols);
	fd = open(data.map_name, O_RDONLY);
	alpha = atan(M_SQRT2);
	beta = M_PI_4;
	sin_a_mtx = sin(alpha);
	cos_a_mtx = cos(alpha);
	sin_b_mtx = sin(beta);
	cos_b_mtx = cos(beta);
	while (aux_rows < rows)
	{
		res = get_next_line(fd);
		split = ft_split(res, ' ');
		while (aux_cols < cols)
		{
			data.map[aux_rows][aux_cols].y = aux_rows * scale;
			data.map[aux_rows][aux_cols].x = aux_cols * scale;
			if (ft_strchr(split[aux_cols], ','))
			{
				z_color = ft_split(split[aux_cols], ',');
				if (aux_cols == cols - 1)
				{
					z_color[1][ft_strlen(z_color[1]) - 1] = '\0';
				}
				data.map[aux_rows][aux_cols].z = ft_atoi(z_color[0]);
				data.map[aux_rows][aux_cols].color = ft_atohexa(z_color[1]);
				free(z_color[3]);
				free(z_color[2]);
				free(z_color[1]);
				free(z_color[0]);
				free(z_color);
			}
			else
			{
				data.map[aux_rows][aux_cols].z = ft_atoi(split[aux_cols]);
				data.map[aux_rows][aux_cols].color = 0xFFFFFF;
			}
			x = data.map[aux_rows][aux_cols].x;
			y = data.map[aux_rows][aux_cols].y;
			z = data.map[aux_rows][aux_cols].z;
			data.map[aux_rows][aux_cols].x = cos_b_mtx * x - sin_b_mtx * y;
			data.map[aux_rows][aux_cols].y = cos_a_mtx * sin_b_mtx * x + cos_a_mtx * cos_b_mtx * y - sin_a_mtx * z;
			aux_cols++;
		}
		aux_free = cols;
		while (aux_free >= 0)
		{
			free(split[aux_free]);
			aux_free--;
		}
		free(split);
		free(res);
		aux_cols = 0;
		aux_rows++;
	}
	center[0] = data.map[0][cols - 1].x - data.map[rows - 1][0].x - 2 * (data.map[0][0].x - data.map[rows - 1][0].x);
	center[1] = data.map[rows - 1][cols - 1].y - data.map[0][0].y;
	aux_rows = 0;
	aux_cols = 0;
	while (aux_rows < rows)
	{
		while (aux_cols < cols)
		{
			data.map[aux_rows][aux_cols].x += (WIN_WIDTH - center[0]) / 2;
			data.map[aux_rows][aux_cols].y += (WIN_HEIGHT - center[1]) / 2;
			aux_cols++;
		}
		aux_cols = 0;
		aux_rows++;
	}
	aux_rows = 0;
	aux_cols = 0;
	while (aux_rows < rows)
	{
		while (aux_cols < cols)
		{
			if (aux_cols < cols -1)
			{
				draw_line.aux_x1 = data.map[aux_rows][aux_cols].x;
				draw_line.aux_y1 = data.map[aux_rows][aux_cols].y;
				draw_line.aux_x2 = data.map[aux_rows][aux_cols + 1].x;
				draw_line.aux_y2 = data.map[aux_rows][aux_cols + 1].y;
				if (draw_line.aux_x1 > draw_line.aux_x2)
				{
					draw_line.temp = draw_line.aux_x1;
					draw_line.aux_x1 = draw_line.aux_x2;
					draw_line.aux_x2 = draw_line.temp;
					draw_line.temp = draw_line.aux_y1;
					draw_line.aux_y1 = draw_line.aux_y2;
					draw_line.aux_y2 = draw_line.temp;
				}
				draw_line.dx = draw_line.aux_x2 - draw_line.aux_x1;
				draw_line.dy = draw_line.aux_y2 - draw_line.aux_y1;
				if (abs(draw_line.dy) > abs(draw_line.dx))
				{	
					draw_line.step = 1;
					draw_line.e = 0;
					if (draw_line.dy < 0)
					{
						draw_line.step = -1;
						draw_line.dy *= -1;
					}
					while (draw_line.aux_y1 != draw_line.aux_y2)
					{
						mlx_put_pixel_img(data.addr_ptr, data.line_length, data.bits_per_pixel, draw_line.aux_x1, draw_line.aux_y1, data.map[aux_rows][aux_cols].color);
						draw_line.aux_y1 += draw_line.step;
						draw_line.e += 2 * draw_line.dx;	
						if (draw_line.e >= 2 * draw_line.dy)
						{
							draw_line.aux_x1++;
							draw_line.e -= 2 * draw_line.dy;
						}
					}
				}
				else
				{
					draw_line.step = 1;
					draw_line.e = 0;
					if (draw_line.dy < 0)
					{
						draw_line.step = -1;
						draw_line.dy *= -1;
					}
					while (draw_line.aux_x1 <= draw_line.aux_x2)
					{
						mlx_put_pixel_img(data.addr_ptr, data.line_length, data.bits_per_pixel, draw_line.aux_x1++, draw_line.aux_y1, data.map[aux_rows][aux_cols].color);
						draw_line.e += 2 * draw_line.dy;
						if (draw_line.e >= 2 * draw_line.dx)
						{
							draw_line.aux_y1 += draw_line.step;
							draw_line.e -= 2 * draw_line.dx;
						}
					}
				}
			}
			if (aux_rows < rows -1)
			{
				draw_line.aux_x1 = data.map[aux_rows][aux_cols].x;
				draw_line.aux_y1 = data.map[aux_rows][aux_cols].y;
				draw_line.aux_x2 = data.map[aux_rows + 1][aux_cols].x;
				draw_line.aux_y2 = data.map[aux_rows + 1][aux_cols].y;
				if (draw_line.aux_x1 > draw_line.aux_x2)
				{
					draw_line.temp = draw_line.aux_x1;
					draw_line.aux_x1 = draw_line.aux_x2;
					draw_line.aux_x2 = draw_line.temp;
					draw_line.temp = draw_line.aux_y1;
					draw_line.aux_y1 = draw_line.aux_y2;
					draw_line.aux_y2 = draw_line.temp;
				}
				draw_line.dx = draw_line.aux_x2 - draw_line.aux_x1;
				draw_line.dy = draw_line.aux_y2 - draw_line.aux_y1;
				if (abs(draw_line.dy) > abs(draw_line.dx))
				{	
					draw_line.step = 1;
					draw_line.e = 0;
					if (draw_line.dy < 0)
					{
						draw_line.step = -1;
						draw_line.dy *= -1;
					}
					while (draw_line.aux_y1 != draw_line.aux_y2)
					{
						mlx_put_pixel_img(data.addr_ptr, data.line_length, data.bits_per_pixel, draw_line.aux_x1, draw_line.aux_y1, data.map[aux_rows][aux_cols].color);
						draw_line.aux_y1 += draw_line.step;
						draw_line.e += 2 * draw_line.dx;	
						if (draw_line.e >= 2 * draw_line.dy)
						{
							draw_line.aux_x1++;
							draw_line.e -= 2 * draw_line.dy;
						}
					}
				}
				else
				{
					draw_line.step = 1;
					draw_line.e = 0;
					if (draw_line.dy < 0)
					{
						draw_line.step = -1;
						draw_line.dy *= -1;
					}
					while (draw_line.aux_x1 <= draw_line.aux_x2)
					{
						mlx_put_pixel_img(data.addr_ptr, data.line_length, data.bits_per_pixel, draw_line.aux_x1++, draw_line.aux_y1, data.map[aux_rows][aux_cols].color);
						draw_line.e += 2 * draw_line.dy;
						if (draw_line.e >= 2 * draw_line.dx)
						{
							draw_line.aux_y1 += draw_line.step;
							draw_line.e -= 2 * draw_line.dx;
						}
					}
				}
			}
			aux_cols++;
		}
		aux_cols = 0;
		aux_rows++;
	}
	mlx_put_image_to_window(data.mlx_ptr, data.win_ptr, data.img_ptr, 0, 0);
	aux_free = rows;
	while (aux_free >= 0)
	{
		free(data.map[aux_free]);
		aux_free--;
	}
	free(data.map);
	mlx_loop(data.mlx_ptr);
	return (0);
}
