/**
 *
 * kGame  Interface
 *
 * Shared for the Sys_Kalm module
 *
 * Copyright (C) 2018 Kasper Sauramo
 * Created: 27/04/2018
 */

#ifndef KALMSHARED_H_
#define KALMSHARED_H_

#include "Systems.h"
#include "Types.h"

const u32 frameBufferWidth = 1280;
const u32 frameBufferHeight = 720;

struct gameImport_t;
struct gameExport_t;

class kGame {
    public:
    virtual b32 Initialize() = 0;
    virtual i32 Loop () = 0;
    virtual void Terminate() = 0;
};

extern "C"
gameExport_t * GetGameAPI( gameImport_t * system);


struct preferences_t {
    b32 drawWireframe = false;
    f32 field_of_view = 45.0f;
};


struct gameExport_t {
    kGame *game;
};

struct gameImport_t {
    i32                     version;        /** not actually used, but Doom3 did so I need to! */
    CommonSystem *          commonSystem;
    MemorySystem *          memorySystem;
    FileSystem *            fileSystem;
    RenderSystem *          renderSystem;
    AssetSystem *           assetSystem;
    ConfigSystem *          configSystem;
};

struct gameButtonState_t {
    s32 toggleCount;
    b32 endedDown;
};

struct mouseInput_t {
    f32 posX;
    f32 posY;
    f32 offsetX;
    f32 offsetY;
    f32 scrollOffsetX;
    f32 scrollOffsetY;
    gameButtonState_t right;
    gameButtonState_t left;
};


struct gameInput_t {
    mouseInput_t mouseInput;
    union {
        struct {
            gameButtonState_t up;
            gameButtonState_t left;
            gameButtonState_t right;
            gameButtonState_t down;

            gameButtonState_t actionQ;
            gameButtonState_t actionE;
            gameButtonState_t actionR;
            gameButtonState_t actionF;
            gameButtonState_t actionSpace;
            gameButtonState_t actionC;
            gameButtonState_t actionEnter;
            gameButtonState_t actionEsc;
            gameButtonState_t action0;
            gameButtonState_t action1;
            gameButtonState_t action2;
            gameButtonState_t action3;
            gameButtonState_t action4;
            gameButtonState_t action5;
            gameButtonState_t action6;
            gameButtonState_t action7;
            gameButtonState_t action8;
            gameButtonState_t action9;

            gameButtonState_t l_shift;
            gameButtonState_t l_ctrl;
            gameButtonState_t l_alt;

            gameButtonState_t actionF1;
            gameButtonState_t actionF2;
            gameButtonState_t actionF3;
            gameButtonState_t actionF4;
            gameButtonState_t actionF5;
            gameButtonState_t actionF6;
            gameButtonState_t actionF7;
            gameButtonState_t actionF8;
            gameButtonState_t actionF9;
            gameButtonState_t actionF10;
            gameButtonState_t actionF11;
            gameButtonState_t actionF12;

        };
        /*
         * TODO(Kasper): Actually get this value properly
         * REMEMBER TO UPDATE THIS WITH WHEN ADDING NEW VALUES
         */
        gameButtonState_t buttons[37];
    };

    /** THIS TOO */
    u32 inputArrayLength = 37;
};

#endif /* end of include guard: KALMSHARED_H_ */
