bool Targa_Save( const char *filename, const uint16_t width, const uint16_t height, const void *texels, const int bitsPerPixel )
{
    bool status = false;

    if( !filename
    ||  !texels
    ||  !width
    ||  !height
    ||  !bitsPerPixel )
        return status;

    if( (bitsPerPixel == 24) || (bitsPerPixel == 32) )
    {
        FILE *pFile = fopen( filename, "wb" );
        if( pFile )
        {
            const int TGA_HEADER_SIZE = 18;
            uint8_t header[ TGA_HEADER_SIZE ];
            memset( header, 0, TGA_HEADER_SIZE );

            // http://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
            // Truevision TGA -- FILE FORMAT SPECIFICATION -- Version 2.0
            //[ 0] // image id
            //[ 1] // colormap present
            header[ 2] = 2; // uncompressed
            //[ 3] // colormap offset 16-bit
            //[ 5] // colormap entries 16-bit
            //[ 7] // colormap bits per pixel
            //[ 8] // x-origin 16-bit
            //[10] // y-origin 16-bit
            header[12] =(width      ) & 0xFF;
            header[13] =(width  >> 8) & 0xFF;
            header[14] =(height     ) & 0xFF;
            header[15] =(height >> 8) & 0xFF;
            header[16] = bitsPerPixel;
            // T R Top/Right
            //+-+-+
            //|5|4| bits = direction to copy to screen
            //+-+-+ 
            // 0 0 bottom left
            // 0 1 bottom right
            // 1 0 top left
            // 1 1 top right
            header[17] = 0x20;

            if( fwrite( header, TGA_HEADER_SIZE, 1, pFile) )
                if( fwrite( texels, (width * height * bitsPerPixel) >> 3, 1, pFile) )
                    status = true;

            fclose( pFile );
        }
    }

    return status;
}


