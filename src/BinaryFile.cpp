#include "BinaryFile.h"

namespace scrap {

Endian BinaryFile::SystemEndian()
{
	unsigned u = 1;
	char *c = (char*)&u;
	if (c[0] == 1)
		return LE;
	return BE;
}


BinaryFile::BinaryFile(string filename, OpenMode mode)
	:	_fendian(UNDEF),
		_mode(mode)
{
	std::ios::openmode flags = std::ios::binary;
	if (_mode == WRITE)
		flags |= std::ios::out;
	else 
		flags |= std::ios::in;

	_file.open(filename, flags); 
	if (!_file.is_open()) 
		THROW(FileException, "Failed to open file " + filename);

	if (_mode == READ) {
		_file.seekg(0, _file.end);
		_size = _file.tellg();
		_file.seekg(0, _file.beg);
	}

	_endian = SystemEndian();
}

void BinaryFile::SetFileEndianess(Endian fendian)
{
	_fendian = fendian;
}

unsigned BinaryFile::RemainingBytes()
{
	if (_mode == WRITE)
		return 0;
	return _size - _file.tellg();
}


byte BinaryFile::ReadByte()
{
	FileReadCheck(sizeof(byte));
	return _file.get();
}

unsigned BinaryFile::ReadUnsigned()
{
	FileReadCheck(sizeof(unsigned));

	unsigned u;
	byte *p = (byte*)&u;

	for (int i=0; i<4; i++)
		p[i] = _file.get();

	return Convert(u);
}

string BinaryFile::ReadString()
{
	string str;

	FileReadCheck(1);
	char c = _file.get();
	while (c) {
		str += c;

		FileReadCheck(1);
		c = _file.get();
	}

	return str;
}


void BinaryFile::WriteByte(byte b)
{
	FileWriteCheck();
	_file.put(b);
}

void BinaryFile::WriteUnsigned(unsigned u)
{
	FileWriteCheck();

	u = Convert(u);
	byte *p = (byte*)&u;
	for (int i=0; i<4; i++) 
		_file.put(p[i]);
}

void BinaryFile::WriteString(string str)
{
	FileWriteCheck();

	for (int i=0; i<str.length(); i++) {
		_file.put(str[i]);
	}

	_file.put(0);
}


unsigned BinaryFile::Convert(unsigned u) const 
{
	if (_fendian == UNDEF)
		THROW(InvalidOperationException,
		"The endianess of the file must be defined before reading > 1 byte");
	if (_fendian == _endian)
		return u;
	
	byte *p, buf[4];
	p = (byte*)&u;

	buf[0] = p[3];
	buf[1] = p[2];
	buf[2] = p[1];
	buf[3] = p[0];

 	u = *buf;
	return u;
}

void BinaryFile::FileReadCheck(unsigned count)
{
	if (_mode != READ) {
		THROW(FileException, "Cannot read on BinaryFile in WRITE-format");
	}
	
	if (RemainingBytes() < count) {
		THROW(FileException, "Attempted to read past end of file");
	}

	if (!_file.good()) {
		THROW(FileException, "Error occurred when reading the file");
	}
}

void BinaryFile::FileWriteCheck()
{
	if (_mode != WRITE) {
		THROW(FileException, "Cannot write from BinaryFile in READ-mode");
	}

	if (!_file.good()) {
		THROW(FileException, "Filestatus is not good");
	}
}

}
