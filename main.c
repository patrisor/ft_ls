#include "inc/ft_ls.h"

/* ************************************************************************** */
/* ******************************** AUXILIARY ******************************* */
/* ************************************************************************** */



/* ************************************************************************** */
/* ********************************* ARGS *********************************** */
/* ************************************************************************** */

/* 
 * Allocates memory for node, then copies contents of new_data to 
 * newly allocated memory.
 */
void	push_arg(t_node **head_ref, void *new_data, size_t data_size) 
{ 
	t_node		*new_node;
	int		i;
	
	new_node = (t_node *)malloc(sizeof(t_node));
	new_node->data = malloc(data_size);
	new_node->next = (*head_ref);
	i = -1;
	while (++i < data_size)
		*(char *)(new_node->data + i) = *(char *)(new_data + i);
	(*head_ref) = new_node;
}

/*
 * Frees linked list
 */
void	free_list(t_node *head)
{
	t_node	*tmp;
	
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

/* ************************************************************************** */
/* **************************** MERGE SORT ********************************** */
/* ************************************************************************** */
// Credits: https://quinston.com/code-snippets/merge-sort-with-linked-list-code

/*
 * Method recursively merges two file lonked-lists together.
 */
t_file	*merge(t_file *a, t_file *b) 
{
	t_file	*merged;

	if (a == NULL || b == NULL)
		return((a == NULL) ? b : a);
	merged = NULL;
	if (ft_strcmp(a->name, b->name) <= 0)
	{
		merged = a;
		merged->next = merge(a->next, b);
	}
	else
	{
		merged = b;
		merged->next = merge(a, b->next);
	}
	return(merged);
}

/*
 * Method partitions a Linked-list.
 */
void	partition(t_file *head, t_file **front, t_file **back)
{
	t_file *fast;
	t_file *slow;
	
	if (head == NULL || head->next == NULL)
	{
		*front = head;
		*back = NULL;
	}
	else
	{
		slow = head;
		fast = head->next;
		while (fast)
		{
			fast = fast->next;
			if (fast)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
		*front = head;
		*back = slow->next;
		slow->next = NULL;
	}
}

/*
 * Method sorts the t_file linked-list using Merge sort.
 */
void	sort_dir(t_file **source)
{
	t_file	*head;
	t_file	*a;
	t_file	*b;

	head = *source;
	if (head == NULL || head->next == NULL)
		return ;
	a = NULL;
	b = NULL;
	partition(head, &a, &b);
	sort_dir(&a);
	sort_dir(&b);
	*source = merge(a, b);
}

/* ************************************************************************** */
/* ******************** SORTED LINKED LIST IMPLEMENTATION ******************* */
/* ************************************************************************** */

/*
 * Frees linked list
 */
void	free_dir(t_file *head)
{
	t_file	*tmp;
	
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
	free(head);
}


/* TODO: Customize file Objects Here
 * Method appends object (dirent) to end of head_ref doubly-linked list
 */
void	append_dirent(t_file **head_ref, struct dirent *dirent)
{
	t_file	*new_node;
	t_file	*last;

	if (!(new_node = (t_file *)malloc(sizeof(t_file))))
		return ;
	last = *head_ref;
	// TODO: Convert to Function of form f(struct dirent *dirent) -> t_file *new_node to shorten.
	// PUT in the data
	ft_strcpy(new_node->name, dirent->d_name);
	stat(dirent->d_name, &new_node->stat);
	new_node->d_type = dirent->d_type;
	// Append
	new_node->next = NULL;
	if (*head_ref == NULL)
	{
		new_node->prev = NULL;
		*head_ref = new_node;
		return ;
	}
	while (last->next != NULL)
		last = last->next; 
	last->next = new_node;
	new_node->prev = last;
}


/* ************************************************************************** */
/* ******************************** FT_LS *********************************** */
/* ************************************************************************** */

/* TODO: VIEW
 * Method prints the directory, based on the different arguments parsed.
 */
void	print_dir(t_file *root) 
{
	t_file	*last;

	while (root) 
	{
		if (root->name[0] == '.')
		{
			root = root->next;
			continue;
		}
		ft_printf("%s\n", root->name);
		last = root;
		root = root->next;
	}
}

/*
 * Method similar to Java's File.listFiles():
 * Returns an array of sorted File Objects
 */
t_file	*get_dir(char *dir_name)
{
	t_file		*root;
	struct dirent	*dirent; // Directory entry
	DIR		*dir;

	if((dir = opendir(dir_name)) == NULL)
		return (NULL);
	root = NULL;
	while ((dirent = readdir(dir)) != NULL)
		append_dirent(&root, dirent);
	sort_dir(&root);
        closedir(dir);
	return(root);
}

// TODO: 
void	recur_search(char *dir_name, int *hit)
{
	t_file		*dir;
	t_file		*tmp;
	t_stringbuilder	*dir_path;

	dir = NULL;
	if ((dir = get_dir(dir_name)) == NULL || dir->d_type != 4)
	{
		free_dir(dir);
		return ;
	}
	if (*hit)
		ft_printf("%s", "\n");
	if (ft_strcmp(dir_name, ".") != 0)
		ft_printf("%s:\n", dir_name);
	print_dir(dir);
	*hit = 1;
	tmp = dir;
	while (tmp)
	{
		if (tmp->name[0] == '.')
		{
			tmp = tmp->next;
			continue;
		}
		if (tmp->d_type == 4) 
		{ // Directory Found
			dir_path = ft_sbnew();
			ft_sbwrite(dir_path, dir_name);
			if (ft_strcmp(dir_name, "/") != 0)
				ft_sbwrite(dir_path, "/");
			ft_sbwrite(dir_path, tmp->name);
			tmp->path = ft_sbtostr(dir_path);
			ft_sbdel(&dir_path);
			recur_search(tmp->path, hit);
		}
		tmp = tmp->next;
	}
	free_dir(dir);
}

/*
 * TODO: Parse Arguments
 * Stores file data to fileobject into Binary Tree
 * Prints the sorted file tree
 */
int	exec_with_args(char *dir_name, t_node *ls_args) 
{
	t_file	*dir;
	int	hit;
	
	// TODO: 
	// 	- Print based off the different parameters (parsed as char *)
	// R Flag

	dir = NULL;
	if ((dir = get_dir(dir_name)) == NULL)
		return (-1);
	if (ls_args) 
	{
		while (ls_args) 
		{
			if(*((char *)ls_args->data) == 'R')
			{
				hit = 0;
				recur_search(dir_name, &hit);
			}
			ls_args = ls_args->next;
		}
	} 
	else
	{
		print_dir(dir);
	}
	free_dir(dir);
	return(0);
}

/* 
 * Method stores ls arguments bytes found in flag into ls_args list
 */
int	parse_flag(char *flag, t_node **ls_args)
{
	char	*tmp;

	tmp = flag;
	while (*tmp) 
	{
		if (*tmp == '-') 
		{
			tmp++;
			continue;
		}
		push_arg(ls_args, &(*tmp), sizeof(char));
		tmp++;
	}
	return(0);
}

/*
 * 
 * Parses arguments
 */
int	main(int argc, char **argv)
{
	t_node		*ls_args;
	char		**tmp;
	int		read_args_flag;
	struct stat	s;

	ls_args = NULL;
	if (argc > 1)
	{
		tmp = ++argv;
		read_args_flag = 1;
		while (*tmp)
		{
			if ((*tmp)[0] == '-' && read_args_flag) // Found argument
			{
				parse_flag(*tmp, &ls_args);
			}
			else // Not an argument to parse 
			{
				if (stat(*tmp, &s) == 0) 
				{
					if (S_ISDIR(s.st_mode)) // Found Directory
						exec_with_args(*tmp, ls_args);
					else // Found File
						ft_printf("%s\n", *tmp);
				}
				else
					ft_printf("ft_ls: %s: No such file or directory", *tmp);
				read_args_flag = 0;
			}
			tmp++;
		}
		free_list(ls_args);
	}
	else 
	{ // Run regular ls command on current directory
		exec_with_args(".", ls_args);
	}
	return(0);
}
