## Basic C++

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

