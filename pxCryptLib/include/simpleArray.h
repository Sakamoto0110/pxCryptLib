#pragma once





template<typename T, size_t sz, bool ShouldPreAllocate = true>
struct SimpleArray {
	SimpleArray() {
		for (size_t i = 0; i < capacity; i++) {
			if (ShouldPreAllocate)
				_data[i] = new T;
			_data[i] = 0;
		}
	}

	void Remove(size_t index) {
		if (_tryToDealloc(index) && !_isContigous()) {
			_removeGaps();
		}
	}

	void Insert(size_t index, const T& dat) {
		if (_tryToAlloc(index))
			*_data[index] = dat;
	}



	static constexpr size_t capacity = sz;
	

	

	constexpr size_t Size() {
		size_t size = 0;
		for (size_t i = 0; i < capacity; i++)
			if (_data[(size = i)] == 0)
				break;
		return size;
	}

	void operator +=(const T& dat) {
		for (size_t i = 0; i < capacity; i++) {
			if (!_isAllocated(i)) {
				this->Insert(i, dat);
				return;
			}
		}
	}

	const T& operator[](size_t index) {
		return *_data[index];
	}

private:
	constexpr bool _isAllocated(size_t index) {
		return _data[index] != 0;
	}
	constexpr bool _isContigous() {
		for (size_t i = 0; i < capacity; i++) {

			if (!_isAllocated(i)) {
				for (size_t j = i; j < capacity; j++) {
					if (_isAllocated(j))
						return false;
				}
			}
		}
		return true;
	}
	bool _tryToDealloc(size_t index) {
		if (_data[index] != 0) {
			delete	_data[index];
			_data[index] = 0;
			return true;
		}
		return false;
	}
	bool _tryToAlloc(size_t index) {
		if (_data[index] == 0) {
			_data[index] = new T;
			return true;
		}
		return false;
	}

	void _removeGaps() {
		for (size_t i = 0; i < capacity - 1; i++) {
			if (_data[i] == 0 && _data[i + 1] != 0) {
				_data[i] = _data[i + 1];
				_data[i + 1] = 0;
			}
		}
	}

	
	T* _data[sz];
};