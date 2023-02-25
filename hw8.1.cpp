
#include<string>
#include<memory>
#include<iostream>
using namespace std;

//// Familiar OO Version, more dynamic, runtime polymorphism
struct AnimalClass {
    virtual string name() = 0;
    virtual string eats() = 0;
};

class CatClass: public AnimalClass {
    string name() override { return "cat"s; }
    string eats() override { return "delicious mice"s; }
};


//// Concept version like normal programming, more efficient, Compile-time polymorphism
template<typename T>
concept AnimalConcept = requires(T a) {
    { a.eats() } -> convertible_to<string>;
    { a.name() } -> convertible_to<string>;
};

struct CatConcept {
    string eats() { return "delicious mice"; }
    string name() { return "cat"; }
};


//// static polymorphism
template <class T>
struct Animal {
    string eats() { return static_cast<T*>(this)->eats();}
    string name() { return static_cast<T*>(this)->name();
    }
};

struct Cat : Animal<Cat> {
    string eats() { return "delicious mice"; }
    string name() { return "cat"; }
};

struct Dog : Animal<Dog> {
    string eats() { return string("meat and bones"); }
    string name() { return string("dog"); }
};




int main() {
    unique_ptr<AnimalClass> cat_class = make_unique<CatClass>();
    cout << "A "s << cat_class->name() << " eats "s << cat_class->eats() << endl;

    AnimalConcept auto cat_concept = CatConcept();
    cout << "A "s << cat_concept.name() << " eats "s << cat_concept.eats() << endl;

    unique_ptr<Animal<Cat>> cat_static_poly = make_unique<Cat>();
    cout << "A "s << cat_static_poly->name() << " eats "s << cat_static_poly->eats() << endl;
}


//What is your opinion of this approach ? Do the SOLID principles shed any light on that ?
//single responsibility check
//open/closed principle check, open for extension, closed for modification
//liskov substitution principle
//interface segregation principle
//dependency inversion principle

/*
* Pros
1. Follows single responsibility principle, each struct is it's smallest single concern piece
2. Follows open closed principle, in that it's somewhat extensible and it's closed for modification. However, static polymorphism is not more extensible than inheritance/OO
* 
* Cons
1. There is no way to make sure that struct Cat and Dog must have eats() and name() methods implemented, like in concept. Therefore not following Liskov substitution principle
2. Interface Segregation Principle is followed.........
3. Dependency Inversion Principle is followed, since the template struct Animal dependency is not concrete. Same as concept and OO method

However, static polymorphism is more performant than inheritance since it is known at compile time, same as concepts.
however, it requires users to know at compile time what partial specialization is required, so it is much less dynamic than Inheritance/OO, same as concepts.
But comapred to concepts, which creates objects on stack, static polymorphism allocates memory for objects at compile time, so less efficent than concepts in that regard



*/