#include <BytesSerializer.hpp>
#include <array>
#include <iostream>
#include <iterator>
#include <stdexcept>


using Byte = std::uint8_t;
using Bytes = std::vector<Byte>;

template <> Bytes toBytes<int32_t>(int32_t val) {
	Bytes bytes;
	bytes.reserve(4);
	Byte *a_begin = reinterpret_cast<Byte *>(&val);
	Byte *a_end = a_begin + 4;
	std::copy(a_begin, a_end, std::back_inserter(bytes));
	return bytes;
}

template <> Bytes toBytes<char>(char val) {
	return toBytes(static_cast<Byte>(val));
}

template <> Bytes toBytes<Byte>(Byte val) {
	Bytes bytes;
	bytes.emplace_back(val);
	return bytes;
}

template <> int32_t fromBytes<int32_t>(Bytes &bytes) {
	size_t len = bytes.size();
	if (len >= 4) {
		int32_t val = 0;
		Byte val_bytes[4];
		// LSB -> MSB
		for (size_t i = 0, pos = len - 1; i < 4; i++, pos--) {
			val_bytes[3 - i] = bytes[pos];
			bytes.pop_back();
		}
		std::memcpy(&val, val_bytes, sizeof(int32_t));
		return val;
	} else {
		throw std::underflow_error("not enought bytes");
	}
}

template <> char fromBytes<char>(Bytes &bytes) {
	return fromBytes<Byte>(bytes);
}
template <> Byte fromBytes<Byte>(Bytes &bytes) {
	if (bytes.size() > 0) {
		Byte val = bytes[bytes.size() - 1];
		bytes.pop_back();
		return val;
	}
	throw std::underflow_error("not enought bytes to read");
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