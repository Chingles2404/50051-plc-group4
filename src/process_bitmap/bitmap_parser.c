#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap_parser.h"

BitmapParser* createBitmapParser() {
    BitmapParser* parser = (BitmapParser*)malloc(sizeof(BitmapParser));
    if (parser == NULL) {
        return NULL;
    }
    
    /* initiliaze the parser states structure */
    parser->file = NULL;
    parser->state = STATE_INIT;
    parser->error = ERROR_NONE;
    parser->error_message = NULL;
    parser->pixelData = NULL;
    parser->current_position = 0;
    
    memset(&parser->fileHeader, 0, sizeof(FILEHEADER));
    memset(&parser->infoHeader, 0, sizeof(INFOHEADER));
    
    return parser;
}

void freeBitmapParser(BitmapParser* parser) {
    if (parser == NULL) {
        return;
    }
    
    if (parser->file != NULL) {
        fclose(parser->file);
    }
    
    if (parser->pixelData != NULL) {
        free(parser->pixelData);
    }
    
    if (parser->error_message != NULL) {
        free(parser->error_message);
    }
    
    free(parser);
}

/* set error state */
void setError(BitmapParser* parser, ErrorCode code, const char* message) {
    parser->state = STATE_ERROR;
    parser->error = code;
    
    if (parser->error_message != NULL) {
        free(parser->error_message);
    }
    
    parser->error_message = strdup(message);
}

/* parse file (STARTS HERE) */
bool parseFile(BitmapParser* parser, const char* filename) {
    if (parser == NULL || filename == NULL) {
        return false;
    }
    
    parser->state = STATE_INIT;
    parser->state = handleInitState(parser, filename);
    
    /* process states until complete or error */
    while (parser->state != STATE_COMPLETED && parser->state != STATE_ERROR) {
        parser->state = processState(parser);
    }
    
    return parser->state == STATE_COMPLETED;
}

ParserState processState(BitmapParser* parser) {
    switch (parser->state) {
        case STATE_INIT:
            return STATE_ERROR; /* Should never happen - initialization requires filename */
        
        case STATE_READING_FILE_HEADER:
            return handleFileHeaderState(parser);
        
        case STATE_READING_INFO_HEADER:
            return handleInfoHeaderState(parser);
        
        case STATE_READING_PIXEL_DATA:
            return handlePixelDataState(parser);
        
        case STATE_COMPLETED:
            return STATE_COMPLETED;
        
        case STATE_ERROR:
            return handleErrorState(parser);
        
        default:
            setError(parser, ERROR_NONE, "Unknown parser state");
            return STATE_ERROR;
    }
}

/* open the file */
ParserState handleInitState(BitmapParser* parser, const char* filename) {
    /* binary read mode */
    parser->file = fopen(filename, "rb");
    if (parser->file == NULL) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "Failed to open file: %s", filename);
        setError(parser, ERROR_FILE_NOT_FOUND, error_msg);
        return STATE_ERROR;
    }
    
    parser->current_position = 0;
    return STATE_READING_FILE_HEADER;
}


ParserState handleFileHeaderState(BitmapParser* parser) {
    fseek(parser->file, 0, SEEK_SET);
    parser->current_position = 0;
    
    /* reading file header */
    size_t read = fread(&parser->fileHeader, sizeof(FILEHEADER), 1, parser->file);
    if (read != 1) {
        setError(parser, ERROR_READING_FILE, "Failed to read BMP file header");
        return STATE_ERROR;
    }
    
    parser->current_position += sizeof(FILEHEADER);
    
    /* validate BM" = 0x4D42 */
    if (parser->fileHeader.type != 0x4D42) {
        setError(parser, ERROR_INVALID_BMP, "Not a valid BMP file (missing BM signature)");
        return STATE_ERROR;
    }
    
    return STATE_READING_INFO_HEADER;
}

ParserState handleInfoHeaderState(BitmapParser* parser) {
    size_t read = fread(&parser->infoHeader, sizeof(INFOHEADER), 1, parser->file);
    if (read != 1) {
        setError(parser, ERROR_READING_FILE, "Failed to read BMP info header");
        return STATE_ERROR;
    }
    
    parser->current_position += sizeof(INFOHEADER);
    
    /* validate BMP format */
    if (parser->infoHeader.bits != 24 && parser->infoHeader.bits != 32) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
                "Unsupported BMP format: %d bits per pixel (only 24 and 32 supported)", 
                parser->infoHeader.bits);
        setError(parser, ERROR_UNSUPPORTED_FORMAT, error_msg);
        return STATE_ERROR;
    }
    
    /* only supports uncompressed bmp */
    if (parser->infoHeader.compression != 0) {
        setError(parser, ERROR_UNSUPPORTED_FORMAT, "Compressed BMP files are not supported");
        return STATE_ERROR;
    }
    
    return STATE_READING_PIXEL_DATA;
}

ParserState handlePixelDataState(BitmapParser* parser) {
    int width = parser->infoHeader.width;
    int height = parser->infoHeader.height;
    int bytesPerPixel = parser->infoHeader.bits / 8;
    
    /* Calculate row padding - rows are padded to multiple of 4 bytes */
    int row_padded = (width * bytesPerPixel + 3) & (~3);
    
    /* Allocate memory for pixel data (grayscale) */
    parser->pixelData = (unsigned char*)malloc(width * height);
    if (parser->pixelData == NULL) {
        setError(parser, ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for pixel data");
        return STATE_ERROR;
    }
    
    /* allocate memory for a single row (including padding) */
    unsigned char* row = (unsigned char*)malloc(row_padded);
    if (row == NULL) {
        free(parser->pixelData);
        parser->pixelData = NULL;
        setError(parser, ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for row buffer");
        return STATE_ERROR;
    }
    
    fseek(parser->file, parser->fileHeader.offset, SEEK_SET);
    parser->current_position = parser->fileHeader.offset;
    
    for (int i = 0; i < height; i++) {
        size_t read = fread(row, 1, row_padded, parser->file);
        if (read != row_padded) {
            free(row);
            free(parser->pixelData);
            parser->pixelData = NULL;
            setError(parser, ERROR_READING_FILE, "failed to read pixel data row");
            return STATE_ERROR;
        }
        
        parser->current_position += row_padded;
        
        for (int j = 0; j < width * bytesPerPixel; j += bytesPerPixel) {
            /* Convert RGB to grayscale using luminance formula */
            int gray = (row[j] * 0.11) + (row[j + 1] * 0.59) + (row[j + 2] * 0.3);
            
            /* store grayscale value */
            /* invert rows */
            if (parser->infoHeader.height > 0) {
                /* Bottom-up image (positive height) */
                parser->pixelData[((height - 1 - i) * width) + (j / bytesPerPixel)] = gray;
            } else {
                /* Top-down image (negative height, rare) */
                parser->pixelData[(i * width) + (j / bytesPerPixel)] = gray;
            }
        }
    }
    
    free(row);
    return STATE_COMPLETED;
}

ParserState handleErrorState(BitmapParser* parser) {
    return STATE_ERROR;
}

bool isGrayscale(INFOHEADER* infoHeader) {
    return (infoHeader->bits == 8 && infoHeader->ncolours == 256);
}

void convertToGrayscale(unsigned char* pixels, int width, int height, int bytesPerPixel) {
    /* not sure if this needs to be implemented */
}