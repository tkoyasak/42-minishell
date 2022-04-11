/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:22:15 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/10 19:43:30 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>

ssize_t		safe_func(ssize_t res);
void		print_error_msg(char *cmd, char *error_msg);
void		error_handler(char *str);
int			fd_error_handler(char *cmd);

/*  signal  */
void		last_proc_signal(int wstatus);
void		install_signal_handle(void);
void		sigint_handler(int sig);
void		xsigaction(int sig, void (*handler)(int));

/*  delete  */
void		delete_env(void *arg);
void		delete_token(void *arg);
void		delete_expd(void *arg);
void		delete_proc(void *arg);
void		delete_expr(void *arg);
void		delete_node(void *arg);
void		delete_astree(t_node *node);

/*  malloc  */
void		*ft_xmalloc(size_t size);
void		*ft_xcalloc(size_t count, size_t size);
char		*ft_xstrdup(const char *s1);
char		*ft_xstrndup(const char *s1, size_t len);
char		*ft_xsubstr(char const *s, unsigned int start, size_t len);
char		*ft_xstrjoin(char *s1, char *s2);
char		*ft_xstrjoin_free(char *s1, char *s2, bool is_s2_free);
char		**ft_xsplit(char *s, char c);
char		*ft_xitoa(int n);
t_list		*ft_xlstnew(void *content);

void	destructor(void)__attribute__((destructor));
void	detect_leak(int line, char *file); //削除

#endif
