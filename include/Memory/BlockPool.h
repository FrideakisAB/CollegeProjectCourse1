template<class T, size_t PageSize = 65536, size_t Alignment = 8 /* sizeof(void*) */>
class BlockPool : PagePool<PageSize>
{
public:
        BlockPool() : head(NULL) {
                BlockSize = align(sizeof(T), Alignment);
                count = PageSize / BlockSize;
        }

        void* AllocBlock() {
                // todo: lock(this)
                if (!head) FormatNewPage();
                void* tmp = head;
                head = *(void**)head;
                return tmp;
        }

        void FreeBlock(void* tmp) {
                // todo: lock(this)
                *(void**)tmp = head;
                head = tmp;
        }
private:
        void* head;
        size_t BlockSize;
        size_t count;

        void FormatNewPage() {
                void* tmp = GetPage();
                head = tmp;
                for(size_t i = 0; i < count-1; i++) {
                        void* next = (char*)tmp + BlockSize;
                        *(void**)tmp = next;
                        tmp = next;
                }
                *(void**)tmp = NULL;
        }
};
