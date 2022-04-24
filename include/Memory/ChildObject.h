template<class T, class A = DefaultAllocator>
struct ChildObject
{
        static void* operator new(size_t s, A& allocator) {
                return allocator.AllocBlock(s);
        }
        static void* operator new(size_t s, A* allocator) {
                return allocator->AllocBlock(s);
        }

        static void operator delete(void*, size_t) { } // *1
        static void operator delete(void*, A*) { }
        static void operator delete(void*, A&) { }
private:
        static void* operator new(size_t s);
};
