/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_b_ld.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalexand <nalexand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/07 21:26:36 by nalexand          #+#    #+#             */
/*   Updated: 2019/08/13 21:47:17 by nalexand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

/*
**	2: ld {T_DIR | T_IND}, {T_REG} DSIZE 4
**
**	Задача этой операции состоит в загрузке значения в регистр.
**	Но её поведение отличается в зависимости от типа первого аргумента:
**
**		Аргумент #1 — T_DIR
**		Число переданное в качестве аргумента будет воспринято «как есть».
**		Задачи операции:
**		- Записать полученное число в регистр,
**		который был передан в качестве второго аргумента.
**
**		Аргумент #1 — T_IND
**		Число представляет собой адрес.
**		Задача операции:
**		- Определить адрес — текущая позиция + <ПЕРВЫЙ_АРГУМЕНТ> % IDX_MOD.
**		- С полученного адреса необходимо считать 4 байта.
**		- Записать считанное число в регистр,
**		который был передан в качестве второго параметра.
**
**		Если в регистр записали число 0, то устанавливаем значение carry в 1.
**		Если было записано не нулевое значение, то устанавливаем carry в 0.
*/

static void	print_process(t_core *core, t_list *pc, t_arg *args)
{
	print_process_header(core, pc);
	ft_printf("%d r%d\n", args[0].value, args[1].value);
}

void		cw_ld(void *core, t_list *pc)
{
	t_arg	args[2];
	int		new_pos;

	init_args((t_arg *)args, pc, 2);
	if (args[0].type == T_IND)
		args[0].value = get_value_from_adr(pc, args[0].value, IDX_MOD);
	PC->reg[args[1].value - 1] = args[0].value;
	if (PC->reg[args[1].value - 1] == 0)
		PC->carry = 1;
	else
		PC->carry = 0;
	new_pos = adr(CURRENT + 2 + args[0].size + args[1].size);
	if (((t_core *)core)->out == 4 || ((t_core *)core)->out == 5)
		print_process((t_core *)core, pc, (t_arg *)args);
	else if (((t_core *)core)->out == 16)
		print_mov(pc, new_pos);
	PC->position = new_pos;
}
