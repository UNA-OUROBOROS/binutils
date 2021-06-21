#include <BytesSerializer.hpp>
#include <algorithm>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

// genericos

template <typename T> Bytes rawToBytes(T val) {
	Bytes bytes;
	size_t len = sizeof(T);
	bytes.reserve(len);
	Byte *a_begin = reinterpret_cast<Byte *>(&val);
	Byte *a_end = a_begin + len;
	std::copy(a_begin, a_end, std::back_inserter(bytes));
	return bytes;
}
template <typename T>
T rawFromBytes(BytesIterator &first, BytesIterator &last) {
	size_t len = std::distance(first, last);
	size_t bytes_n = sizeof(T);
	if (len >= bytes_n) {
		T val;
		Byte val_bytes[bytes_n];
		// LSB -> MSB
		std::copy(first, first + bytes_n, val_bytes);
		first += bytes_n;
		std::memcpy(&val, val_bytes, sizeof(T));
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
// genericos

// especializaciones de escalares
template <> Bytes toBytes<int8_t>(int8_t val) {
	return rawToBytes<int8_t>(val);
}
template <> Bytes toBytes<int16_t>(int16_t val) {
	return rawToBytes<int16_t>(val);
}
template <> Bytes toBytes<int32_t>(int32_t val) {
	return rawToBytes<int32_t>(val);
}
template <> Bytes toBytes<int64_t>(int64_t val) {
	return rawToBytes<int64_t>(val);
}
template <> Bytes toBytes<float>(float val) { return rawToBytes<float>(val); }
template <> Bytes toBytes<double>(double val) {
	return rawToBytes<double>(val);
}
template <> Bytes toBytes<bool>(bool val) {
	return toBytes(static_cast<Byte>(val));
}
template <> Bytes toBytes<char>(char val) {
	return toBytes(static_cast<Byte>(val));
}

// fromBytes
// genericos

// especializacion de escalares
template <>
int8_t fromBytes<int8_t>(BytesIterator &first, BytesIterator &last) {
	return rawFromBytes<int8_t>(first, last);
}
template <>
int16_t fromBytes<int16_t>(BytesIterator &first, BytesIterator &last) {
	return rawFromBytes<int16_t>(first, last);
}
template <>
int32_t fromBytes<int32_t>(BytesIterator &first, BytesIterator &last) {
	return rawFromBytes<int32_t>(first, last);
}
template <>
int64_t fromBytes<int64_t>(BytesIterator &first, BytesIterator &last) {
	return rawFromBytes<int64_t>(first, last);
}
template <> float fromBytes<float>(BytesIterator &first, BytesIterator &last) {
	return rawFromBytes<float>(first, last);
}
template <>
double fromBytes<double>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<double>(first, last);
}
template <> bool fromBytes<bool>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<Byte>(first, last);
}
template <> char fromBytes<char>(BytesIterator &first, BytesIterator &last) {
	return fromBytes<Byte>(first, last);
}

// arreglos y cadenas

// toBytes
template <typename T> Bytes vectorToBytes(std::vector<T> val);
template <> Bytes toBytes(std::string val) {
	Bytes bytes;
	appendBytes(rawToBytes(val.size()), bytes);
	std::copy(val.begin(), val.end(), std::back_inserter(bytes));
	return bytes;
}

// fromBytes

template <> std::string fromBytes(BytesIterator &first, BytesIterator &last) {
	size_t len = rawFromBytes<size_t>(first, last);
	std::string str;
	str.reserve(len);
	std::copy(first, first + len, std::back_inserter(str));
	first += len;
	return str;
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