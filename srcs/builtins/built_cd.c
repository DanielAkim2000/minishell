/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:20:31 by demane            #+#    #+#             */
/*   Updated: 2026/01/30 13:58:09 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

int	ft_print_err_cd(void)
{
	ft_putstr_fd("minishield: cd: too many arguments\n", STDERR_FILENO);
	return (1);
}

int	ft_print_err_cd_no_directory(char *dir)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("minishield: cd: ", STDERR_FILENO);
		if (dir)
			ft_putstr_fd(dir, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
	else if (ft_is_directory(dir) == FALSE)
	{
		ft_putstr_fd("minishield: cd: ", STDERR_FILENO);
		if (dir)
			ft_putstr_fd(dir, STDERR_FILENO);
		ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("minishield: cd: ", STDERR_FILENO);
		if (dir)
			ft_putstr_fd(dir, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
	return (1);
}

t_bool	ft_assign_path(char **args, t_shell *shell, char **path)
{
	t_vars	*var;

	if (!args[1])
	{
		var = ft_find_vars("HOME", shell);
		if (var && var->value)
		{
			*path = ft_strdup(var->value);
			if (!*path)
				return (FALSE);
		}
		else
			*path = NULL;
	}
	else
	{
		*path = ft_strdup(args[1]);
		if (!(*path))
			return (FALSE);
	}
	return (TRUE);
}

t_bool	ft_check_sign(char **path, t_shell *shell)
{
	t_vars	*var;
	char	*new_path;

	var = NULL;
	new_path = NULL;
	if (ft_strncmp(*path, "-", ft_strlen(*path)) == 0 && ft_strlen(*path) == 1)
	{
		var = ft_find_vars("OLDPWD", shell);
		if (var && var->value)
		{
			new_path = ft_strdup(var->value);
			if (!new_path)
				return (FALSE);
		}
		ft_free((void **)path);
		*path = new_path;
	}
	return (TRUE);
}

void	ft_assign_new_pwd(t_shell *shell, char *last_path)
{
	char	*cwd;
	t_vars	*var;
	char	*tmp;

	var = ft_find_vars("PWD", shell);
	if (var)
	{
		tmp = var->value;
		var->value = getcwd(NULL, 0);
		ft_free((void **)&tmp);
	}
	else
	{
		cwd = getcwd(NULL, 0);
		var = ft_create_new_vars("PWD", TRUE);
		if (!var)
			return (ft_free((void **)&last_path),
				shell->should_exit = 1, (void)0);
		var->value = cwd;
		ft_add_vars(shell, var);
	}
}
