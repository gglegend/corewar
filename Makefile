# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nalexand <nalexand@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/03 20:54:43 by nalexand          #+#    #+#              #
#    Updated: 2019/08/07 22:11:29 by nalexand         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

VPATH = src/corewar:src/asm:src/common:includes

.SECONDARY: $(COR_OBJ) $(ASM_OBJ) $(COM_OBJ)

override ARFLAGS = rcs

#МЫ НЕ ЗАБУДЕМ ДОБАВИТЬ ФЛАГИ -Wall -Werror -Wextra
CFLAGS = -g

HEADER = -I includes -I lib/libftprintf/includes
INCLUDES = corewar.h op.h

CORE = corewar
ASM = asm

COM_LIB = lib/common.a
COR_LIB = lib/corewar.a
ASM_LIB = lib/asm.a
LIBFT = lib/libftprintf.a
LIBFT_PATH = lib/libftprintf/

OBJ_DIR = obj/
LIB_DIR = lib/

COM_SRC =	print_memory.c \
			op.c \
			mem_rev.c \
			byte_functions.c \
			db_print_operation_info.c \
			op_a_live.c \
			op_b_ld.c \
			op_c_st.c \
			op_d_add.c \
			op_e_sub.c \
			op_f_and.c \
			op_g_or.c \
			op_h_xor.c \
			op_i_zjmp.c \
			op_j_ldi.c \
			op_k_sti.c \
			op_l_fork.c \
			op_m_lld.c \
			op_n_lldi.c \
			op_o_lfork.c \
			op_p_aff.c

COR_SRC =	corewar.c \
			cw_clear_exit.c \
			read_input.c \
			init_warriors.c \
			init_carriages.c \
			validate_operation.c \
			cw_debug.c

ASM_SRC =	asm.c \
			first_validation.c \
			command_val.c \
			print_basic.c \

COM_OBJ = $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(COM_SRC)))
COR_OBJ = $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(COR_SRC)))
ASM_OBJ = $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(ASM_SRC)))

all: $(CORE) $(ASM)

$(CORE): $(COM_LIB) $(COR_LIB) $(LIBFT)
	gcc $(CFLAGS) -o $@ $^
$(ASM): $(COM_LIB) $(ASM_LIB) $(LIBFT)
	gcc $(CFLAGS) -o $@ $^

$(COR_LIB):: $(OBJ_DIR) $(LIB_DIR)
$(COR_LIB):: $(COR_LIB)($(COR_OBJ))
$(ASM_LIB):: $(OBJ_DIR) $(LIB_DIR)
$(ASM_LIB):: $(ASM_LIB)($(ASM_OBJ))
$(COM_LIB):: $(OBJ_DIR) $(LIB_DIR)
$(COM_LIB):: $(COM_LIB)($(COM_OBJ))
$(LIBFT): $(LIBFT_PATH)libftprintf.a
	cp $< lib/

$(LIBFT_PATH)libftprintf.a:
	make -C $(LIBFT_PATH)

$(OBJ_DIR):
	mkdir -p $@
$(LIB_DIR):
	mkdir -p $@

$(OBJ_DIR)%.o: %.c $(INCLUDES)
	@gcc $(CFLAGS) -c $< -o $@ $(HEADER)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(COM_LIB)
	@rm -f $(COR_LIB)
	@rm -f $(ASM_LIB)
	@rm -f $(LIBFT)
	@rm -rf *.dSYM

fclean: clean
	@rm -f $(CORE)
	@rm -f $(ASM)

re: fclean all

relib:
	make -C lib/libftprintf re
