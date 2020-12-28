
#ifndef __aBitmapMan_h__
#define __aBitmapMan_h__

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

#define PALETTEPOS 54
#define MONO(rd,gn,bl) ( ((int)(rd)*11 + (int)(gn)*16 + (int)(bl)*5) >> 5)
#define RANGE(x,Li,Ls) { if (x < Li) x = Li;  if (x > Ls) x = Ls; }

const char ErrorsBitMap[11][57] =
{
	"0: There was no error",
	"1: Error opening .bmp file. Possible cause: do not exist",
	"2: Error reading BitMapFileHeader from file",
	"3: Failed to close the file",
	"4: Error moving file pointer",
	"5: Error reading BitMapInfoHeader",
	"6: There is no memory to read the palette",
	"7: Error reading palette from image",
	"8: Wrong BMP file format",
	"9: No memory to read the image",
	"10: Error reading image"
};

typedef struct RGB_type
{
	BYTE red, green, blue;
} RGB_type;

class BitmapMan
{

protected:
	char filename[80];     // File name on disk
	char bfType[3];        // ASCII "BM"
	int bfSize;           // File size in bytes (Longword)
	int bfOffBits;        // Distance in bytes from the end of the header.
						  // (byte 14) where the image as such begins	                  
	int ErrorCode;		 // Integer denoting the error code when reading
						 // from the file (see error table)
	int biSize;          // Header size (40 bytes)
	int biWidth;         // Image width in pixels
	int biHeight;        // Image length in pixels
	short int  biPlanes;        // Number of planes in the image. It should be 1
	short int  biBitCount;      // Bits per pixel
	int  biCompression;   // Compression type
	int biSizeImage;     // Size in bytes of the compressed image
	int biXPelsPerMeter; // Horizontal resolution in pixels / meters
	int biYPelsPerMeter; // Vertical horizontal in pixels / meters
	int biClrUsed;       // Number of colors used
	int biClrImportant;  // Number of "important" colors
	int PaletteSize;     // Number of entries in color map. It is deduced
	BYTE *bmiColors, 	 // Color Map (blue,green,red,0) ...
		*image;	 // Bytes corresponding to the image

public:
	int GetPaletteSize();
	BYTE * GetPalette();

	BitmapMan();
	// Default constructor

	BitmapMan(const char theFilename[]);
	// Constructor given the name of a file

	BitmapMan(const BitmapMan& source);
	// Copy Constructor

	~BitmapMan();
	// Destructor

	int LoadImage(const char theFilename[]);
	// Load image into memory

	static int LoadDimensions(const char filename[], int &width, int &height);
	// Read bitmap dimensions from disk, without loading image
	// Returns 0 if there is no error.

	inline int Width(void);
	// Width in pixels

	inline int Height(void);
	// Height in pixels

	inline int theError(void);
	// Returns if there was an error code when reading

	inline short int BitsPerPixel(void);
	// Returns the number of bits per pixel in the image

	inline int GetNColors(void);


	void PrintColorMap();
	// Print the palette as tuples (R,G,B)

	void PrintHeader();
	// Prints the bitmap header

	int  SaveBMP(const char *filename);
	// Save the BMP to disk with filename
	// Returns 0 if there are no problems, or any error code
	// otherwise...

	void FreeBitMap();

	int SetRGBData(int nRows, int nCols, void* data);
	int SetRGBAData(int nRows, int nCols, void* data);
	int Set8bitsData(int nRows, int nCols, void *data, void *palette);
	int Set2bitsData(int nRows, int nCols, void *data, void *palette);
	void SetPalette(BYTE *newPalete);
	void *getBits();
	bool LoadResource(UINT resourceNumber);

private:
	BYTE GetErodePixelBAW(int i, int j, int n, int count);
	// Returns the erode of pixel i,j with matrix nxn
	// and with blank counter == count

};

/////////////////////////////////////////////////////
// Inline functions that must be defined in the .h //
/////////////////////////////////////////////////////

inline int BitmapMan::Width(void)
{
	return biWidth;
}

inline int BitmapMan::Height(void)
{
	return biHeight;
}

inline int BitmapMan::theError(void)
{
	return ErrorCode;
}

inline short int BitmapMan::BitsPerPixel(void)
{
	return biBitCount;
}

inline int BitmapMan::GetNColors(void)
{
	return PaletteSize;
}

#endif
