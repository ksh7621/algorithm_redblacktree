#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Node {
public:
	int ID; //id
	string name; //이름
	int capacity; //용량
	int price; //가격
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

		//같은 노드 있으면 깊이 출력하고 등록거절
		Node* exNode = searchNode(newnode->ID);
		if (exNode != NULL) {
			depth = printDepth(exNode); //깊이 출력
			return depth;
		}
		else {
			//삽입하고 높이 출력
			if (root == NULL) { //루트 블랙
				root = newnode;
				newnode->color = "black";
			}
			else { //아니면 레드로 들어감_일반적으로 삽입
				while (currentnode != NULL) {
					parentnode = currentnode;
					if (currentnode->ID > id) //작->왼
						currentnode = currentnode->lchild;
					else //클->오
						currentnode = currentnode->rchild;
				}
				//부모설정
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
					//uncle 찾기 루트는 예외
					if (parentnode == this->root) {
						continue;
					}
					else {
						if (parentnode == parentnode->parent->lchild) unclenode = parentnode->parent->rchild;
						else if (parentnode == parentnode->parent->rchild) unclenode = parentnode->parent->lchild;
					}
					//restructuring
					if (unclenode == NULL || unclenode->color == "black") { //uncle이 leaf다음의 null이거나 node가 black일때						
						if (parentnode == parentnode->parent->lchild && currentnode == parentnode->lchild) { //왼왼
							string tmpcol = parentnode->parent->color; //조부모 부모 색 변경
							parentnode->parent->color = parentnode->color;
							parentnode->color = tmpcol;
							RotateRight(parentnode->parent);
							currentnode = currentnode->parent;
						}
						else if (parentnode == parentnode->parent->lchild && currentnode == parentnode->rchild) { //왼오
							string tmpcol = parentnode->parent->color; //조부모 자식 색 변경
							parentnode->parent->color = currentnode->color;
							currentnode->color = tmpcol;
							RotateLeft(parentnode);
							parentnode = currentnode->parent;
							RotateRight(parentnode);
							parentnode = currentnode->parent;
						}
						else if (parentnode == parentnode->parent->rchild && currentnode == parentnode->rchild) { //오오
							string tmpcol = parentnode->parent->color; //조부모 부모 색 변경
							parentnode->parent->color = parentnode->color;
							parentnode->color = tmpcol;
							RotateLeft(parentnode->parent);
							currentnode = currentnode->parent;
						}
						else if (parentnode == parentnode->parent->rchild && currentnode == parentnode->lchild) { //오왼
							string tmpcol = parentnode->parent->color; //조부모 자식 색 변경
							parentnode->parent->color = currentnode->color;
							currentnode->color = tmpcol;
							RotateRight(parentnode);
							parentnode = currentnode->parent;
							RotateLeft(parentnode);
							parentnode = currentnode->parent;
						}
						currentnode = currentnode->parent; //current 정렬된 곳 까지 올리기
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
			depth = printDepth(newnode); //깊이 출력
			return depth;
		}

	}

	void RotateRight(Node* yNode) //오른쪽 회전 
	{
		Node* xNode;
		xNode = yNode->lchild;
		yNode->lchild = xNode->rchild; //옆으로 한칸씩
		if (xNode->rchild != NULL) xNode->rchild->parent = yNode; //자식과 연결

		xNode->parent = yNode->parent; //부모값 변환

		if (xNode->parent == NULL) this->root = xNode; //루트일때
		else if (yNode == yNode->parent->lchild) yNode->parent->lchild = xNode;
		else yNode->parent->rchild = xNode;

		xNode->rchild = yNode;
		yNode->parent = xNode;
	}

	void RotateLeft(Node* xNode) //왼쪽 회전
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
			if (temp == this->root) { //루트 일때 깊이 출력
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
				return tmpnode; //node 자체를 리턴
			}
			else if (tmpnode->ID > id) {
				tmpnode = tmpnode->lchild;
			}
			else {
				tmpnode = tmpnode->rchild;
			}
		}
		return NULL;  //없을때 NULL 출력
	}

	void updateNode(int id, string name, int capacity, int price) {
		//정보 업데이트 depth출력 or 존재하지 않으면 NULL
		Node* findNode = searchNode(id); //찾기

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

	void discount_preorder(int x, int y, int per, Node* _curNode) { //할인율 적용 전체를 돌면서	
		if (_curNode == NULL) return;
		else {
			if (_curNode->ID >= x && _curNode->ID <= y) { //범위 내 할인율 적용				
				int pr = _curNode->price;
				pr = static_cast<int>(pr * (100 - per) / 100); //소수점 캐스팅 주의
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