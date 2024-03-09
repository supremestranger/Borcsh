#include "EcsWorld.hpp"

namespace fef {
    template <typename T1, typename... Args>
    struct With {
        EcsMask build(EcsWorld* world) {
            return world->GetMask<T1, Args...>();
        }
    };

    template <typename T1, typename T2, typename... Args>
    struct Without {
        EcsMask build(EcsWorld* world) {
            return world->GetMask<T1, T2, Args...>(false);
        }
    };
}
