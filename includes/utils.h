#ifndef UTILS_H
#define UTILS_H

# include <stdlib.h>

void		delete_env(void *arg);
void		delete_token(void *arg);
void		delete_expansion(void *arg);
void		delete_process(void *arg);
void		delete_expression(void *arg);
void		delete_node(void *arg);
void		delete_astree(t_node *node);

void		sigint_handler(int sig);

ssize_t		safe_func(ssize_t res);

#endif
