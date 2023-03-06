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
}	t_point;

int	main(int argc, char	*argv[])
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		rows;
	int		cols;
	int		aux_rows;
	int		aux_cols;
	int		aux_free;
	int		fd;
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
	t_point	**map;
	char	*res;
	char	**split;
	char	**z_color;

	rows = 1;
	cols = 1;
	aux_rows = 0;
	aux_cols = 0;
	alpha = atan(M_SQRT2);
	beta = 3 * M_PI_4;
	sin_a_mtx = sin(alpha);
	cos_a_mtx = cos(alpha);
	sin_b_mtx = sin(beta);
	cos_b_mtx = cos(beta);
	if (argc != 2)
	{
		ft_putstr_fd("Error: Invalid parameters.\n", 1);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	res = get_next_line(fd);
	if (res[0] == ' ')
				cols--;
	while (res[aux_rows])
	{
		if (res[aux_rows] == ' ')
		{
			while (res[aux_rows] == ' ')
				aux_rows++;
			if (res[aux_rows] != '\n')
				cols++;
		}
		aux_rows++;
	}
	while (res != NULL)
	{
		res = get_next_line(fd);
		if (res != NULL)
			rows++;
	}
	printf("i = %d, j = %d", rows, cols);
	close(fd);
	map = malloc(sizeof(t_point) * (rows + 1));
	map[rows] = NULL;
	aux_rows = rows - 1;
	while (aux_rows >= 0)
	{
		map[aux_rows] = malloc(sizeof(t_point) * (cols + 1));
		map[aux_rows][cols] = (t_point){0};
		aux_rows--;
	}
	aux_rows = 0;
	scale = WIN_HEIGHT / sqrt(pow(rows, 2) + pow(cols, 2));
	printf("\nscale = %d\n", scale);
	fd = open(argv[1], O_RDONLY);
	while (aux_rows < rows)
	{
		res = get_next_line(fd);
		split = ft_split(res, ' ');
		while (aux_cols < cols)
		{
			map[aux_rows][aux_cols].x = aux_rows * scale;
			map[aux_rows][aux_cols].y = aux_cols * scale;
			if (ft_strchr(split[aux_cols], ','))
			{
				z_color = ft_split(split[aux_cols], ',');
				if (aux_cols == cols - 1)
				{
					z_color[1][ft_strlen(z_color[1]) - 1] = '\0';
					if (aux_rows == 0)
						printf("\nCOR ULTIMA LINHA: %s---\n", z_color[1]);
				}
				map[aux_rows][aux_cols].z = ft_atoi(z_color[0]);
				map[aux_rows][aux_cols].color = ft_atohexa(z_color[1]);
				//free(z_color[3]);
				free(z_color[2]);
				free(z_color[1]);
				free(z_color[0]);
				free(z_color);
			}
			else
			{
				map[aux_rows][aux_cols].z = ft_atoi(split[aux_cols]);
				map[aux_rows][aux_cols].color = 0xFFFFFF;
			}
			x = map[aux_rows][aux_cols].x;
			y = map[aux_rows][aux_cols].y;
			z = map[aux_rows][aux_cols].z;
			map[aux_rows][aux_cols].x = cos_b_mtx * x + sin_b_mtx * y;
			map[aux_rows][aux_cols].y = cos_a_mtx * sin_b_mtx * x - cos_a_mtx * cos_b_mtx * y - sin_a_mtx * z;
			aux_cols++;
		}
		aux_free = cols;
		while (aux_free >= 0)
		{
			free(split[aux_free]);
			aux_free--;
		}
		free(split);
		aux_cols = 0;
		aux_rows++;
	}
	printf("COR?: %f %f", map[0][cols - 2].z, map[0][cols - 2].color);
	center[0] = map[0][cols - 1].x - map[rows - 1][0].x - 2 * (map[0][0].x - map[rows - 1][0].x);
	center[1] = map[rows - 1][cols - 1].y - map[0][0].y;
	printf("extremos: %f %f\n", map[0][cols - 1].x, map[0][cols - 1].y);
	printf("extremos: %f %f\n", map[rows - 1][cols - 1].x, map[rows - 1][cols - 1].y);
	printf("extremos: %f %f\n", map[0][0].x, map[0][0].y);
	printf("extremos: %f %f\n", map[rows - 1][0].x, map[rows - 1][0].y);
	printf("diferenças: %f %f\n", center[0], center[1]);
	printf("HEXA: %i\n", 00123);
	printf("DEU CERTO?: %i\n", ft_atohexa("FFFFFF"));
	aux_rows = 0;
	aux_cols = 0;
	while (aux_rows < rows)
	{
		while (aux_cols < cols)
		{
			map[aux_rows][aux_cols].x += (WIN_WIDTH - center[0]) / 2;
			map[aux_rows][aux_cols].y += (WIN_HEIGHT - center[1]) / 2;
			aux_cols++;
		}
		aux_cols = 0;
		aux_rows++;
	}
	//printf("\n%f %f %f %f\n", map[2][2].x, map[2][2].y, map[2][2].z, map[2][2].color);
	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "FdF_lucade-s");
	aux_rows = 0;
	aux_cols = 0;
	while (aux_rows < rows)
	{
		while (aux_cols < cols)
		{
			mlx_pixel_put(mlx_ptr, win_ptr, map[aux_rows][aux_cols].x, map[aux_rows][aux_cols].y, map[aux_rows][aux_cols].color);
			aux_cols++;
		}
		aux_cols = 0;
		aux_rows++;
	}
	aux_free = rows;
	while (aux_free >= 0)
	{
		free(map[aux_free]);
		aux_free--;
	}
	free(map);
	mlx_loop(mlx_ptr);
	return (0);
}
