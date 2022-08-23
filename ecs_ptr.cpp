#include <iostream>
template <typename T>
class ecs_ptr
{
private:
    struct Management
    {
        bool is_alive = true;
        int reference_count = 1;
    };
    Management * manager;
    T * pointer;
public:
    T * get() { return pointer; }
    bool is_alive() { return manager->is_alive; }
    void kill() 
    { 
        delete pointer;
        manager->is_alive = false;

    }
    ecs_ptr create_reference()
    {
        manager->reference_count++;
        return *this;
    }
    ecs_ptr(const T & arg) 
    { 
        pointer = new T(arg);
        manager = new Management();
    }
    ecs_ptr() 
    { 
        pointer = new T;
        manager = new Management;
    }
    ~ecs_ptr()
    {
        manager->reference_count--;
        if (manager->reference_count <= 0)
        {
            if (manager->is_alive) delete pointer;
            delete manager;
            std::cout << "Deleting Manager." << std::endl;
        }
    }
};
struct Player { double health; double location[2]; };
int main()
{
    {
        ecs_ptr<Player> ptr1 = ecs_ptr<Player>(Player{100.0, {0.0, 0.0}});
        ecs_ptr<Player> ptr2 = ptr1.create_reference();
        std::cout << "Alive? ptr1: " << ptr1.is_alive() << ", ptr2" << ptr2.is_alive() << std::endl;
        std::cout << "ptr1: " << ptr1.get()->health << "hp, (" << ptr1.get()->location[0] << ", "
            << ptr1.get()->location[1] << ")" << std::endl;
        std::cout << "ptr2: " << ptr2.get()->health << "hp, (" << ptr2.get()->location[0] 
            << ", " << ptr2.get()->location[1] << ")"<< std::endl;
        ptr1.kill();    
        std::cout << "Killed. Alive? " << ptr1.is_alive() << ", " << ptr2.is_alive() << std::endl;
    }
    return 0;
}
