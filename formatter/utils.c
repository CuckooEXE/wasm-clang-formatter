#include <stddef.h>
void* mmap(void* addr, size_t length, int prot, int flags, int fd, long long offset) { return NULL; }
int munmap(void* addr, size_t length) { return 0; }
