/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags_mods.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfeltz <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/07 17:28:09 by brfeltz           #+#    #+#             */
/*   Updated: 2019/08/07 22:27:19 by brfeltz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../HEADERS/ft_printf.h"

int		flag_mods(t_ops *ops, char *str)
{
	char	*output;
	t_vals	vals;
	t_vals	wvals;

	ft_bzero(&vals, sizeof(t_vals));
	ft_bzero(&wvals, sizeof(t_vals));
	output = ft_strnew(4864);
	if (ops->conversion != 'c')
	{
		str = percision_flags(ops, str);
		wvals.length = ft_strlen(str);
		if (!ops->hashplaced && (ops->hash || ops->conversion == 'p'))
			ops->width -= (ops->conversion == 'o' ? 1 : 2);
		output = width_flags(output, str, ops, &wvals);
	}
	if (ops->conversion == 'c' && ops->zero != 1)
	{
		wvals.length = ft_strlen(str);
		output = width_flags(output, str, ops, &wvals);
	}
	ops->printed = ft_putstrf(output, ops);
	return (ops->printed);
}

char	*width_flags(char *output, char *str, t_ops *ops, t_vals *wvals)
{
    if (str[0] == '-' && ops->zero)
    {
       output[0] = '-';
       wvals->i++;
       wvals->x++;
    }
    else if (ops->add && str[0] != '-' && ops->zero) {
        output[0] = '+';
        wvals->i++;
        wvals->x++;
    }
    if (ops->hash && conversion_check("xXpo", ops->conversion) && ops->zero)
    {
       ft_strcat(output, ops->conversion == 'X' ? "0X" : "0x");
       wvals->i += 2;
      ops->hashplaced = 1;
    }
	while (!ops->width && str[wvals->x] && output[wvals->i])
		output[wvals->i++] = str[wvals->x++];
	while (!ops->minus && ops->width-- > wvals->length)
		output[wvals->i++] = (ops->zero ? '0' : ' ');
	if ((ops->hash || ops->conversion == 'p') && ops->width && !ops->hashplaced)
    {
		if (ops->conversion != 'o')
			ft_strcat(output, ops->conversion == 'X' ? "0X" : "0x");
		else
			output[wvals->i] = '0';
        wvals->i += (ops->conversion == 'o' ? 1 : 2);
	}
	while (str[wvals->x])
		output[wvals->i++] = str[wvals->x++];
	while (ops->minus && ops->width-- > wvals->length)
		output[wvals->i++] = (ops->zero ? '0' : ' ');
	return (output);
}

int		conversion_check(char *str, char c)
{
	int		i;

	i = -1;
	while (str[++i])
	{
		if (c == str[i])
			return (1);
	}
	return (0);
}

char	*no_perc_width(char *temp, char *str, t_ops *ops)
{
	int		i;
	int		x;

	i = 0;
	x = 0;
	while (ft_isflag(str[i]) && ops->conversion == 's' && ops->percision == 0)
		temp[x++] = str[i++];
	while (str[i] && ops->conversion == 'p' && ops->percision == 0)
		temp[x++] = str[i++];
	return (!str ? 0 : temp);
}

char	*percision_flags(t_ops *ops, char *str)
{
	char	*temp;
	t_vals	vals;

	temp = ft_strnew(ft_strlen(str) + ops->percision);
	temp = no_perc_width(temp, str, ops);
	check_prec_flag(ops, temp, 0);
	if (temp[0] == ft_isflag(temp[0]))
	{
		vals = prec_checker(temp, str, ops);
		while (ops->percision-- > vals.length && ops->conversion != 's')
			temp[vals.x++] = (ops->zero ||
					conversion_check("diouxXb", ops->conversion) ? '0' : ' ');
		if (conversion_check("diouxXb", ops->conversion) == 1)
			ops->percision = ft_strlen(str);
		while (str[vals.i] && ops->percision >= 0)
		{
			temp[vals.x++] = str[vals.i++];
			if (conversion_check("cs", ops->conversion) == 1)
				ops->percision--;
		}
	}
	return (temp);
}
