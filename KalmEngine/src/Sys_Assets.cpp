/**
 *
 * Assets.cpp
 *
 * Copyright (C) 2018 Kasper Sauramo
 * Created: 28/04/2018
 */

#include "Sys_Assets.h"

void kAssets::LoadScene( kScene_t *scene) const {

    /** TODO(Kasper): Get shaders from the scene information. Materials etc. */

    /** compile and store shaders */
    /** textures */
    /** create vertice group */
    /** load data to the renderer */
}

/**
 * Currently supports only BMP files
 */
kImage_t* kAssets::GetImage( const char *filename) const {
    u64 fileSize = 0;
    if( ! g_Filesystem->GetWholeFileSize( filename, &fileSize) || fileSize == 0) {
        printf("Getting file size from: %s failed, or fileSize was 0\n", filename);
        return nullptr;
    }
    // TODO(Kasper): Allocate memory for the filebuffer from the memory subsystem instead */
    void * fileBuffer = std::malloc( fileSize);
    if( ! fileBuffer ) {
        PRINTL_STR( "Could not receive filebuffer"); }
    if( ! g_Filesystem->ReadWholeFile( filename, (u32)fileSize, fileBuffer)) {
        printf("kAssets: Reading file: %s (filesize: %llu) to buffer failed\n", filename, fileSize);
        return nullptr;
    }
    kImage_t *imageObject = LoadBMPFile(filename, fileBuffer);
    if( !imageObject) {
        PRINTL_STR( "Loading image file failed.");
    }

    std::free( fileBuffer);
    return imageObject;
}


/**
 * TODO(Kasper): Widen functionality (read additional types of BMP)
 *
 * Returns the image object
 */
kImage_t* kAssets::LoadBMPFile( const char * filename, const void *filebuffer) const {
    BitmapHeader_t header = *((BitmapHeader_t*)filebuffer);

    /** gimp seems to give compression value 3, and that apparently means no compression, so check only
     * for the 2 different compression types we know we can't handle, RLE-8 and RLE-4.
     * We'll shoot ourselves in the foot later! */
    if( header.type != 0x4D42 && header.compression != 1 && header.compression != 2 )  {  // "BM"
        PRINT_STR("Tried to read a non BMP file as BMP, or the BMP type is not recognized: ");
        PRINTL_STR( filename );
    }
    kImage_t *imageObject = (kImage_t*)g_Memory->Alloc( sizeof( kImage_t));
    imageObject->assetMark = g_Memory->GetMarker();
    imageObject->width = safeCastU32ToU16( header.width);
    imageObject->height = safeCastU32ToU16( header.height);
    imageObject->bufferSize = header.sizeOfImageData;
    imageObject->imageBuffer = g_Memory->AllocAligned( header.sizeOfImageData, header.bitsPerPixel);

    void *startOfImage = (u8*)filebuffer + header.bytesToImage;

    /** Here we assume an ABGR format (0xRRGGBBAA), which can't be used in core OpenGL, so we swizzle them
     *  to RGBA (0xAABBGGRR) */

    //TODO(Kasper): This is starting to become a bit sketchy, even for me. Can we get the color masks at least
    // from the headers?
    u32 alphaMask = 0x000000FF;
    u32 blueMask = 0x0000FF00;
    u32 greenMask = 0x00FF0000;
    u32 redMask = 0xFF000000;

    u32 *ptr_i = (u32*)startOfImage;
    u32 *ptr_j = (u32*)(imageObject->imageBuffer);

    for (u32 i = 0; i < header.sizeOfImageData / 4; ++i) {
        u32 swizzledColor = {};

        u32 redByte = (*ptr_i) & redMask;
        u32 greenByte = (*ptr_i) & greenMask;
        u32 blueByte = (*ptr_i) & blueMask;
        u32 alphaByte = (*ptr_i) & alphaMask;

        swizzledColor = ( redByte >> 24) |
                        ( greenByte >> 8) |
                        ( blueByte << 8) |
                        ( alphaByte << 24);

        *ptr_j++ = swizzledColor;
        ptr_i++;
    }

    return imageObject;
}

/** Corners + Element indices */
vertices_t* kAssets::GetAABBVertices( const kAABB &aabb) const {

    /** 3 floats per corner, 8 corners */
    u32 nrOfVerts = 3*8;
    /** 3 floats per triangle, 2 triangles per side, 6 sides */
    u32 nrOfIndices = 3*2*6;

    vertices_t* result = (vertices_t*)g_Memory->Alloc( sizeof(vertices_t));
    result->vertices = (f32*)g_Memory->Alloc( nrOfVerts * sizeof(f32) );
    result->indices = (u32*)g_Memory->Alloc( nrOfIndices * sizeof(f32) );

    /** corners + texture coords*/
    vec3 vertice_array[] = {
        aabb.center - aabb.half,
        aabb.center + Vec3( aabb.half.x, -aabb.half.y, -aabb.half.z),
        aabb.center + Vec3( -aabb.half.x, -aabb.half.y, aabb.half.z),
        aabb.center + Vec3( aabb.half.x, -aabb.half.y, aabb.half.z),
        aabb.center + Vec3( -aabb.half.x, aabb.half.y, -aabb.half.z),
        aabb.center + Vec3( aabb.half.x, aabb.half.y, -aabb.half.z),
        aabb.center + Vec3( -aabb.half.x, aabb.half.y, aabb.half.z),
        aabb.center + aabb.half
    };
    /** Wind clockwise */
    u32 index_array[] = {
        0, 4, 5,    1, 0, 5,    /** front */
        2, 6, 4,    0, 2, 4,    /** left */
        3, 7, 6,    2, 3, 6,    /** back */
        1, 5, 7,    3, 1, 7,    /** right */
        4, 6, 7,    5, 4, 7,    /** top */
        2, 0, 1,    3, 1, 2     /** bottom */
    };

    std::memcpy( result->vertices, vertice_array, nrOfVerts * sizeof(f32));
    std::memcpy( result->indices, index_array, nrOfIndices * sizeof(u32));

    result->vertices_n = nrOfVerts;
    result->indices_n = nrOfIndices;

    return result;
}
