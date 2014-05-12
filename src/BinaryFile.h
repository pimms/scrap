#pragma once

#include "Scrap.h"
#include <fstream>

using std::fstream;


namespace scrap {

enum Endian {
	UNDEF		= 0x00,
	BE			= 0x01,
	B_ENDIAN	= 0x01,
	LE			= 0x02,
	L_ENDIAN	= 0x02,
};

enum OpenMode {
	WRITE	= 0x01,
	READ 	= 0x02,
};

/* Binary File
 * Endian sensitive reading and writing binary files.
 */
class BinaryFile {
public:
	BinaryFile(string filename, OpenMode mode);

	// The endianess of the file must be defined before any non-byte
	// values are read from it.
	void SetFileEndianess(Endian fendian);

	unsigned RemainingBytes();

	byte ReadByte();
	unsigned ReadUnsigned();
	string ReadString();

	void WriteByte(byte b);
	void WriteUnsigned(unsigned u);
	void WriteString(string s);

	static Endian SystemEndian();

private:
	unsigned Convert(unsigned u) const;

	// Ensure that there is at least "count" bytes left in the file
	void FileReadCheck(unsigned count);

	fstream _file;
	Endian _endian;		// Endianess of the system
	Endian _fendian;	// Endianess of the file

	OpenMode _mode;
	unsigned _size;		// Length of the file in bytes
};

}
