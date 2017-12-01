#pragma once
#include <algorithm>

template <class funcType, typename valueType>
struct segmentTree
{
private:
	typedef typename std::vector<valueType>::iterator VI;
	int n;
	valueType* tree;
	funcType& func;
	valueType defaultValue = valueType();

	void build(int u, VI first, VI last) {
		if (last == first + 1)
			return void(tree[u] = *first);

		VI mid = first + std::distance(first, last) / 2;

		build(u << 1, first, mid);
		build(u << 1 | 1, mid, last);

		tree[u] = func(tree[u << 1], tree[u << 1 | 1]);
	}

	valueType get(int u, int l, int r, int L, int R) {
		if (L < l || r < R || L >= R) return defaultValue;

		if (l == L && r == R) return tree[u];

		int m = (l + r) / 2;
		return func(get(u << 1, l, m, L, std::min(m, R)), get(u << 1 | 1, m, r, std::max(L, m), R));
	}

	void set(int u, int l, int r, int i, valueType x) {
		if (l == r - 1) return void(tree[u] = x);

		int m = (l + r) / 2;

		if (i < m)
			set(u << 1, l, m, i, x);
		else
			set(u << 1 | 1, m, r, i, x);

		tree[u] = func(tree[u << 1], tree[u << 1 | 1]);
	}

public:
	segmentTree(funcType& func, VI first, VI last, valueType defaultValue = T()) : func(func), defaultValue(defaultValue) {
		n = std::distance(first, last);
		tree = new valueType[4 * n];
		std::fill_n(tree, n, defaultValue);
		build(0, first, last);
	}

	segmentTree(funcType& func, int n, valueType defaultValue = T()) : func(func), n(n), defaultValue(defaultValue) {
		tree = new valueType[4 * n];
		std::fill_n(tree, 4 * n, defaultValue);
	}

	valueType get(int l, int r) {
		return get(1, 0, n, l, r + 1);
	}

	void set(int i, valueType x) {
		return set(1, 0, n, i, x);
	}

	~segmentTree() {
		delete[] tree;
	}
};