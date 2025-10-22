#include "ring_buffer.h"

bool CircularBuffer::is_empty() const {
	return _size == 0 || data_array.size() == 0;
}

int CircularBuffer::capacity() const {
	return data_array.size();
}

int CircularBuffer::size() const {
	return _size;
}

int CircularBuffer::head() const {
	return _head;
}

void CircularBuffer::advance(int p_steps) {
	int capacity = data_array.size();
	if (capacity == 0) {
		return;
	}

	// Allow arbitrary large positive or negative values
	_head = (_head + (p_steps % capacity) + capacity) % capacity;

	if (p_steps > 0) {
		_size += p_steps;
		if (_size > capacity) {
			_size = capacity;
		}
	}
}

void CircularBuffer::clear() {
	// Do not actually clear anything, just reset head and size
	_size = 0;
	_head = 0;
}

void CircularBuffer::resize(const int p_capacity) {
	ERR_FAIL_COND(p_capacity < 0);

	clear();
	fill(Variant());
}

void CircularBuffer::fill(const Variant &p_value) {
	if (data_array.size() == 0) {
		return;
	}

	_head = 0;
	_size = data_array.size();
	for (uint32_t i = 0; i < data_array.size(); i++) {
		data_array[i] = p_value;
	}
}

void CircularBuffer::append(const Variant &p_value) {
	int capacity = data_array.size();
	if (capacity == 0) {
		return;
	}

	// ERR_FAIL_INDEX_MSG(_head, capacity, "Head is out of bounds.");
	if (_head >= capacity) {
		_head = 0;
	}

	data_array[_head] = p_value;

	_head = (_head + 1) % capacity;
	if (_size < capacity) {
		_size++;
	}
}

void CircularBuffer::append_array(const Array &p_array) {
	int capacity = data_array.size();
	if (capacity == 0) {
		return;
	}

	// ERR_FAIL_INDEX_MSG(_head, capacity, "Head is out of bounds.");
	if (_head >= capacity) {
		_head = 0;
	}

	int p_size = p_array.size();
	for (int i = 0; i < p_size; i++) {
		data_array[_head] = p_array[i];
		_head = (_head + 1) % capacity;
		if (_size < capacity) {
			_size++;
		}
	}
}

Variant CircularBuffer::at(int p_index) const {
	// Index must be within bounds, from -size to size-1
	ERR_FAIL_COND_V(p_index < -_size || p_index >= _size, Variant());

	if (p_index < 0) {
		p_index += _size;
	}

	p_index = (data_array.size() + _head - _size + p_index) % data_array.size();

	return data_array[p_index];
}

Variant CircularBuffer::array_get(int p_index) const {
	if (_size == 0 || data_array.size() == 0 || p_index < 0 || p_index >= _size) {
		return Variant();
	}

	p_index = (data_array.size() + _head - _size + p_index) % data_array.size();

	return data_array[p_index];
}

void CircularBuffer::array_set(int p_index, const Variant &p_value) {
	if (_size == 0 || data_array.size() == 0 || p_index < 0 || p_index >= _size) {
		return;
	}

	p_index = (data_array.size() + _head - _size + p_index) % data_array.size();

	data_array[p_index] = p_value;
}

void CircularBuffer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_empty"), &CircularBuffer::is_empty);
	ClassDB::bind_method(D_METHOD("capacity"), &CircularBuffer::capacity);
	ClassDB::bind_method(D_METHOD("size"), &CircularBuffer::size);
	ClassDB::bind_method(D_METHOD("head"), &CircularBuffer::head);

	ClassDB::bind_method(D_METHOD("advance", "steps"), &CircularBuffer::advance, DEFVAL(1));

	ClassDB::bind_method(D_METHOD("clear"), &CircularBuffer::clear);
	ClassDB::bind_method(D_METHOD("resize", "capacity"), &CircularBuffer::resize);
	ClassDB::bind_method(D_METHOD("fill", "value"), &CircularBuffer::fill);

	ClassDB::bind_method(D_METHOD("append_array", "array"), &CircularBuffer::append_array);
	ClassDB::bind_method(D_METHOD("append", "value"), &CircularBuffer::append);

	ClassDB::bind_method(D_METHOD("at", "index"), &CircularBuffer::at);
	ClassDB::bind_method(D_METHOD("array_get", "index"), &CircularBuffer::array_get);
	ClassDB::bind_method(D_METHOD("array_set", "index", "value"), &CircularBuffer::array_set);
}
