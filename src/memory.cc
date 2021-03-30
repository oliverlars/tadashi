
internal Arena
make_arena(u64 number_of_bytes){
    Arena arena = {};
    arena.capacity = number_of_bytes;
    arena.data = (u8*)malloc(number_of_bytes);
    arena.used = 0;
    return arena;
}

#define push_type(arena, type) (type*)_push_size(arena, sizeof(type))
#define push_type_zero(arena, type) (type*)_push_size_zero(arena, sizeof(type))
#define push_size(arena, bytes, type) (type*)_push_size(arena, sizeof(type)*bytes)

internal void*
_push_size(Arena* arena, u64 size){
    if(arena->used + size > arena->capacity){
        assert(0);
    }else {
        void* result = arena->data + arena->used;
        arena->used += size;
        return result;
    }
    return nullptr;
}

internal void*
_push_size_zero(Arena* arena, u64 size){
    void* result = _push_size(arena, size);
    memset(result, 0, size);
    return result;
}