# cpp-simple-vector
Developing my own container — vector<br>
<br>
Read in other languages:[English](README.md), [Русский](README.RUS.md)<br>

# Program Description
This is a highly simplified analogue of the standard container **vector**, with a similar structure and functionality. For a better understanding of the device and the features of the standard **vector**.

# Bild using Cmake
To build this project on linux you need:<br>
1) If you don't have Cmake installed, install Cmake<br>
2) If the "Debug" or "Release" folders are not created:<br>

```
mkdir Debug
mkdir Release
```
3) Run the command for Debug and/or Release conf:<br>

```
cmake -E chdir Debug/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Debug
cmake -E chdir Release/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release 
```
4) Go to "Debug" or "Release" folder and build:<br>

```
cmake --build .
```

5) To **Run** program - in the debug or release folder run:<br>

```
./simple_vector
```
# System requirements:
  1. C++17(STL)
  2. GCC (MinG w64) 11.2.0  

# Technology Stack:
  1. Copy elision и NRVO ( Named Return Value Optimization )
  2. move - семантика
  3. rvalue-ссылки
  4. Three levels of exception security<br>

## Before you start:
  0. Installation and configuration of all required components in the development environment to run the application<br>
  1. The use case and tests are shown in main.cpp .<br>

# Detailed description of the project:
`SimpleVector` is based on an array in dynamic memory. The container itself stores only the address of the beginning of the array, as well as information about its current size and capacity:<br>
 **Size** - the number of elements contained in the array;<br>
 **Capacity** - is the maximum number of elements that can fit into the array.<br>
`SimpleVector` - supports move semantics.<br>
<br>
You can draw an analogy with a bucket of water — `capacity_` defines the maximum volume of water that is placed in the bucket, and `size_` — the current amount of water in the bucket.<br>
The vector constructed by default has zero size and capacity, and the pointer to the array is **nullptr**.<br>
<br>
Like `std::vector`, the `SimpleVector` class can change its size in the direction of increasing and decreasing. To do this, use the `Resize` method<br>
<br>
The `SimpleVector` is cleaned by the `Clear` method. It does not change its capacity and is semantically equivalent to calling `Resize` with an argument equal to zero. The `Clear` method does not throw exceptions.<br>
# Template class SimpleVector<Type> - 
**simplified analog of the vector container, with the following functionality:**<br>
 - Copy constructor. A copy of the vector must have a capacity sufficient to store a copy of the elements of the original vector.<br>
 - By default. Creates an empty vector with zero capacity. Does not allocate dynamic memory and does not throw exceptions.<br>
 - Parameterized constructor that creates a vector of a given size. The elements of the vector are initialized with the default value for Type. The vector must have the same size and capacity. If the size is zero, dynamic memory for its elements should not be allocated.<br>
 - Constructor from `std::initializer_list`. The vector elements must contain a copy of the `initializer_list` elements. Has a size and capacity that matches the size and capacity of the passed `initializer_list`.<br>
 - The `GetSize` method to get the number of elements in the vector. Does not throw exceptions.<br>
 - The `GetCapacity` method to get the capacity of the vector. Does not throw exceptions.<br>
 - The `IsEmpty` method, which tells if the vector is empty. Does not throw exceptions.<br>
 - Operator `[]` to access an element of a vector by its index. It has two versions — constant and non-constant. Does not throw exceptions. For the operator to work correctly, the index of the array element must not go beyond the array.<br>
 - The `At` method for accessing an element of a vector by its index, analogous to the at method of the `vector` class. If the index goes outside the array, it should throw an exception `std::out_of_range`.<br>
 - The `Clear` method for clearing an array without changing its capacity. Does not throw exceptions.<br>
 - The `Resize` method for changing the number of elements in the array. The method should provide a strict guarantee of exception safety.<br>
 - Methods `begin, end, cbegin and cend` that return iterators to the beginning and end of the array. Pointers are used as iterators. These methods are declared with the **noexcept** specifier.<br>
 - When a vector is destroyed, the memory occupied by its elements is freed.<br>
 - Assignment operator. Must provide a strict guarantee of the safety of exceptions.<br>
 - The `PushBack` method, which adds an element to the end of the vector. Must provide a strict guarantee of the safety of exceptions.<br>
 - The `PopBack` method that removes the last element of the vector. Does not throw exceptions.<br>
 - The `Insert` method, which inserts an element into an arbitrary place in the container. Provides a basic guarantee of exception safety.<br>
 - The `Erase` method, which removes an element at an arbitrary position of the vector. Provides a basic guarantee of exception safety.<br>
 - The `swap` method that exchanges the contents of a vector with another vector. Throws no exceptions, has a runtime of **O(1)**.<br>
 - Operators `== and !=`. Two vectors are equal if their dimensions are equal and contain equal elements. Equality of capacity is not required.<br>
 - Operators `<, >, <=, >=`, performing lexicographic comparison of the contents of two vectors.
 - `Reserve` sets the capacity of the vector. This method increases the efficiency of the code in the case when the user knows in advance at least the approximate number of elements in the vector. `Reserve` immediately allocates the required amount of memory. When adding new elements to the vector, copying will occur either much less frequently or not at all.<br>
If `new_capacity` is greater than the current `capacity`, the memory must be reallocated and the vector elements copied to a new memory segment.<br>
In addition, it is possible to call the `SimpleVector` constructor, which immediately reserves the required amount of memory.<br>