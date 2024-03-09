#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "EcsMask.hpp"
#include "EcsPool.hpp"
#include "EcsQuery.hpp"

typedef uint32_t Entity;

namespace fef {
    class EcsSystems;
    class EcsWorld {
    public:
        EcsWorld() : _poolMap(), _pools(), _gens() {
            _gens.reserve(128);
        }

        ~EcsWorld() { }

        Entity NewEntity() {
            Entity entity = 0;
            if (!_recycledEntities.empty()) {
                entity = _recycledEntities.back();
                _recycledEntities.pop_back();
            }
            else {
                entity = CreateNewEntity();
            }
            return entity;
        }

        template <typename T>
        EcsPool<T>* GetPool() {
            if (_poolMap.contains(typeid(T))) {
                return static_cast<EcsPool<T> *>(_poolMap.at(typeid(T)));
            }
            auto pool = new EcsPool<T>(_pools.size(), _gens.capacity());
            _poolMap.emplace(typeid(T), pool);
            _pools.push_back(pool);
            pool->World = this;
            return pool;
        }

        template <typename WithT>
        EcsQuery* Query() noexcept {
            WithT inc;
            auto mask = inc.build(this);
            auto queryPools = new std::vector<BaseEcsPool*>();
            for (auto includePool : mask.IncludePools) {
                queryPools->push_back(_pools[includePool]);
            }
            auto query = new EcsQuery(queryPools);
            return query;
        }

        void AddSystems(EcsSystems* systems) {
            _systems.push_back(systems);
        }

        template <typename WithT, typename WithoutT>
        EcsQuery* Query() {
            WithT inc;
            WithoutT exc;
            auto mask = inc.build(this).Merge(exc.build(this));
            auto queryPools = new std::vector<BaseEcsPool*>();
            for (auto includePool : mask.IncludePools) {
                queryPools->push_back(_pools[includePool]);
            }
            auto query = new EcsQuery(queryPools);
            return query;
        }

        template <typename T>
        EcsMask GetMask(bool include = true) {
            auto pool_id = GetPool<T>()->id;
            return std::move(EcsMask(pool_id, include));
        }

        template <typename T1, typename T2, typename... Args>
        EcsMask GetMask(bool include = true) {
            return std::move(GetMask<T1>(include).Merge(GetMask<T2, Args...>(include)));
        }

        void DelEntity(Entity entity) {
            for (auto pool : _pools) {
                if (pool->Has(entity)) {
                    pool->Del(entity);
                }
            }
            _gens[entity] = -(_gens[entity] + 1);
        }

    private:
        std::unordered_map<std::type_index, BaseEcsPool*> _poolMap;
        std::vector<BaseEcsPool*> _pools;
        std::vector<size_t> _recycledEntities;
        std::vector<EcsSystems*> _systems;
        std::vector<int> _gens;

        Entity CreateNewEntity() {
            if (_gens.size() == _gens.capacity()) {
                for (auto pool : _pools) {
                    pool->Indices.resize(_gens.size() * 2);
                }
            }
            auto entity = _gens.size();
            _gens.push_back(1);
            return entity;
        }
    };
}