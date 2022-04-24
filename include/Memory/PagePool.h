inline size_t align(size_t x, size_t a) { return ((x-1) | (a-1)) + 1; }
//#define align(x, a) ((((x)-1) | ((a)-1)) + 1)

template<size_t PageSize = 65536>
class PagePool
{
public:
        void* GetPage() {
                void* page = VirtualAlloc(NULL, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                pages.push_back(page);
                return page;
        }

        ~PagePool() {
                for (vector<void*>::iterator i = pages.begin(); i != pages.end(); ++i) {
                        VirtualFree(*i, 0, MEM_RELEASE);
                }
        }
private:
        vector<void*> pages;
};
