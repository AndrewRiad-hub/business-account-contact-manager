#include "CompanyBST.h"

CompanyBST::Node::Node(std::string name, int id)
    : companyName(name), accountID(id), left(nullptr), right(nullptr)
{
}

CompanyBST::CompanyBST()
    : root(nullptr)
{
}

CompanyBST::~CompanyBST()
{
    clear(root);
}

void CompanyBST::insert(Node*& subtreeRoot, const std::string& companyName, int accountID)
{
    if (subtreeRoot == nullptr)
    {
        subtreeRoot = new Node(companyName, accountID);
    }
    else if (companyName < subtreeRoot->companyName)
    {
        insert(subtreeRoot->left, companyName, accountID);
    }
    else
    {
        insert(subtreeRoot->right, companyName, accountID);
    }
}

void CompanyBST::inorder(Node* subtreeRoot, const std::function<void(const std::string&, int)>& visit) const
{
    if (subtreeRoot == nullptr)
    {
        return;
    }

    inorder(subtreeRoot->left, visit);
    visit(subtreeRoot->companyName, subtreeRoot->accountID);
    inorder(subtreeRoot->right, visit);
}

void CompanyBST::clear(Node*& subtreeRoot)
{
    if (subtreeRoot == nullptr)
    {
        return;
    }

    clear(subtreeRoot->left);
    clear(subtreeRoot->right);
    delete subtreeRoot;
    subtreeRoot = nullptr;
}

void CompanyBST::insert(const std::string& companyName, int accountID)
{
    insert(root, companyName, accountID);
}

void CompanyBST::inorder(const std::function<void(const std::string&, int)>& visit) const
{
    inorder(root, visit);
}

void CompanyBST::clear()
{
    clear(root);
}
