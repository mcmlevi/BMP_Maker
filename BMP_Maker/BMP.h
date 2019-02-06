#pragma once
#include  <iostream>
#include  <fstream>
#include <sstream>
#include <string>
#include <random> // for std::mt19937
#include <ctime> // for std::time

class BMP
{
	// Public variables
public:
	unsigned long long m_width;
	unsigned long long m_widthPadding;
	unsigned long long m_height;
	// Public functions
public:
	// Constructors
	BMP();
	BMP(unsigned long long width, unsigned long long height);

	// Function to call when making the file
	void makefile();

	// Function to close current BMPfile
	void close_file();
	void makeHeader();
	void makeDIBHeader();
	void randomImage();


	// Private variables
private:
	std::stringstream sstream;
	std::ofstream bmpfile{};
	// Private Functions
private:
	std::string toLittleEndian(unsigned long long input);
};