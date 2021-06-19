#include <BytesSerializer.hpp>
#include <fstream>
#include <iostream>

struct Foo {
	int32_t numero;
	char caracter;
};

template <> Bytes toBytes(Foo val) {
	Bytes bytes;
	appendBytes(toBytes(val.numero), bytes);
	appendBytes(toBytes(val.caracter), bytes);
	return bytes;
}

template <> Foo fromBytes(Bytes &bytes) {
	// es obligatorio leer en el orden inverso
	// aunque podríamos invertir los bytes no vale la pena
	char caracter = fromBytes<char>(bytes);
	int32_t numero = fromBytes<int32_t>(bytes);

	return {numero, caracter};
}

int main(int, char **) {
	std::string nombreArchivo = "datos.bin";
	int32_t b = 1;
	char c = 'Z';
	std::cout << "b: " << b << "\n"
	          << "c: " << c << "\n";
	{

		std::ofstream archivo(nombreArchivo, std::ofstream::out |
		                                         std::ofstream::trunc |
		                                         std::ofstream::binary);

		if (archivo) {
			Foo a{b, c};

			archivo << toBytes(a);
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
			Foo val = fromBytes<Foo>(bytes);
			std::cout << "val {" << val.numero << ", " << val.caracter << "}.\n";
		}
	}
}
