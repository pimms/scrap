#include "ScrapTest.h"

#define FILE_UNSIGNED	"testfiles/binary_unsigned"
#define FILE_STRING		"testfiles/binary_string"
#define FILE_TEMP		"testfiles/temp"

#define DELETE_TEMP() 	system("rm " FILE_TEMP)


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
		unsigned u = 1;
		BinaryFile out(FILE_TEMP, WRITE);
		out.SetFileEndianess(BinaryFile::SystemEndian());
		out.WriteUnsigned(u);
	}
	
	{
		unsigned u;
		BinaryFile in(FILE_TEMP, READ);
		in.SetFileEndianess(BinaryFile::SystemEndian());
		ASSERT_NO_THROW(u = in.ReadUnsigned());
		ASSERT_EQ(u, 1);
	}

	DELETE_TEMP();
}
