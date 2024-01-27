extern int external_memory
__attribute__((space(external(size(1024)))));

__external__ int external_array[256]
__attribute__((space(external(external_memory))));

int main(void)
{}

