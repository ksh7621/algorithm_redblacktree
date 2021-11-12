#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Node {
public:
	int ID; //id
	string name; //�̸�
	int capacity; //�뷮
	int price; //����
	string color; //red or black
	int flag = 0; //visit

	Node* parent;
	Node* rchild;
	Node* lchild;

	Node(int ID, string name, int capacity, int price) {
		this->ID = ID;
		this->name = name;
		this->capacity = capacity;
		this->price = price;
		this->color = "red";
		this->parent = NULL;
		this->rchild = NULL;
		this->lchild = NULL;
		this->flag = 0;
	}
	~Node() {}
};

class RBT {
public:
	Node* root;
	int depth;

	RBT() {
		root = NULL;
		depth = 0;
	}

	~RBT() {
		treeDestructor(this->root);
	}

	Node* getRoot() { return this->root; }

	int insertNode(int id, string name, int capacity, int price) {
		Node* newnode = new Node(id, name, capacity, price);
		Node* currentnode = this->root;
		Node* parentnode = NULL;
		Node* unclenode = NULL;

		//���� ��� ������ ���� ����ϰ� ��ϰ���
		Node* exNode = searchNode(newnode->ID);
		if (exNode != NULL) {
			depth = printDepth(exNode); //���� ���
			return depth;
		}
		else {
			//�����ϰ� ���� ���
			if (root == NULL) { //��Ʈ ��
				root = newnode;
				newnode->color = "black";
			}
			else { //�ƴϸ� ����� ��_�Ϲ������� ����
				while (currentnode != NULL) {
					parentnode = currentnode;
					if (currentnode->ID > id) //��->��
						currentnode = currentnode->lchild;
					else //Ŭ->��
						currentnode = currentnode->rchild;
				}
				//�θ���
				if (parentnode->ID > id) {
					newnode->parent = parentnode;
					parentnode->lchild = newnode;
					currentnode = newnode;
				}
				else {
					newnode->parent = parentnode;
					parentnode->rchild = newnode;
					currentnode = newnode;
				}

				//double red
				while (currentnode != this->root && parentnode->color == "red" && currentnode->color == "red") {
					//uncle ã�� ��Ʈ�� ����
					if (parentnode == this->root) {
						continue;
					}
					else {
						if (parentnode == parentnode->parent->lchild) unclenode = parentnode->parent->rchild;
						else if (parentnode == parentnode->parent->rchild) unclenode = parentnode->parent->lchild;
					}
					//restructuring
					if (unclenode == NULL || unclenode->color == "black") { //uncle�� leaf������ null�̰ų� node�� black�϶�						
						if (parentnode == parentnode->parent->lchild && currentnode == parentnode->lchild) { //�޿�
							string tmpcol = parentnode->parent->color; //���θ� �θ� �� ����
							parentnode->parent->color = parentnode->color;
							parentnode->color = tmpcol;
							RotateRight(parentnode->parent);
							currentnode = currentnode->parent;
						}
						else if (parentnode == parentnode->parent->lchild && currentnode == parentnode->rchild) { //�޿�
							string tmpcol = parentnode->parent->color; //���θ� �ڽ� �� ����
							parentnode->parent->color = currentnode->color;
							currentnode->color = tmpcol;
							RotateLeft(parentnode);
							parentnode = currentnode->parent;
							RotateRight(parentnode);
							parentnode = currentnode->parent;
						}
						else if (parentnode == parentnode->parent->rchild && currentnode == parentnode->rchild) { //����
							string tmpcol = parentnode->parent->color; //���θ� �θ� �� ����
							parentnode->parent->color = parentnode->color;
							parentnode->color = tmpcol;
							RotateLeft(parentnode->parent);
							currentnode = currentnode->parent;
						}
						else if (parentnode == parentnode->parent->rchild && currentnode == parentnode->lchild) { //����
							string tmpcol = parentnode->parent->color; //���θ� �ڽ� �� ����
							parentnode->parent->color = currentnode->color;
							currentnode->color = tmpcol;
							RotateRight(parentnode);
							parentnode = currentnode->parent;
							RotateLeft(parentnode);
							parentnode = currentnode->parent;
						}
						currentnode = currentnode->parent; //current ���ĵ� �� ���� �ø���
						if (currentnode == NULL) {
							currentnode = this->root;
							parentnode = currentnode->parent;
						}
						else parentnode = currentnode->parent;
					}
					//recoloring
					else if (unclenode->color == "red") {
						parentnode->color = "black";
						unclenode->color = "black";
						parentnode->parent->color = "red";
						currentnode = parentnode->parent;
						parentnode = currentnode->parent;
					}
				}
				this->root->color = "black";
			}
			depth = printDepth(newnode); //���� ���
			return depth;
		}

	}

	void RotateRight(Node* yNode) //������ ȸ�� 
	{
		Node* xNode;
		xNode = yNode->lchild;
		yNode->lchild = xNode->rchild; //������ ��ĭ��
		if (xNode->rchild != NULL) xNode->rchild->parent = yNode; //�ڽİ� ����

		xNode->parent = yNode->parent; //�θ� ��ȯ

		if (xNode->parent == NULL) this->root = xNode; //��Ʈ�϶�
		else if (yNode == yNode->parent->lchild) yNode->parent->lchild = xNode;
		else yNode->parent->rchild = xNode;

		xNode->rchild = yNode;
		yNode->parent = xNode;
	}

	void RotateLeft(Node* xNode) //���� ȸ��
	{
		Node* yNode;
		yNode = xNode->rchild;
		xNode->rchild = yNode->lchild;
		if (xNode->rchild != NULL) xNode->rchild->parent = xNode;

		yNode->parent = xNode->parent;

		if (xNode->parent == NULL) this->root = yNode;
		else if (xNode == xNode->parent->lchild) xNode->parent->lchild = yNode;
		else xNode->parent->rchild = yNode;

		yNode->lchild = xNode;
		xNode->parent = yNode;
	}

	int printDepth(Node* node) {
		int depth = 0;
		Node* temp = node;
		while (1) {
			if (temp == this->root) { //��Ʈ �϶� ���� ���
				return depth;
			}
			else {
				temp = temp->parent;
				depth++;
			}
		}
	}

	Node* searchNode(int id) {
		Node* tmpnode = this->root;
		while (tmpnode != NULL) {
			if (tmpnode->ID == id) {
				return tmpnode; //node ��ü�� ����
			}
			else if (tmpnode->ID > id) {
				tmpnode = tmpnode->lchild;
			}
			else {
				tmpnode = tmpnode->rchild;
			}
		}
		return NULL;  //������ NULL ���
	}

	void updateNode(int id, string name, int capacity, int price) {
		//���� ������Ʈ depth��� or �������� ������ NULL
		Node* findNode = searchNode(id); //ã��

		if (findNode == NULL) {
			cout << "NULL" << endl;
		}
		else if (findNode->ID == id) {
			findNode->name = name;
			findNode->capacity = capacity;
			findNode->price = price;
			cout << printDepth(findNode) << endl;
		}
		return;
	}

	void discount_preorder(int x, int y, int per, Node* _curNode) { //������ ���� ��ü�� ���鼭	
		if (_curNode == NULL) return;
		else {
			if (_curNode->ID >= x && _curNode->ID <= y) { //���� �� ������ ����				
				int pr = _curNode->price;
				pr = static_cast<int>(pr * (100 - per) / 100); //�Ҽ��� ĳ���� ����
				_curNode->price = pr;
			}
			discount_preorder(x, y, per, _curNode->lchild);
			discount_preorder(x, y, per, _curNode->rchild);
		}
	}

	void treeDestructor(Node* root) {
		if (root == NULL) return;
		if (root->lchild != NULL)
			this->treeDestructor(root->lchild);
		if (root->rchild != NULL)
			this->treeDestructor(root->rchild);
		delete(root);
	}
};

int main() {
	int T; //test case
	cin >> T;

	RBT rbt;
	string func, name, color;
	int id, capacity, price, ran1, ran2, per;

	for (int j = 0; j < T; j++) {
		cin >> func;

		if (func == "I") {
			cin >> id >> name >> capacity >> price;
			cout << rbt.insertNode(id, name, capacity, price) << endl;
		}
		else if (func == "F") {
			cin >> id;
			Node* tmpnode = rbt.searchNode(id);
			if (tmpnode == NULL) cout << "NULL" << endl;
			else cout << rbt.printDepth(tmpnode) << " " << tmpnode->name << " " << tmpnode->capacity << " " << tmpnode->price << endl;
		}
		else if (func == "R") {
			cin >> id >> name >> capacity >> price;
			rbt.updateNode(id, name, capacity, price);
		}
		else if (func == "D") {
			cin >> ran1 >> ran2 >> per;
			rbt.discount_preorder(ran1, ran2, per, rbt.getRoot());
		}
	}
	return 0;
}