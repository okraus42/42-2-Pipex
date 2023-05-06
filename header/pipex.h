/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:43:08 by okraus            #+#    #+#             */
/*   Updated: 2023/05/06 18:52:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

// INCLUDES

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <sys/wait.h> //check if needed, but it must be
# include <time.h> //check if needed
# include "../libft/header/libft.h"

// DEFINITIONS

// STRUCTURES
// PIPEX structure

typedef struct s_pipex_info
{
	int		ac;
	char	**av;
	char	**ev;
	char	**paths;
	char	**args;
}	t_pipex_info;

typedef struct s_ps_arr
{
	int	i;
	int	j;
	int	min;
	int	tmp;
}		t_ps_arr;

// PROTOTYPES

//void	*ft_prefill_array(int ***array);

#endif
