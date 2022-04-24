template<size_t PageSize = 65536, size_t Alignment = 8 /* sizeof(void*) */>
class PointerBumpAllocator
{
public:
        PointerBumpAllocator() : free(0) { }

        void* AllocBlock(size_t block) {
                // todo: lock(this)
                block = align(block, Alignment);
                if (block > free) {
                        free = align(block, PageSize);
                        head = GetPage(free);
                }
                void* tmp = head;
                head = (char*)head + block;
                free -= block;
                return tmp;
        }

        ~PointerBumpAllocator() {
                for (vector<void*>::iterator i = pages.begin(); i != pages.end(); ++i) {
                        VirtualFree(*i, 0, MEM_RELEASE);
                }
        }

private:
        void* GetPage(size_t size) {
                void* page = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                pages.push_back(page);
                return page;
        }

        vector<void*> pages;
        void* head;
        size_t free;
};
typedef PointerBumpAllocator<> DefaultAllocator;
