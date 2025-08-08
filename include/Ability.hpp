#pragma once
#include <utility>
#include <string>

class Ability {
public:
    explicit Ability(std::string name) : name_(std::move(name)) {}
    virtual ~Ability() = default;
    const std::string& name() const { return name_; }
    virtual void use() {}
private:
    std::string name_;
};
