#include<utility>
#include<ctime>
#include<cstdlib>
#include<stdexcept>
#include<string.h>
#include<algorithm>
#define NLEVELS 20

namespace cs540{
	template <typename Key_T, typename Mapped_T> class Map;
	template <typename Key_T, typename Mapped_T> class Node{
		public:
			friend class Map<Key_T, Mapped_T>;
			typedef std::pair<const Key_T, Mapped_T> ValueType;
			Node(int n){
				val = nullptr;
				next = new Node*[n+1]();
				prev = nullptr;
			}
			Node(int n, const ValueType& value){
				val = new std::pair<const Key_T, Mapped_T>(value);
				next = new Node*[n+1]();
				prev = nullptr;
			}
			~Node(){
				delete val;
				delete [] next;
			}
		private:
			ValueType *val;
			Node **next;
			Node *prev;
	};
	template <typename Key_T, typename Mapped_T> class Map{
		public:
			typedef std::pair<const Key_T, Mapped_T> ValueType;
			class Iterator;
			class ConstIterator;
			class ReverseIterator;
			Map(){
				head = new Node<Key_T, Mapped_T>(NLEVELS);
				tail = new Node<Key_T, Mapped_T>(NLEVELS);
				head->next[0]=tail;
				tail->prev = head;
				head->prev = nullptr;
				sizeM = 0;
				max = 0;
				srand(time(nullptr));
			}
			Map(const Map& map){
				head = new Node<Key_T, Mapped_T>(NLEVELS);
				tail = new Node<Key_T, Mapped_T>(NLEVELS);
				head->next[0]=tail;
				tail->prev = head;
				head->prev = nullptr;
				sizeM = 0;
				max = 0;
				srand(time(nullptr));
				Node<Key_T, Mapped_T> *temp = map.getHead()->next[0];
				while(temp != map.getTail()){
					insert(*(temp->val));
					temp = temp->next[0];
				}
			}
			Map& operator=(const Map& map){
				clear();
				Node<Key_T, Mapped_T> *temp = map.getHead()->next[0];
				while(temp != map.getTail()){
					insert(*(temp->val));
					temp = temp->next[0];
				}
				return *this;
			}
			Map(std::initializer_list<std::pair<const Key_T, Mapped_T> > list){
				head = new Node<Key_T, Mapped_T>(NLEVELS);
				tail = new Node<Key_T, Mapped_T>(NLEVELS);
				head->next[0]=tail;
				tail->prev = head;
				head->prev = nullptr;
				sizeM = 0;
				max = 0;
				srand(time(nullptr));
				auto iter = list.begin();
				while(iter != list.end()){
					insert(*iter);
					iter++;
				}
			}
			~Map(){
				Node<Key_T, Mapped_T> *temp1 = head;
				Node<Key_T, Mapped_T> *temp2;
				while(temp1 != nullptr){
					temp2 = temp1->next[0];
					delete temp1;
					temp1 = temp2;
				}
			}
			Node<Key_T, Mapped_T>* getHead() const{
				return head;
			}
			Node<Key_T, Mapped_T>* getTail() const{
				return tail;
			}
			size_t size() const{
				return sizeM;
			}
			bool empty() const{
				if(sizeM == 0) return true;
				return false;
			}
			Iterator begin(){
				return Iterator(head->next[0]);
			}
			Iterator end(){
				return Iterator(tail);
			}
			ConstIterator begin() const{
				return ConstIterator(head->next[0]);
			}
			ConstIterator end() const{
				return ConstIterator(tail);
			}
			ReverseIterator rbegin(){
				return ReverseIterator(tail->prev);
			}
			ReverseIterator rend(){
				return ReverseIterator(head);
			}
			Iterator find(const Key_T& k){
				return Iterator(search(k));
			}
			ConstIterator find(const Key_T& k) const{
				return ConstIterator(search(k));
			}
			Mapped_T& at(const Key_T& k){
				Node<Key_T, Mapped_T> *temp = search(k);
				if(temp == tail) throw std::out_of_range("Key not in map");
				else return temp->val->second;
			}
			const Mapped_T& at(const Key_T& k) const{
				Node<Key_T, Mapped_T> *temp = search(k);
				if(temp == tail) throw std::out_of_range("Key not in map");
				else return temp->val->second;
			}
			Mapped_T& operator[](const Key_T& k){
				Node<Key_T, Mapped_T> *temp = search(k);
				if(temp == tail){
					Iterator iter = insert(std::make_pair(k, Mapped_T())).first;
					return (*iter).second;
				}
				else return temp->val->second;
			}
			std::pair<Iterator, bool> insert(const ValueType& val){
				const Key_T& k = val.first;
				Node<Key_T, Mapped_T> *temp = head;
				Node<Key_T, Mapped_T>** nodes = new Node<Key_T, Mapped_T>*[NLEVELS+1]();
				int i = max;
				while(i>=0){
					while((temp->next[i] != nullptr && temp->next[i] != tail) && temp->next[i]->val->first < k) temp = temp->next[i];
					nodes[i] = temp;
					i--;
				}
				if(nodes[0]->next[0] != tail && nodes[0]->next[0]->val->first == k){
					temp = nodes[0]->next[0];
					delete [] nodes;
					return std::make_pair(Iterator(temp), false);
				}
				int level = 0;
				while(rand()%2 && level < NLEVELS) level++;
				if(level > max){
					for(i = max+1; i<=level; i++) nodes[i] = head;
					max = level;
				}
				temp = new Node<Key_T, Mapped_T>(level, val);
				for(i=0; i<=level; i++){
					temp->next[i] = nodes[i]->next[i];
					nodes[i]->next[i] = temp;
				}
				temp->prev = nodes[0];
				temp->next[0]->prev = temp;
				sizeM++;
				delete [] nodes;
				return std::make_pair(Iterator(temp), true);
			}
			template <typename IT_T> void insert(IT_T range_beg, IT_T range_end){
				while(range_beg != range_end){
					insert(*range_beg);
					range_beg++;
				}
			}
			void erase(const Key_T& k){
				Node<Key_T, Mapped_T> *temp = head;
				int i = max;
				Node<Key_T, Mapped_T>** nodes = new Node<Key_T, Mapped_T>*[max+1]();
				while(i>=0){
					while((temp->next[i] != nullptr && temp->next[i] != tail) && temp->next[i]->val->first < k) temp = temp->next[i];
					nodes[i] = temp;
					i--;
				}
				if(nodes[0]->next[0] == tail || nodes[0]->next[0]->val->first != k){
					delete [] nodes;
					throw std::out_of_range("Key not in map");
				}
				temp = nodes[0]->next[0];
				for(i=0; i<=max && nodes[i]->next[i]==temp; i++) nodes[i]->next[i] = temp->next[i];
				nodes[0]->next[0]->prev = nodes[0];
				while(head->next[max] == nullptr) max--;
				sizeM--;
				delete [] nodes;
				delete temp;
			}
			void erase(Iterator pos){
				erase((*pos).first);
			}
			void clear(){
				Node<Key_T, Mapped_T> *temp1 = head->next[0];
				Node<Key_T, Mapped_T> *temp2;
				while(temp1 != tail){
					temp2 = temp1->next[0];
					delete temp1;
					temp1 = temp2;
				}
				tail->prev = head;
				std::fill(head->next+1, head->next+max+1, nullptr);
				head->next[0] = tail;
				sizeM = 0;
				max = 0;
			}
			Node<Key_T, Mapped_T>* search(const Key_T& k) const{
				Node<Key_T, Mapped_T> *temp = head;
				int i = max;
				while(i>=0){
					while((temp->next[i] != nullptr && temp->next[i] != tail) && temp->next[i]->val->first < k) temp = temp->next[i];
					i--;
				}
				temp = temp->next[0];
				if(temp == tail) return tail;
				if(temp->val->first == k) return temp;
				return tail;
			}
			bool operator==(const Map& map2){
				if(sizeM != map2.size()) return false;
				auto iter1 = (*this).begin();
				auto iter2 = map2.begin();
				while(iter1 != (*this).end()){
					if((*iter1) != (*iter2)) return false;
					iter1++;
					iter2++;
				}
				return true;
			}
			bool operator!=(const Map& map2){
				return !((*this)==map2);
			}
			bool operator<(const Map& map2){
				auto iter1 = (*this).begin();
				auto iter2 = map2.begin();
				while(iter1 != (*this).end() && iter2 != map2.end()){
					if((*iter1).first<(*iter2).first)return true;
					if((*iter2).first<(*iter1).first)return false;
					iter1++;
					iter2++;
				}
				if(sizeM < map2.size()) return true;
				return false;
			}
			class Iterator{
				public:
					Iterator(Node<Key_T, Mapped_T>* node){
						cur = node;
					}
					Node<Key_T, Mapped_T>* getCur() const{
						return cur;
					}
					Iterator& operator++(){
						cur = cur->next[0];
						return *this;
					}
					Iterator operator++(int){
						Map<Key_T, Mapped_T>::Iterator newIt = *this;
						cur = cur->next[0];
						return newIt;
					}
					Iterator& operator--(){
						cur = cur->prev;
						return *this;
					}
					Iterator operator--(int){
						Map<Key_T, Mapped_T>::Iterator newIt = *this;
						cur = cur->prev;
						return newIt;
					}
					ValueType& operator*() const{
						return *cur->val;
					}
					ValueType* operator->() const{
						return cur->val;
					}
				private:
					Node<Key_T, Mapped_T> *cur;
			};
			class ConstIterator{
				public:
					ConstIterator(const Iterator& iter){
						cur = iter.getCur();
					}
					ConstIterator(Node<Key_T, Mapped_T> *node){
						cur = node;
					}
					Node<Key_T, Mapped_T>* getCur() const{
						return cur;
					}
					ConstIterator& operator++(){
						cur = cur->next[0];
						return *this;
					}
					ConstIterator operator++(int){
						ConstIterator newIt = *this;
						cur = cur->next[0];
						return newIt;
					}
					ConstIterator& operator--(){
						cur = cur->prev;
						return *this;
					}
					ConstIterator operator--(int){
						ConstIterator newIt = *this;
						cur = cur->prev;
						return newIt;
					}
					const ValueType& operator*() const{
						return *cur->val;
					}
					const ValueType* operator->() const{
						return cur->val;
					}
				private:
					Node<Key_T, Mapped_T> *cur;
			};
			class ReverseIterator{
				public:
					ReverseIterator(Node<Key_T, Mapped_T> *node){
						cur = node;
					}
					Node<Key_T, Mapped_T>* getCur() const{
						return cur;
					}
					ReverseIterator& operator++(){
						cur = cur->prev;
						return *this;
					}
					ReverseIterator operator++(int){
						ReverseIterator newIt = *this;
						cur = cur->prev;
						return newIt;
					}
					ReverseIterator& operator--(){
						cur = cur->next[0];
						return *this;
					}
					ReverseIterator operator--(int){
						ReverseIterator newIt = *this;
						cur = cur->next[0];
						return newIt;
					}
					ValueType& operator*() const{
						return *cur->val;
					}
					ValueType* operator->() const{
						return cur->val;
					}
				private:
					Node<Key_T, Mapped_T> *cur;
			};
			friend bool operator==(const Iterator& iter1, const Iterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return true;
				return false;
			}
			friend bool operator==(const ConstIterator& iter1, const ConstIterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return true;
				return false;
			}
			friend bool operator==(const Iterator& iter1, const ConstIterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return true;
				return false;
			}
			friend bool operator==(const ConstIterator& iter1, const Iterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return true;
				return false;
			}
			friend bool operator!=(const Iterator& iter1, const Iterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return false;
				return true;
			}
			friend bool operator!=(const ConstIterator& iter1, const ConstIterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return false;
				return true;
			}
			friend bool operator!=(const Iterator& iter1, const ConstIterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return false;
				return true;
			}
			friend bool operator!=(const ConstIterator& iter1, const Iterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return false;
				return true;
			}
			friend bool operator==(const ReverseIterator& iter1, const ReverseIterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return true;
				return false;
			}
			friend bool operator!=(const ReverseIterator& iter1, const ReverseIterator& iter2){
				if(iter1.getCur() == iter2.getCur()) return false;
				return true;
			}
		private:
			Node<Key_T, Mapped_T> *head, *tail;
			size_t sizeM;
			int max;
	};
}
