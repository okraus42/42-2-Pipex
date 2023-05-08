/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:43:08 by okraus            #+#    #+#             */
/*   Updated: 2023/05/07 15:11:20 by okraus           ###   ########.fr       */
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
# include <sys/types.h> //open function1
# include <sys/stat.h> //open function2
# include <fcntl.h> //open function3
# include "../libft/header/libft.h"

// DEFINITIONS

// STRUCTURES
// PIPEX structure

typedef struct s_pipex_info
{
	int		ac;
	int		fdi;
	int		fdo;
	int		arg;
	int		*pids;
	int		**pipes;
	char	**av;
	char	**ev;
	char	**paths;
	char	***args;
}	t_pipex_info;

// typedef struct s_pipex_info
// {
// 	int		ac;			//argc
// 	int		fdi;		//fd of infile
// 	int		fdo;		//fd of outfile
//	int		arg;		//number of arguments
//	int		*pids;		//pids of child processes
//	int		**pipes;	//pipe fds
// 	char	**av;		//*argv[]
// 	char	**ev;		//*envp[]
// 	char	**paths;	//array of paths from envp
// 	char	**args;		//array of arguments
// }	t_pipex_info;

// PROTOTYPES

//void	*ft_prefill_array(int ***array);

#endif
