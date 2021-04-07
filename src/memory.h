#define ARENA_MAX          Gigabytes(4)
#define ARENA_COMMIT_SIZE  Kilobytes(4)

struct Arena {
    void* base;
    u64 size;
    u64 alloc_position;
    u64 commit_position;
    
};