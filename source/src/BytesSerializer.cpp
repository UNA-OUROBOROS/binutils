#include <BytesSerializer.hpp>
#include <istream>
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
template <typename int_t, size_t bytes_n> int_t fromBytes(Bytes &bytes) {
	size_t len = bytes.size();
	if (len >= bytes_n) {
		int_t val = 0;
		Byte val_bytes[bytes_n];
		// LSB -> MSB
		for (size_t i = 0, pos = len - 1; i < bytes_n; i++, pos--) {
			val_bytes[(bytes_n - 1) - i] = bytes[pos];
			bytes.pop_back();
		}
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
template <> Byte fromBytes<Byte>(Bytes &bytes) {
	if (bytes.size() > 0) {
		Byte val = bytes[bytes.size() - 1];
		bytes.pop_back();
		return val;
	}
	throw std::underflow_error("not enought bytes to read");
}

// toBytes
template <> Bytes toBytes<int32_t>(int32_t val) {
	return toBytes<int32_t, 4>(val);
}
template <> Bytes toBytes<int64_t>(int64_t val) {
	return toBytes<int64_t, 8>(val);
}
template <> Bytes toBytes<char>(char val) {
	return toBytes(static_cast<Byte>(val));
}

// fromBytes
template <> int32_t fromBytes<int32_t>(Bytes &bytes) {
	return fromBytes<int32_t, 4>(bytes);
}
template <> int64_t fromBytes<int64_t>(Bytes &bytes) {
	return fromBytes<int64_t, 8>(bytes);
}
template <> char fromBytes<char>(Bytes &bytes) {
	return fromBytes<Byte>(bytes);
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