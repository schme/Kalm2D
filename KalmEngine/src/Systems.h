/**
 *
 * Systems.h
 *
 * File that holds all the platform layer interfaces for exporting
 *
 * Copyright (C) 2018 Kasper Sauramo
 * Created: 28/04/2018
 */

#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include "Scene.h"

/** forward declaration */
struct gameInput_t;
struct preferences_t;

class CommonSystem {
    public:
    virtual b32             IfWindowShouldClose() const =0;
    virtual f64             GetTime() const =0;
    virtual void            PollEvents() const =0;
    virtual gameInput_t *   GetInputState() =0;
    virtual gameInput_t *   GetNewState() =0;
    virtual gameInput_t *   GetOldState() =0;
    virtual void            SwapAndClearState() =0;
};

class FileSystem {
    public:
    virtual i32     ReadWholeFile( const char *filename, const u32 buffer_size, void *buffer ) const =0;
    virtual i32     WriteWholeFile( const char *filename, const u32 buffer_size, const void *buffer) const =0;
    virtual b32     GetWholeFileSize( const char *filename, u64 *file_size ) const =0;
};

class MemorySystem {
    public:
    typedef u64     Marker;
    virtual void *  Alloc( u32 size_bytes ) =0;
    virtual void *  AllocAligned( u32 size_bytes, u32 alignment ) =0;
    virtual void    Free( Marker mark ) =0;
};

class RenderSystem {
    public:
    virtual void            Draw() const =0;
    virtual void            SetModelViewMatrix( const u32 shaderID, const mat4 modelView) const =0;
    virtual void            SetProjectionMatrix( const u32 shaderID, const mat4 projection) const =0;
    virtual void            SetMatrixUniform( const u32 shaderID, const char *name, const mat4 matrix) const =0;

    virtual void            LoadTestScene( kScene_t *scene) const =0;
    //virtual void          LoadTestTestScene( kScene_t *scene) const =0;
    virtual void            DrawTestScene( kScene_t *scene) const =0;
    //virtual void          DrawTestTestScene( kScene_t *scene) const =0;
    virtual kTexture_t *    LoadTexture( kImage_t *image ) =0;
};

class AssetSystem {
    public:
    virtual kImage_t*   LoadImage( const char *filename) const =0;
    virtual kMesh_t*    LoadMesh( const char *filename) const =0;
};

class ConfigSystem {
    public:
    virtual void            Initialize() =0;
    virtual ConfigSystem *  Get() =0;
    virtual preferences_t * GetPreferences() =0;
    virtual void            Input() =0;
};

#endif /* end of include guard: SYSTEMS_H_ */
