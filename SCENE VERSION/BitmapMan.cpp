#include "BitmapMan.h"

BitmapMan::BitmapMan()
{
	memset(filename, 0, 80);
	memset(bfType, 0, 3);

	bfSize = biSize = biWidth = biHeight = 0;
	biPlanes = biBitCount = 0;
	bfOffBits = ErrorCode = biCompression = biSizeImage =
		biXPelsPerMeter = biYPelsPerMeter = biClrUsed =
		biClrImportant = PaletteSize = 0;
	bmiColors = image = NULL;
}

int BitmapMan::LoadDimensions(const char filename[], int &width, int &height)
{
	FILE *f;
	fopen_s(&f, filename, "wb");
	if (f == NULL)
		return 1;
	if (fseek(f, 18, SEEK_SET))
	{
		fclose(f);
		return 2;
	}
	if (fread(&width, 4, 1, f) != 1 || fread(&height, 4, 1, f) != 1)
	{
		fclose(f);
		return 3;
	}
#ifdef __CHANGE_ENDIAN__
	width = intChangeEndian(width);
	height = intChangeEndian(height);
#endif
	fclose(f);
	return 0;
}

BitmapMan::BitmapMan(const char theFilename[])
{
	bmiColors = image = NULL;
	ErrorCode = LoadImage(theFilename);
}

BitmapMan::BitmapMan(const BitmapMan& source)
{
	int longitud;

	strcpy_s<80>(filename, source.filename);
	memcpy(bfType, source.bfType, 3);
	bfSize = source.bfSize;
	bfOffBits = source.bfOffBits;
	ErrorCode = source.ErrorCode;
	biSize = source.biSize;
	biWidth = source.biWidth;
	biHeight = source.biHeight;
	biPlanes = source.biPlanes;
	biBitCount = source.biBitCount;
	biCompression = source.biCompression;
	biSizeImage = source.biSizeImage;
	biXPelsPerMeter = source.biXPelsPerMeter;
	biYPelsPerMeter = source.biYPelsPerMeter;
	biClrUsed = source.biClrUsed;
	biClrImportant = source.biClrImportant;
	PaletteSize = source.PaletteSize;
	image = NULL;
	bmiColors = NULL;
	if (PaletteSize > 0)
	{
		bmiColors = new BYTE[4 * PaletteSize];
		if (bmiColors == NULL)
		{
			ErrorCode = 6;
			return;
		}
		memcpy(bmiColors, source.bmiColors, 4 * PaletteSize);
	}
	longitud = (biSizeImage > 0) ? biSizeImage : bfSize - bfOffBits;
	image = new BYTE[longitud];
	if (image == NULL)
	{
		ErrorCode = 9;
		return;
	}
	memcpy(image, source.image, longitud);
}


void BitmapMan::FreeBitMap()
{
	if (bmiColors != NULL) delete[](BYTE *)bmiColors;
	if (image != NULL) delete[](BYTE *)image;
	image = NULL;
	bmiColors = NULL;
	biWidth = 0;
}

BitmapMan::~BitmapMan()
{
	FreeBitMap();
}

void BitmapMan::PrintHeader()
{
	printf("\n%s\n", filename);
	printf("------------------------------\n");
	printf("bfType              = %s\n", bfType);
	printf("bfSize              = %ld\n", bfSize);
	printf("bfOffBits           = %ld\n", bfOffBits);
	printf("biSize              = %ld\n", biSize);
	printf("biWidth             = %ld\n", biWidth);
	printf("biHeight            = %ld\n", biHeight);
	printf("biPlanes            = %hd\n", biPlanes);
	printf("biBitCount          = %hd\n", biBitCount);
	printf("biCompression       = %ld\n", biCompression);
	printf("biSizeImage         = %ld\n", biSizeImage);
	printf("biXPelsPerMeter     = %ld\n", biXPelsPerMeter);
	printf("biYPelsPerMeter     = %ld\n", biYPelsPerMeter);
	printf("biClrUsed           = %ld\n", biClrUsed);
	printf("biClrImportant      = %ld\n", biClrImportant);
	printf("Press <Enter>\n");
	fflush(stdin);
	getchar();
}

void BitmapMan::PrintColorMap()
{
	int i;

	printf("\nColor Map\n---------\n");
	/*
	for (i=0; i<PaletteSize; i++)
	printf("Color %3d = (%3d,%3d,%3d)\n", i,
	bmiColors[i*4+2], bmiColors[i*4+1],  bmiColors[i*4]);
	*/
	for (i = 0; i<PaletteSize; i++)
		printf("Color %3d = (%3d,%3d,%3d,%3d)\n", i,
			bmiColors[i * 4], bmiColors[i * 4 + 1],
			bmiColors[i * 4 + 2], bmiColors[i * 4 + 3]);
	printf("Press <Enter>\n");
	fflush(stdin);
	getchar();
}


int BitmapMan::SaveBMP(const char *filename)
{
	FILE *f;
	int cero = 0;
	size_t longitud = bfSize - bfOffBits;
	bfOffBits = 54 + PaletteSize * 4;
	bfSize = bfOffBits + longitud;
	//size_t longitud=(biSizeImage > 0) ? biSizeImage : bfSize-bfOffBits;

	fopen_s(&f, filename, "wb");
	if (f == NULL)
	{
		ErrorCode = 1;
		return ErrorCode;
	}
	if (fwrite(&bfType, 1, 2, f) != 2 || fwrite(&bfSize, 4, 1, f) != 1)
	{
		ErrorCode = 2;
		fclose(f);
		return ErrorCode;
	}
	if (fwrite(&cero, 4, 1, f) != 1)
	{
		ErrorCode = 2;
		fclose(f);
		return ErrorCode;
	}

#ifdef __CHANGE_ENDIAN__
	bfSize = intChangeEndian(bfSize);
	bfOffBits = intChangeEndian(bfOffBits);
	biSize = intChangeEndian(biSize);
	biWidth = intChangeEndian(biWidth);
	biHeight = intChangeEndian(biHeight);
	biPlanes = short intChangeEndian(biPlanes);
	biBitCount = short intChangeEndian(biBitCount);
	biCompression = intChangeEndian(biCompression);
	biSizeImage = intChangeEndian(biSizeImage);
	biXPelsPerMeter = intChangeEndian(biXPelsPerMeter);
	biYPelsPerMeter = intChangeEndian(biYPelsPerMeter);
	biClrUsed = intChangeEndian(biClrUsed);
	biClrImportant = intChangeEndian(biClrImportant);
#endif

	if (fwrite(&bfOffBits, 4, 1, f) != 1 ||
		fwrite(&biSize, 4, 1, f) != 1 ||
		fwrite(&biWidth, 4, 1, f) != 1 ||
		fwrite(&biHeight, 4, 1, f) != 1 ||
		fwrite(&biPlanes, 2, 1, f) != 1 ||
		fwrite(&biBitCount, 2, 1, f) != 1 ||
		fwrite(&biCompression, 4, 1, f) != 1 ||
		fwrite(&biSizeImage, 4, 1, f) != 1 ||
		fwrite(&biXPelsPerMeter, 4, 1, f) != 1 ||
		fwrite(&biYPelsPerMeter, 4, 1, f) != 1 ||
		fwrite(&biClrUsed, 4, 1, f) != 1 ||
		fwrite(&biClrImportant, 4, 1, f) != 1)
	{
		ErrorCode = 5;
		fclose(f);
		return ErrorCode;
	}

#ifdef __CHANGE_ENDIAN__
	bfSize = intChangeEndian(bfSize);
	bfOffBits = intChangeEndian(bfOffBits);
	biSize = intChangeEndian(biSize);
	biWidth = intChangeEndian(biWidth);
	biHeight = intChangeEndian(biHeight);
	biPlanes = short intChangeEndian(biPlanes);
	biBitCount = short intChangeEndian(biBitCount);
	biCompression = intChangeEndian(biCompression);
	biSizeImage = intChangeEndian(biSizeImage);
	biXPelsPerMeter = intChangeEndian(biXPelsPerMeter);
	biYPelsPerMeter = intChangeEndian(biYPelsPerMeter);
	biClrUsed = intChangeEndian(biClrUsed);
	biClrImportant = intChangeEndian(biClrImportant);
#endif

	if (biBitCount != 24)
	{
		if (fwrite(bmiColors, 1, (unsigned int)4 * PaletteSize, f) != (unsigned int)4 * PaletteSize)
		{
			ErrorCode = 7;
			fclose(f);
			return ErrorCode;
		}
	}
	if (fwrite(image, 1, longitud, f) != longitud)
	{
		ErrorCode = 10;
		fclose(f);
		return ErrorCode;
	}
	if (fclose(f) == -1)
	{
		ErrorCode = 3;
		fclose(f);
		return ErrorCode;
	}
	ErrorCode = 0;
	return ErrorCode;
}


int BitmapMan::SetRGBData(int nRows, int nCols, void* data)
{
	bfType[0] = 'B'; bfType[1] = 'M';  bfType[2] = 0;
	bfOffBits = 54;	// + nothing because there is not palette
	biSize = 40;
	biWidth = nCols;
	biHeight = nRows;
	biPlanes = 1;
	biBitCount = 24;
	biCompression = 0;
	biSizeImage = 0;
	biXPelsPerMeter = 0;
	biYPelsPerMeter = 0;
	biClrUsed = 0;
	biClrImportant = 0;
	PaletteSize = 0;
	bmiColors = NULL;
	int lineWidth = biWidth * 3;
	if (lineWidth & 3)
		lineWidth += 4 - (lineWidth & 3);
	image = new BYTE[lineWidth*biHeight];
	memcpy(image, data, lineWidth*biHeight);
	biSizeImage = lineWidth*biHeight;
	bfSize = bfOffBits + biSizeImage;
	return 0;
}

int BitmapMan::SetRGBAData(int nRows, int nCols, void* data)
{
	bfType[0] = 'B'; bfType[1] = 'M';  bfType[2] = 0;
	bfOffBits = 54;	// + nothing because there is not palette
	biSize = 40;
	biWidth = nCols;
	biHeight = nRows;
	biPlanes = 1;
	biBitCount = 32;
	biCompression = 0;
	biSizeImage = 0;
	biXPelsPerMeter = 0;
	biYPelsPerMeter = 0;
	biClrUsed = 0;
	biClrImportant = 0;
	PaletteSize = 0;
	bmiColors = NULL;
	int lineWidth = biWidth * 4;
	image = new BYTE[lineWidth*biHeight];
	memcpy(image, data, lineWidth*biHeight);
	biSizeImage = lineWidth*biHeight;
	bfSize = bfOffBits + biSizeImage;
	return 0;
}
int BitmapMan::Set8bitsData(int nRows, int nCols, void *data, void *palette)
{
	bfType[0] = 'B'; bfType[1] = 'M';  bfType[2] = 0;
	bfOffBits = 54 + 1024;	// +1024 (palette size)
	biSize = 40;
	biWidth = nCols;
	biHeight = nRows;
	biPlanes = 1;
	biBitCount = 8;
	biCompression = 0;
	biXPelsPerMeter = 0;
	biYPelsPerMeter = 0;
	biClrUsed = 0;
	biClrImportant = 256;	// all colors are important
	PaletteSize = 256;
	bmiColors = new BYTE[1024];
	memcpy(bmiColors, palette, 1024);
	int realWidth = biWidth;
	if (realWidth & 3)
		realWidth += 4 - (realWidth & 3);
	image = new BYTE[realWidth*biHeight];
	if (image == NULL)
		return 1;
	memcpy(image, data, realWidth*biHeight);
	biSizeImage = realWidth*biHeight;
	bfSize = bfOffBits + biSizeImage;
	return 0;
}

int BitmapMan::Set2bitsData(int nRows, int nCols, void *data, void *palette)
{
	bfType[0] = 'B'; bfType[1] = 'M';  bfType[2] = 0;
	bfOffBits = 54 + 8;	//  +8 (palette size)
	biSize = 40;
	biWidth = nCols;
	biHeight = nRows;
	biPlanes = 1;
	biBitCount = 1;
	biCompression = 0;
	biXPelsPerMeter = 0;
	biYPelsPerMeter = 0;
	biClrUsed = 0;
	biClrImportant = 0;	// all ones
	PaletteSize = 2;
	bmiColors = new BYTE[8];
	memcpy(bmiColors, palette, 8);
	int nBitsPerRow = biWidth;
	if ((biWidth % 32) != 0)
		nBitsPerRow += 32 - biWidth % 32;
	int realWidth = nBitsPerRow / 8;
	image = new BYTE[realWidth*biHeight];
	if (image == NULL)
		return 1;
	memcpy(image, data, realWidth*biHeight);
	biSizeImage = realWidth*biHeight;
	bfSize = bfOffBits + biSizeImage;
	return 0;
}

int BitmapMan::LoadImage(const char theFilename[])
{
	FILE *f;
	size_t longitud;

	FreeBitMap();
	strcpy_s<80>(filename, theFilename);
	bmiColors = NULL;
	image = NULL;
	fopen_s(&f, filename, "rb");
	if (f == NULL)
	{
		ErrorCode = 1;
		return ErrorCode;
	}
	bfType[2] = 0x00;
	if (fread(&bfType, 2, 1, f) != 1 || fread(&bfSize, 4, 1, f) != 1)
	{
		ErrorCode = 2;
		fclose(f);
		return ErrorCode;
	}
	/*
	if (fseek(f, 10, SEEK_SET))
	{
	ErrorCode = 4;
	fclose(f);
	return;
	}
	*/
	fread(&bfOffBits, 4, 1, f);
	// byte 10
	if (fread(&bfOffBits, 4, 1, f) != 1 ||
		fread(&biSize, 4, 1, f) != 1 ||
		fread(&biWidth, 4, 1, f) != 1 ||
		fread(&biHeight, 4, 1, f) != 1 ||
		fread(&biPlanes, 2, 1, f) != 1 ||
		fread(&biBitCount, 2, 1, f) != 1 ||
		fread(&biCompression, 4, 1, f) != 1 ||
		fread(&biSizeImage, 4, 1, f) != 1 ||
		fread(&biXPelsPerMeter, 4, 1, f) != 1 ||
		fread(&biYPelsPerMeter, 4, 1, f) != 1 ||
		fread(&biClrUsed, 4, 1, f) != 1 ||
		fread(&biClrImportant, 4, 1, f) != 1)
	{
		ErrorCode = 5;
		fclose(f);
		return ErrorCode;
	}


	if (biBitCount != 24)
	{
		PaletteSize = (biClrUsed != 0) ? biClrUsed : (1 << biBitCount);
		if (PaletteSize > 0)
		{
			bmiColors = new BYTE[4 * PaletteSize];
			if (bmiColors == NULL)
			{
				ErrorCode = 6;
				fclose(f);
				return ErrorCode;
			}
			if (fread(bmiColors, 1, 4 * PaletteSize, f) != (unsigned int)4 * PaletteSize)
			{
				ErrorCode = 7;
				fclose(f);
				return ErrorCode;
			}
		}
		else
		{
			ErrorCode = 8;
			fclose(f);
			return ErrorCode;
		}
	}
	else
		PaletteSize = 0;
	if (fseek(f, bfOffBits, SEEK_SET))
	{
		ErrorCode = 10;
		fclose(f);
		return ErrorCode;
	}
	longitud = (biSizeImage > 0) ? biSizeImage : bfSize - bfOffBits;
	image = new BYTE[longitud];
	if (image == NULL)
	{
		ErrorCode = 9;
		fclose(f);
		return ErrorCode;
	}
	if (fread(image, 1, longitud, f) != longitud)
	{
		ErrorCode = 10;
		fclose(f);
		return ErrorCode;
	}
	if (fclose(f) == -1)
	{
		ErrorCode = 3;
		fclose(f);
		return ErrorCode;
	}
	ErrorCode = 0;
	return ErrorCode;
}

void BitmapMan::SetPalette(BYTE *newPalete)
{
	if (biBitCount != 24)
		memcpy(bmiColors, newPalete, PaletteSize * 4);
}

void *BitmapMan::getBits()
{
	return (void *)image;
}

bool BitmapMan::LoadResource(UINT resourceNumber)
{
	FreeBitMap();
	HRSRC hr = FindResource(NULL, MAKEINTRESOURCE(resourceNumber), RT_BITMAP);
	if (hr == NULL)
		return false;

	HGLOBAL hg = ::LoadResource(NULL, hr);
	LPBITMAPINFOHEADER lpbihInfo = (LPBITMAPINFOHEADER)LockResource(hg);
	BYTE *pPalette, *pImage;
	if (lpbihInfo->biBitCount < 24)
	{
		pPalette = (PBYTE)(lpbihInfo + 1); // skip header
		PaletteSize = (1 << (lpbihInfo->biBitCount)) * sizeof(RGBQUAD);
		bfOffBits = 54 + PaletteSize;
		pImage = (PBYTE)(pPalette)+bfOffBits;
		bmiColors = new BYTE[PaletteSize];
		memcpy(bmiColors, pPalette, PaletteSize);
	}
	else
	{
		PaletteSize = 0;
		bmiColors = NULL;
		bfOffBits = 54;
		pImage = (PBYTE)(lpbihInfo + 1);
	}

	// pImage points to the image

	biWidth = (unsigned short)lpbihInfo->biWidth;
	biHeight = (unsigned short)lpbihInfo->biHeight;
	biBitCount = lpbihInfo->biBitCount;
	biSize = lpbihInfo->biSize;
	biPlanes = lpbihInfo->biPlanes;
	biCompression = lpbihInfo->biCompression;
	int lineWidth = biWidth * 3;
	if (lineWidth & 3)
		lineWidth += 4 - (lineWidth & 3);
	biSizeImage = lineWidth*biHeight;
	bfSize = bfOffBits + biSizeImage;
	biXPelsPerMeter = biYPelsPerMeter = biClrUsed = biClrImportant = 0;
	image = new BYTE[biSizeImage];
	memcpy(image, pImage, biSizeImage);
	UnlockResource(hg);
	FreeResource(hg);
	return true;
}


BYTE * BitmapMan::GetPalette()
{
	if (biBitCount <= 8)
		return bmiColors;
	return NULL;
}

int BitmapMan::GetPaletteSize()
{
	return PaletteSize;
}
