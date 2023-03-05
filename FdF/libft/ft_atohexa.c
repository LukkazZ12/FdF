/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atohexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucade-s <lucade-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 21:32:02 by lucade-s          #+#    #+#             */
/*   Updated: 2022/09/22 20:33:29 by lucade-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atohexa(const char *nptr)
{
	int		num;
	int		i;
	int		pow_hexa;

	num = 0;
	i = 2;
	pow_hexa = pow(16, 5);
	while (nptr[i])
	{
		num += (ft_strchr(HEXA, nptr[i]) - &HEXA[0]) * pow_hexa;
		pow_hexa /= 16;
		i++;
	}
	return (num);
}