//
// Created by yretenai on 6/5/2020.
//

#ifndef FMT_DRAGON_ARRAY_H
#define FMT_DRAGON_ARRAY_H

#include <algorithm>
#include <memory>

#ifdef WIN32
#ifdef USE_NOESIS
#include "../noesis/pluginshare.h"
#endif
#endif

namespace dragon {
    template <typename T> class Array {
      private:
        size_t Length;

      protected:
        std::unique_ptr<T[]> Pointer;

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
                return Parent->Pointer == rhs.Parent->Pointer && Index == rhs.Index && Parent->size() == rhs.Parent->size();
            }

            bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

            T operator*() const { return Parent->get(Index); }

            Iterator& operator++() {
                if (Index >= Parent->size()) {
                    throw std::exception();
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
            Pointer = std::unique_ptr<T[]>(new T[size]);
            for (size_t i = 0; i < size; ++i)
                Pointer[i] = T();
            Length = size;
        }

        template <typename U> static Array<T> cast(U* buffer, size_t size) {
            return Array<T>(reinterpret_cast<T*>(buffer), size * sizeof(U) / sizeof(T));
        }

        T& operator[](int index) const { return get(index); }

        T& get(int index) const {
            if (index < 0 || index >= Length) {
                throw std::exception();
            }
            return Pointer.get()[index];
        }

        template <typename U> U cast(int index) {
            if (index < 0 || index >= Length) {
                throw std::exception();
            }
            return reinterpret_cast<U*>(Pointer.get() + index)[0];
        }

        template <typename U> U lpcast(int* index) {
            U tmp = cast<U>(*index);
            *index += sizeof(U) / sizeof(T);
            return tmp;
        }

        template <typename U> Array<U> cast(int index, int size) {
            if (index < 0 || index >= Length || size < 0 || index + size > Length) {
                throw std::exception();
            }
            return Array<U>(reinterpret_cast<U*>(Pointer.get() + index), size);
        }

        template <typename U> Array<U> lpcast(int* index, int size) {
            Array<U> tmp = cast<U>(*index, size);
            (*index) += size * sizeof(U) / sizeof(T);
            return tmp;
        }

        Array<T> slice(int index, int size) {
            if (index < 0 || index >= Length || size < 0 || index + size > Length) {
                throw std::exception();
            }
            return Array<T>((Pointer.get() + index), size);
        }

        Array<T> lpslice(int* index, int size) {
            Array<T> tmp = slice(*index, size);
            (*index) += size;
            return tmp;
        }

        T* data() const { return Pointer.get(); }

        size_t size() const { return Length; }

        bool empty() const { return Length == 0; }

        Iterator begin() const { return Iterator(this, 0); }

        Iterator end() const { return Iterator(this, Length); }

#if USE_NOESIS
        T* to_noesis(noeRAPI_s* rapi) {
            T* buffer = (T*)rapi->Noesis_UnpooledAlloc(sizeof(T) * Length);
            std::copy_n(begin(), Length, buffer);
            return buffer;
        }
#endif
    };
} // namespace dragon

#endif // FMT_DRAGON_ARRAY_H
