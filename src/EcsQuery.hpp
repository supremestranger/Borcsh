#pragma once

#include <iostream>
#include <iterator>
#include <vector>

#include "EcsPool.hpp"

namespace fef {
    class EcsWorld;
    class BaseEcsPool;

    struct QueryIterator {
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = uint32_t;
        using pointer = uint32_t*;
        using reference = uint32_t&;

        pointer ptr;
        pointer beginPtr;
        std::vector<BaseEcsPool*>* pools;
        BaseEcsPool* smallestPool;

        QueryIterator(std::vector<BaseEcsPool*>* pools, bool end) : pools(pools) {
            uint32_t min = SIZE_MAX;
            for (auto pool : *pools) {
                if (auto size = pool->Len; size < min) {
                    min = size;
                    smallestPool = pool;
                }
            }

            beginPtr = &smallestPool->Entities[0] - 1;
            this->ptr = end || smallestPool->Len == 0 ? beginPtr : &smallestPool->Entities[smallestPool->Len - 1]; // if this is "end" iterator or there are no entities, then it must be first entity
        }

        reference operator*() const noexcept { return *ptr; }

        pointer operator->() noexcept { return ptr; }

        QueryIterator& operator++() noexcept {
            while (true) {
                if (ptr <= beginPtr) {
                    return *this;
                }
                bool ok = true;
                for (auto pool : *pools) {
                    if (pool == smallestPool) {
                        continue;
                    }
                    if (pool->Has(*ptr)) {
                        ok = false;
                        break;
                    }
                }
                ptr--;
                if (!ok) {
                    continue;
                }
                return *this;
            }
        }

        QueryIterator operator++(int) noexcept {
            QueryIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const QueryIterator& a, const QueryIterator& b) noexcept {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(const QueryIterator& a, const QueryIterator& b) noexcept {
            return a.ptr != b.ptr;
        }

        QueryIterator begin() {
            return *this;
        }

        ~QueryIterator() {

        }
    };

    //int locks;

    class EcsQuery {
    public:
        std::vector<BaseEcsPool*>* poolsInc;
        std::vector<BaseEcsPool*>* poolsExc;

        EcsQuery(std::vector<BaseEcsPool*>* pools) : poolsInc(pools) {

        }

        ~EcsQuery() {
            delete poolsInc;
            //delete poolsExc;
        }

        QueryIterator begin() {
            return QueryIterator(poolsInc, false);
        }

        QueryIterator end() noexcept {
            return QueryIterator(poolsInc, true);
        }
    };
}
