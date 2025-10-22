#include "ring_buffer.h"

bool CircularBuffer::is_empty() const {
	return _size == 0;
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

void CircularBuffer::clear() {
	// Do not actually clear anything, just reset head and size
	_head = 0;
	_size = 0;
}

void CircularBuffer::resize(const int p_capacity) {
	ERR_FAIL_COND(p_capacity < 0);

	_head = 0;
	_size = 0;
	data_array.resize(p_capacity);
	for (int i = 0; i < p_capacity; i++) {
		data_array[i] = Variant();
	}
}

void CircularBuffer::fill(const Variant &p_value) {
	const int s = data_array.size();
	if (s == 0) {
		return;
	}

	_head = 0;
	_size = s;
	for (int i = 0; i < s; i++) {
		data_array[i] = p_value;
	}
}

void CircularBuffer::advance(int p_steps) {
	const int s = data_array.size();
	if (s == 0) {
		return;
	}

	// Allow arbitrary large positive or negative values
	_head = (_head + (p_steps % s) + s) % s;
}

void CircularBuffer::append(const Variant &p_value) {
	const int s = data_array.size();
	if (s == 0) {
		return;
	}

	data_array[_head] = p_value;

	_head = (_head + 1) % s;
	if (_size < s) {
		_size++;
	}
}

void CircularBuffer::append_array(const Array &p_array) {
	const int s = data_array.size();
	if (s == 0) {
		return;
	}

	const int p_size = p_array.size();
	for (int i = 0; i < p_size; i++) {
		data_array[_head] = p_array[i];
		_head = (_head + 1) % s;
		if (_size < s) {
			_size++;
		}
	}
}

Variant CircularBuffer::front() const {
	ERR_FAIL_COND_V_MSG(_size == 0, Variant(), "Can't take value from empty array.");

	const int s = data_array.size();
	const int index = (_head - 1 + s) % s;
	return data_array[index];
}

Variant CircularBuffer::back() const {
	ERR_FAIL_COND_V_MSG(_size == 0, Variant(), "Can't take value from empty array.");

	const int s = data_array.size();
	const int index = (_head - _size + s) % s;
	return data_array[index];
}

Variant CircularBuffer::at(int p_index) const {
	const int s = data_array.size();

	// Index must be within bounds, from -size to size-1
	ERR_FAIL_COND_V_MSG(p_index < -_size || p_index >= _size, Variant(), "Index " + itos(p_index) + " is out of bounds (size: " + itos(_size) + ").");

	if (p_index < 0) {
		p_index += _size;
	}

	p_index = (s + _head - _size + p_index) % s;

	return data_array[p_index];
}

Variant CircularBuffer::array_get(int p_index) const {
	const int s = data_array.size();

	if (_size == 0 || s == 0 || p_index < 0 || p_index >= _size) {
		return Variant();
	}

	p_index = (s + _head - _size + p_index) % s;

	return data_array[p_index];
}

void CircularBuffer::array_set(int p_index, const Variant &p_value) {
	const int s = data_array.size();
	if (_size == 0 || s == 0 || p_index < 0 || p_index >= _size) {
		return;
	}

	p_index = (s + _head - _size + p_index) % s;

	data_array[p_index] = p_value;
}

Array CircularBuffer::slice(int p_begin, int p_end, int p_step) const {
	Array result;

	ERR_FAIL_COND_V_MSG(p_step == 0, result, "Slice step cannot be zero.");

	const int s = _size;

	if (s == 0 || (p_begin < -s && p_step < 0) || (p_begin >= s && p_step > 0)) {
		return result;
	}

	int begin = CLAMP(p_begin, -s, s - 1);
	if (begin < 0) {
		begin += s;
	}
	int end = CLAMP(p_end, -s - 1, s);
	if (end < 0) {
		end += s;
	}

	ERR_FAIL_COND_V_MSG(p_step > 0 && begin > end, result, "Slice step is positive, but bounds are decreasing.");
	ERR_FAIL_COND_V_MSG(p_step < 0 && begin < end, result, "Slice step is negative, but bounds are increasing.");

	int result_size = (end - begin) / p_step + (((end - begin) % p_step != 0) ? 1 : 0);
	result.resize(result_size);

	for (int src_idx = begin, dest_idx = 0; dest_idx < result_size; ++dest_idx) {
		result[dest_idx] = at(src_idx);
		src_idx += p_step;
	}

	return result;
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

	ClassDB::bind_method(D_METHOD("front"), &CircularBuffer::front);
	ClassDB::bind_method(D_METHOD("back"), &CircularBuffer::back);

	ClassDB::bind_method(D_METHOD("append_array", "array"), &CircularBuffer::append_array);
	ClassDB::bind_method(D_METHOD("append", "value"), &CircularBuffer::append);

	ClassDB::bind_method(D_METHOD("at", "index"), &CircularBuffer::at);
	ClassDB::bind_method(D_METHOD("array_get", "index"), &CircularBuffer::array_get);
	ClassDB::bind_method(D_METHOD("array_set", "index", "value"), &CircularBuffer::array_set);

	ClassDB::bind_method(D_METHOD("slice", "begin", "end", "step"), &CircularBuffer::slice, DEFVAL(INT_MAX), DEFVAL(1));
}
