#include "stdafx.h"
#include "BMP.h"

// Empty constructor
BMP::BMP() :m_width{}, m_height{}, m_widthPadding{}
{
	std::cout << "Please enter a width: ";
	std::cin >> m_width;
	std::cout << "Please enter a height: ";
	std::cin >> m_height;
	m_widthPadding = m_width;
	while (m_widthPadding % 4 != 0)
		m_widthPadding++;

	makefile();
}

// Constructor with user given width and height
BMP::BMP(unsigned long long width, unsigned long long height) :m_width{ width }, m_height{ height }, m_widthPadding{ width }
{
	while (m_widthPadding % 4 != 0)
		m_widthPadding++;

	makefile();
}

void BMP::makefile()
{
	std::string filename{};
	while (filename.find(".bmp") == std::string::npos)
	{
		std::cout << "Please enter the filename example.bmp: ";
		std::cin >> filename;
	}

	bmpfile = std::ofstream(filename);
	makeHeader();
	makeDIBHeader();
}

void BMP::close_file()
{
	bmpfile.close();
}

void BMP::makeHeader()
{
	// Header opening
	bmpfile << (char)0x42 << (char)0x4D;

	// Size of the file in bytes 4 bytes lenght ordered little edian

	// convert file size to little endian
	unsigned long long filesize{ 54 + m_widthPadding * m_height * 3 };

	std::string resultFileSize{ toLittleEndian(filesize) };

	// Export size to file
	bmpfile << (char)std::stoi(resultFileSize.substr(0, 2), nullptr, 16) << (char)std::stoi(resultFileSize.substr(2, 2), nullptr, 16) <<
		(char)std::stoi(resultFileSize.substr(4, 2), nullptr, 16) << (char)std::stoi(resultFileSize.substr(6, 2), nullptr, 16);

	// number used to indentify the aplication that made the bitmap
	bmpfile << (char)0x00 << (char)0x00 << (char)0x00 << (char)0x00;

	// starting adress of the pixel array
	bmpfile << (char)0x36 << (char)0x00 << (char)0x00 << (char)0x00;
}

void BMP::makeDIBHeader()
{
	// DIB header construction

	// Header size 40 bytes
	bmpfile << (char)0x28 << (char)0x00 << (char)0x00 << (char)0x00;

	// bitmap width in pixels signed interger

	std::string resultWidth = toLittleEndian(m_width);
	//Export width to file
	bmpfile << (char)std::stoi(resultWidth.substr(0, 2), nullptr, 16) << (char)std::stoi(resultWidth.substr(2, 2), nullptr, 16) <<
		(char)std::stoi(resultWidth.substr(4, 2), nullptr, 16) << (char)std::stoi(resultWidth.substr(6, 2), nullptr, 16);

	std::string resultHeight = toLittleEndian(m_height);

	// Export height to file
	bmpfile << (char)std::stoi(resultHeight.substr(0, 2), nullptr, 16) << (char)std::stoi(resultHeight.substr(2, 2), nullptr, 16) <<
		(char)std::stoi(resultHeight.substr(4, 2), nullptr, 16) << (char)std::stoi(resultHeight.substr(6, 2), nullptr, 16);

	// number of color panes (must be 1)
	bmpfile << (char)0x01 << (char)0x00;

	// the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
	bmpfile << (char)0x18 << (char)0x00;

	// compression method used (empty in this case)
	bmpfile << (char)0x00 << (char)0x00 << (char)0x00 << (char)0x00;

	// the image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
	bmpfile << (char)0x00 << (char)0x00 << (char)0x00 << (char)0x00;

	// the horizontal resolution of the image. (pixel per metre, signed integer) (similar to DPI)
	bmpfile << (char)0xc3 << (char)0x0E << (char)0x00 << (char)0x00;
	// 	the vertical resolution of the image. (pixel per metre, signed integer) (similar to DPI)
	bmpfile << (char)0xc3 << (char)0x0E << (char)0x00 << (char)0x00;

	// the number of colors in the color palette, or 0 to default to 2n
	bmpfile << (char)0x00 << (char)0x00 << (char)0x00 << (char)0x00;
	// the number of important colors used, or 0 when every color is important; generally ignored
	bmpfile << (char)0x00 << (char)0x00 << (char)0x00 << (char)0x00;
}

void BMP::randomImage()
{
	// Initialize our mersenne twister with a random seed based on the clock
	std::mt19937 mersenne(static_cast<unsigned int>(std::time(nullptr)));

	// Create a reusable random number generator that generates uniform numbers between 1 and 255
	std::uniform_int_distribution<> die(0, 255);

	// assemble image from bottom left, left to right, bottom to top
	for (int i = 0; i < m_height; i++)
	{
		for (int i2 = 0; i2 < m_width; i2++)
			bmpfile << (char)die(mersenne) << (char)die(mersenne) << (char)die(mersenne);

		// add padding each row
		switch ((m_width * 3) % 4)
		{
		case 0:
			break;
		case 1:
			bmpfile << (char)0x00 << (char)0x00 << (char)0x00;
			break;
		case 2:
			bmpfile << (char)0x00 << (char)0x00;
			break;
		case 3:
			bmpfile << (char)0x00;
			break;
		}
	}
}

std::string BMP::toLittleEndian(unsigned long long input)
{
	// empty string stream
	sstream.str("");

	unsigned long long byteSwappedNumber = (unsigned long long)_byteswap_uint64(input);
	// convert unsinged long long to string
	sstream << std::hex << byteSwappedNumber;
	std::string outString = sstream.str();

	return outString;
}