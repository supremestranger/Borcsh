#pragma once

#include <vector>

namespace fef {
    class EcsMask {
    public:
        std::vector<size_t> IncludePools;
        std::vector<size_t> ExcludePools;

        EcsMask(size_t id, bool include) {
            IncludePools = std::vector<size_t>();
            ExcludePools = std::vector<size_t>();
            if (include) {
                IncludePools.push_back(id);
            }
            else {
                ExcludePools.push_back(id);
            }
        }

        EcsMask& Merge(EcsMask other) {
            for (auto id : other.IncludePools) {
                IncludePools.push_back(id);
            }
            for (auto id : other.ExcludePools) {
                ExcludePools.push_back(id);
            }
            return *this;
        }

        EcsMask(const EcsMask& other) : IncludePools(other.IncludePools), ExcludePools(other.ExcludePools) {
            //std::cout << "Move EcsMask" << std::endl;
        }

        EcsMask(EcsMask&& other) : IncludePools(std::move(other.IncludePools)), ExcludePools(std::move(other.ExcludePools)) {
            //std::cout << "Move EcsMask" << std::endl;
        }
    };
}