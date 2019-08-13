/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cw_debug.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalexand <nalexand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/04 18:45:18 by nalexand          #+#    #+#             */
/*   Updated: 2019/08/13 00:14:59 by nalexand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

static void	print_carriage_on_map(t_core *core, int i)
{
	t_list	*carriage;
	char	*color;

	carriage = core->carriages;
	while (carriage)
	{
		if (i == CRG->position)
		{
			if (CRG->owner->nb == 1)
				color = GRE;
			else if (CRG->owner->nb == 2)
				color = YEL;
			else if (CRG->owner->nb == 3)
				color = RED;
			else if (CRG->owner->nb == 4)
				color = BLU;
			ft_printf("%s%.2hhx%s", color, core->map[i], EOC);
			return ;
		}
		carriage = carriage->next;
	}
	ft_printf("%.2hhx",  core->map[i]);
}

void	print_map(t_core *core, int bar)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	line_size;

	line_size = MEM_SIZE / 64;
	i = 0;
	j = line_size;
	ft_printf("0x0000 : ");
	while (i < MEM_SIZE / bar)
	{
		if (i >= 64 && !(i % 64))
			ft_printf("%#.4x : ", i);
		ft_printf("%.2hhx ",  core->map[i++]);
		//print_carriage_on_map(core, i++);
		if (!--j)
		{
			ft_putchar('\n');
			j = line_size;
		}
	}
}

void	print_input(t_list *tmp)
{
	while (tmp)
	{
		ft_printf("%s", STR(tmp));
		tmp = tmp->next;
	}
}

void	print_warriros(t_core *core)
{
	int		i;

	i = 0;

	while (core->warriors[i])
	{
		ft_printf("number: %d\n", core->warriors[i]->nb);
		ft_printf("name: %s\n", core->warriors[i]->name);
		ft_printf("comment: %s\n", core->warriors[i]->comment);
		ft_printf("start_position: %d\n", core->warriors[i]->start_position);
		ft_printf("code_size: %d\n", core->warriors[i]->code_size);
		print_memory(core->warriors[i]->exec_code, core->warriors[i]->code_size);
		ft_printf("--------------------------------------\n");
		++i;
	}
}

void	print_carriages(t_list *carriage, int count)
{
	int		i;

	ft_printf(" number | position | live_cycle | op_cycle | carry | operation | registers\n");
	while (carriage && count)
	{
		ft_printf("%8d|%10d|%12d|%10d|%7d|%11s| ",
		CRG->nb, CRG->position, CRG->cycle, CRG->cycle_for_op,
		CRG->carry, (CRG->op_info) ? CRG->op_info->op_name : "NULL");
		i = 0;
		while (i < REG_NUMBER)
			ft_printf("%.*x ", REG_SIZE, CRG->reg[i++]);
		ft_putchar('\n');
		carriage = carriage->next;
		--count;
	}
}

int		print_process_header(t_core *core, t_list *carriage)
{
	int		ret;

	ret = 0;
//	ret += ft_printf("%-5d ", core->cycle_after_start);
	ret += ft_printf("P%5d |", CRG->nb);
	ft_printf(" %s ", CRG->op_info->op_name);
	return (ret);
}

void		print_mov(t_list *carriage, int new)
{
	int		i;
	int		size;

	size = get_function_size(BYTE(ARG_BYTE), CRG->op_info);
	ft_printf("ADV %d (%.4p -> %.4p) ", size, CURRENT, new);
	i = 0;
	while (i < size)
		ft_printf("%.2hhx ",  BYTE(CURRENT + i++));
	ft_putchar('\n');
}
