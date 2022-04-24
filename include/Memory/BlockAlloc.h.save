template<class T>
class BlockAlloc
{
public:
        static void* operator new(size_t s) {
                if (s != sizeof(T)) {
                        return ::operator new(s);
                }
                return pool.AllocBlock();
        }
        static void operator delete(void* m, size_t s) {
                if (s != sizeof(T)) {
                        ::operator delete(m);
                } else if (m != NULL) {
                        pool.FreeBlock(m);
                }
        }

        // todo: implement nothrow_t overloads

        // Avoid hiding placement new that's needed by the stl containers...
        static void* operator new(size_t, void* m) {
                return m;
        }
        // ...and the warning about missing placement delete...
        static void operator delete(void*, void*) {
        }

private:
        static BlockPool<T> pool;
};
template<class T> BlockPool<T> BlockAlloc<T>::pool;
