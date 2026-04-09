/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:13:22 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/09 17:13:25 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int is_blank_char(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
        || c == '\v' || c == '\f');
}

void    strip_line_end(char *line)
{
    size_t  len;

    len = ft_strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
    {
        line[len - 1] = '\0';
        len--;
    }
}

char    *trim_spaces(char *str)
{
    size_t  start;
    size_t  end;

    start = 0;
    while (str[start] && is_blank_char(str[start]))
        start++;
    end = ft_strlen(str);
    while (end > start && is_blank_char(str[end - 1]))
        end--;
    return (ft_substr(str, start, end - start));
}

void    free_split(char **parts)
{
    int i;

    if (!parts)
        return ;
    i = 0;
    while (parts[i])
    {
        free(parts[i]);
        i++;
    }
    free(parts);
}

int is_only_digits(char *str)
{
    int i;

    i = 0;
    if (!str || !str[0])
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}
