#pragma once

template<class funcType, class invFuncType, typename valueType>
struct fenwickTree {
private:
	typedef typename std::vector<valueType>::iterator VI;
	valueType *fenwick, *array;
	funcType& func;
	invFuncType& invFunc;
	int n;
	valueType defaultValue;

public:
	valueType get(int x) {
		valueType result = defaultValue;
		for (; x >= 0; x = (x & (x + 1)) - 1)
			result = func(result, fenwick[x]);
		return result;
	}

	valueType get(int l, int r) {
		return invFunc(get(r), get(l - 1));
	}

	void set(int x, valueType newValue) {
		auto& oldValue = array[x];
		for (; x < n; x |= (x + 1))
			fenwick[x] = func(invFunc(fenwick[x], oldValue), newValue);
		oldValue = newValue;
	}

	fenwickTree(funcType& func, invFuncType& invFunc, int n, valueType defaultValue = valueType()) : 
		func(func), invFunc(invFunc), n(n), defaultValue(defaultValue) {
		fenwick = new valueType[n];
		std::fill_n(fenwick, n, defaultValue);

		array = new valueType[n];
		std::fill_n(array, n, defaultValue);
	}

	fenwickTree(funcType& func, invFuncType& invFunc, VI first, VI last, valueType defaultValue = valueType()) :
		func(func), invFunc(invFunc), defaultValue(defaultValue) {
		n = std::distance(first, last);

		fenwick = new valueType[n];
		std::fill_n(fenwick, n, defaultValue);

		array = new valueType[n];
		std::fill_n(array, n, defaultValue);

		for (int i = 0; first != last; first++, i++) {
			set(i, *first);
			array[i] = *first;
		}
	}

	~fenwickTree() {
		delete[] fenwick;
		delete[] array;
	}
};
