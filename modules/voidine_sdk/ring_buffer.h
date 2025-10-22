#pragma once

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/templates/local_vector.h"
#include "core/templates/ring_buffer.h"
#include "core/variant/typed_array.h"
#include "core/variant/variant.h"

class CircularBuffer : public RefCounted {
	GDCLASS(CircularBuffer, RefCounted)

private:
	int64_t _size;
	uint64_t _head;
	LocalVector<Variant> data_array;

protected:
	static void _bind_methods();

public:
	void clear();
	void resize(const int p_capacity);
	void fill(const Variant &p_value);

	bool is_empty() const;
	int capacity() const;
	int size() const;
	int head() const;

	void advance(const int p_steps = 1);

	void append(const Variant &p_value);
	void append_array(const Array &p_array);

	// Index from -size to size-1, throw if out bounds
	Variant at(const int p_index) const;

	// Identical to [] operator, but return null on out of bounds
	Variant array_get(const int p_index) const;

	// Identical to [] operator, but does nothing on out of bounds
	void array_set(const int p_index, const Variant &p_value);

	CircularBuffer() = default;
};
