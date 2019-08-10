/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   byte_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skrystin <skrystin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 22:04:27 by nalexand          #+#    #+#             */
/*   Updated: 2019/08/10 18:20:34 by skrystin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

int		adr(int current_adr)
{
	if (current_adr >= MEM_SIZE)
		return (current_adr % MEM_SIZE);
	else if (current_adr < 0)
		return (MEM_SIZE + current_adr % MEM_SIZE);
	return (current_adr);
}

int		get_arg_type(char arg_byte)
{
	char	mask;

	mask = (arg_byte & 0xC0) >> 6;
	if (mask == DIR_CODE)
		return (T_DIR);
	else if (mask == IND_CODE)
		return (T_IND);
	else if (mask == REG_CODE)
		return (T_REG);
	return (0);
}

int		get_arg_code(char arg_type)
{
	if (arg_type == T_DIR)
		return (DIR_CODE);
	else if (arg_type == T_IND)
		return (IND_CODE);
	else if (arg_type == T_REG)
		return (REG_CODE);
	return (0);
}

int		get_arg_ofset(int arg_type, t_op *op)
{
	if (arg_type == T_DIR)
		return (DIR_OFSET);
	else if (arg_type == T_REG)
		return (REG_OFSET);
	else if (arg_type == T_IND)
		return (IND_OFSET);
	return (0);
}

t_op	*get_cmd(char *cmd)
{
	int		i;

	i = 0;
	while (op_tab[i].op_name)
	{
		if (ft_strequ(cmd, op_tab[i].op_name))
			return (&op_tab[i]);
		i++;
	}
	return (NULL);
}

int		get_function_size(char arg_byte, t_op *op)
{
	int		size;
	int		i;
 
	size = 1;
	if (!op->arg_type)
		return (1 + get_arg_ofset(op->args[0], op));
	i = 0;
	while (i < op->arg_count)
	{
		if ((arg_byte & 0xC0) == (REG_CODE << 6))
			size += REG_OFSET;
		else if ((arg_byte & 0xC0) == (DIR_CODE << 6))
			size += DIR_OFSET;
		else if ((arg_byte & 0xC0) == (IND_CODE << 6))
			size += IND_OFSET;
		arg_byte <<= 2;
		i++;
	}
	return (size + 1);
}
