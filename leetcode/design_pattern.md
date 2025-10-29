# Design patterns 
- They are reusable solutions to common problems in software design. 
- Some of the most important patterns to understand include creational patterns, structrual patterns and behavioral patterns.
---
## Creational Patterns:
- Creational patterns are a type of design pattern that deal with object creation mechanisms. These patterns provide solutions to instantiate objects in a way that is flexible, reusable, and decoupled from the system's main logic. The goal is to abstract the instantiation process, which allows the system to be more flexible and easier to maintain.
---
### Singleton: 
- **Intent**: Ensure a class has only one instance and provide a global access point to it.
- **Structure**:
    - Private constructor.
    - Static instance.
    - Public static method to get the instance.
- **When to Use**: When you need exactly one instance across the system, like a configuration manager or logger.
```cpp
class Singleton {
private:
    static Singleton* instance; //Note: The static keyword means that the method does not operate on an instance of the class but instead operates on the class itself
    Singleton() {} // Private constructor

public:
    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }
    //Note: the public static method refers to a method that belongs to the class itself rather than any instance of the class.
};

Singleton* Singleton::instance = nullptr;
```
---
### Factory Method: 
- **Intent**: The Factory Method pattern is used to define an interface for creating objects, but the actual instantiation is left to subclasses. It allows a class to delegate the responsibility of object creation to its subclasses.
- **Structure**:
    - Abstract class with a factory method.
    - Concrete classes implement the factory method.
- **When to Use**: When the exact type of the object isn't known until runtime.
```cpp
class Product {
public:
    virtual void use() = 0;
};

class ConcreteProductA : public Product {
public:
    void use() override { std::cout << "Product A\n"; }
};

class Creator {
public:
    virtual Product* factoryMethod() = 0;
};

class ConcreteCreatorA : public Creator {
public:
    Product* factoryMethod() override { return new ConcreteProductA(); }
};
```
---
### Abstract Factory: 
- **Intent**: Provide an interface for creating families of related or dependent objects without specifying their concrete classes.
- **Structure**:
    - Abstract factory interface with multiple factory methods.
    - Concrete factories implement the methods.
- **When to Use**: When you need to create a set of related objects without specifying their concrete classes.
```cpp
class AbstractProductA {
public:
    virtual void doSomething() = 0;
};

class ConcreteProductA1 : public AbstractProductA {
public:
    void doSomething() override { std::cout << "Product A1\n"; }
};

class AbstractFactory {
public:
    virtual AbstractProductA* createProductA() = 0;
};

class ConcreteFactory1 : public AbstractFactory {
public:
    AbstractProductA* createProductA() override { return new ConcreteProductA1(); }
};
```
---
### Builder: 
- **Intent**: The Builder pattern is used to separate the construction of a complex object from its representation, allowing the same construction process to create different representations. It provides a way to construct a complex object step by step.
- **Structure**:
    - **Builder** interface defines methods for building parts of the object.
    - **ConcreteBuilder** implements the Builder interface and assembles the product.
    - **Product** is the complex object being built.
    - **Director** controls the construction process, using a Builder to create the object.
- **When to Use**:
    - You need to construct a complex object step by step.
    - The object needs to be assembled in different ways (variations).
    - You want to encapsulate the creation logic, making it easier to construct the object.
```cpp
// Product - the complex object being built
class Product {
public:
    void addPart(const std::string& part) {
        parts.push_back(part);
    }
    
    void show() {
        for (const auto& part : parts) {
            std::cout << part << std::endl;
        }
    }

private:
    std::vector<std::string> parts;
};

// Builder interface
class Builder {
public:
    virtual void buildPartA() = 0;
    virtual void buildPartB() = 0;
    virtual Product* getResult() = 0;
};

// ConcreteBuilder
class ConcreteBuilder : public Builder {
private:
    Product* product;
public:
    ConcreteBuilder() : product(new Product()) {}
    
    void buildPartA() override {
        product->addPart("Part A");
    }

    void buildPartB() override {
        product->addPart("Part B");
    }

    Product* getResult() override {
        return product;
    }
};

// Director - orchestrates the construction process
class Director {
private:
    Builder* builder;
public:
    Director(Builder* b) : builder(b) {}
    
    Product* construct() {
        builder->buildPartA();
        builder->buildPartB();
        return builder->getResult();
    }
};
```
---
### Prototype: 
- **Intent**: The Prototype pattern is used to create new objects by copying an existing object, known as a prototype. Instead of creating new objects from scratch, the prototype allows cloning to obtain new instances with the same attributes and behaviors.
- **Structure**:
    - Prototype defines a method for cloning itself.
    - ConcretePrototype implements the cloning method and creates a copy of itself.
    - The client code can create new objects by cloning an existing one.
- **When to Use**:
    - The creation of an object is costly or complex.
    - You want to clone existing objects to create new instances with the same state.
    - You need to create several identical objects that share the same structure but may vary in content.
```cpp
// Prototype interface
class Prototype {
public:
    virtual Prototype* clone() = 0;
    virtual void show() = 0;
};

// ConcretePrototype
class ConcretePrototype : public Prototype {
private:
    std::string data;
public:
    ConcretePrototype(const std::string& d) : data(data) {}

    Prototype* clone() override {
        return new ConcretePrototype(*this); // Copy the data
    }

    void show() override {
        std::cout << "Data: " << data << std::endl;
    }
};
```
---
### Object Pool
- **Intent**: The **Object Pool** design pattern is a **creational** pattern that deals with the management of a pool of reusable objects. Instead of frequently creating and destroying objects, the pattern allows objects to be reused, reducing overhead and improving performance, especially in resource-intensive systems.
- **Structure**:
    - **Pool**: Manages the collection of reusable objects.
    - **Object**: The individual object being pooled (e.g., database connection).
    - **Client**: Requests and uses objects from the pool.
    - **Factory Method** (optional): A method to create objects if needed (when the pool is empty).
    - **Object Reuse**: After use, objects are returned to the pool for reuse.
- **When to Use**:
    - Reuses objects to avoid costly object creation and destruction.
    - Manages a collection of objects that can be borrowed and returned.
    - Ideal for scenarios where object creation is expensive (e.g., database connections, threads).
```cpp
class ObjectPool {
private:
    std::queue<std::shared_ptr<Object>> pool;  // Pool of objects

public:
    ObjectPool(size_t size);
    
    // Acquire an object from the pool
    std::shared_ptr<Object> acquireObject();
    
    // Return an object to the pool
    void releaseObject(std::shared_ptr<Object> obj);
};
```
---
## Structural Patterns:
- Structural patterns are design patterns that deal with the composition of classes and objects to form larger structures. They focus on simplifying the design by organizing and composing objects in ways that make the system easier to understand and more efficient. These patterns help create a structure that enables easier communication between components while maintaining flexibility.
---
### Adapter: 
- **Intent**: Convert the interface of a class into another interface that a client expects. Allows incompatible interfaces to work together by creating a wrapper that converts one interface to another.
- **Structure**:
    - class that implements the interface expected by the client.
    - It holds an instance of the class to be adapted.
- **When to Use**: When you want to integrate classes with incompatible interfaces.
```cpp
class Target {
public:
    virtual void request() = 0;
};

class Adaptee {
public:
    void specificRequest() {
        std::cout << "Specific request\n";
    }
};

class Adapter : public Target {
private:
    Adaptee* adaptee;
public:
    Adapter(Adaptee* a) : adaptee(a) {}
    void request() override {
        adaptee->specificRequest();
    }
};
```
---
### Decorator: 
- **Intent**: Attach additional responsibilities to an object dynamically.
- **Structure**:
    - Component interface.
    - ConcreteComponent class implements the interface.
    - Decorator class implements the same interface and contains a pointer to a Component object.
- **When to Use**: When you need to add responsibilities to objects in a flexible and reusable way.
```cpp
class Component {
public:
    virtual void operation() = 0;
};

class ConcreteComponent : public Component {
public:
    void operation() override { std::cout << "Operation\n"; }
};

class Decorator : public Component {
private:
    Component* component;
public:
    Decorator(Component* c) : component(c) {}
    void operation() override {
        component->operation();
        std::cout << "Additional operation\n";
    }
};
```
---
### Facade: 
- **Intent**: The Facade pattern is used to provide a simplified interface to a complex subsystem, making it easier to interact with. It defines a higher-level interface that makes the subsystem easier to use.
- **Structure**:
    - **Facade**: The primary class that simplifies interactions with the subsystem. It encapsulates the complexities of the subsystem and exposes a simplified interface to the client.
    - **Subsystem classes**: These represent the complex classes of the subsystem that the facade is hiding.
- **When to Use**:
    - You want to provide a simplified interface to a complex subsystem.
    - You want to decouple a client from the subsystem, making it easier to maintain or change the subsystem without affecting the client.
    - You want to make the system easier to use, reducing dependencies and interactions with multiple components.
```cpp
// Subsystem classes
class Light {
public:
    void turnOn() { std::cout << "Light is on\n"; }
    void turnOff() { std::cout << "Light is off\n"; }
};

class Heater {
public:
    void turnOn() { std::cout << "Heater is on\n"; }
    void turnOff() { std::cout << "Heater is off\n"; }
};

class SecuritySystem {
public:
    void activate() { std::cout << "Security system activated\n"; }
    void deactivate() { std::cout << "Security system deactivated\n"; }
};

// Facade class
class HomeAutomationFacade {
private:
    Light* light;
    Heater* heater;
    SecuritySystem* security;

public:
    HomeAutomationFacade(Light* l, Heater* h, SecuritySystem* s)
        : light(l), heater(h), security(s) {}

    void turnOnEverything() {
        light->turnOn();
        heater->turnOn();
        security->activate();
    }

    void turnOffEverything() {
        light->turnOff();
        heater->turnOff();
        security->deactivate();
    }
};
```
---
### Proxy: 
- **Intent**: The Proxy pattern provides an object representing another object, controlling access to it. It acts as a surrogate or placeholder for the real object and can manage access, perform additional tasks (like lazy loading or access control), or add functionality.
- **Types of Proxies**:
    - Virtual Proxy: Controls access to an object that is expensive to create by initializing it lazily.
    - Remote Proxy: Represents an object that is located in a different address space (for example, across a network).
    - Protective Proxy: Controls access to an object by enforcing security or permissions.
    - Cache Proxy: Manages caching and controls access to cached data.
- **Structure**:
    - Subject: The interface that both the real object and the proxy implement.
    - RealSubject: The actual object being represented, which performs the real operations.
    - Proxy: Represents the real object, controlling access to it and potentially adding additional behavior.
- **When to Use**:
    - You want to control access to an object in a variety of ways (e.g., lazy loading, access control, logging, etc.).
    - You need to act as a placeholder for a resource that is expensive to initialize.
    - You want to add functionality to an object without modifying the object itself.
```cpp
// Subject interface
class RealSubject {
public:
    virtual void request() = 0;
};

class RealSubjectImpl : public RealSubject {
public:
    void request() override {
        std::cout << "RealSubject request\n";
    }
};

// Proxy class
class Proxy : public RealSubject {
private:
    RealSubject* realSubject;

public:
    Proxy() : realSubject(nullptr) {}

    void request() override {
        if (!realSubject) {
            realSubject = new RealSubjectImpl();  // Lazy initialization
        }
        realSubject->request();
    }
};
```
---
### Composite: 
- **Intent**: Compose objects into tree structures to represent part-whole hierarchies.
- **Structure**:
    - Component interface (could be abstract class or interface).
    - Leaf class implements the component interface.
    - Composite class also implements the component interface and contains a list of child components.
- **When to Use**: When you need to represent hierarchical structures like trees.
```cpp
class Component {
public:
    virtual void operation() = 0;
};

class Leaf : public Component {
public:
    void operation() override { std::cout << "Leaf operation\n"; }
};

class Composite : public Component {
private:
    std::vector<Component*> children;
public:
    void add(Component* component) {
        children.push_back(component);
    }

    void operation() override {
        for (auto& child : children) {
            child->operation();
        }
    }
};
```
---
### Bridge
- **Intent**: The **Bridge** design pattern is a **structural pattern** that decouples an abstraction from its implementation, allowing both to evolve independently. It is used when both the abstraction and implementation should be extended independently without affecting each other.
- **Structure**:
    - Separates the abstraction from the implementation.
    - Allows changing the abstraction and implementation independently.
- **When to Use**: Useful for cases where you have multiple possible implementations for a single abstraction.
```cpp
// Implementor: Defines the interface for implementation
class Implementor {
public:
    virtual void operationImpl() = 0;  // Abstract method for implementation
};

// ConcreteImplementor: Implements the interface
class ConcreteImplementorA : public Implementor {
public:
    void operationImpl() override {
        std::cout << "ConcreteImplementorA operation." << std::endl;
    }
};

class ConcreteImplementorB : public Implementor {
public:
    void operationImpl() override {
        std::cout << "ConcreteImplementorB operation." << std::endl;
    }
};

// Abstraction: Contains the reference to an Implementor object
class Abstraction {
protected:
    Implementor* implementor;
public:
    Abstraction(Implementor* imp) : implementor(imp) {}
    virtual void operation() = 0; 
};

// RefinedAbstraction: Extends the Abstraction
class RefinedAbstraction : public Abstraction {
public:
    RefinedAbstraction(Implementor* imp) : Abstraction(imp) {}
    void operation() override {
        std::cout << "RefinedAbstraction performing operation:" << std::endl;
        implementor->operationImpl();
    }
};
```
---
### Flyweight
- **Intent**: The Flyweight design pattern is a structural pattern that aims to reduce memory usage by sharing common data among multiple objects. It is used when a large number of objects need to be created, but many of them share common states.
- **Structure**:
    - Shares common data between objects to reduce memory consumption.
    - Divides an object’s state into intrinsic (shared) and extrinsic (unique) states.
    - Only the intrinsic state is shared among objects, while the extrinsic state is passed by the client.
- **When to Use**:
    - When your application has a large number of objects with shared data.
    - When objects can be divided into intrinsic and extrinsic states.
```cpp
// Flyweight: Interface for flyweight objects
class Flyweight {
public:
    virtual void operation(int extrinsicState) = 0;
};

// ConcreteFlyweight: Implements the Flyweight interface and stores intrinsic state
class ConcreteFlyweight : public Flyweight {
private:
    int intrinsicState;
public:
    ConcreteFlyweight(int state) : intrinsicState(state) {}
    void operation(int extrinsicState) override {
        std::cout << "Flyweight with intrinsicState " << intrinsicState 
                  << " and extrinsicState " << extrinsicState << std::endl;
    }
};

// FlyweightFactory: Manages the pool of flyweight objects
class FlyweightFactory {
private:
    std::map<int, Flyweight*> flyweights;
public:
    Flyweight* getFlyweight(int intrinsicState) {
        if (flyweights.find(intrinsicState) == flyweights.end()) {
            flyweights[intrinsicState] = new ConcreteFlyweight(intrinsicState);
        }
        return flyweights[intrinsicState];
    }
};
```
___ 
## Behavioral Patterns:
- Behavioral patterns are design patterns that focus on the interaction between objects and how responsibilities are distributed among them. These patterns help to simplify complex control flows and provide solutions for the way objects communicate with each other. They deal with the behavior of an application or system, making it easier to understand and extend the interaction logic.
---
### Observer:
- **Intent**: Define a one-to-many dependency between objects, so when one object changes state, all its dependents are notified and updated automatically.
- **Structure**:
    - Subject maintains a list of observers.
    - Observer updates itself when the subject changes.
- **When to Use**: When an object’s state changes and all dependent objects need to be notified.
```cpp
class Observer {
public:
    virtual void update() = 0;
};

class Subject {
private:
    std::vector<Observer*> observers;
public:
    void addObserver(Observer* o) { observers.push_back(o); }
    void notify() {
        for (auto& observer : observers) {
            observer->update();
        }
    }
};

class ConcreteObserver : public Observer {
public:
    void update() override { std::cout << "Observer updated\n"; }
};
```
---
### Strategy: 
- **Intent**: Define a family of algorithms, encapsulate each one, and make them interchangeable.
- **Structure**:
    - Context class that contains a reference to a Strategy object.
    - Strategy interface that defines a method for the algorithm.
    - Concrete strategies that implement the strategy interface.
- **When to Use**: When you have multiple algorithms to choose from, and you want to be able to change them dynamically.
```cpp
class Strategy {
public:
    virtual void execute() = 0;
};

class ConcreteStrategyA : public Strategy {
public:
    void execute() override { std::cout << "Strategy A\n"; }
};

class Context {
private:
    Strategy* strategy;
public:
    Context(Strategy* s) : strategy(s) {}
    void setStrategy(Strategy* s) { strategy = s; }
    void executeStrategy() { strategy->execute(); }
};
```
---
### Command: 
- **Intent**: Encapsulate a request as an object, thereby allowing for parameterization of clients with different requests, queuing of requests, and logging of the requests.
- **Structure**:
    - Command interface with an execute method.
    - ConcreteCommand that implements the execute method.
    - Invoker class that triggers the command.
- **When to Use**: When you need to parameterize objects by an action to be performed.
```cpp
class Command {
public:
    virtual void execute() = 0;
};

class ConcreteCommand : public Command {
private:
    Receiver* receiver;
public:
    ConcreteCommand(Receiver* r) : receiver(r) {}
    void execute() override {
        receiver->action();
    }
};

class Receiver {
public:
    void action() { std::cout << "Action performed\n"; }
};

class Invoker {
private:
    Command* command;
public:
    void setCommand(Command* cmd) { command = cmd; }
    void invoke() { command->execute(); }
};
```
---
### State: 
- **Intent**: The State design pattern allows an object to alter its behavior when its internal state changes. It appears as though the object changed its class. This is useful when an object needs to change its behavior based on its state.
- **Structure**:
    - Context: Maintains the current state and delegates behavior to the state object.
    - State (Abstract): Defines an interface for handling requests that depend on the context’s state.
    - ConcreteState: Implements the behavior associated with a particular state.
- **When to Use**:
    - When an object's behavior depends on its state, and it must change its behavior at runtime.
    - When using conditional statements (e.g., if-else or switch) to manage states becomes too complex.
```cpp
// Abstract State
class State {
public:
    virtual void handleRequest() = 0;
};

// ConcreteStateA
class ConcreteStateA : public State {
public:
    void handleRequest() override {
        cout << "State A: Handling request\n";
    }
};

// ConcreteStateB
class ConcreteStateB : public State {
public:
    void handleRequest() override {
        cout << "State B: Handling request\n";
    }
};

// Context
class Context {
private:
    State* state;
public:
    Context(State* state) : state(state) {}
    
    void setState(State* newState) {
        state = newState;
    }
    
    void request() {
        state->handleRequest();
    }
};
```
---
### Iterator: 
- **Intent**: The Iterator design pattern provides a way to access elements of a collection (like an array, list, etc.) sequentially without exposing its underlying representation (e.g., internal data structures).
- **Structure**:
    - Iterator (Abstract): Defines the interface for accessing elements in a collection.
    - ConcreteIterator: Implements the iteration logic for a specific collection.
    - Aggregate (Abstract): Defines the interface for creating an iterator.
    - ConcreteAggregate: Concrete implementation of the aggregate, provides an iterator for its collection.
- **When to Use**:
    - When you want to provide a standard way to traverse a collection of objects.
    - When you want to decouple the collection object from the traversal logic.
```cpp 
// Iterator (Abstract)
template <typename T>
class Iterator {
public:
    virtual T& next() = 0;
    virtual bool hasNext() = 0;
};

// ConcreteIterator
template <typename T>
class ConcreteIterator : public Iterator<T> {
private:
    vector<T>& collection;
    size_t index;
public:
    ConcreteIterator(vector<T>& collection) : collection(collection), index(0) {}
    
    T& next() override {
        return collection[index++];
    }
    
    bool hasNext() override {
        return index < collection.size();
    }
};

// Aggregate (Abstract)
template <typename T>
class Aggregate {
public:
    virtual Iterator<T>* createIterator() = 0;
};

// ConcreteAggregate
template <typename T>
class ConcreteAggregate : public Aggregate<T> {
private:
    vector<T> collection;
public:
    ConcreteAggregate(vector<T> collection) : collection(collection) {}
    
    Iterator<T>* createIterator() override {
        return new ConcreteIterator<T>(collection);
    }
    
    vector<T>& getCollection() {
        return collection;
    }
};
```
---
### Mediator: 
- **Intent**: The Mediator design pattern defines an object that centralizes communication between multiple objects, reducing the direct dependencies between them. Instead of objects referring to each other, they communicate via the mediator. This promotes loose coupling and easier maintenance.
- **Structure**:
    - Mediator (Abstract): Defines the interface for communication between the colleague objects.
    - ConcreteMediator: Implements the communication logic and knows about all the colleagues.
    - Colleague (Abstract): Each colleague knows its mediator and communicates with the mediator to interact with other colleagues.
    - ConcreteColleague: Implements the communication with other colleagues via the mediator.
- **When to Use**:
    - When you have a system with many objects that communicate with each other directly, leading to a complex web of interactions.
    - When you want to centralize communication and control between components, reducing dependencies and complexity.
```cpp 
// Mediator (Abstract)
class Mediator {
public:
    virtual void sendMessage(const string& message, class Colleague* colleague) = 0;
};

// Colleague (Abstract)
class Colleague {
protected:
    Mediator* mediator;
public:
    Colleague(Mediator* mediator) : mediator(mediator) {}
    virtual void send(const string& message) = 0;
    virtual void receive(const string& message) = 0;
};

// ConcreteColleagueA
class ConcreteColleagueA : public Colleague {
public:
    ConcreteColleagueA(Mediator* mediator) : Colleague(mediator) {}
    
    void send(const string& message) override {
        cout << "Colleague A sends message: " << message << endl;
        mediator->sendMessage(message, this);
    }
    
    void receive(const string& message) override {
        cout << "Colleague A received message: " << message << endl;
    }
};

// ConcreteColleagueB
class ConcreteColleagueB : public Colleague {
public:
    ConcreteColleagueB(Mediator* mediator) : Colleague(mediator) {}
    
    void send(const string& message) override {
        cout << "Colleague B sends message: " << message << endl;
        mediator->sendMessage(message, this);
    }
    
    void receive(const string& message) override {
        cout << "Colleague B received message: " << message << endl;
    }
};

// ConcreteMediator
class ConcreteMediator : public Mediator {
private:
    ConcreteColleagueA* colleagueA;
    ConcreteColleagueB* colleagueB;
public:
    ConcreteMediator(ConcreteColleagueA* colleagueA, ConcreteColleagueB* colleagueB) {
        this->colleagueA = colleagueA;
        this->colleagueB = colleagueB;
    }

    void sendMessage(const string& message, Colleague* colleague) override {
        if (colleague == colleagueA) {
            colleagueB->receive(message);
        } else {
            colleagueA->receive(message);
        }
    }
};
```
---
### Chain of Responsibility: 
- **Intent**: The Chain of Responsibility design pattern allows multiple objects to handle a request without knowing which one will ultimately handle it. The request is passed along the chain of potential handlers until one of them processes it.
- **Structure**:
    - Handler (Abstract): Defines the interface for handling requests and the link to the next handler in the chain.
    - ConcreteHandler: Implements the handling of requests and forwards the request to the next handler if it cannot handle it.
    - Client: Initiates the request and passes it to the first handler in the chain.
- **When to Use**:
    - When you have multiple potential handlers for a request, but you don't know in advance which one will handle it.
    - When you want to decouple the sender and receiver of a request.
    - When you need a flexible system to add new handlers without altering the request processing flow.
```cpp
// Handler (Abstract)
class Handler {
protected:
    Handler* nextHandler;
public:
    Handler() : nextHandler(nullptr) {}
    virtual void handleRequest(int request) = 0;
    void setNextHandler(Handler* handler) {
        nextHandler = handler;
    }
};

// ConcreteHandlerA
class ConcreteHandlerA : public Handler {
public:
    void handleRequest(int request) override {
        if (request >= 0 && request < 10) {
            cout << "Handler A processed the request: " << request << endl;
        } else if (nextHandler) {
            nextHandler->handleRequest(request);
        }
    }
};

// ConcreteHandlerB
class ConcreteHandlerB : public Handler {
public:
    void handleRequest(int request) override {
        if (request >= 10 && request < 20) {
            cout << "Handler B processed the request: " << request << endl;
        } else if (nextHandler) {
            nextHandler->handleRequest(request);
        }
    }
};
```
---
### Interpreter:
- **Intent**: The Interpreter design pattern defines a way to evaluate sentences in a language. It involves defining a grammar for the language and implementing a way to interpret or evaluate statements written in that language. This pattern is often used in compilers or similar systems where complex expressions need to be evaluated.
- **Structure**:
    - AbstractExpression: Declares an abstract method for interpreting the expressions.
    - TerminalExpression: Implements the interpretation for terminal expressions (those that cannot be divided further).
    - NonTerminalExpression: Implements the interpretation for non-terminal expressions (those that can be divided into other expressions).
    - Context: Contains information that's needed for the interpretation process.
    - Client: Invokes the interpreter and provides input to the system.
- **When to Use**:
    - When you have a problem that can be represented as a grammar, and you need to interpret or evaluate expressions based on that grammar.
    - In situations where you need to evaluate mathematical expressions, language processing, or implementing a simple language.
```cpp
// AbstractExpression
class Expression {
public:
    virtual int interpret(map<string, int>& context) = 0;
};

// TerminalExpression
class Number : public Expression {
private:
    int number;
public:
    Number(int num) : number(num) {}
    int interpret(map<string, int>& context) override {
        return number;
    }
};

// NonTerminalExpression
class Add : public Expression {
private:
    Expression* left;
    Expression* right;
public:
    Add(Expression* left, Expression* right) : left(left), right(right) {}
    int interpret(map<string, int>& context) override {
        return left->interpret(context) + right->interpret(context);
    }
};

// NonTerminalExpression
class Subtract : public Expression {
private:
    Expression* left;
    Expression* right;
public:
    Subtract(Expression* left, Expression* right) : left(left), right(right) {}
    int interpret(map<string, int>& context) override {
        return left->interpret(context) - right->interpret(context);
    }
};
```
---
### Memento:
- **Intent**: The Memento design pattern allows an object's state to be captured and restored later without violating encapsulation. It is particularly useful in situations where you need to maintain the history of an object's state for undo/redo functionality, or when you want to capture the state of an object at a certain point in time.
- **Structure**:
    - Memento: Holds the state of the originator object.
    - Originator: The object whose state is being captured. It creates a memento to store its state.
    - Caretaker: Keeps track of the mementos and is responsible for restoring the state of the originator using the memento.
    - Client: Interacts with the Caretaker to request the saving and restoring of mementos.
- **When to Use**:
    - When you need to provide undo/redo capabilities in your application.
    - When you need to capture and restore the state of an object at different points without exposing the internal details.
    - In scenarios where you need to save the state of an object to be restored later without modifying its encapsulation.
```cpp
// Memento
class Memento {
private:
    string state;
public:
    Memento(const string& state) : state(state) {}
    string getState() const { return state; }
};

// Originator
class Originator {
private:
    string state;
public:
    void setState(const string& state) {
        this->state = state;
    }

    string getState() const {
        return state;
    }

    // Create Memento
    Memento* createMemento() {
        return new Memento(state);
    }

    // Restore Memento
    void restoreMemento(Memento* memento) {
        state = memento->getState();
    }
};

// Caretaker
class Caretaker {
private:
    vector<Memento*> mementoList;
public:
    void saveMemento(Memento* memento) {
        mementoList.push_back(memento);
    }

    Memento* getMemento(int index) {
        return mementoList.at(index);
    }

    ~Caretaker() {
        for (Memento* m : mementoList) {
            delete m;
        }
    }
};
```
---
### Template Method:
- **Intent**: The Template Method design pattern defines the skeleton of an algorithm in the superclass but lets subclasses override specific steps of the algorithm without changing its structure. It provides a framework where common functionality is shared, and specific parts can be customized by subclasses.
- **Structure**:
    - AbstractClass: Defines the template method and some basic steps of the algorithm. Some steps are abstract, so subclasses need to provide specific implementations.
    - ConcreteClass: Implements the abstract steps of the algorithm.
    - Client: Uses the TemplateMethod to invoke the algorithm.
- **When to Use**:
    - When you have multiple classes with similar algorithmic steps, but the implementation details vary.
    - When you want to ensure that certain steps in the algorithm are followed, but leave other steps to be implemented by subclasses.
    - When you want to avoid code duplication for common steps across classes.
```cpp
// AbstractClass
class CaffeineBeverage {
public:
    // Template Method
    void prepareRecipe() {
        boilWater();
        brew();
        pourInCup();
        addCondiments();
    }

protected:
    virtual void brew() = 0; // Step to be implemented by subclasses
    virtual void addCondiments() = 0; // Step to be implemented by subclasses

    void boilWater() {
        cout << "Boiling water" << endl;
    }

    void pourInCup() {
        cout << "Pouring into cup" << endl;
    }
};

// ConcreteClass 1
class Tea : public CaffeineBeverage {
protected:
    void brew() override {
        cout << "Steeping the tea" << endl;
    }

    void addCondiments() override {
        cout << "Adding lemon" << endl;
    }
};

// ConcreteClass 2
class Coffee : public CaffeineBeverage {
protected:
    void brew() override {
        cout << "Dripping coffee through filter" << endl;
    }

    void addCondiments() override {
        cout << "Adding sugar and milk" << endl;
    }
};
```
---
### Visitor:
- **Intent**: The Visitor design pattern lets you add further operations to objects without having to modify the objects themselves. This pattern is used to perform operations on elements of an object structure without changing the classes of the elements on which it operates.
- **Structure**:
    - Visitor: Declares a visit method for each type of element in the object structure.
    - ConcreteVisitor: Implements the visit methods for specific types of elements.
    - Element: Defines an accept() method that takes a visitor.
    - ConcreteElement: Implements the accept() method to pass itself to the visitor.
    - ObjectStructure: A collection of elements that can be iterated and processed by the visitor.
- **When to Use**:
    - When you need to perform various unrelated operations across the classes of an object structure.
    - When the classes of the objects in an object structure are stable and you want to avoid modifying them to introduce new functionality.
    - In scenarios where operations need to be performed on elements of an object structure without modifying their classes.
```cpp
// Visitor
class Visitor {
public:
    virtual void visit(class ElementA& element) = 0;
    virtual void visit(class ElementB& element) = 0;
};

// Element
class Element {
public:
    virtual void accept(Visitor& visitor) = 0;
};

// ConcreteElementA
class ElementA : public Element {
public:
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    void operationA() {
        cout << "ElementA operation" << endl;
    }
};

// ConcreteElementB
class ElementB : public Element {
public:
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    void operationB() {
        cout << "ElementB operation" << endl;
    }
};

// ConcreteVisitor
class ConcreteVisitor : public Visitor {
public:
    void visit(ElementA& element) override {
        cout << "Visiting ElementA" << endl;
        element.operationA();  // Perform operation specific to ElementA
    }

    void visit(ElementB& element) override {
        cout << "Visiting ElementB" << endl;
        element.operationB();  // Perform operation specific to ElementB
    }
};

// Traverse the element structure and apply the visitor to each element
```
---