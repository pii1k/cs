## Runtime Cost
Templates arnd references are resolved entirely at compile time.
Which means:
- The compiler generates specialized versions of the function for each array size.
- There is no zero runtime overhead from using a template.
- Passing an array by reference does not involve copying: it's just passing the address, just like pointers.

2. Code size / Binary size
One real cost you may care about is code bloat.
- If you call the template with many different array size, the compiler will generate a seperate function for each version
- This increase code size(not ruuntime cost), similar to what happends with std::vector<T> or any generic template class.
But this is usually negligible unless you're in an extremely constrained embedded environment

3. Why template can be better than raw pointers
Using a template lets the compiler
- Inline more code
- Do bounds-aware optimizations (e.g unroll loops)
- Catch size mismatches at compile time (zero-cost safety)

<Recommendataion>
If you're writing performance-critical code and want compile-time size guarantees, templates are actually your best option
If you're writing generic APIs that operate on arrays of many unknown sizes, use pointers or std::span

---

## Preventing code bloat from template array parameters for embedded context
In embedded systems, minimizing code size if often more important than abstraction or flexibility
- Why template can cause code bloat
    When you use a function template like this,
    ```c++
    template <size_t N>
    void process(int (&arr)[N])
    { // Do something }
    ```
    Each different value of N results in seperate function instantiation.
    For example,
    ```c++
    int a[5];
    int b[10];
    process(a); // Instatiates process<5>
    process(b); // Instatiates process<10>
    ```
    This results in
    - Two copies of the function being generated, one for N=5, one for N=10
    - If many array sizes are used throughout the codebase, code bloat happens

- Then, whow to aviod blot?
    1. Use fixed array sizes where possible
        If you can ensure all arrays are of the same size in a module, this aviods multiple temlpate instatiations
        ```c++
        constexpr size_t FIXED_SIZE=8;
        template <size_t N>
        void process(int (&arr)[N])
        {
            static_assert(N==FIXED_SIZE, "Only support fixed size arrays");
            ...
        }
        ```
        This forces the template to only instatiate once
    2. Avoid template if you don't need compile-time size
    3. LTO(Link-Time optimization)
        Enable LTO in your compiler (-flto in GCC/Clang).
        Which allows:
            - Dead code elimination of unused template instatiations
            - Better inlining decisions

## What is runtime overhead?
Runtime overhead refers to extra time and/or resource usage that happens while the program runs, not during compilation.

Examples of runtime overhead(Case | Overhead type | Reason)
- Function call via pointer | time | extra indirection, can't inline easily
- Virtual functoins | time + memory | vtable lookup, can't inline
- Heap allocation | time + fragmentation | slower than stack allocation
- Using std::vector | time + memory | allocations, bounds checking
- Recursive calls | time + stack usage | deep call stacks, overhead for each frame

Template-based array passing has zero runtime overhead
Reasons:
- The compiler knows everything at compile time
- Function calls are often inlined
- There's no indirection or dynamic allocation


3. What are bounds-aware optimizations?
Basic Idea:
If the compiler knows the size of an array at compile time, it can generate tighter, more optimized loops

- Without compile-time bounds
```c++
void sum(int* arr, int size)
{
    int total=0;
    for(int i=0; i<size; i++)
    {
        totla += arr[i];
    }
}
```
    - The compiler assumes size could be anything
    - Can't unroll the loof safely
    - Might insert bounds checks if using safe containers

- With compile-time known bounds
```c++
template <size N>
int sum(int (&arr)[N])
{
    int total = 0;
    for(size_t i=0; i<N; i++)
    {
        total += arr[i];
        return total;
    }
}
```
    - Compiler knows N is constant.
    - It can:
        - Unroll the loop
        - Perform vectorization
        - Omit bounds checks
        - Allocate registers more effectively


## Custom memory allocators in c++
Reason why using a custom allocator
- Default dynamic memory(new, malloc) may cause
    - Heap fragmenatation (especially in long-running or embedded apps)
    - Non-deterministic behavior (allocation time varies)
    - Memory overhead (metadata, alignment, ...)
    - Lack of control over allocation patterns
- So you write your own allocator tailroed to your
    - memory constraints
    - allocation patterns
    - performance requirements

### Core concepts
1. Memory pool
    A fixed-size buffer pre-allocated at startup or compile-time
    ```c++
    char buf[1024]; //static pool
    ```
    You manage allocation inside this manually - like slicing up the bufer for each object

2. Free list
    A data structure that tracks which parts of the pool are free or used.
    Exapmle
    - Each free block stores a pointer to the next free block
    - When you allocate(), pop from the list
    - When you deallocate(), push back into the list

3. Placement new
    Allows object consturction at a specific memory address
    ```c++
    void* mem = allocate(); // from your pool
    MyClass* obj = new(mem) Myclass(...) // placement new
    ```

    To destroy
    ```c++
    obj->~MyClass(); // manually call destructor
    deallocate(mem); // return memory to pool
    ```

### About Alignment
Alignment is a property related with memory address. Simply we can say than if an address X is aligned to Z then x is a multiple of Z ,that is X = Zn+0. Here the important thing is Z is always a power of 2.

Alignment is a property of a memory address, expressed as the numeric address modulo a power of 2. For example, the address 0x0001103F modulo 4 is 3. That address is said to be aligned to 4n+3, where 4 indicates the chosen power of 2. The alignment of an address depends on the chosen power of 2. The same address modulo 8 is 7. An address is said to be aligned to X if its alignment is Xn+0.

If a data item is stored in the memory with an address which is aligned to its size, then that data item is said to be naturally aligned, else misaligned.
For example:
    If an integer variable with size 4 bytes is stored in an address which is aligned to 4, then we can say that the variable is naturally aligned, that is the address of the variable should be a multiple of 4.

The compilers always tries to avoid misalignments. For simple datatypes the addresses are chosen such that it is a multiple of the size of the variable in bytes. The complier also pads suitably in the case of structures for natural alignment and access. Here the structure will be aligned to the maximum of the sizes of different data items in the structure.
For example :
```c++
   struct Bad // 1 byte + pad(3) + 4 bytes = 8 bytes
   {
        char a; // 1 byte
        int b; // 4 bytes
   };

    struct Good // 4 bytes + 1 byte + pad(3) = 8 bytes. But pad could be ignored by compiler options
    {
        int B; // 4 bytes
        char a; // 1 byte
    }

```
    Here the structure abc is aligned to 4 which is the size of int member which is obviously greater than 1 byte(size of char member).

alignas

This specifier is used to align user defined types like structure, class etc to a particular value which is a power of 2.

alignof

This is a kind of operator to get the value to which the structure or class type is aligned.
For example:
```c++
#include <iostream>
struct alignas(16) Bar
{
    int i; // 4 bytes
    int n; // 4 bytes
    short s; // 2 bytes
};
int main()
{
    std::cout << alignof(Bar) << std::endl; // output: 16
}
```
### Simple example: Fixed size allocator
```c++
template <typename T, size_t N>
class FixedAllocator
{
private:
    alignas(T) char buffer[sizeof(T) * N];
    T* freeList[N];
    size_t top = 0;

public:
    FixedAllocator()
    {
        for (size_t i=0; i<N; ++i)
        {
            freeList[i] = reinterpert_cast<T*>(&buffer[i * sizeof(T)]);
            top = N;
        }
    }

    T* allocate()
    {
        if(top == 0) return nullptr;
        return freeList[--top];
    }

    void deallocate(T* ptr)
    {
        if(top < N) freeList[top++] = ptr;
    }
};

// Usage
FixedAllocator<MyClass, 100> pool;
MyClass* obj = new(pool.allocate()) MyClass(...);
obj->~MyClass();
pool.deallocate(obj);


```
