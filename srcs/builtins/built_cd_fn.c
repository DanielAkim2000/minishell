/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd_fn.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 22:44:02 by demane            #+#    #+#             */
/*   Updated: 2026/01/30 13:58:13 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

static void	ft_update_oldpwd_value(t_vars *var, char *last_path, t_shell *shell)
{
	char	*tmp;

	tmp = var->value;
	if (last_path)
	{
		var->value = ft_strdup(last_path);
		if (!var->value)
		{
			ft_free((void **)&last_path);
			shell->should_exit = 1;
			return ;
		}
		ft_free((void **)&tmp);
	}
}

static void	ft_create_oldpwd(t_shell *shell, char *last_path)
{
	t_vars	*var;

	var = ft_create_new_vars("OLDPWD", TRUE);
	if (last_path)
	{
		var->value = ft_strdup(last_path);
		if (!var->value)
		{
			ft_free((void **)&last_path);
			shell->should_exit = 1;
			return ;
		}
	}
	else
		var->value = NULL;
	ft_add_vars(shell, var);
}

void	ft_assign_oldpwd(t_shell *shell, char *last_path)
{
	t_vars	*var;

	var = ft_find_vars("OLDPWD", shell);
	if (var)
		ft_update_oldpwd_value(var, last_path, shell);
	else
		ft_create_oldpwd(shell, last_path);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*l_path;

	l_path = NULL;
	path = NULL;
	if (args[1] && args[2])
		return (ft_print_err_cd());
	if (ft_assign_path(args, shell, &path) == FALSE)
		return (ft_free((void **)&path), shell->should_exit = 1, 1);
	if (!path)
		return (ft_putstr_fd("minishield: cd: << HOME >> not defined", \
STDERR_FILENO), 1);
	if (ft_check_sign(&path, shell) == FALSE)
		return (ft_free((void **)&path), shell->should_exit = 1, 1);
	if (!path)
		return (ft_putstr_fd("minishield: cd: << OLDPWD >> not defined", \
STDERR_FILENO), 1);
	l_path = getcwd(NULL, 0);
	if (!l_path)
		ft_putstr_fd("minishield: chdir: error retrieving current directory: getcwd:\
cannot access parent directories: No such file or directory\n", STDERR_FILENO);
	if (chdir(path) == -1)
		return (ft_free((void **)&path), ft_free((void **)&l_path),
			ft_print_err_cd_no_directory(args[1]));
	return (ft_assign_new_pwd(shell, l_path), ft_assign_oldpwd(shell, l_path),
		ft_free((void **)&path), ft_free((void **)&l_path), 0);
}
