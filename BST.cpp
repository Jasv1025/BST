#include <algorithm> 
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <climits>
#include <cmath>
#include "Node.h"

class BST{
    private:
        std::shared_ptr<Treeable<E>> root;
    public:
        BST(){root = nullptr;}
        BST(const E& value){root = std::make_shared<Treeable<E>>(value);}
        
        /*
        *Used to add intergers from a file to the tree
        *Intergers can be seperated by spaces or newlines, commas will be ignored
        */
        BST(std::string filename){
            std::vector<int> numbers = readIntegersFromFile(filename);
            for(int num : numbers) insert(num);
        }

        /*
        *Used to add values to the tree
        */
        void insert(const E& value){
            std::shared_ptr<Treeable<E>> current = root;
            if(root == nullptr){
                root = std::make_shared<Treeable<E>>(value);
                return;
            }

            while(true){
                if(value < current->getValue()){
                    if(current->getLeft() == nullptr){
                        current->setLeft(std::make_shared<Node>(value));
                        break;
                    }current = current->getLeft();
                }else{
                    if(current->getRight() == nullptr){
                        current->setRight(std::make_shared<Node>(value));
                        break;
                    }current = current->getRight();
                }
            }
        }

        /*
        *Used to remove values from the tree
        */
        void remove(const E& value){
            std::shared_ptr<Treeable<E>> toDelete = findNode(value);
            if(toDelete == nullptr){
                std::cout << "Node not in tree" << std::endl;
                return;
            }

            std::shared_ptr<Treeable<E>> parent = parentOf(value);
            std::shared_ptr<Treeable<E>> left = toDelete->getLeft();
            std::shared_ptr<Treeable<E>> right = toDelete->getRight();
            int loc = -2;
            if(parent != nullptr){loc = toDelete->compareTo(parent);}

            if(left == nullptr && right == nullptr){
                if(loc == -2){
                    root = nullptr;
                }else if(loc == -1){
                    parent->setLeft(nullptr);
                }else{parent->setRight(nullptr);}
            }else if(left == nullptr || right == nullptr){
                if(left != nullptr){
                    if(loc == -2){
                        root = left;
                    }else if(loc == -1){
                        parent->setLeft(left);
                    }else{
                        parent->setRight(left);
                    }
                }else{
                    if(loc == -2){
                        root = right;
                    }else if(loc == -1){
                        parent->setLeft(right);
                    }else{
                        parent->setRight(right);
                    }
                }
            }else{
                std::shared_ptr<Treeable<E>> minParent = right;
                while(minParent->getLeft()->getLeft() != nullptr){minParent = minParent->getLeft();}
                std::shared_ptr<Treeable<E>> min = minParent->getLeft();
                minParent->setLeft(nullptr);

                min->setLeft(left);
                min->setRight(right);

                if(loc == -2){
                    root = min;
                }else if(loc == -1){
                    parent->setLeft(min);
                }else{
                    parent->setRight(min);
                }
            }
        }

        int height(){return calculateLevels(root);}
        std::shared_ptr<Treeable<E>> getRoot(){return root;}

        bool contains(const E& value){
            if(findNode(value)){
                return true;
            }return false;
        }

        std::vector<E> inOrderList(){
            std::vector<E> nodeList;
            preOrderRunner(root, nodeList);
            return nodeList;
        }

        std::vector<E> preOrderList(){
            std::vector<E> nodeList;
            preOrderRunner(root, nodeList);
            return nodeList;
        }

        std::vector<E> postOrderList(){
            std::vector<E> nodeList;
            postOrderRunner(root, nodeList);
            return nodeList;
        }

        std::vector<E> levelOrderList(){
            std::vector<E> nodeList;
            int levels = calculateLevels(root);
            for(int i=0; i<levels; i++){levelOrderRunner(root, nodeList, i);}
            return nodeList;
        }

    private:
        std::shared_ptr<Treeable<E>> findNode(const E& value){
            std::shared_ptr<Treeable<E>> current = root;
            if(root == nullptr){
                std::cout << "Tree is empty" << std::endl;
                return nullptr;
            }

            while(true){
                if(current->getValue() == value){
                        return current;
                }else if(value < current->getValue()){
                    if(current->getLeft() == nullptr){
                        std::cout << "Value not in tree" << std::endl;
                        break;
                    }current = current->getLeft();
                }else{
                    if(current->getRight() == nullptr){
                        std::cout << "Value not in tree" << std::endl;
                        break;
                    }current = current->getRight();
                }
            }return nullptr;
        }

        std::shared_ptr<Treeable<E>> parentOf(const E& value){
            std::shared_ptr<Treeable<E>> current = root;
            if(root == nullptr){
                std::cout << "Tree is empty" << std::endl;
                return nullptr;
            }

            while(true){
                if(current->getLeft()->getValue() == value || current->getRight()->getValue() == value){
                        return current;
                }else if(value < current->getValue()){
                    if(current->getLeft() == nullptr){
                        std::cout << "Value not in tree" << std::endl;
                        break;
                    }current = current->getLeft();
                }else{
                    if(current->getRight() == nullptr){
                        std::cout << "Value not in tree" << std::endl;
                        break;
                    }current = current->getRight();
                }
            }return nullptr;
        }

        int calculateLevels(std::shared_ptr<Treeable<E>> node){
            if(node == nullptr){return 0;}

            int leftHeight = calculateLevels(node->getLeft());
            int rightHeight = calculateLevels(node->getRight());

            if(leftHeight < rightHeight){
                return rightHeight + 1;
            }return leftHeight + 1;
        }

        void inOrderRunner(std::shared_ptr<Treeable<E>> node, std::vector<E>& nodeList){
            if(node!=nullptr){
                if(node->getLeft() != nullptr){inOrderRunner(node->getLeft(), nodeList);}
                nodeList.push_back(node->getValue());
                if(node->getRight() != nullptr){inOrderRunner(node->getRight(), nodeList);}
            }
        }

        void preOrderRunner(std::shared_ptr<Treeable<E>> node, std::vector<E>& nodeList){
            if(node!=nullptr){
                nodeList.push_back(node->getValue());
                if(node->getLeft() != nullptr){inOrderRunner(node->getLeft(), nodeList);}
                if(node->getRight() != nullptr){inOrderRunner(node->getRight(), nodeList);}
            }
        }

        void postOrderRunner(std::shared_ptr<Treeable<E>> node, std::vector<E>& nodeList){
            if(node!=nullptr){
                if(node->getLeft() != nullptr){inOrderRunner(node->getLeft(), nodeList);}
                if(node->getRight() != nullptr){inOrderRunner(node->getRight(), nodeList);}
                nodeList.push_back(node->getValue());
            }
        }

        void levelOrderRunner(std::shared_ptr<Treeable<E>> node, std::vector<E>& nodeList, int distance){
            if(node == nullptr){
                for(int i=0; i<std::pow(2, distance); i++){nodeList.push_back(INT_MIN);}
            }else if(distance == 0){
                nodeList.push_back(node->getValue());
            }else if(distance>0){
                levelOrderRunner(node->getLeft(), nodeList, distance-1);
                levelOrderRunner(node->getRight(), nodeList, distance-1);
            }
        }
};

std::vector<int> readIntegersFromFile(const std::string& filename){
    std::vector<int> numbers;
    std::ifstream file(filename);

    if (!file.is_open()){
        std::cerr << "Could not open file: " << filename << std::endl;
        return numbers;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        while (ss >> token){
            token.erase(std::remove(token.begin(), token.end(), ','), token.end());
            if(!token.empty() && std::all_of(token.begin(), token.end(), ::isdigit)){numbers.push_back(std::stoi(token));}
        }
    }file.close();

    return numbers;
}