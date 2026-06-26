#ifndef COMPANYBST_H
#define COMPANYBST_H

#include <functional>
#include <string>

class CompanyBST
{
private:
    struct Node
    {
        std::string companyName;
        int accountID;
        Node* left;
        Node* right;

        Node(std::string name, int id);
    };

    Node* root;

    void insert(Node*& subtreeRoot, const std::string& companyName, int accountID);
    void inorder(Node* subtreeRoot, const std::function<void(const std::string&, int)>& visit) const;
    void clear(Node*& subtreeRoot);

public:
    CompanyBST();
    ~CompanyBST();

    void insert(const std::string& companyName, int accountID);
    void inorder(const std::function<void(const std::string&, int)>& visit) const;
    void clear();
};

#endif
