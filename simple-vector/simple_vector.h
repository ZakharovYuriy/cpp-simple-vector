#pragma once

#include "array_ptr.h"
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <array>
#include <stdexcept>
#include <iterator>

using namespace std;

class ReserveProxyObj {
public:
    ReserveProxyObj(size_t capacity_to_reserve) :capacity_to_reserve_(capacity_to_reserve) {};
    size_t capacity_to_reserve_;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(std::size_t size):mass(size), size_(size), capacity_(size){
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value):size_(size), capacity_(size) {
        ArrayPtr<Type>tmp(size);
        fill(&tmp[0], &tmp[size], value);
        mass.swap(tmp);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) :mass(init.size()), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), begin());
    }

    SimpleVector(const SimpleVector& other) :size_(other.GetSize()), capacity_(other.GetCapacity()) {
        std::copy(other.begin(), other.end(), begin());
    }

    SimpleVector(SimpleVector&& other)noexcept :mass(other.GetSize()), size_(other.GetSize()), capacity_(other.GetCapacity())  {
        copy(make_move_iterator(other.begin()), make_move_iterator(other.end()), begin());
        other.Clear();
    }

    SimpleVector(ReserveProxyObj obj) {
        Reserve(obj.capacity_to_reserve_);
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<Type>tmp(new_capacity);
            copy(begin(), end(), &tmp[0]);
            capacity_ = new_capacity;
            mass.swap(tmp);
        }
    };

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (rhs==*this) {
            return *this;
        }
        else if(rhs.GetSize()==0){
            Clear();
            return *this;
        }
        else {
            ArrayPtr<Type>tmp(rhs);
            mass.swap(tmp);
            return *this;
        }
    }

    SimpleVector& operator=(SimpleVector&& rhs) {
        if (rhs == *this) {
            return *this;
        }
        else if (rhs.GetSize() == 0) {
            Clear();
            return *this;
        }
        else {
            ArrayPtr<Type>tmp(rhs.GetSize());
            copy(make_move_iterator(rhs.begin()), make_move_iterator(rhs.end()), &tmp[0]);
            size_ = rhs.GetSize();
            capacity_ = rhs.GetCapacity();
            mass.swap(tmp);
            rhs.Clear();
            return *this;
        }
    }

    // Добавляет элемент в конец вектора
	// При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (size_ < capacity_) {
            mass[size_] = item;
            ++size_;
            return;
        }
        else if (capacity_ == 0) {
            ArrayPtr<Type>tmp(1);
            copy(begin(), end(), &tmp[0]);
            tmp[size_] = item;
            size_ = 1;
            capacity_ = 1;
            mass.swap(tmp);
        }
        else {
            ArrayPtr<Type>tmp(capacity_ * 2);
            copy(begin(), end(), &tmp[0]);
            tmp[size_] = item;
            ++size_;
            capacity_ *= 2;
            mass.swap(tmp);
        }
    }

    void PushBack(Type&& item) {
        if (size_ < capacity_) {
            mass[size_] = move(item);
            ++size_;
            return;
        }
        else if (capacity_ == 0) {
            ArrayPtr<Type>tmp(1);
            copy(make_move_iterator(begin()), make_move_iterator(end()), &tmp[0]);
            tmp[size_] = move(item);
            size_ = 1;
            capacity_ = 1;
            mass.swap(tmp);
        }
        else {
            ArrayPtr<Type>tmp(capacity_ * 2);
            copy(make_move_iterator(begin()), make_move_iterator(end()), &tmp[0]);
            tmp[size_] = move(item);
            ++size_;
            capacity_ *= 2;
            mass.swap(tmp);
        }
    }

    // Вставляет значение value в позицию pos.
	// Возвращает итератор на вставленное значение
	// Если перед вставкой значения вектор был заполнен полностью,
	// вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        size_t quantity = 0;
        for (auto it = begin(); it != pos; ++it) {
            ++quantity;
        }

        assert(quantity <= GetSize());

        if (size_ < capacity_) {
            size_t i = size_;
            for (auto it = end(); it > &mass[quantity]; --it, --i) {
                mass[i] = mass[i];
            }
            mass[quantity] = value;
            ++size_;
        }
        else if (capacity_ == 0) {
            ArrayPtr<Type>tmp(1);
            tmp[0] = value;
            size_ = 1;
            capacity_ = 1;
            mass.swap(tmp);
        }
        else {
            ArrayPtr<Type>tmp(capacity_ * 2);
            copy(begin(), &mass[quantity], &tmp[0]);
            copy(&mass[quantity], end(), &tmp[quantity + 1]);
            tmp[quantity] = value;
            mass.swap(tmp);
            ++size_;
            capacity_ = capacity_ * 2;
        }
        return &mass[quantity];
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        size_t quantity = 0;
        for (auto it = begin(); it != pos; ++it) {
            ++quantity;
        }

        assert(quantity <= GetSize());

        if (size_ < capacity_) {
            size_t i = size_;
            for (auto it = end(); it > &mass[quantity]; --it, --i) {
                mass[i] = move(mass[i]);
            }
            mass[quantity] = move(value);
            ++size_;
        }
        else if (capacity_ == 0) {
            ArrayPtr<Type>tmp(1);
            tmp[0] = move(value);
            size_ = 1;
            capacity_ = 1;
            mass.swap(tmp);
        }
        else {
            ArrayPtr<Type>tmp(capacity_ * 2);
            copy(make_move_iterator(begin()), make_move_iterator (&mass[quantity]), &tmp[0]);
            copy(make_move_iterator (&mass[quantity]), make_move_iterator(end()), &tmp[quantity + 1]);
            tmp[quantity] = move(value);
            mass.swap(tmp);
            ++size_;
            capacity_ = capacity_ * 2;
        }
        return &mass[quantity];
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(size_ != 0);
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        size_t quantity = 0;
        for (auto it = begin(); it != pos; ++it) {
            ++quantity;
        }

        assert(quantity <= GetSize());

        size_t i = quantity + 1;
        for (auto it = &mass[quantity + 1]; it < end(); ++it, ++i) {
            mass[i - 1] = move(mass[i]);
        }
        --size_;
        return &mass[quantity];
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        size_t size = size_;
        size_t capacity = capacity_;
        mass.swap(other.mass);

        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }


    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index<=size_);
        return mass[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index <= size_);
        return mass[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("out_of_range At "+ index);
        }
        return mass[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("out_of_range At " + index);
        }
        return mass[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
	// При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size > size_) {
            ArrayPtr<Type>tmp(new_size);
            for (size_t i = 0; i < size_; ++i) {
                tmp[i] = mass[i];
            }
            size_ = new_size;
            capacity_ = new_size;
            mass.swap(tmp);
        }
        else {
            size_ = new_size;
        }
    }

    // Возвращает итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return &mass[0];
    }

    // Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return &mass[size_];
    }

    // Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return &mass[0];
    }

    // Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return &mass[size_];
    }

    // Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return &mass[0];
    }

    // Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return &mass[size_];
    }

private:
    ArrayPtr <Type> mass;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() == 0 && rhs.GetSize() == 0) {
        return true;
    }
    else if (lhs.GetSize() != 0 && rhs.GetSize() != 0) {
        return equal(lhs.begin(), lhs.end(), rhs.begin());
    }
    return false;
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() == 0 && rhs.GetSize() != 0) {
        return true;
    }
    else if (lhs.GetSize() != 0 && rhs.GetSize() = 0) {
        return false;
    }
    else{
        return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs>rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() == 0 && rhs.GetSize() != 0) {
        return false;
    }
    else if (lhs.GetSize() != 0 && rhs.GetSize() = 0) {
        return true;
    }
    else {
        return !lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) & !equal(lhs.begin(), lhs.end(), rhs.begin());
    }
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}

