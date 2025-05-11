#pragma once
#include <string>
namespace ex3 {
    class RoleStrategy {
    public:
        virtual ~RoleStrategy() = default;
        virtual void gather() = 0;
        virtual void tax() = 0;
        virtual void bribe() = 0;
        virtual void arrest() = 0;
        virtual void sanction() = 0;
        virtual void coup() = 0;
        virtual std::string name() const = 0;
    };
}