void *my_bestfit_malloc(int size);
void my_free(void *ptr);
void print_list();
struct Node* find_best_fit(int size);
struct Node* new_node(int size);