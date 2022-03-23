#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>

void		delete_env(void *arg);
void		delete_token(void *arg);
void		delete_expansion(void *arg);
void		delete_process(void *arg);
void		delete_expr(void *arg);
void		delete_node(void *arg);
void		delete_astree(t_node *node);

void		sigint_handler(int sig);
ssize_t		safe_func(ssize_t res);
void		error_handler(char *str);
void		*ft_xmalloc(size_t size);
void		*ft_xcalloc(size_t count, size_t size);
char		*ft_xstrdup(const char *s1);
char		*ft_xstrndup(const char *s1, size_t len);
char		*ft_xsubstr(char const *s, unsigned int start, size_t len);
char		*ft_xstrjoin(char *s1, char *s2);
char		**ft_xsplit(char *s, char c);
char		*ft_xitoa(int n);
t_list		*ft_xlstnew(void *content);

#endif
