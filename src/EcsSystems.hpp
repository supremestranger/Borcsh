#pragma once

#include <vector>

#include "EcsWorld.hpp"

namespace fef {
    struct EcsInitSystem;
    struct EcsRunSystem;
    struct EcsDestroySystem;

    struct EcsSystem {
        virtual EcsInitSystem* AsInitSystem() { return nullptr; }
        virtual EcsRunSystem* AsRunSystem() { return nullptr; }
        virtual EcsDestroySystem* AsDestroySystem() { return nullptr; }
    };

    struct EcsRunSystem : public EcsSystem {
        virtual void Run() = 0;
        EcsRunSystem* AsRunSystem() { return this; }
    };

    struct EcsInitSystem : public EcsSystem {
        virtual void Init() = 0;
        EcsInitSystem* AsInitSystem() { return this; }
    };

    struct EcsDestroySystem : EcsSystem {
        virtual void Destroy() = 0;
        EcsDestroySystem* AsDestroySystem() { return this; }
    };

    struct EcsSystems {
    private:
        std::vector<EcsInitSystem*> _initSystems;
        std::vector<EcsRunSystem*> _runSystems;
        std::vector<EcsDestroySystem*> _destroySystems;
    public:
        EcsSystems(EcsWorld* world) {
            world->AddSystems(this);
        }

        EcsSystems* Add(EcsSystem* system) {
            auto initSystem = system->AsInitSystem();
            if (initSystem != nullptr) {
                _initSystems.push_back(initSystem);
            }

            auto runSystem = system->AsRunSystem();
            if (runSystem != nullptr) {
                _runSystems.push_back(runSystem);
            }

            auto destroySystem = system->AsDestroySystem();
            if (destroySystem != nullptr) {
                _destroySystems.push_back(destroySystem);
            }
            return this;
        }

        void Init() {
            for (auto system : _initSystems) {
                system->Init();
            }
        }

        void Run() {
            for (auto system : _runSystems) {
                system->Run();
            }
        }
    };
}
