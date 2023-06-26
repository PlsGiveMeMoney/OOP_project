#include <iostream>
#include <vector>

template <typename T>
class MyContainer {
    T* data;
    size_t container_size;
    size_t container_capacity;

    void ensure_capacity(size_t new_size) {
        if (new_size > container_capacity) {
            container_capacity = container_capacity + container_capacity / 2 + 1;
            T* new_data = new T[container_capacity];
            for (size_t i = 0; i < container_size; ++i) {
                new_data[i] = std::move(data[i]);
            }
            delete[] data;
            data = new_data;
        }
    }
    class Iterator {
        T* ptr;

    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        Iterator() : ptr(nullptr) {}
        explicit Iterator(pointer p) : ptr(p) {}

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }
        reference operator[](difference_type n) const { return *(ptr + n); }

        Iterator& operator++() { ++ptr; return *this; }
        Iterator& operator--() { --ptr; return *this; }
        Iterator operator++(int) { Iterator tmp(*this); ++ptr; return tmp; }
        Iterator operator--(int) { Iterator tmp(*this); --ptr; return tmp; }

        Iterator& operator+=(difference_type n) { ptr += n; return *this; }
        Iterator& operator-=(difference_type n) { ptr -= n; return *this; }

        friend Iterator operator+(const Iterator& it, difference_type n) { return Iterator(it.ptr + n); }
        friend Iterator operator+(difference_type n, const Iterator& it) { return it + n; }
        friend Iterator operator-(const Iterator& it, difference_type n) { return Iterator(it.ptr - n); }
        friend Iterator operator-(const Iterator& it1, const Iterator& it2) { return Iterator(it1.ptr - it2.ptr); }

        friend bool operator==(const Iterator& it1, const Iterator& it2) { return it1.ptr == it2.ptr; }
        friend bool operator!=(const Iterator& it1, const Iterator& it2) { return !(it1 == it2); }
        friend bool operator<(const Iterator& it1, const Iterator& it2) { return it1.ptr < it2.ptr; }
        friend bool operator>(const Iterator& it1, const Iterator& it2) { return it2 < it1; }
        friend bool operator<=(const Iterator& it1, const Iterator& it2) { return !(it2 < it1); }
        friend bool operator>=(const Iterator& it1, const Iterator& it2) { return !(it1 < it2); }
    };

public:
    MyContainer()
        : data(nullptr), container_size(0), container_capacity(0)
    {}
    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + container_size); }
    const Iterator begin() const { return Iterator(data); }
    const Iterator end() const { return Iterator(data + container_size); }

    // Copy constructor
    MyContainer(const MyContainer<T>& other)
        : data(new T[other.container_size]),
        container_size(other.container_size),
        container_capacity(other.container_capacity)
    {
        std::copy(other.data, other.data + other.container_size, data);
    }

    // Move constructor
    MyContainer(MyContainer<T>&& other)
        : data(other.data),
        container_size(other.container_size),
        container_capacity(other.container_capacity)
    {
        other.data = nullptr;
        other.container_size = 0;
        other.container_capacity = 0;
    }

    // Copy assignment operator
    MyContainer<T>& operator=(const MyContainer<T>& other) {
        if (this != &other) {
            MyContainer<T> tmp(other);
            std::swap(this->data, tmp.data);
            std::swap(this->container_size, tmp.container_size);
            std::swap(this->container_capacity, tmp.container_capacity);
        }
        return *this;
    }

    // Move assignment operator
    MyContainer<T>& operator=(MyContainer<T>&& other) {
        std::swap(this->data, other.data);
        std::swap(this->container_size, other.container_size);
        std::swap(this->container_capacity, other.container_capacity);
        return *this;
    }

    ~MyContainer() {
        delete[] data;
    }

    size_t size() const {
        return container_size;
    }

    bool empty() const {
        return container_size == 0;
    }

    void push_back(const T& value) {
        ensure_capacity(container_size + 1);
        data[container_size++] = value;
    }

    void push_back(T&& value) {
        ensure_capacity(container_size + 1);
        data[container_size++] = std::move(value);
    }

    void erase(size_t index) {
        if (index >= container_size) {
            throw std::out_of_range("Index is out of range");
        }
        for (size_t i = index + 1; i < container_size; ++i) {
            data[i - 1] = std::move(data[i]);
        }
        --container_size;
    }
};




class Device {
public:
    bool isTurn = false;
    int volume = 0;
    int screenBrightness = 0;

    void TurnOnOff(bool& isTurn) {
        isTurn = !isTurn;
    }
    void ChangeVolume(int& volume, int volumeChange) {
        volume += volumeChange;
    }
private:
    int seria;
};

class Computer : public Device {
public:
    int cost;
    void TurnOnOffComputer(bool& isTurn) {
        TurnOnOff(isTurn);
        if (isTurn) {
            std::cout << "Computer is on!\n";
        }
        else std::cout << "Computer is off :(\n";
    }
    void ShowVolume(int volume) {
        std::cout << volume;
    }
    friend void ChangeScreenBrightness(int& screenBrightness, int change);

private:
    int size[3];
};

bool operator < (Computer comp1, Computer comp2)
{
    return comp1.cost < comp2.cost;
}

void ChangeScreenBrightness(int& screenBrightness, int change) {
    screenBrightness += change;
}

class Laptop {
public:
    bool isFold = true;
    friend Computer;
    void UnFoldLaptop(bool& isFold) {
        isFold = !isFold;

    }
};
class Devices {
public:
    MyContainer<Computer> computers;
    MyContainer<Laptop> laptops;

    size_t TotalNumberOfDevices() const {
        return computers.size() + laptops.size();
    }

};




int main() {


    Computer comp1;
    Computer comp2;
    Devices AllDevices;
    comp1.cost = 100;
    comp2.cost = 800;
    comp1.volume = 10;
    comp2.volume = 20;
    comp1.screenBrightness = 100;
    comp2.screenBrightness = 80;
    AllDevices.computers.push_back(comp1);
    AllDevices.computers.push_back(comp2);
    int a;
    a = AllDevices.TotalNumberOfDevices();
    std::cout << a << std::endl;

    for (auto it = AllDevices.computers.begin(); it != AllDevices.computers.end(); ++it) {
        std::cout << it->cost << "  " << it->volume << "  " << it->screenBrightness << std::endl;
    }

    return 0;
}
