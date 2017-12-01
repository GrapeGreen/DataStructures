template<typename indexType, typename valueType>
struct hashTable {
private:
	vector<vector<pair<indexType, valueType>>> buckets;
	int bucketCount = 1;
	valueType defaultValue = valueType();

public:
	hashTable(int bucketCount, valueType defaultValue = valueType()) :bucketCount(bucketCount), defaultValue(defaultValue) {
		buckets.resize(bucketCount);
	}

	valueType & operator[](indexType key) {
		int pos = std::hash<indexType>()(key) % bucketCount; // nothing's better than the default std hasher

		for (auto &v : buckets[pos])
			if (v.first == key) 
				return v.second;

		buckets[pos].push_back(mp(key, defaultValue));
		return buckets[pos].back().second;
	}

	void erase(indexType key) {
		int pos = std::hash<indexType>()(key) % bucketCount;
		auto it = std::find_if(all(buckets[pos]), [key](const std::pair<indexType, valueType> &keyValuePair) {
			return keyValuePair.first == key;
		});

		if (it == buckets[pos].end()) return;

		buckets[pos].erase(it);
	}
};