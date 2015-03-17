void *my_bestfit_malloc(int size);
void my_free(void *ptr);
struct Node* new_node(int size);
struct Node* find_best_fit(int size);