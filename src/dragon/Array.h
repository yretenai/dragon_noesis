//
// Created by yretenai on 6/5/2020.
//

#ifndef DRAGON_ARRAY_H
#define DRAGON_ARRAY_H

#include <algorithm>
#include <memory>
#include <vector>

#ifdef _WIN32
#ifdef USE_NOESIS
#include "../noesis/pluginshare.h"
#endif
#endif

namespace dragon {
    class out_of_bounds_exception : public std::exception {};

    template <typename T> class Array {
      private:
        size_t Length;
        size_t Offset;

      protected:
        std::shared_ptr<T[]> Pointer;

      public:
        class Iterator {
          private:
            const Array<T>* Parent;
            size_t Index;

          public:
            Iterator() {}

            Iterator(const Array<T>* parent, size_t index = 0) {
                Parent = parent;
                Index = index;
            }

            bool operator==(const Iterator& rhs) const {
                return Parent->Pointer == rhs.Parent->Pointer && Index == rhs.Index && Parent->size() == rhs.Parent->size() &&
                       Parent->Offset == rhs.Parent->Offset;
            }

            bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

            T operator*() const { return Parent->get(Index); }

            Iterator& operator++() {
                if (Index >= Parent->size()) {
                    throw out_of_bounds_exception();
                }
                Index++;
                return *this;
            }

            Iterator operator++(int) {
                Iterator temp(*this);
                ++*this;
                return temp;
            }
        };

        Array() : Array(0) {}

        Array(T* buffer, size_t size) : Array(size) { std::copy_n(buffer, Length, Pointer.get()); }

        Array(size_t size) {
            Pointer = std::shared_ptr<T[]>(new T[size]);
            for (size_t i = 0; i < size; ++i)
                Pointer[i] = T();
            Offset = 0;
            Length = size;
        }

        Array(Array<T>* parent, size_t offset) {
            Pointer = parent->Pointer;
            Length = parent->Length;
            Offset = offset;
        }

        template <typename U> static Array<T> ptr_cast(U* buffer, size_t size) {
            return Array<T>(reinterpret_cast<T*>(buffer), size * sizeof(U) / sizeof(T));
        }

        T& operator[](int index) const { return get(index); }

        T& get(int index) const {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return data()[index];
        }

        void set(int index, T value) const {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            data()[index] = value;
        }

        template <typename U> U cast(int index) {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return reinterpret_cast<U*>(data() + index)[0];
        }

        template <typename U> U lpcast(int* index) {
            U tmp = cast<U>(*index);
            *index += sizeof(U) / sizeof(T);
            return tmp;
        }

        template <typename U> Array<U> cast(int index, int size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<U>(reinterpret_cast<U*>(data() + index), size);
        }

        template <typename U> Array<U> lpcast(int* index, int size) {
            Array<U> tmp = cast<U>(*index, size);
            (*index) += size * sizeof(U) / sizeof(T);
            return tmp;
        }

        Array<T> slice(int index, int size) {
            if (index < 0 || index >= this->size() || size < 0 || index + size > this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<T>((data() + index), size);
        }

        Array<T> shift(int index) {
            if (index < 0 || index >= this->size()) {
                throw out_of_bounds_exception();
            }
            return Array<T>(this, index);
        }

        Array<T> lpslice(int* index, int size) {
            Array<T> tmp = slice(*index, size);
            (*index) += size;
            return tmp;
        }

        std::vector<T> const to_vector() { return std::vector<T>(data(), data() + size()); }

        T* data() const { return Pointer.get() + Offset; }

        std::shared_ptr<T[]> pointer() const { return Pointer; }

        size_t size() const { return Length - Offset; }

        size_t byte_size() const { return size() * sizeof(T); }

        bool empty() const { return this->size() <= 0; }

        Iterator begin() const { return Iterator(this, 0); }

        Iterator end() const { return Iterator(this, size()); }

#if USE_NOESIS
        T* to_noesis(noeRAPI_t* rapi) {
            T* buffer = (T*)rapi->Noesis_UnpooledAlloc(byte_size());
            std::copy_n(begin(), size(), buffer);
            return buffer;
        }
#endif
    };
} // namespace dragon

#endif // DRAGON_ARRAY_H
