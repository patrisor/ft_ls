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
