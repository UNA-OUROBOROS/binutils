#include <BytesSerializer.hpp>
#include <algorithm>
#include <istream>
#include <iterator>
#include <ostream>

// genericos

template <typename int_t, size_t bytes_n> Bytes toBytes(int_t val) {
	Bytes bytes;
	bytes.reserve(bytes_n);
	Byte *a_begin = reinterpret_cast<Byte *>(&val);
	Byte *a_end = a_begin + bytes_n;
	std::copy(a_begin, a_end, std::back_inserter(bytes));
	return bytes;
}
template <typename int_t, size_t bytes_n>
int_t fromBytes(BytesIterator &first, BytesIterator &last) {
	size_t len = std::distance(first, last);
	if (len >= bytes_n) {
		int_t val = 0;
		Byte val_bytes[bytes_n];
		// LSB -> MSB
		std::copy(first, first + bytes_n, val_bytes);
		first += bytes_n;
		std::memcpy(&val, val_bytes, sizeof(int_t));
		return val;
	} else {
		throw std::underflow_error("not enought bytes");
	}
}
template <> Bytes toBytes<Byte>(Byte val) {
	Bytes bytes;
	bytes.emplace_back(val);
	return bytes;
}
template <> Byte fromBytes<Byte>(BytesIterator &first, BytesIterator &last) {
	if (std::distance(first, last) > 0) {
		Byte val = *first;
		first++;
		return val;
	}
	throw std::underflow_error("not enought bytes to read");
}

// toBytes
template <> Bytes toBytes<int8_t>(int8_t val) {
	return toBytes<int8_t, 1>(val);
}
template <> Bytes toBytes<int16_t>(int16_t val) {
	return toBytes<int16_t, 2>(val);
}
template <> Bytes toBytes<int32_t>(int32_t val) {
	return toBytes<int32_t, 4>(val);
}
template <> Bytes toBytes<int64_t>(int64_t val) {
	return toBytes<int64_t, 8>(val);
}
template <> Bytes toBytes<float>(float val) { return toBytes<float, 4>(val); }
template <> Bytes toBytes<double>(double val) {
	return toBytes<double, 8>(val);
}
template <> Bytes toBytes<bool>(bool val) {
	return toBytes(static_cast<Byte>(val));
}
template <> Bytes toBytes<char>(char val) {
	return toBytes(static_cast<Byte>(val));
}

// fromBytes
template <>
int8_t fromBytes<int8_t>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<int8_t, 1>(first, last);
}
template <>
int16_t fromBytes<int16_t>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<int16_t, 2>(first, last);
}
template <>
int32_t fromBytes<int32_t>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<int32_t, 4>(first, last);
}
template <>
int64_t fromBytes<int64_t>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<int64_t, 8>(first, last);
}
template <> float fromBytes<float>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<float, 4>(first, last);
}
template <>
double fromBytes<double>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<double, 8>(first, last);
}
template <> bool fromBytes<bool>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<Byte>(first, last);
}
template <> char fromBytes<char>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<Byte>(first, last);
}

// sobrecarga de operador de ostream
std::ostream &operator<<(std::ostream &os, const Bytes &bytes) {
	for (Byte byte : bytes) {
		os << byte;
	}
	return os;
}

// sobrecarga de operador de istream
std::istream &operator>>(std::istream &is, Bytes &bytes) {
	// no ignorar nuevas lineas
	is.unsetf(std::ios::skipws);

	// recuperar el tamaño
	std::streampos isSize;

	is.seekg(0, std::ios::end);
	isSize = is.tellg();
	is.seekg(0, std::ios::beg);

	// reservar la capacidad capacity
	bytes.reserve(isSize);

	// copiar los bytes mediante un iterador
	bytes.insert(bytes.begin(), std::istream_iterator<Byte>(is),
	             std::istream_iterator<Byte>());
	return is;
}