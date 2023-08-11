/// @file priorityqueue.h
///
///
///
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

/// @author Faaizuddin Farooqui
/// COURSE: CS 251, Spring 2023 UIC
/// SYSTEM: VS CODE
/// PROJECT: PRIORITY QUEUE
/// @date April 11th, 2023
/// @brief Priority Queue Class Implementation using a Binary Search Tree with Linked List

#pragma once

#include <iostream>
#include <sstream>
#include <set>
#include <queue>

using namespace std;

template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority;  // used to build BST
        T value;  // stored data for the p-queue
        bool dup;  // marked true when there are duplicate priorities
        NODE* parent;  // links back to parent
        NODE* link;  // links to linked list of NODES with duplicate priorities
        NODE* left;  // links to left child
        NODE* right;  // links to right child
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of elements in the pqueue
    NODE* curr;  // pointer to next item in pqueue (see begin and next)
    
    // HElPER FUNCTIONS

    /// @brief Finds highest priority node
    NODE * _findMinimumNode(NODE * node){

        while (node->left != nullptr){
            node = node->left;
        }

        return node; // least priority node within the subtree 
    }

    /// @brief Recursively traverses queue building string returned by toString()
    void _recursiveToStringHelper(NODE * node, ostream & output){

        // base case
        if (node == nullptr) 
            return;

        // IN ORDER TRAVERSAL

        // left sub-tree
        _recursiveToStringHelper(node->left, output);

        // print current (sub-root node)
        output << node->priority << " value: " << node->value << "\n";
        // print duplicate nodes (linked list nodes)
        NODE * curr = node->link;
        while (curr != nullptr) {
            output << curr->priority << " value: " << curr->value << "\n";
            curr = curr->link;
        }

        // go to right sub-tree
        _recursiveToStringHelper(node->right, output);
    }

    /// @brief recursivley copies linked list, used by _copyingTree()
    NODE * _recursiveCopyLink(NODE * node){
        
        if (node == nullptr){
            return nullptr;
        }

        NODE * newNode = new NODE();
        newNode->value = node->value;
        newNode->priority = node->priority;

        newNode->link = _recursiveCopyLink(node->link);

        return newNode;
    }

    /// @brief recursively traverses queue copying ever node used by assignment operator
    NODE * _copyingTree(NODE * node){
        
        // base case
        if (node == nullptr) 
            return nullptr;

        // PRE-ORDER TRAVERSAL

        // Create copy of node
        NODE* newNode = new NODE();
        newNode->value = node->value;
        newNode->priority = node->priority;
        newNode->dup = node->dup;
        newNode->parent = nullptr;

        // recursively copy the linked list (wont copy if there aren't duplicates)
        newNode->link = _recursiveCopyLink(node->link);
        
        // recursively call left and right sub-trees
        newNode->left = _copyingTree(node->left);
        newNode->right = _copyingTree(node->right);

        return newNode;
    }

    /// @brief recursively deletes (frees memory) for all nodes in our BST queue 
    void _clearRecursiveHelper(NODE * node){
        
        // base case;
        if(node == nullptr)
            return;
        
        // POST ORDER TRAVERSAL

        // call left and right sub-trees
        _clearRecursiveHelper(node->left);
        _clearRecursiveHelper(node->right);

        // Handle duplicates
        NODE * duplicate = node->link;
        while (duplicate != nullptr) {
            NODE * next_duplicate = duplicate->link;
            delete duplicate;
            duplicate = next_duplicate;
        }

        // delete (sub-tree) root
        delete node;
        
    }

    /// @brief recursively traverses tree checking that every node in two trees are the same
    /// @param node1 - first trees node
    /// @param node2 - second trees node
    /// @return true or false depending on if the nodes are equal
    bool _equal(NODE * node1, NODE * node2) const {
        
        // both are emtpy
        if (node1 == nullptr && node2 == nullptr) return true;

        // either is empty
        else if (node1 == nullptr || node2 == nullptr) return false;

        // check duplicate nodes
        else {
            
            // handle duplicate nodes
            NODE * link1 = node1->link;
            NODE * link2 = node2->link;

            while (link1 != nullptr && link2 != nullptr){
                
                // check if their values and priorities are the same
                if (link1->value != link2->value || link1->priority != link2->priority)
                    return false;

                // advance linked lists
                link1 = link1->link;
                link2 = link2->link;
            }

            if (link1 != nullptr || link2 != nullptr){
                return false;
            }
        }

        // check exact nodes recursively
        if (_equal(node1->left, node2->left) && (node1->value == node2->value) && (node1->priority == node1->priority) && (node1->dup == node2->dup) && _equal(node1->right, node2->right)){
            return true;
        }
        return false;
    }
    

public:
    //
    // default constructor:
    //
    // Creates an empty priority queue.
    // O(1)
    //
    priorityqueue() {
        
        // TO DO: write this function.
        root = nullptr;
        curr = nullptr;
        size = 0;
        
    }
    
    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    // Sets all member variables appropriately.
    // O(n), where n is total number of nodes in custom BST
    //
    priorityqueue& operator=(const priorityqueue& other) {
        
        // setting the same priority queue to itself
        if (this == &other) return *this;
        
        // clear the current tree
        this->clear();

        // copying tree
        this->root = _copyingTree(other.root);
        this->size = other.size;

        return *this;
    }
    
    //
    // clear:
    //
    // Frees the memory associated with the priority queue but is public.
    // O(n), where n is total number of nodes in custom BST
    //
    void clear() {  
        _clearRecursiveHelper(root);
        this->root = nullptr;
        this->size = 0;
    }
    
    //
    // destructor:
    //
    // Frees the memory associated with the priority queue.
    // O(n), where n is total number of nodes in custom BST
    //
    ~priorityqueue() {
        clear();
    }
    
    //
    // enqueue:
    //
    // Inserts the value into the custom BST in the correct location based on
    // priority.
    // O(logn + m), where n is number of unique nodes in tree and m is number 
    // of duplicate priorities
    //
    void enqueue(T value, int priority) {

        // create our node to be added to our tree:
        NODE * node = new NODE();
        node->value = value;
        node->priority = priority;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
        node->link = nullptr;

        // empty tree
        if (this->root == nullptr) {
            this->root = node;
            size++;
            return;
        }

        NODE * current = this->root; // walks along list

        while (true){

            // node to be inserted is greater than current node
            if (current->priority < priority){
                if (current->right != nullptr){ // if we can move right, move right
                    current = current->right;
                } else {
                    current->right = node;   // current's right is where node is going to be inserted
                    node->parent = current;  // set node's parent to current
                    break;
                }
                    
            } else if (priority < current->priority){ // node to be inserted is less than current node
                if (current->left != nullptr){ // if we can move left, move left
                    current = current->left;
                } else {
                    current->left = node;    // current's left is where node is going to be inserted
                    node->parent = current;  // set node's parent to current
                    break;
                }
            } else { // node to be inserted is equal to the current node, thus linked list is made
                
                node->dup = true; // set duplicate value to true

                // move along linked list till we can no longer, then insert
                while (current->link != nullptr){
                    current = current->link;
                }
                current->link = node;  // current's next is our new node
                node->parent = current; // set parent to current
                break;
            }
        }
        
        size++; 
    }
    
    //
    // dequeue:
    //
    // returns the value of the next element in the priority queue and removes
    // the element from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number 
    // of duplicate priorities
    //
    T dequeue() {

        T valueOut;

        // Empty tree, cannot dequeue anything
        if (this->root == nullptr) {
            return valueOut;
        }

        // Find the node with the highest priority (leftmost leaf)
        NODE* current = this->root;
        while (current->left != nullptr) {
            current = current->left;
        }

        valueOut = current->value; // save value of dequeued element

        // Check if there are duplicate priorities
        if (current->link != nullptr) {
            // update pointers
            NODE* temp = current->link;
            temp->parent = current->parent;
            temp->left = current->left;
            temp->right = current->right;

            // update parent's child's pointers
            if (current->parent != nullptr) {
                if (current->parent->left == current) {
                    current->parent->left = temp;
                } else {
                    current->parent->right = temp;
                }
            } else {
                this->root = temp; // Update root if necessary, if no parent
            }

            delete current;
        } else {
            // No duplicate priorities, remove the node from the tree
            if (current->right != nullptr) {
                // If current has a right child, promote the right child to current's position
                current->right->parent = current->parent;

                // update parent's child's pointer
                if (current->parent != nullptr) {
                    if (current->parent->left == current) {
                        current->parent->left = current->right;
                    } else {
                        current->parent->right = current->right;
                    }
                } else {
                    this->root = current->right; // Update root if necessary, if no parent
                }
            } else {
                // Current has no children, just remove it from the tree

                // update parent's child's pointer
                if (current->parent != nullptr) {
                    if (current->parent->left == current) {
                        current->parent->left = nullptr;
                    } else {
                        current->parent->right = nullptr;
                    }
                } else {
                    this->root = nullptr; // Update root if necessary
                }
            }

            delete current;
        }

        size--;          // decrement size of queue
        return valueOut;
    }

    //
    // Size:
    //
    // Returns the # of elements in the priority queue, 0 if empty.
    // O(1)
    //
    int Size() {
        return size; 
    }
    
    //
    // begin
    //
    // Resets internal state for an inorder traversal.  After the
    // call to begin(), the internal state denotes the first inorder
    // node; this ensure that first call to next() function returns
    // the first inorder node value.
    //
    // O(logn), where n is number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    void begin() {
        this->curr = this->root;

        // Go to the leftmost node in the tree, which has the highest priority
        this->curr = _findMinimumNode(this->curr);
    }
    
    //
    // next
    //
    // Uses the internal state to return the next inorder priority, and
    // then advances the internal state in anticipation of future
    // calls.  If a value/priority are in fact returned (via the reference
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more values/priorities are available.  This is the end of the
    // inorder traversal.
    //
    // O(logn), where n is the number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (pq.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    //
    bool next(T& value, int &priority) {   
        // END OF TRAVERSAL
        if (curr == nullptr)
            return false;
        
        // Save the value and priority of the current node
        value = curr->value;
        priority = curr->priority;

        // ADVANCE CURRENT
        // If there are duplicates, traverse the linked list
        if (curr->link != nullptr) {
            curr = curr->link;
            return true;
        }
        // walking back to left 
        while (curr->dup){
            curr = curr->parent;
        }
        
        if (curr->right != nullptr) { // Traverse the right subtree first, if available
            curr = curr->right;
            // Go to the leftmost node of the current subtree
            while (curr->left != nullptr) {
                curr = curr->left;
            }
            return true;
        } 
        else {
            // No right subtree, move up the tree until a node is found where the current node is in the left subtree
            NODE* temp = curr->parent;
            while (temp != nullptr && (curr == temp->right)) {
                curr = temp;
                temp = temp->parent;
            }
            curr = temp;
            if (curr == nullptr){
                return false;
            }
        }
        return true;
    }
    
    //
    // toString:
    //
    // Returns a string of the entire priority queue, in order.  Format:
    // "1 value: Ben
    //  2 value: Jen
    //  2 value: Sven
    //  3 value: Gwen"
    //
    string toString() {
        stringstream output;
        _recursiveToStringHelper(this->root, output);
        return output.str();
    }
    
    //
    // peek:
    //
    // returns the value of the next element in the priority queue but does not
    // remove the item from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number 
    // of duplicate priorities
    //
    T peek() {
        T valueOut;

        NODE * highestPriority = _findMinimumNode(this->root);

        if (highestPriority != nullptr)
            return highestPriority->value;
        else 
            return valueOut;
    }
    
    //
    // ==operator
    //
    // Returns true if this priority queue as the priority queue passed in as
    // other.  Otherwise returns false.
    // O(n), where n is total number of nodes in custom BST
    //
    bool operator==(const priorityqueue& other) const {  
        return _equal(this->root, other.root);
    }
    
    //
    // getRoot - Do not edit/change!
    //
    // Used for testing the BST.
    // return the root node for testing.
    //
    void* getRoot() {
        return root;
    }
};
