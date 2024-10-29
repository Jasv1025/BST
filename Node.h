#ifndef NODE_H
#define NODE_H

#include "Treeable.h"

class Node : public Treeable<E>{
    private:
        E data;
        std::shared_ptr<Treeable<E>> left;
        std::shared_ptr<Treeable<E>> right;
    public:
        Node(E value){
            data = value; 
            left = nullptr;
            right = nullptr;
        }

        E getValue() const override{return data;}
        std::shared_ptr<Treeable<E>> getLeft() const override{return left;}
        std::shared_ptr<Treeable<E>> getRight() const override{return right;}

        void setValue(const E& value) override{this->data = value;}
        void setLeft(std::shared_ptr<Treeable<E>> left) override{this->left = std::move(left);}
        void setRight(std::shared_ptr<Treeable<E>> right) override{this->right = std::move(right);}

        bool equals(const std::shared_ptr<Treeable<E>>& node) const override{
            if(compareTo(node) == 0){
                return true;
            }return false;
        }
        int compareTo(const std::shared_ptr<Treeable<E>>& node) const override {
            if (!node) throw std::invalid_argument("Cannot compare to a null node.");

            return std::visit(
                [](auto&& a, auto&& b) -> int {
                    using AType = std::decay_t<decltype(a)>;
                    using BType = std::decay_t<decltype(b)>;
                    if constexpr (std::is_same_v<AType, BType>) {
                        if (a < b) return -1;
                        if (a > b) return 1;
                        return 0;
                    } else {
                        throw std::invalid_argument("Incompatible types for comparison");
                    }
                },
                data, node->getValue());
    }
};

#endif