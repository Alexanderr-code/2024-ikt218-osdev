extern "C"
{
    #include <libc/memory.h>
    #include <libc/screen.h>
    #include <libc/common.h>
    #include "libc/song.h"
}

extern "C" int kernel_main(void);

// Existing global operator new overloads
void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

// Existing global operator delete overloads
void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// Add sized-deallocation functions
void operator delete(void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}
int kernel_main(){

    // Allocate some memory using the kernel memory manager
		// THIS IS PART OF THE ASSIGNMENT
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);
    char* memory4 = new char[1000]();
	
    Song* songs[] = {
        new Song{starwars_theme, sizeof(starwars_theme)/sizeof(Note)}
    };

    SongPlayer* player = create_song_player();

    player->play_song(songs[0]);
	
    while (1);
    
    return 0;
}
