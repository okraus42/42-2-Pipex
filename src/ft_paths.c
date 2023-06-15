/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paths.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 16:47:30 by okraus            #+#    #+#             */
/*   Updated: 2023/05/09 17:18:21 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

//get paths
void	ft_fix_first_path(char **s)
{
	int		i;
	int		l;
	char	*str;

	i = 0;
	str = *s;
	l = ft_strlen(str);
	while (str[i + 5])
	{
		str[i] = str[i + 5];
		i++;
	}
	while (i < l)
	{
		str[i] = '\0';
		i++;
	}
}

char	*ft_pathjoin(char *path, char *cmd)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = ft_calloc(ft_strlen(path) + 2 + ft_strlen(cmd), sizeof(char));
	while (path[i])
	{
		str[i] = path[i];
		i++;
	}
	str[i] = '/';
	i++;
	while (cmd[j])
	{
		str[i + j] = cmd[j];
		j++;
	}
	return (str);
}

int	ft_paths(t_pipex_info *info)
{
	int	i;

	i = 0;
	info->paths = NULL;
	while (info->ev[i])
	{
		if (info->ev[i][0] == 'P' && info->ev[i][1] == 'A'
			&& info->ev[i][2] == 'T' && info->ev[i][3] == 'H'
			&& info->ev[i][4] == '=')
		{
			info->paths = ft_split(info->ev[i], ':');
		}
		i++;
	}
	ft_fix_first_path(&info->paths[0]);
	return (0);
}
