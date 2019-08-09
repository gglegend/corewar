/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_val.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skrystin <skrystin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/07 17:10:00 by skrystin          #+#    #+#             */
/*   Updated: 2019/08/08 22:05:29 by skrystin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
# define COM ((t_comm *)tmp->content)
# define COM2 &((t_comm *)tmp->content)

int				find_counter(char *str)
{
	int		counter;

	counter = 0;
	while (counter < 16)
	{
		if (ft_strstr(str, op_tab[counter].op_name) == str
		&& (*(str + ft_strlen(op_tab[counter].op_name))) <= 32)
			break;
		counter++;
	}
	return (counter);
}

int				check_command(char *str, char **f, t_as **all, int counter)
{
	int		after;
	char	**args;

	if (counter == 16)
		invalid_comm(all, &f, str);
	str = str + ft_strlen(op_tab[counter].op_name);
	if (ft_strstr(str, "#"))
		*ft_strstr(str, "#") = '\0';
	if ((after = ft_strccount(str, SEPARATOR_CHAR)) + 1 != op_tab[counter].arg_count)
		invalid_comm(all, &f, str);
	while (after--)
		str = str + ft_strindex(str, SEPARATOR_CHAR) + 1;
	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == 'r' || *str == '-' || *str == '%')
		str++;
	while (ft_isdigit(*str))
		str++;
	if (!to_ignore(str, 0) && *str != LABEL_CHAR)
		invalid_comm(all, &f, str);
	return (counter);
}

unsigned char	find_type_arg(t_as **all, char *str, int i, t_op op_tb)
{
	unsigned char	res;
	int				arg;

	arg = 0;
	res = 0;
	while (str && arg < op_tb.arg_count)
	{
		if (*str == 'r' && (op_tb.args[arg] & T_REG) == T_REG)
			res = res | REG_CODE;
		else if (*str == '%' && (op_tb.args[arg] & T_DIR) == T_DIR)
			res = res | DIR_CODE;
		else if ((ft_isdigit(*str) || *str == '-' || *str == ':') && (op_tb.args[arg] & T_IND) == T_IND)
			res = res | IND_CODE;
		else
			invalid_comm(all, 0, str);
		res <<= 2;
		arg++;
		str += ft_strindex(str, SEPARATOR_CHAR) + 1;
		while (*str == ' ' || *str == '\t')
			str++;
	}
	res <<= ((3 - op_tb.arg_count) * 2);
	return (res);
}

void			add_arg(t_comm *com, char *str, t_as **all, t_op op_tab)
{
	int	x;
	int	c;
	char *label;
	char **tmp;

	x = 0;
	while (*str == ' ' || *str == '\t')
		str++;
	(com)->arg_type = find_type_arg(all, str, 0, op_tab);
	tmp = ft_strsplit(str, SEPARATOR_CHAR);
	while (tmp[x])
	{
		c = 0;
		while (tmp[x][c] == ' ' || tmp[x][c] == '\t')
			c++;
		if (tmp[x][c] == 'r' || tmp[x][c] == '%')
			c++;
		if (ft_isint(tmp[x] + c) && x == 0)
			(com)->arg_f = ft_atoi(tmp[x] + c);
		if (ft_isint(tmp[x] + c) && x == 1)
			(com)->arg_s = ft_atoi(tmp[x] + c);
		if (ft_isint(tmp[x] + c) && x == 2)
			(com)->arg_t = ft_atoi(tmp[x] + c);
		if (!ft_isint(tmp[x] + c) && *(tmp[x] + c) != LABEL_CHAR)
			invalid_comm(all, &tmp, str);
		if (!ft_isint(tmp[x] + c) && *(tmp[x] + c) == LABEL_CHAR)
		{
			if (!(label = ft_strnew(ft_strlen(tmp[x] + c))))
				invalid_comm(all, &tmp, str);
			ft_strcpy(label, tmp[x] + c);
			if (x == 0)
				(com)->label_f = label;
			if (x == 1)
				(com)->label_s = label;
			if (x == 2)
				(com)->label_t = label;
		}
		x++;
	}
	ft_arraydel((void ***)&tmp);
}

int				check_arg(char *str, t_op op_tab, t_as **all)
{
	char	**tmp;
	int		i;
	int		x;

	if (ft_strccount(str, SEPARATOR_CHAR) != op_tab.arg_count - 1)
		return (0);
	tmp = ft_strsplit(str, SEPARATOR_CHAR);
	i = 0;
	while (tmp[i])
	{
		x = 0;
		while (tmp[i][x] == ' ' || tmp[i][x] == '\t')
			x++;
		if (tmp[i][x] == '%' || (tmp[i][x] == 'r' && tmp[i][x + 1] != '-'))
			x++;
		if (!ft_isint(tmp[i++] + x) && *(tmp[i - 1] + x) != LABEL_CHAR)
			invalid_comm(all, &tmp, str);
	}
	if (tmp[op_tab.arg_count - 1])
		str = ft_strstr(str, tmp[op_tab.arg_count - 1]) + ft_strlen(tmp[op_tab.arg_count - 1]);
	else
		invalid_comm(all, 0, str);
//	ft_printf("this - %s", str);
	ft_arraydel((void ***)&tmp);
	if (!to_ignore(str, 0))
		return (0);
//	ft_printf("this - %s", str);
	return (1);
}

void			label_to_com(t_as **all, t_list *comm, t_list *tmp)
{
	tmp = (*all)->labels;
	while (tmp && (*all)->labels->next && ((t_label *)(*all)->labels->content)->link)
		(*all)->labels = (*all)->labels->next;
	// if ((*all)->labels)
	// 	ft_printf("HI - %s", ((t_label *)(*all)->labels->content)->link);
	while ((*all)->labels && ((t_label *)(*all)->labels->content)->link == 0)
	{
		// ft_printf("HI");
		((t_label *)(*all)->labels->content)->link = comm;
		(*all)->labels = (*all)->labels->next;
	}
	(*all)->labels = tmp;
}

void			add_command(t_as **all, t_list *tmp, char **f, char *str)
{
	int		counter;

	while (*str == ' ' || *str == '\t')
		str++;
	counter = check_command(str, f, all, find_counter(str));
	// ft_printf("arg - %s", str);
	if (!(tmp = (t_list *)malloc(sizeof(t_list))))
		invalid_comm(all, &f, str);// do more
	if (!(tmp->content = (t_comm *)malloc(sizeof(t_comm))))
	{
		free(tmp);
		invalid_comm(all, &f, str);//do more
	}
	// ft_printf("arg - %s\n", str);
	if (counter == 16 || *str == '\0' ||
	!check_arg(str + ft_strlen(op_tab[counter].op_name) + 1, op_tab[counter], all))
		invalid_comm(all, 0, str);// do more
	// ft_printf("arg - %s", str);
	ft_bzero(COM, sizeof(t_comm));
	COM->instr = op_tab[counter].op_name;
	//ft_printf("arg - %s\n", str);
	add_arg((COM), str + ft_strlen(op_tab[counter].op_name) + 1, all, op_tab[counter]);
	//ft_printf("arg_f - %d\n", (COM)->arg_f);
	COM->len = get_arg_size(&(op_tab[counter]), COM->arg_type);
//	ft_printf("byte - %d", COM->len);
	// ft_printf("arg - %s", str);
	tmp->next = 0;
	ft_lstpushback(&(*all)->comm, tmp);
	// ft_printf("arg_f - %d\n", ((t_comm *)((*all)->comm)->content)->arg_f);
	label_to_com(all, tmp, 0);
}

int				add_label(t_as **all, char **f, char *str, int y)
{
	char	*new;
	t_list	*tmp;
	int		i;
	t_label	*lab;

	i = 0;
	while (*str == ' ' || *str == '\t')
		str++;
	while (ft_strindex(LABEL_CHARS, str[i]) != -1)
		i++;
	if (str[i] != LABEL_CHAR)
		return (1);
	if (!(new = ft_strnew(i + 1)))
		invalid_comm(all, &f, str);//do more
	if (!(lab = (t_label *)malloc(sizeof(t_label))))
	{
		free(new);
		invalid_comm(all, 0, str);
	}//do more
	ft_strncpy(new, str, i + 1);
	lab->name = new;
	lab->link = 0;
	if (!(tmp = ft_lstnew(lab, sizeof(t_label))))
	{
		free(new);
		free(lab);
		invalid_comm(all, 0, str);
	}//do more
	free(lab);
	ft_lstpushback(&((*all)->labels), tmp);
	// ft_printf("\nlabel - %s\n", ((t_label *)tmp->content)->name);
	while (i >= 0)
		f[y][i--] = ' ';
	return (1);
}

int				len_to_label(t_list *dst, t_list *label, char *find, t_as **all)
{
	t_list	*src;
	int		flag;
	int		res;
	t_list	*begin_c;

	begin_c = (*all)->comm;
	flag = 0;
	res = 0;
	while (label && (ft_strncmp(((t_label *)label->content)->name, find + 1, ft_strlen(find + 1))
	|| ft_strlen(find) != ft_strlen(((t_label *)label->content)->name)))
		label = label->next;
	if (!label)
		invalid_comm(all, 0, find);
	src = ((t_label *)label->content)->link;
	//if (dst->content)
	//ft_printf("\nSRC that - %s, DST that - %s\n", ((t_comm *)src->content)->instr, ((t_comm *)dst->content)->instr);
	while (begin_c)
	{
		if (begin_c == src && flag == 0)
			flag = -1;
		else if (begin_c->content == dst->content && flag == 0)
		{
		//	ft_printf("HERE I GO AGAIN\n");
			flag = 1;
		}
		else if ((begin_c == src || begin_c->content == dst->content) && flag != 0)
		{
			// ft_printf("IS IT WORK???????\n");
			flag = 0;
		}
		res += flag * (((t_comm *)begin_c->content)->len);
		begin_c = begin_c->next;
	}
//	ft_printf("I find this - %d to that name - %s\n", res, find);
	return (res);
}

void			label_to_nbr(t_as **all, t_list *label, t_list *begin_c, t_list	*tmp)
{
	tmp = (*all)->comm;
	while (tmp)
	{
		// if (*COM->instr == 'f')
		// 	ft_printf("FIND THIS MASAFUKER %d\n", COM->arg_f);
		if (COM->label_f || COM->label_s || COM->label_t)
		{
			if (COM->label_f)
			{
				COM->arg_f = len_to_label(tmp, label, COM->label_f, all);
				free(COM->label_f);
				COM->label_f = NULL;
			}
			if (COM->label_s)
			{
				COM->arg_s = len_to_label(tmp, label, COM->label_s, all);
				free(COM->label_s);
				COM->label_s = NULL;
			}
			if (COM->label_t)
			{
				COM->arg_t = len_to_label(tmp, label, COM->label_t, all);
				free(COM->label_t);
				COM->label_t = NULL;
			}
				//ft_printf("HI");
		}
		else
			tmp = tmp->next;
	}
	(*all)->comm = begin_c;
}
