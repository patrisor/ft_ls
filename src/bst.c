//BINARY TREE IMPLEMENTATION

// TODO: Fill-in data for t_file object
t_file  *new_file(char *filepath, struct dirent *dirent)
{
        t_stringbuilder *full_path;
        t_file          *out;

        if ((out = (t_file *)malloc(sizeof(t_file))) == NULL)
                return(NULL);
        ft_strcpy(out->name, dirent->d_name);
        out->left = NULL;
        out->right = NULL;
        stat(dirent->d_name, out->stat);
        full_path = ft_sbnew();
        ft_sbwrite(full_path, filepath);
        ft_sbwrite(full_path, "/");
        ft_sbwrite(full_path, dirent->d_name);
        out->path = ft_sbtostr(full_path);
        ft_sbdel(&full_path);
        return (out);
}

/*
 * Inserts file sorted by name into BST
 */
t_file  *insert_file(t_file *root, char *filepath, struct dirent *dirent)
{
        if (root == NULL)
                return (new_file(filepath, dirent));
        if (ft_strcmp(dirent->d_name, root->name) < 0)
                root->left = insert_file(root->left, filepath, dirent);
        else if (ft_strcmp(dirent->d_name, root->name) > 0)
                root->right = insert_file(root->right, filepath, dirent);
        return(root);
}

/*
 * A utility function to do inorder traversal of BST
 */
void    print_file_tree(t_file *root)
{
        if (root != NULL)
        {
                print_file_tree(root->left);
                // PRINT HERE
                if (ft_strcmp(root->name, ".") != 0 ||
                        ft_strcmp(root->name, "..") != 0)
                        ft_printf("%s\n", root->name);
                // Recurse
                print_file_tree(root->right);
        }
}

/*
 * A utility function to delete BST
 */
void    delete_file_tree(t_file *root)
{
    if (root == NULL)
            return;
    delete_file_tree(root->left);
    delete_file_tree(root->right);
    free(root);
}

 /*
 * Opens Directory, reads each file from filepath into a BST, then prints inorder
 * Similar to System.IO.Directory.GetFiles in C#
 * Prints all sorted Files in a Directory
 */
void    get_files_FileTree(char *filepath)
{
        t_file          *out;
        struct dirent   *dirent; // Directory entry
        DIR             *dir;
        int             empty;

        if((dir = opendir(filepath)) == NULL)
                return ;
        out = NULL;
        empty = 1;
        while ((dirent = readdir(dir)) != NULL)
        {
                if (empty--)
                        out = insert_file(out, filepath, dirent);
                insert_file(out, filepath, dirent);
        }
        closedir(dir);
        print_file_tree(out);
        delete_file_tree(out); // Frees tree
}


/*
 * Similar to System.IO.Directory.GetDirectories in C#: Reads and Stores Only full
 * path of directories within another directory
 * D_TYPE:
 * 4: Directory
 * 8: File
 */
void    recur_search_FileTree(char *dir_name)
{
        t_stringbuilder *dir_path;
        struct dirent   *dirent; // Directory entry
        DIR             *dir;
        char            *d;

        if((dir = opendir(dir_name)) == NULL)
                return ;
        while ((dirent = readdir(dir)) != NULL)
        {
                if (ft_strcmp(dirent->d_name, ".") == 0 ||
                        ft_strcmp(dirent->d_name, "..") == 0)
                        continue;
                if (dirent->d_type == 4) { // Found directory
                        dir_path = ft_sbnew();
                        ft_sbwrite(dir_path, dir_name);
                        if (ft_strcmp(dir_name, "/") != 0)
                                ft_sbwrite(dir_path, "/");
                        ft_sbwrite(dir_path, dirent->d_name);
                        d = ft_sbtostr(dir_path);
                        ft_sbdel(&dir_path);
                        ft_printf("%s:\n", d);
                        get_files(d);
                        recur_search(d);
                }
        }
        closedir(dir);
}
