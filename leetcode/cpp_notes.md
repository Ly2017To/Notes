## C/C++ Notes
---
### Key Concepts in C
- **Variables & Data Types**: int, char, float, double, pointers, arrays, structures, etc.
- **Control Structures**: if-else, switch, loops (for, while, do-while).
- **Functions**: Definition, Declaration, Return types, Parameters, Recursion.
- **Pointers**: Memory addresses, dereferencing, pointer arithmetic, function pointers.
- **Memory Management**: malloc(), free(), calloc(), realloc().
- **Arrays**: Static, dynamic arrays, multidimensional arrays.
- **String Manipulation**: strcpy(), strcat(), strcmp(), strlen(), etc.

---
### Key Concepts in C++
- **Classes & Objects**: Object-oriented programming basics in C++.
- **Constructors & Destructors**: Initialization and cleanup in classes.
- **Operator Overloading**: Custom behavior for operators.
- **Inheritance**: Single, multiple, and virtual inheritance.
- **Polymorphism**: Function overloading (static), overriding (dynamic), and virtual functions.
- **Encapsulation & Abstraction**: Hiding implementation details.
- **Templates**: Function templates, class templates for generic programming.
- **Exception Handling**: try, catch, throw, custom exception classes.

---
### Callback Functions
- A **callback function** is a function passed as an argument to another function. The receiving function can then call the callback function at a certain point in its execution, enabling flexibility and modularity.
- Callback functions allow functions to be passed as arguments.
- They are often used to handle events, signals, or asynchronous operations.
    - Asynchronous Operations: Callbacks allow functions to perform operations asynchronously and notify the caller when the task is completed.
    - Event Handling: In event-driven programming, callbacks are used to handle events like mouse clicks, key presses, etc.
    - Custom Behavior: Pass different callback functions to customize the behavior of a function, such as sorting with different comparison functions.
- In C, callbacks are typically implemented using **function pointers**. To declare a function pointer, you specify the return type, the pointer syntax, and the function parameters. To declare a function that accepts a callback, you define a **function pointer** in the parameter list of the function.
```c
return_type (*pointer_name)(parameter_types); //declare a function pointer
func_ptr = function_name;  // Assign function to pointer
void myFunction(void (*callback)(parameter_types)); //a function accepts a callback

//simulate an asynchronous example with callback function
#include <stdio.h>
#include <unistd.h> // For sleep()

void myCallbackFunction() {
    printf("Callback function executed!\n");
}

// Function that takes a callback function as an argument
void performAsyncOperation(void (*callback)()) {
    printf("Performing asynchronous operation...\n");
    sleep(2); // Simulate some delay
    callback(); // Call the callback function
}

int main() {
    // Pass the callback function to performAsyncOperation
    performAsyncOperation(myCallbackFunction);
    return 0;
}

//another example, quick sort function in C standard library stdlib.h
qsort(void *base, size_t n_items, size_t size, int(*compare)(const void *, const void *));
```

---
### Endianess
- Big Endian: the MSB is stored at the lowest memory address.
- Small Endian: the LSB is stored at the lowest memory address.
- Some older systems like the Motorola 68k series and certain RISC processors (e.g., SPARC) use Big Endian.
- Most modern systems, including x86 (Intel/AMD processors), ARM (in Little Endian mode), and many embedded systems use Little Endian.
```c
//a function to swap endianess
unsigned int swap_endian(unsigned int num) {
    return ((num >> 24) & 0xFF) | 
           ((num << 8) & 0xFF0000) | 
           ((num >> 8) & 0xFF00) | 
           ((num << 24) & 0xFF000000);
}

//check endianess
#include <stdio.h>

int main() {
    union {
        int i;
        char c[4];
    } test;

    test.i = 0x12345678;

    // Check the first byte
    if (test.c[0] == 0x78)
        printf("Little Endian\n");
    else
        printf("Big Endian\n");

    return 0;
}
```

---
### Struct and Union

| **Feature**            | **Struct**                                      | **Union**                                         |
|------------------------|-------------------------------------------------|--------------------------------------------------|
| **Memory Allocation**   | Allocates memory for all members separately.    | Allocates memory for the largest member only.    |
| **Size**                | Size is the sum of sizes of all members, plus padding for alignment. | Size is the size of the largest member, plus padding for alignment. |
| **Access**              | All members can be accessed independently.      | Only one member can be accessed at a time.       |
| **Use Case**            | Used when you need to store multiple data types independently. | Used when you need to store multiple data types in the same memory location, but only one at a time. |
| **Memory Efficiency**   | Less memory efficient as it stores all members. | More memory efficient as it stores only the largest member. |
| **Initialization**      | Can initialize all members separately.          | Only the first member is initialized; others are overwritten when accessed. |
| **Example**             | `struct Student { int age; float height; };`    | `union Data { int i; float f; };`               |

---
### Key Differences (with Alignment Considerations)
1. **Memory Allocation**: A `struct` reserves memory for each of its members with proper alignment, while a `union` uses a single memory location shared by all its members. This makes unions more memory efficient when only one of the members is needed at a time, but both `struct` and `union` sizes are impacted by alignment rules.
2. **Access**: In a `struct`, each member can be accessed independently, but in a `union`, accessing one member overwrites the others.
3. **Size**: The size of a `struct` is the sum of its members (with any required padding for alignment), while the size of a `union` is the size of its largest member (also including any padding for alignment).

---
### Class
- Inheritance allows a derived class to inherit attributes and methods from a base class, enabling reuse of code.
- Polymorphism allows methods to behave differently based on the object type, even if they're called through a base class pointer or reference.
- Virtual functions are crucial for achieving polymorphism. When a base class method is marked as virtual, the program uses the derived class method if the object is of a derived type.
- For derived class, methods from the base class can be used if they are protected or public.

```
Class Car{

private:
    string brand;
    int year;

public:
    //constructor
    Car (string brand, int year){
        this->brand = brand;
        this->year = year;
    }

    //destructor
    // Virtual destructor for proper cleanup in derived classes
    virtual ~Car() {
        cout << "Car object destroyed" << endl;
    }

    //to set brand
    void set_brand(string brand){
        this->brand=brand;
    }

    //to get brand
    string get_brand(){
        return brand;
    }

    // Virtual function for polymorphism
    virtual void display_info() {
        cout << "Brand: " << brand << ", Year: " << year << endl;
    }
};

class Electric_car: public Car {

private:
    int battery_life;

public:
    //constructor
    Electric_car (string band, int year, int battery_life):  Car(brand, year) {
       
        this->battery_life=battery_life;
    }

    // Destructor for Electric_car
    ~Electric_car() {
        cout << "Electric_car destructor called!" << endl;
    }

    ......

    //overiding display_info
    void display_info(){
        cout << "Brand: " << brand << ", Year: " << year << ", Battery Life: " << battery_life << " hours" << endl;
    }

};
```
---
### Virtual
- Runtime Polymorphism: When a base class pointer or reference points to a derived class object, calling a virtual function will invoke the derived class's version of the method, not the base class's version, even if the pointer/reference is of the base class type.
- Dynamic Dispatch: The decision of which function to call (base class or derived class version) is made at runtime, not at compile-time.
- Virtual Destructor: If you have a class hierarchy and you dynamically allocate objects, it’s important to have a virtual destructor in the base class. This ensures that the derived class’s destructor is called correctly when the object is deleted through a base class pointer.

---
### Memory Management
- Use 'new/delete' when working in C++ and dealing with objects, as it properly handles constructors and destructors.
    - 'new' calls the constructor of the object, ensuring proper initialization.
    - 'delete' calls the destructor of the object, ensuring proper cleanup.
- Use 'malloc/free' for C programming or when working with raw memory. It doesn't involve constructors or destructors, so it's suitable for non-object data allocation, such as arrays or basic types.
    - malloc doesn't call any constructors or initialize the memory (it simply allocates a raw memory block).
    - free doesn't call any destructors either.

### Difference between void *, nullptr and NULL
| Feature                | `void *`                            | `nullptr`                             | `NULL`                         |
|------------------------|-------------------------------------|---------------------------------------|--------------------------------|
| **Purpose**            | A generic pointer type that can point to any data type | A constant representing a null pointer | A macro used to represent a null pointer |
| **Type**               | Can point to any type (not type-safe)           | Type `std::nullptr_t` (type-safe)     | Typically `0` or `(void *)0` (not type-safe) |
| **Dereferencing**      | Cannot be dereferenced directly (needs casting) | Can be assigned to any pointer type   | Can be used as a null pointer but can cause ambiguity |
| **Introduced**         | In the C standard, available since C | C++11                                 | C (but used in C++)           |
| **Type safety**        | Not type-safe                        | Type-safe (no ambiguity)              | Not type-safe                 |

---
### C vs. C++ String Handling
| Feature                 | C (C-style strings)                                    | C++ (std::string)                                          |
|-------------------------|--------------------------------------------------------|------------------------------------------------------------|
| **Representation**      | Array of characters (e.g., `char str[] = "Hello";`)    | `std::string` class                                        |
| **Size**                | Fixed or needs manual tracking                         | Dynamic, grows/shrinks as needed                           |
| **Null-Termination**    | Yes (null-terminated)                                  | No, uses internal size management                          |
| **Memory Management**   | Manual (e.g., `malloc()`/`free()`) for dynamic strings | Automatic, managed by `std::string`                        |
| **Functionality**       | Limited (requires `<string.h>` functions)              | Rich set of member functions (e.g., `append()`, `substr()`)|
| **Safety**              | Prone to errors like buffer overflow or invalid access | Type-safe, less prone to errors (no manual memory handling)|
| **Performance**         | Faster (no overhead of object management)              | Slightly slower due to object management and dynamic resizing |
| **Conversion**          | Convert to/from `std::string` manually                 | Can easily convert to/from C-style strings with `.c_str()` |
| **Example Usage**       | `char str[] = "Hello";`                                | `std::string str = "Hello";`                      |

---
### RAII
- RAII stands for Resource Acquisition Is Initialization.
- It is a fundamental C++ programming idiom used for automatic resource management, where the lifetime of resources (like memory, file handles, sockets, locks) is tied to the lifetime of objects.
- Key Idea:
    - When an object is created, it acquires some resource.
    - When the object goes out of scope, its destructor automatically releases the resource.
    - This ensures no resource leaks and exception safety, because resources are released even if an exception occurs.
- How RAII Works:
    - Constructor acquires the resource.
    - Destructor releases the resource.
    - Because destructors are automatically called when objects go out of scope, resource management is automatic.

---
### Comparison of Pointer vs. Reference in C++
| **Feature**             | **Pointer**                                                | **Reference**                                              |
|-------------------------|------------------------------------------------------------|------------------------------------------------------------|
| **Definition**          | A pointer is a variable that holds the memory address of another variable. | A reference is an alias (alternative name) for an existing variable. |
| **Initialization**      | Must be explicitly initialized to point to a variable (can be `nullptr`). | Must be initialized when declared (cannot be null or uninitialized). |
| **Re-assignment**       | Can be reassigned to point to a different variable or `nullptr`. | Cannot be reassigned once it's bound to a variable.        |
| **Memory Address**      | Stores the memory address of another variable.            | Does not store an address; it’s simply an alias to another variable. |
| **Nullability**         | Can be `nullptr`, meaning it doesn't point to anything.   | Cannot be null; always refers to an existing object.       |
| **Syntax**              | `int* ptr = &x;`                                           | `int& ref = x;`                                            |
| **Dereferencing**       | Requires explicit dereferencing using `*` (e.g., `*ptr = 10;`). | Directly used like the variable itself (no need for `*`).   |
| **Memory Size**         | Typically stores a memory address (usually 4 or 8 bytes, depending on platform). | Typically does not take additional memory (size is the same as the referred variable). |
| **Use in Functions**    | Can be passed to functions to modify the pointed-to object (can be `nullptr`). | Used to pass arguments by reference, ensuring modifications on the actual variable. |
| **Example of Use**      | Used for dynamic memory allocation (`new/delete`), array access, and traversing data structures. | Used for passing large objects efficiently (no copies), function chaining, and operator overloading. |
| **Use in Arrays**       | A pointer can be used to iterate over array elements.     | A reference is often used when passing arrays to functions to avoid copying. |
| **Memory Management**   | Pointers require manual memory management (e.g., `new/delete` or `malloc/free`). | No need for manual memory management (automatically cleaned up when out of scope). |

---
### Smart Pointers
- **`unique_ptr`**: Best for exclusive ownership with automatic memory management and no copying.
- **`shared_ptr`**: Ideal when shared ownership is needed, but involves reference counting overhead.
- **`weak_ptr`**: Used to avoid circular references, providing a non-owning reference that doesn't affect the lifetime of the object.

| Feature               | **`std::unique_ptr`**                                      | **`std::shared_ptr`**                                       | **`std::weak_ptr`**                                         |
|-----------------------|------------------------------------------------------------|------------------------------------------------------------|------------------------------------------------------------|
| **Ownership**          | Exclusive ownership (only one `unique_ptr` can own the object). | Shared ownership (multiple `shared_ptr`s can own the same object). | Non-owning reference to an object managed by `shared_ptr`.   |
| **Memory Management**  | Automatically deletes the object when the `unique_ptr` goes out of scope. | Automatically deletes the object when the last `shared_ptr` goes out of scope. | Does not affect the lifetime of the object. Can check the object's validity using `lock()`. |
| **Copying**            | Cannot be copied; can only be moved (ownership is transferred). | Can be copied (shared ownership). | Cannot be copied; can be converted to `shared_ptr` via `lock()`. |
| **Usage**              | Used when an object has a single owner and needs exclusive access. | Used when multiple parts of code need shared ownership of an object. | Used to prevent circular references or to observe an object without extending its lifetime. |
| **Reference Count**    | No reference counting; single owner.                      | Uses reference counting to track the number of owners.     | No reference counting; does not contribute to the reference count. |
| **Performance**        | Lightweight with no overhead (preferred for unique ownership scenarios). | Can have performance overhead due to reference counting.    | No overhead if not locked, but converting to `shared_ptr` introduces overhead. |
| **Nullptr Safety**     | Ensures the object is always valid until it goes out of scope. | Ensures the object is always valid as long as at least one `shared_ptr` exists. | Can be empty, and requires `lock()` to convert to `shared_ptr` before usage. |
| **Common Use Case**    | Resource management where the object is exclusively owned by one part of the program. | Resource management where multiple parts of the program need to access the same object. | Avoiding circular dependencies, observing an object without extending its lifetime. |

---
### Lambda Expressions
- A lambda expression in C++ is an anonymous function that can be defined directly in the code. It allows you to write small functions on the fly, especially for tasks like passing a function to algorithms or custom callbacks.
- Basic Syntax: [capture](parameters) -> return_type { function_body }
    - Capture Clause [capture]: Specifies which external variables can be accessed by the lambda.
        - []: No capture (doesn't capture any external variables).
        - [=]: Capture all external variables by value.
        - [&]: Capture all external variables by reference.
        - [x]: Capture variable x by value.
        - [&x]: Capture variable x by reference.
        - [this]: Capture the this pointer for member functions.
    - Parameters (parameters): List of input parameters (optional).
    - Return Type -> return_type: Specifies the return type of the lambda (optional, can be inferred).
    - Function Body { function_body }: The code executed by the lambda.



