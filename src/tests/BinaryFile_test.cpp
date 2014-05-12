#include "ScrapTest.h"

#define FILE_UNSIGNED	"testfiles/binary_unsigned"
#define FILE_STRING		"testfiles/binary_string"
#define FILE_TEMP		"testfiles/temp"

#define DELETE_TEMP() 	system("rm -f " FILE_TEMP)


TEST (BinaryFileTest, TestInvalidFile)
{
	ASSERT_ANY_THROW(BinaryFile("HEY HO AND A BUCKET OF RUM", READ));
}

TEST (BinaryFileTest, TestValidFile)
{
	// This also ensures that the two test files exist
	ASSERT_NO_THROW(BinaryFile(FILE_UNSIGNED, READ));
	ASSERT_NO_THROW(BinaryFile(FILE_STRING, READ));
}

TEST (BinaryFileTest, TestReadString)
{
	BinaryFile file(FILE_STRING, READ);

	string str = file.ReadString();
	ASSERT_EQ(str, "SCRAP YO");
}

TEST (BinaryFileTest, TestReadUnsigned)
{
	Endian endian;
	unsigned u;

	BinaryFile file(FILE_UNSIGNED, READ);

	// The file contains two unsigned values, prefixed with different
	// endian types. The unsigned values does in all cases contain the
	// number 1.
	for (int i=0; i<2; i++) {
		ASSERT_NO_THROW({
			endian = (Endian)file.ReadByte();
			file.SetFileEndianess(endian);
			u = file.ReadUnsigned();
			ASSERT_EQ(u, 1);
		});
	}
}

TEST (BinaryFileTest, TestReadTooMuch)
{
	BinaryFile file(FILE_STRING, READ);
	string str = file.ReadString();

	// The file contains no more than the string
	ASSERT_EQ(file.RemainingBytes(), 0);
	ASSERT_ANY_THROW(file.ReadByte());
}


TEST (BinaryFileTest, TestWriteCorrectEndian)
{
	DELETE_TEMP();

	{
		BinaryFile out(FILE_TEMP, WRITE);
		out.SetFileEndianess(BinaryFile::SystemEndian());
		out.WriteUnsigned(1);
	}
	
	{
		unsigned u = 0;
		BinaryFile in(FILE_TEMP, READ);
		in.SetFileEndianess(BinaryFile::SystemEndian());
		ASSERT_NO_THROW(u = in.ReadUnsigned());
		ASSERT_EQ(u, 1);
	}

	DELETE_TEMP();
}

TEST (BinaryFileTest, TestWriteOtherEndian)
{
	DELETE_TEMP();
	Endian otherEndian = BinaryFile::SystemEndian();
	otherEndian = ((otherEndian ==LE) ? (BE) : (LE));

	{
		BinaryFile out(FILE_TEMP, WRITE);
		out.SetFileEndianess(otherEndian);

		unsigned u = 0;
		byte *p = (byte*)&u;
		if (otherEndian == L_ENDIAN)
			p[0] = 1;
		else
			p[3] = 1;
		out.WriteUnsigned(u);
	}
	
	{
		unsigned u = 0;
		BinaryFile in(FILE_TEMP, READ);
		in.SetFileEndianess(BinaryFile::SystemEndian());
		ASSERT_NO_THROW(u = in.ReadUnsigned());
		ASSERT_EQ(u, 1);
	}

	DELETE_TEMP();
}

TEST (BinaryFileTest, TestWriteString)
{
	DELETE_TEMP();
	string str = "Scrappy dappy dapp";

	{
		BinaryFile file(FILE_TEMP, WRITE);
		file.WriteString(str);
	}
	{
		BinaryFile file(FILE_TEMP, READ);
		ASSERT_EQ(file.ReadString(), str);
	}

	DELETE_TEMP();
}

TEST (BinaryFileTest, TestWriteByte)
{
	DELETE_TEMP();

	{
		BinaryFile file(FILE_TEMP, WRITE);
		file.WriteByte(0x0F);
	}
	{
		BinaryFile file(FILE_TEMP, READ);
		ASSERT_EQ(file.ReadByte(), 0x0F);
	}

	DELETE_TEMP();
}


TEST (BinaryFileTest, WriteInReadMode)
{
	DELETE_TEMP();
	
	// Write some test data to ensure that opening the file
	// is actually possible
	{
		BinaryFile file(FILE_TEMP, WRITE);
		file.WriteString("This will never be read, lol");
	}

	BinaryFile file(FILE_TEMP, READ);
	file.SetFileEndianess(BinaryFile::SystemEndian());
	
	ASSERT_ANY_THROW(file.WriteByte(0x0F));
	ASSERT_ANY_THROW(file.WriteUnsigned(1));
	ASSERT_ANY_THROW(file.WriteString("hey hey"));

	DELETE_TEMP();
}

TEST (BinaryFileTest, ReadInWriteMode)
{
	DELETE_TEMP();

	// Write some test data to ensure that reading will work if 
	// things doesn't work.
	{
		BinaryFile file(FILE_TEMP, WRITE);
		file.WriteString("This will never be read, lol");
	}

	BinaryFile file(FILE_TEMP, WRITE);
	file.SetFileEndianess(BinaryFile::SystemEndian());
	
	// No bytes remain when writing
	ASSERT_EQ(file.RemainingBytes(), 0);
	
	ASSERT_ANY_THROW(file.ReadByte());
	ASSERT_ANY_THROW(file.ReadUnsigned());
	ASSERT_ANY_THROW(file.ReadString());

	DELETE_TEMP();
}
