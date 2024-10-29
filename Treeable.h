#ifndef TREEABLE_H
#define TREEABLE_H

#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <string>
#include <stdexcept>

using E = std::variant<int, double, std::string>;
bool areEqual(const E& lhs, const E& rhs){
    return std::visit(
        [](const auto& a, const auto& b) -> bool{
            using AType = std::decay_t<decltype(a)>;
            using BType = std::decay_t<decltype(b)>;
            if constexpr (std::is_same_v<AType, BType>){
                return a == b;
            }else{return false;}
        },
        lhs, rhs);
}
std::string variantToString(const E& value){
    return std::visit(
        [](const auto& v) -> std::string {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::string>){
                return v;
            } else if constexpr (std::is_same_v<std::decay_t<decltype(v)>, int>){
                return std::to_string(v);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(v)>, double>){
                return std::to_string(v);
            } else{
                return "Unsupported type";
            }
        },
        value);
}

template <typename E>
class Treeable {
public:
    //static_assert(std::is_same<decltype(std::declval<E>() < std::declval<E>()), bool>::value, "E must be comparable");

    virtual ~Treeable() = default;

    virtual E getValue() const = 0;
    virtual std::shared_ptr<Treeable<E>> getLeft() const = 0;
    virtual std::shared_ptr<Treeable<E>> getRight() const = 0;

    virtual void setValue(const E& value) = 0;
    virtual void setLeft(std::shared_ptr<Treeable<E>> left) = 0;
    virtual void setRight(std::shared_ptr<Treeable<E>> right) = 0;

    virtual bool equals(const std::shared_ptr<Treeable<E>>& node) const = 0;
    virtual int compareTo(const std::shared_ptr<Treeable<E>>& node) const = 0;
};

#endif