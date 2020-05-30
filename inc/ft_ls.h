#ifndef FT_LS
#define FT_LS

#include "../lib/ft_printf/inc/libftprintf.h"
// LS
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <stdlib.h>
#include <errno.h>

# define NUM_ARGS 5
# define FTLS_NAME_SIZE 1024
# define FTLS_USER_SIZE 32
# define FTLS_GROUP_SIZE 32
# define FTLS_LINK_SIZE 1023
# define FTLS_XATTR_SIZE 1023
# define FTLS_WINSIZE_PAD 20

typedef struct			s_file
{
	/* BST Implementation (Slower)
	struct s_file		*left;
	struct s_file		*right;
	struct s_file		*parent;
	*/

	// Doubly Linked List implementation
	struct s_file		*next;
	struct s_file		*prev;
	char			name[FTLS_NAME_SIZE + 1];
	char			*path;
	int			d_type;
	/* TODO: Uncomment and implement more attributes
	char					user[FTLS_USER_SIZE + 1];
	char					group[FTLS_GROUP_SIZE + 1];
	char					*link;
	char					*xattr;
	ssize_t					xattr_len;
	acl_t					acl; */
	struct stat		stat;
}				t_file;

typedef struct			s_node 
{ 
    // Any data type can be stored in this node 
    void			*data; 
    struct s_node		*next; 
}				t_node; 

#endif
