/*
 * Method inserts a new node on the front of a Doubly-linked list.
 */
void	push_dirent(t_file **head_ref, struct dirent *dirent)
{
	t_file	*new_node;

	if (!(new_node = (t_file *)malloc(sizeof(t_file))))
		return ;
	// TODO: PUT in the data
	ft_strcpy(new_node->name, dirent->d_name);
	// Doubly-linked
	new_node->next = (*head_ref);
	new_node->prev = NULL;
	// change prev of head node to new node
	if ((*head_ref) != NULL)
		(*head_ref)->prev = new_node;
	// move the head to point to the new node
	(*head_ref) = new_node;
}

/*
 * Method inserts a new node after the given node.
 */
void	insert_dirent_after(t_file *prev_node, struct dirent *dirent)
{
	t_file  *new_node;

	if (prev_node == NULL || !(new_node = (t_file *)malloc(sizeof(t_file))))
		return;
	// TODO: PUT in the data
	ft_strcpy(new_node->name, dirent->d_name);
	// Insert after
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	new_node->prev = prev_node;
	if (new_node->next != NULL)
		new_node->next->prev = new_node;
}

/*
 * Method inserts a new node before the given node.
 */
void	insert_dirent_before(t_file **head_ref, t_file *next_node, struct dirent *dirent)
{
	t_file  *new_node;

	if (next_node == NULL || !(new_node = (t_file *)malloc(sizeof(t_file))))
		return;
	// TODO: PUT in the data
	ft_strcpy(new_node->name, dirent->d_name);
	// Insert before
	new_node->prev = next_node->prev;
	next_node->prev = new_node;
	new_node->next = next_node;
	if (new_node->prev != NULL)
		new_node->prev->next = new_node;
	else
		(*head_ref) = new_node;
}
