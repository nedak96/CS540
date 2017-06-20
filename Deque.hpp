#ifndef DEQUE_HPP
#define DEQUE_HPP

#define Deque_DEFINE(t)\
	struct Obj_##t{\
		t item;\
		bool (*comp)(const t& o1,const t& o2);\
	};\
	int compare_##t(const void *a, const void *b){\
		Obj_##t a1 = *(Obj_##t*)a, b1 = *(Obj_##t*)b;\
		if(a1.comp(a1.item, b1.item)) return -1;\
		return 1;\
	}\
	struct Deque_##t##_Iterator{\
		int pos;\
		Obj_##t* items;\
		void (*inc)(Deque_##t##_Iterator *);\
		void (*dec)(Deque_##t##_Iterator *);\
		t& (*deref)(Deque_##t##_Iterator *);\
	} it_##t;\
	void inc(Deque_##t##_Iterator *it){\
		it->pos++;\
	}\
	void dec(Deque_##t##_Iterator *it){\
		it->pos--;\
	}\
	t& deref(Deque_##t##_Iterator* it){\
		return it->items[it->pos].item;\
	}\
	struct Deque_##t{\
		int num;\
		Obj_##t* items;\
		int front1;\
		int back1;\
		int max;\
		bool (*comp)(const t& o1,const t& o2);\
		const char type_name[sizeof("Deque_"#t)] = "Deque_"#t;\
		int (*size)(Deque_##t *);\
		t& (*at)(Deque_##t *, int);\
		bool (*empty)(Deque_##t *);\
		void (*push_back)(Deque_##t *, t);\
		void (*push_front)(Deque_##t *, t);\
		t& (*pop_back)(Deque_##t *);\
		t& (*pop_front)(Deque_##t *);\
		t& (*front)(Deque_##t *);\
		t& (*back)(Deque_##t *);\
		void (*dtor)(Deque_##t *);\
		void (*clear)(Deque_##t *);\
		Deque_##t##_Iterator& (*begin)(Deque_##t *);\
		Deque_##t##_Iterator& (*end)(Deque_##t *);\
		void (*sort)(Deque_##t *q, Deque_##t##_Iterator, Deque_##t##_Iterator);\
	};\
	int size(Deque_##t *q){\
		return q->num;\
	}\
	t& at(Deque_##t *q, int pos){\
		return q->items[q->front1 + pos].item;\
	}\
	bool empty(Deque_##t *q){\
		if(q->num == 0) return 1;\
		return 0;\
	}\
	void push_back(Deque_##t *q, t item){\
		q->num++;\
		q->back1++;\
		if(q->back1 > q->max){\
			q->items = (Obj_##t*)realloc(q->items, sizeof(Obj_##t)*(q->max+1000000));\
			q->max+=1000000;\
		}\
		q->items[q->back1-1].item = item;\
		q->items[q->back1-1].comp = q->comp;\
	}\
	void push_front(Deque_##t *q, t item){\
		q->num++;\
		q->front1--;\
		if(q->front1<0){\
			q->items = (Obj_##t*)realloc(q->items, sizeof(Obj_##t)*(q->max+1000000));\
			for(int i=q->back1-1; i>=0; i--) q->items[i+1000000] = q->items[i];\
			q->front1 += 1000000;\
			q->max += 1000000;\
			q->back1 += 1000000;\
		}\
		q->items[q->front1].item = item;\
		q->items[q->front1].comp = q->comp;\
	}\
	t& pop_back(Deque_##t *q){\
		q->num--;\
		q->back1--;\
		return q->items[q->back1].item;\
	}\
	t& pop_front(Deque_##t *q){\
		q->num--;\
		q->front1++;\
		return q->items[q->front1-1].item;\
	}\
	t& front(Deque_##t *q){\
		return q->items[q->front1].item;\
	}\
	t& back(Deque_##t *q){\
		return q->items[q->back1-1].item;\
	}\
	void dtor(Deque_##t *q){\
		free(q->items);\
	}\
	void clear(Deque_##t *q){\
		q->num = 0;\
		q->front1 = q->max/2;\
		q->back1 = q->max/2;\
	}\
	Deque_##t##_Iterator& begin(Deque_##t *q){\
		it_##t.pos=q->front1;\
		it_##t.items = q->items;\
		it_##t.inc = inc;\
		it_##t.dec = dec;\
		it_##t.deref = deref;\
		return it_##t;\
	}\
	Deque_##t##_Iterator& end(Deque_##t *q){\
		it_##t.pos=q->back1;\
		it_##t.items = q->items;\
		it_##t.inc = inc;\
		it_##t.dec = dec;\
		it_##t.deref = deref;\
		return it_##t;\
	}\
	void sort(Deque_##t *q, Deque_##t##_Iterator it1, Deque_##t##_Iterator it2){\
		qsort(q->items+it1.pos, it2.pos-it1.pos, sizeof(Obj_##t), compare_##t);\
	}\
	void Deque_##t##_ctor(Deque_##t *q, bool (*fptr)(const t& o1, const t& o2)){\
		q->num=0;\
		q->items = (Obj_##t*)malloc(sizeof(Obj_##t)*1000);\
		q->max = 1000;\
		q->front1 = q->max/2;\
		q->back1 = q->max/2;\
		q->comp = fptr;\
		q->size = size;\
		q->at = at;\
		q->empty = empty;\
		q->push_back = push_back;\
		q->push_front = push_front;\
		q->pop_back = pop_back;\
		q->pop_front = pop_front;\
		q->front = front;\
		q->back = back;\
		q->dtor = dtor;\
		q->clear = clear;\
		q->begin = begin;\
		q->end = end;\
		q->sort = sort;\
	}\
	bool Deque_##t##_equal(Deque_##t& q1, Deque_##t& q2){\
	  	if(q1.num != q2.num) return false;\
		for(int i=0; i<q1.num; i++) if(q1.comp(q1.at(&q1,i), q2.at(&q2,i)) || q1.comp(q2.at(&q2,i), q1.at(&q1,i))) return false;\
  		return true;\
	}\
	bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator& it1, Deque_##t##_Iterator& it2){\
		if(it1.pos != it2.pos) return false;\
		return true;\
	}\

#endif
