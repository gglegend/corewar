/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_p_aff.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalexand <nalexand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/07 21:41:59 by nalexand          #+#    #+#             */
/*   Updated: 2019/08/12 22:49:41 by nalexand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

/*
**	16: aff {T_REG} DSIZE 4
**
**	Эта операция берет значение из регистра,
**	который был передан в качестве единственного аргумента.
**	Приводит его к типу char. И выводит на экран как ASCII-символ.
**
**	В оригинальной виртуальной машине corewar по умолчанию
**	режим отображения вывода операции aff выключен.
**	Чтобы его включить нужно воспользоваться флагом -a.
*/

void	cw_aff(void *core, t_list *carriage)
{
	t_arg	arg;
	int		new_pos;

	init_args(&arg, carriage, 1);
//	write(1, &CRG->reg[arg.value - 1], 1);
//	if (((t_core *)core)->out == 4)
//		print_process(carriage, &arg, 1);
	new_pos = adr(CURRENT + 2 + arg.size);
	if (((t_core *)core)->out == 16)
		print_mov(carriage, new_pos);	
	CRG->position = new_pos;
}
