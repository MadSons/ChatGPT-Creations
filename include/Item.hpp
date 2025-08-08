#pragma once
#include <string>
#include <utility>

class Item {
public:
    explicit Item(std::string name) : name_(std::move(name)) {}
    virtual ~Item() = default;
    const std::string& name() const { return name_; }
    virtual void use() {}
private:
    std::string name_;
};
