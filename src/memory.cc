
internal Arena
make_arena(){
    Arena arena = {};
    arena.size = ARENA_MAX;
    arena.base = VirtualAlloc(0, arena.size, MEM_RESERVE, PAGE_NOACCESS);
    assert(arena.base);
    arena.alloc_position = 0;
    arena.commit_position = 0;
    return arena;
}

#define push_type(arena, type) (type*)_push_size(arena, sizeof(type))
#define push_type_zero(arena, type) (type*)_push_size_zero(arena, sizeof(type))
#define push_size(arena, bytes, type) (type*)_push_size(arena, sizeof(type)*bytes)

internal void*
_push_size(Arena* arena, u64 size){
    void *memory = 0;
    if(arena->alloc_position + size > arena->commit_position) {
        u64 commit_size = size;
        commit_size += ARENA_COMMIT_SIZE-1;
        commit_size -= commit_size % ARENA_COMMIT_SIZE;
        VirtualAlloc((u8*)arena->base + arena->commit_position, commit_size, MEM_COMMIT, PAGE_READWRITE);
        assert(arena->base);
        arena->commit_position += commit_size;
    }
    memory = (u8 *)arena->base + arena->alloc_position;
    arena->alloc_position += size;
    
    return memory;
}

internal void*
_push_size_zero(Arena* arena, u64 size){
    void* result = _push_size(arena, size);
    memset(result, 0, size);
    return result;
}

internal Arena
subdivide_arena(Arena* arena, u64 size){
    Arena result = {};
    result.base = push_size(arena, size, u8);
    result.size = size;
    return result;
}


Arena global_arena;