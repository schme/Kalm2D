/**
 * KalmGame
 *
 * Copyright (C) 2018 Kasper Sauramo
 * Created: 27/04/2018
 */

#include "KalmGame.h"
#include "GameShared.h"
#include "AABB.cpp"
#include "Player.cpp"
#include "Camera.cpp"
#include "Object.cpp"
#include "Scene.cpp"

/** Entry point for KalmGame.exe to the Game64.dll */
gameExport_t * GetGameAPI( gameImport_t * system) {

    static KalmGame engine;
    g_System = system;
    engine.gameExport.game = &engine;
    return &engine.gameExport;
}


/**
 * KalmGame
 */
gameExport_t KalmGame::gameExport = {};

/**
 * Dev function to quickly set a test scene. This should be somewhere else
 * and scenes shouldn't be loaded from C++, but created scripts presumably
 */
kScene_t * KalmGame::CreateTestScene() {

    kScene_t * scene = (kScene_t*)GetMemory(sizeof( kScene_t));
    ASSERT( scene );

    scene->ID = 0;

    kPlayer * player = (kPlayer*)GetMemory( sizeof( kPlayer));
    kCamera * camera = (kCamera*)GetMemory( sizeof( kCamera));
    *player = {};
    *camera = {};

    /** positive y is the up axis */

    scene->player = player;
    scene->camera = camera;

#if 1
    kMesh_t *mesh = g_System->assetSystem->LoadMesh( "Assets/Models/dragon_vrip_res2.ply" );

    vec3 testPositions[] = {
        Vec3( 0.0f, 0.0f, 0.0f),
        Vec3( 0.2f, 2.1f, 2.0f),
        Vec3( 3.2f, 3.1f, 5.0f),
        Vec3( -2.2f, -1.1f, 2.0f),
        Vec3( -4.2f, 0.1f, -2.0f)
    };

    for( int i=0; i < 5; ++i ) {
        kObject *obj = (kObject*)GetMemory( sizeof( kObject ));
        MeshComponent *meshComp = (MeshComponent*)GetMemory( sizeof( MeshComponent));
        meshComp->mesh = mesh;
        obj->components[0] = meshComp;
        obj->position = testPositions[i];
        scene->objects[i] = obj;
    }
#else

    f32 vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 testPositions[] = {
        Vec3( 0.0f,  0.0f,  0.0f),
        Vec3( 2.0f,  5.0f, -15.0f),
        Vec3(-1.5f, -2.2f, -2.5f),
        Vec3(-3.8f, -2.0f, -12.3f),
        Vec3( 2.4f, -0.4f, -3.5f),
        Vec3(-1.7f,  3.0f, -7.5f),
        Vec3( 1.3f, -2.0f, -2.5f),
        Vec3( 1.5f,  2.0f, -2.5f),
        Vec3( 1.5f,  0.2f, -1.5f),
        Vec3(-1.3f,  1.0f, -1.5f)
    };

    MeshComponent *meshComp = (MeshComponent*)GetMemory( sizeof( MeshComponent));
    meshComp->mesh = (kMesh_t*)GetMemory( sizeof(kMesh_t));
    meshComp->mesh->vertices = (f32*)GetMemory( sizeof( vertices));
    meshComp->mesh->vertices_n = 41;

    std::memcpy( meshComp->mesh->vertices, vertices, sizeof(vertices));

    for( int i=0; i < 5; i++) {
        kObject *obj = (kObject*)GetMemory( sizeof( kObject ));
        obj->components[0] = meshComp;
        obj->position = testPositions[i];
        scene->objects[i] = obj;
    }

#endif
    return scene;
}

void KalmGame::LoadTestScene( kScene_t *scene) {
    g_System->renderSystem->LoadTestScene( scene );
}

void KalmGame::LoadTestTestScene( kScene_t *scene) {
    g_System->renderSystem->LoadTestTestScene( scene );
}



void KalmGame::LoadScene( kScene_t *scene ) {
}

void KalmGame::RunCurrentScene() {
    /** Run Logic on Objects */
}

void KalmGame::RenderCurrentScene() {
    /** Setup per-frame modifications */

}


b32 KalmGame::Initialize() {

    return true;
}


/**
 * Main Loop
 */
i32 KalmGame::Loop() {

    kScene_t *testScene = this->CreateTestScene();
    this->SetCurrentScene( testScene );
    this->LoadTestScene( testScene);

    while(!g_System->commonSystem->IfWindowShouldClose()) {

        /** Get input */

        static r64 lastTime = 0;
        static r64 time = g_System->commonSystem->GetTime();


        RunCurrentScene();
        RenderCurrentScene();

        g_System->renderSystem->DrawTestScene( currentScene);

        g_System->commonSystem->PollEvents();
        lastTime = time;
    }
    return true;
}


void KalmGame::Terminate() {

}


/**
 * Private
 */


void KalmGame::SetCurrentScene( kScene_t *scene) {
    this->currentScene = scene;
}




/**
 * end of KalmGame
 */
