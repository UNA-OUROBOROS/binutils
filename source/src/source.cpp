#include <BytesSerializer.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Foo {
	int32_t numero;
	char caracter;

	operator std::string() {
		return "{" + std::to_string(numero) + ", '" + caracter + "'}";
	}
};

struct complex {
	std::string complex_id;
	std::vector<Foo> foo;

	operator std::string() {
		std::stringstream s;
		s << "id: " << complex_id << "[" << foo.size() << "]\n";
		s << "[";
		for (Foo f : foo) {
			s << (std::string)f << " ";
		}
		s << "]\n";
		return s.str();
	}
};

template <> Bytes toBytes(Foo val) {
	Bytes bytes;
	appendBytes(toBytes(val.numero), bytes);
	appendBytes(toBytes(val.caracter), bytes);
	return bytes;
}
template <> Bytes toBytes(complex val) {
	Bytes bytes;
	appendBytes(toBytes(val.complex_id), bytes);
	appendBytes(vectorToBytes(val.foo), bytes);
	return bytes;
}

template <> Foo fromBytes(BytesIterator &first, BytesIterator &last) {
	return {
	    fromBytes<int32_t>(first, last), // numero
	    fromBytes<char>(first, last)     // caracter
	};
}
template <> complex fromBytes(BytesIterator &first, BytesIterator &last) {
	return {
	    fromBytes<std::string>(first, last), // complex_id
	    vectorFromBytes<Foo>(first, last)    // foo (vector)
	};
}

int main(int, char **) {
	std::string nombreArchivo = "datos.bin";
	{
		std::ofstream archivo(nombreArchivo, std::ofstream::out |
		                                         std::ofstream::trunc |
		                                         std::ofstream::binary);

		if (archivo) {
			Foo a{1, 'X'};
			Foo b{2, 'Y'};
			Foo c{3, 'Z'};
			complex bar{"complejo", {a, b, c}};

			archivo << toBytes(bar);
		} else {
			std::cerr << "no se pudo abrir el archivo\n";
			return 0;
		}
	}
	{
		std::ifstream entrada(nombreArchivo,
		                      std::ifstream::in | std::ifstream::binary);

		if (entrada.is_open()) {
			Bytes bytes;
			entrada >> bytes;
			BytesIterator begin = bytes.cbegin();
			BytesIterator end = bytes.cend();
			complex bar = fromBytes<complex>(begin, end);
			std::cout << (std::string)bar << "\n";
		}
	}
}
