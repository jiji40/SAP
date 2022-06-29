#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _BFHSIZE_ 14 
#define _BIHSIZE_ 40 

// command line compiling
// cl.exe /FoZoomBMP ex11.cpp
//		/Fo<file> different name for the output exe file (ZoomBMP.exe instead of ex11.exe)

struct BitmapFileHeader {
    char bfType[2];		//must always be set to 'BM' to declare that this is a .bmp-file (stdvalue: 19778)
    long int bfSize;		//specifies the size of the file in bytes
    char bfReserved1[2];	//must always be set to zero (stdvalue: 0)
    char bfReserved2[2];	//must always be set to zero (stdvalue: 0)
    char bfOffBits[4];		//specifies the offset from the beginning of the file to the bitmap data (stdvalue: 1078)
				//BMP picture has a reverse way of storage with padding 0 
				//untill the number of bytes is multiple of 4
}; //all 14 bytes data structure

struct BitmapInfoHeader {
    long int biSize;		//specifies the size of the BITMAPINFOHEADER structure, in bytes (stdvalue: 40)
    long int biWidth;		//specifies the width of the image, in pixels (stdvalue: 100)
    long int biHeight;		//specifies the height of the image, in pixels (stdvalue: 100)
    char biPlanes[2];		//specifies the number of planes of the target device, must be set to zero (stdvalue: 1)
    short int biBitCount;	//specifies the number of bits per pixel (stdvalue: 8)
    char biCompression[4];  //specifies the type of compression, usually set to zero (no compression) (stdvalue: 0)  
    long int biSizeImage;	//specifies the size of the image data, in bytes. If there is no compression,  
                            //it is valid to set this member to zero (stdvalue: 0)
    char biXPelsPerMeter[4]; 	//specifies the the horizontal pixels per meter on the designated targer device, usually set to zero
								//(stdvalue: 0)
    char biYPelsPermeter[4]; 	//specifies the the vertical pixels per meter on the designated targer device, usually set to zero 
								//(stdvalue: 0)
    char biClrUsed[4];		//specifies the number of colors used in the bitmap, if set to zero 
							//the number of colors is calculated using the biBitCount member (stdvalue: 0)
    char biClrImportant[4]; //specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important
							//(stdvalue: 0)
}; // 40 bytes

int main (int argc, char* argv[])
{
	FILE *fSrc;	FILE *fDst;

	BitmapFileHeader bfhSrc, bfhDst;
	BitmapInfoHeader bihSrc, bihDst;
		
	long int buffer = NULL;
	int i = 0, j = 0, k = 0, n = 0; int zoom = 0;
	
	int oldWidth = 0;	int oldHeight = 0;
	int newWidth = 0; int newHeight = 0;

	char srcFile[100]; char dstFile[100];
	char optZoom[2];

	if ( argc < 4 )
		printf("\nWrong number of parameters. Sample: ZooomBMP.exe 2 f1.bmp f2.bmp");
	else {
		strcpy(optZoom, argv[1]);
		zoom = atoi(optZoom);
		strcpy(srcFile, argv[2]); strcpy(dstFile, argv[3]);
		fSrc = fopen(srcFile, "rb");

		printf("\n bfh length: %d bih length: %d", sizeof(BitmapFileHeader), sizeof(BitmapInfoHeader));
		fread(&bfhSrc, _BFHSIZE_, 1, fSrc); // it is multiple of 8 in memory alignament
		fread(&bihSrc, _BIHSIZE_, 1, fSrc);

		if ((strncmp(bfhSrc.bfType, "BM", 2) == 0) && (bihSrc.biSize == 40) && (bihSrc.biBitCount == 24)) {
            		printf("\n Let's fun begin! %d ", bihSrc.biSize);
			fDst = fopen(dstFile, "wb+");
			bfhDst = bfhSrc;
			bihDst = bihSrc;

			bihDst.biWidth = bihSrc.biWidth * zoom;
			bihDst.biHeight = bihSrc.biHeight * zoom;
			long int padW = (bihDst.biWidth * ((bihDst.biBitCount)/8));
			padW = padW + (padW % 12); //12 = 3 bytes/pixel x 4 (the multiple value mandatory per line)
			bfhDst.bfSize = bihDst.biHeight * padW;

			fwrite(&bfhDst, _BFHSIZE_, 1, fDst);
			fwrite(&bihDst, _BIHSIZE_, 1, fDst);

			//COPY AND MULTIPLYING BYTES FROM SOURCE TO DESTINATION FILE (ZOOM)
			for (i = 0; i < bihSrc.biHeight; i++)
			{
				//it is multiplied one line from height
				long int relPos = ftell(fSrc);
				for (j = 0; j < bihSrc.biWidth; j++)
				{
					buffer = 0x00000000;
					fread(&buffer, sizeof(char), 3, fSrc);
					//multiplying by zoom the Bytes from one line 
					for(k = 0; k < zoom; k++)
						fwrite(&buffer, sizeof(char), 3, fDst);
				}

				//multiplying the line by zoom-1 in destination file 
				for(int n = 0; n < zoom-1; n++)
				{
					//going to the beginning of the same line in source file 
					//fseek(fSrc, -3*oldWidth, SEEK_CUR);
					fseek(fSrc, relPos, SEEK_SET);
					//read the line and multiplying of this one 
					for (j=0; j < bihSrc.biWidth; j++)
					{
						buffer = 0x00000000;
						fread(&buffer, sizeof(char), 3, fSrc);
						for(k = 0; k < zoom; k++)
							fwrite(&buffer, sizeof(char), 3, fDst);
					}
				}
			}

			fclose(fDst);			
		} else {
			printf("\n The file seems to not be an BMP with 24 bits code per pixel");
			return 1;			
		}
	
		fclose(fSrc);
	}
	return 0; //everything is ok
}
