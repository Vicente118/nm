/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:09 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/05 16:01:16 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	if (!str1 && str2)
		return -1;
	else if (!str2 && str1)
		return 1;
	else if (!str1 && !str2)
		return 0;

	size_t	i;

	i = 1;
	if (n == 0)
		return (0);
	while (*str1 && (ft_tolower(*str1) == ft_tolower(*str2) && i < n))
	{
		str1++;
		str2++;
		i++;
	}
	return (ft_tolower(*str1) - ft_tolower(*str2));
}
/*
int main(void)
{
	const char *str1 = "Bonjour";
	const char *str2 = "Bonsoir";
	printf("%d", ft_strncmp(str1, str2, 3));
	return (0);
}
*/