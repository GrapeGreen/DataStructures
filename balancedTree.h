#pragma once

#include <random>
#include <ctime>

std::mt19937 gen(time(NULL));

template<class funcType, typename indexType, typename valueType>
struct balancedTree {
private:
	typedef typename std::vector<std::pair<indexType, valueType>>::iterator VI;
	valueType defaultValue = valueType();
	funcType& func;

	struct node {
		node *l = nullptr, *r = nullptr;
		indexType key;
		valueType value, accumulatedValue;
		int y = gen();
		node(indexType key, valueType value) : key(key), value(value), accumulatedValue(value) {}
	};

	typedef node * tree;
	typedef std::pair<tree, tree> ptt;

	valueType get(tree t) {
		if (!t) return defaultValue;
		return t->accumulatedValue;
	}

	tree mend(tree t) {
		if (!t) return t;
		t->accumulatedValue = func(t->value, func(get(t->l), get(t->r)));
		return t;
	}

	tree root = nullptr;

	ptt split(tree t, indexType key) {
		if (!t) return std::make_pair(t, t);

		if (key < t->key) {
			ptt p = split(t->l, key);
			t->l = p.second;
			return std::make_pair(p.first, mend(t));
		}
		else {
			ptt p = split(t->r, key);
			t->r = p.first;
			return std::make_pair(mend(t), p.second);
		}
	}

	tree merge(tree u, tree v) {
		if (!u) return v;
		if (!v) return u;

		if (u->y > v->y) {
			u->r = merge(u->r, v);
			return mend(u);
		}
		else {
			v->l = merge(u, v->l);
			return mend(v);
		}
	}

public:
	balancedTree(funcType& func, valueType defaultValue = valueType()) : func(func), defaultValue(defaultValue) {}

	void insert(std::pair<indexType, valueType> keyValuePair) {
		insert(keyValuePair.first, keyValuePair.second);
	}

	void insert(indexType key, valueType value) {
		ptt p = split(root, key - 1);
		ptt q = split(p.second, key);
		root = merge(p.first, merge(new node(key, value), q.second));
	}

	void erase(indexType key) {
		ptt p = split(root, key - 1);
		ptt q = split(p.second, key);
		root = merge(p.first, q.second);
	}

	valueType get(indexType l, indexType r) {
		ptt p = split(root, l - 1);
		ptt q = split(p.second, r);
		valueType ans = get(q.first);
		root = merge(p.first, merge(q.first, q.second));
		return ans;
	}

	balancedTree(funcType& func, VI first, VI last, valueType defaultValue = valueType()) : func(func), defaultValue(defaultValue) {
		for (auto it = first; it != last; it++)
			insert(*it);
	}
};