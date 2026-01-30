NAME        = minishell
CC          = @cc
CFLAGS      = -Wall -Wextra -Werror -I./includes

DIR_SRCS    = srcs
DIR_OBJS    = objs
DIR_INCLUDES= includes

DIR_LEXER   = lexer
DIR_PARSER  = parser
DIR_EXPANDER= expander
DIR_BUILTINS= builtins
DIR_ENV     = env
DIR_EXECUTOR= executor
DIR_UTILS   = utils
DIR_SIGNALS = signals

## FILES

FILES_LEXER = lexer_free_utils.c lexer_operators.c lexer_print_err.c \
	      lexer_reader.c lexer_segs.c lexer_tokens.c lexer_utils.c \
	      lexer.c

FILES_PARSER = parser_ast_builder.c parser.c parser_print_err.c \
	       parser_utils2.c parser_builder.c parser_free.c \
	       parser_segs.c parser_utils.c parser_builder2.c

FILES_EXPANDER = expander.c exec_cmd_expand.c exec_cmd_expand2.c \
		   exec_cmd_rebuild.c exec_cmd_rebuild2.c exec_heredoc_expand.c \
		   exec_heredoc_expand2.c expander_utils.c expander_utils2.c \
		   expander_redirs.c exec_globbing_checker.c expander_error.c \
		   exec_globbing_redirs.c exec_globbing_any.c expander_utils3.c \
		   expander_utils4.c exec_globbing_redirs2.c expander_utils5.c \
		   exec_cmd_rebuild_noifs.c exec_cmd_rebuild_noifs2.c expander_redirs_heredoc.c

FILES_BUILTINS = built_cd_fn.c built_cd.c built_echo.c built_env.c built_exit.c \
		 built_export.c built_pwd.c built_unset.c built_utils.c

FILES_EXECUTOR = exec_and.c exec_append.c exec_ast.c exec_cmd.c \
		 exec_or.c exec_pipe.c exec_redirect_in.c exec_redirect_out.c \
		 exec_subshell.c exec_get_path.c exec_throw_error.c exec_built.c \
		 exec_here_doc.c

FILES_ENV = env_cpy.c env_free.c env_make.c env_envp.c env_vars.c

FILES_UTILS = utils.c

FILES_SIGNALS = signals.c signals_handle.c signals_ignore.c

## SRCS

SRCS_LEXER     = $(addprefix $(DIR_SRCS)/$(DIR_LEXER)/, $(FILES_LEXER))
SRCS_PARSER   = $(addprefix $(DIR_SRCS)/$(DIR_PARSER)/, $(FILES_PARSER))
SRCS_EXPANDER = $(addprefix $(DIR_SRCS)/$(DIR_EXPANDER)/, $(FILES_EXPANDER))
SRCS_BUILTINS = $(addprefix $(DIR_SRCS)/$(DIR_BUILTINS)/, $(FILES_BUILTINS))
SRCS_EXECUTOR = $(addprefix $(DIR_SRCS)/$(DIR_EXECUTOR)/, $(FILES_EXECUTOR))
SRCS_ENV      = $(addprefix $(DIR_SRCS)/$(DIR_ENV)/, $(FILES_ENV))
SRCS_UTILS   = $(addprefix $(DIR_SRCS)/$(DIR_UTILS)/, $(FILES_UTILS))
SRCS_SIGNALS   = $(addprefix $(DIR_SRCS)/$(DIR_SIGNALS)/, $(FILES_SIGNALS))

SRCS =  $(SRCS_LEXER) $(SRCS_PARSER) $(SRCS_EXPANDER) \
	$(SRCS_BUILTINS) $(SRCS_EXECUTOR) $(SRCS_ENV)\
	$(SRCS_UTILS) $(SRCS_SIGNALS) $(DIR_SRCS)/process_minishell2.c \
	$(DIR_SRCS)/process_minishell.c $(DIR_SRCS)/main.c

## OBJS

OBJS = $(SRCS:$(DIR_SRCS)/%.c=$(DIR_OBJS)/%.o)

## LIBFT

LIBFT = $(DIR_INCLUDES)/libft/libft.a

## STYLE


RESET   = \033[0m
GREEN   = \033[32m
CYAN    = \033[36m
BOLD  = \033[1m


BAR_WIDTH = 30
FILE_WIDTH = 40
TOTAL     = $(words $(SRCS))
COUNT     = 0

define progress_bar
	@$(eval COUNT=$(shell echo $$(( $(COUNT) + 1 ))))
	@PERCENT=$$(($(COUNT) * 100 / $(TOTAL))); \
	FILLED=$$(($(COUNT) * $(BAR_WIDTH) / $(TOTAL))); \
	EMPTY=$$(($(BAR_WIDTH) - $$FILLED)); \
	FILE="$<"; \
	SHORT_FILE=$$(printf "%-*.*s" $(FILE_WIDTH) $(FILE_WIDTH) "$$FILE"); \
	printf "\r\033[K$(BOLD)$(CYAN)[BUILD] [MINISHELL]$(RESET) %s $(CYAN)[" "$$SHORT_FILE"; \
	printf "%0.s=" $$(seq 1 $$FILLED); \
	printf "%0.s " $$(seq 1 $$EMPTY); \
	printf "] %3d%%$(RESET)" $$PERCENT
endef




## RULES

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline -lhistory -lncurses
	@printf "\n$(GREEN)✔ Build MINISHELL terminé$(RESET)\n"

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	$(progress_bar)

$(LIBFT):
	@make -C $(DIR_INCLUDES)/libft

clean:
	@rm -rf $(DIR_OBJS)
	@make -C $(DIR_INCLUDES)/libft clean

fclean: clean
	@rm -rf $(NAME)
	@make -C $(DIR_INCLUDES)/libft fclean

re: fclean all

.PHONY: all clean fclean re

## CMDS VALGRIND

# valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
