#ifndef BST_H
#define BST_H
#include <string>
#include <sstream>
#include "BSTInterface.h"

/** A binary tree node with data, left and right child pointers */
template<typename T>
class BST :
	public BSTInterface<T> {
private:
	struct Node {
		struct Node* left;
		struct Node* right;
		T data;

		Node(T data) :
			data(data), left(NULL), right(NULL) {}
		~Node() = default;

		friend std::ostream& operator<<(std::ostream& oss, const struct Node& node) {
			oss << node.data;
			return oss;
		}
	};
	Node* rootNode = NULL; //The root node
	size_t nodeCounter = 0;
public:
	BST(void) :
		rootNode(NULL), nodeCounter(0) {}
	~BST(void) { clearTree(); }

	/** Return true if node added to BST, else false */
	bool addNode(const T& value) {
		return addNodeRecursor(value, rootNode);
	}

	/** Add node if possible using recursion */
	bool addNodeRecursor(const T& value, Node*& node) {
		//Base case: if node is leaf node or if tree is empty, insert node
		if (node == NULL) {
			node = new Node(value);
			++nodeCounter;
			return true;
		}
		//Recursive cases: iterate to the left or the right node
		if (value < node->data) {
			return addNodeRecursor(value, node->left);
		}
		if (value > node->data) {
			return addNodeRecursor(value, node->right);
		}
		return false;
	}

	/** Return true if node removed from BST, else false */
	bool removeNode(const T& value) {
		return removeNodeRecursor(value, rootNode);
	}

	/** Remove node if possible using recursion */
	bool removeNodeRecursor(const T& value, Node*& node) {
		if (node == NULL) {
			return false;
		}
		if (value < node->data) {
			return removeNodeRecursor(value, node->left);
		}
		if (value > node->data) {
			return removeNodeRecursor(value, node->right);
		}

		Node* oldNode = node;
		if (node->left == NULL) {
			node = node->right;
		}
		else if (node->right == NULL) {
			node = node->left;
		}
		else {
			Node* predecessor = node->left;
			while (predecessor->right != NULL) {
				predecessor = predecessor->right;
			}
			std::swap(node->data, predecessor->data);
			removeNodeRecursor(value, node->left);
			return true;
		}
		delete oldNode;
		--nodeCounter;
		return true;
	}

	/** Return true if BST cleared of all nodes, else false */
	bool clearTree() {
		clearTreeRecursor(rootNode);
		rootNode = NULL;
		return true;
	}

	/** Clear every node in the tree */
	bool clearTreeRecursor(Node*& node) {
		if (node == NULL) {
			return true;
		}
		clearTreeRecursor(node->left);
		clearTreeRecursor(node->right);
		delete node;
		--nodeCounter;
		return true;
	}

	/** Return true if value is found in BST, false if not */
	bool findNode(const T& value) {
		return findNodeRecursor(value, rootNode);
	}

	/** Recurse through tree to find value */
	bool findNodeRecursor(const T& value, Node*& node) {
		if (node == NULL) {
			return false;
		}
		if (node->data == value) {
			return true;
		}
		if (value < node->data) {
			return findNodeRecursor(value, node->left);
		}
		else {
			return findNodeRecursor(value, node->right);
		}
	}

	/** Return the number of nodes in the tree */
	size_t getNodeCounter() const {
		return nodeCounter;
	}

	/** Output the BST by level, starting with the root */
	bool outputLevel(Node* root, int level, std::stringstream& out) const {
		if (root == NULL) {
			return false;
		}
		if (level == 1) {
			out << " " << root->data;
			if ((root->left != NULL) || (root->right != NULL)) {
				return true;
			}
			return false;
		}
		if ((level == 2) && !root->left && root->right) {
			out << " _";
		}
		bool left = outputLevel(root->left, level - 1, out);
		bool right = outputLevel(root->right, level - 1, out);
		if ((level == 2) && root->left && !root->right) {
			out << " _";
		}
		return left || right;
	}

	/** Output the BST using in-order traversal */
	std::string outputInOrder(Node* root, std::stringstream& out) const {
		if (root == NULL) {
			return "";
		}
		outputInOrder(root->left, out);
		out << root->data << ",";
		outputInOrder(root->right, out);
		return out.str();
	}

	std::string toStringInOrder() const {
		std::stringstream ss;
		outputInOrder(rootNode, ss);

		std::string tempStr = "";
		std::string newStr = "";
		while (ss >> tempStr) {
			newStr = tempStr + newStr;
		}
		newStr = newStr.substr(0, newStr.size() - 1);
		return newStr;
	}


	/** Return a level order traversal of a BST as a string */
	std::string toString() const {
		std::stringstream out;
		//out << "Root: " << rootNode->data;
		if (rootNode == NULL) {
			out << "empty";
		}
		else {
			int level = 0;
			do {
				out << std::endl << " " << ++level << ":";
			} while (outputLevel(rootNode, level, out));
		}
		return out.str();
	}

	friend std::ostream& operator<<(std::ostream& oss, const BST bst) {
		oss << bst.toString();
		return oss;
	}
};
#endif	// BST_INTERFACE_H