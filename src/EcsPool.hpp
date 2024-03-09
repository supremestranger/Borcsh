#pragma once

#include <cassert>
#include <type_traits>
#include <vector>

typedef uint32_t Entity;

namespace fef {
    class EcsWorld;

    template <typename T>
    class BaseAutoReseter {
    public:
        void virtual auto_reset(T& arg) = 0;
    };

    struct DelayedRemove {
        size_t entity;
    };

    class BaseEcsPool {
    public:
        std::vector<uint32_t> Indices;
        std::vector<uint32_t> Entities;
        std::vector<DelayedRemove> delayed_ops;
        size_t id;
        uint32_t Size;

        EcsWorld *World;

        bool Has(size_t entity) { return Indices[entity] != 0; }
        virtual void Del(Entity entity) = 0;

    protected:
        size_t _locksCount;
    };

    template <typename T>
    class EcsPool : public BaseEcsPool {
    public:
        std::vector<T> Components;

        EcsPool(size_t pool_id, size_t cap) {
            id = pool_id;
            Indices.resize(cap);
            Entities.resize(128);
            Components.resize(128);
            Size = 0;
        }

        T& Add(size_t entity) noexcept {
            assert(entity < Indices.size());

            if (Components.size() == Size) {
                Components.resize(Size << 1);
                Entities.resize(Size << 1);
            }

            Components[Size] = T{};
            Entities[Size] = entity;
            Size++;

            Indices[entity] = Size;

            if constexpr (std::is_base_of_v<BaseAutoReseter<T>, T>) {
                Components[Size - 1].auto_reset(Components[Size - 1]);
            }
            return Components[Size - 1];
        }

        void AddBlocker() {
            _locksCount++;
        }

        void DelBlocker() {
            _locksCount--;
            if (_locksCount == 0) {
                ApplyDelayedOperations();
            }
        }

        void ApplyDelayedOperations() {

        }

        void Del(Entity entity) noexcept {
            assert(entity < Indices.size());

            //if (_locksCount > 0) {
            //    delayed_ops.push_back(DelayedRemove{ entity });
            //    return;
            //}
            auto idx = Indices[entity];
            if (idx > 0) {
                if constexpr (std::is_base_of_v<BaseAutoReseter<T>, T>) {
                    Components[idx - 1].auto_reset(Components[idx - 1]);
                }
                else {
                    Components[idx - 1] = T{};
                }
                Indices[entity] = 0;
                Entities[idx - 1] = 0;
                if (idx < Size - 1) {
                    std::swap(Components[idx - 1], Components[Size - 1]);
                    std::swap(Entities[idx - 1], Entities[Size - 1]);
                }
                Size--;
            }
        }

        T& GetMut(size_t entity) noexcept { return Components[Indices[entity] - 1]; }
        const T& Get(size_t entity) noexcept { return Components[Indices[entity] - 1]; }
    };
}