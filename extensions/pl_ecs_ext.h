/*
   pl_ecs_ext.h
*/

/*
Index of this file:
// [SECTION] header mess
*/

//-----------------------------------------------------------------------------
// [SECTION] header mess
//-----------------------------------------------------------------------------

#ifndef PL_ECS_EXT_H
#define PL_ECS_EXT_H

//-----------------------------------------------------------------------------
// [SECTION] apis
//-----------------------------------------------------------------------------

#define PL_API_ECS "PL_API_ECS"
typedef struct _plEcsI plEcsI;

#define PL_API_CAMERA "PL_API_CAMERA"
typedef struct _plCameraI plCameraI;

//-----------------------------------------------------------------------------
// [SECTION] defines
//-----------------------------------------------------------------------------

#ifndef PL_MAX_NAME_LENGTH
    #define PL_MAX_NAME_LENGTH 1024
#endif

#ifndef PL_INVALID_ENTITY_HANDLE
    #define PL_INVALID_ENTITY_HANDLE 0
#endif

//-----------------------------------------------------------------------------
// [SECTION] includes
//-----------------------------------------------------------------------------

#include <stdint.h> // uint*_t
#include "pl_graphics.inl"
#include "pl_math.h"

//-----------------------------------------------------------------------------
// [SECTION] forward declarations & basic types
//-----------------------------------------------------------------------------

// entity component system
typedef struct _plComponentLibrary plComponentLibrary;
typedef struct _plComponentManager plComponentManager;

// ecs components
typedef struct _plTagComponent       plTagComponent;
typedef struct _plMeshComponent      plMeshComponent;
typedef struct _plSubMesh            plSubMesh;
typedef struct _plTransformComponent plTransformComponent;
typedef struct _plMaterialComponent  plMaterialComponent;
typedef struct _plObjectComponent    plObjectComponent;
typedef struct _plCameraComponent    plCameraComponent;
typedef struct _plHierarchyComponent plHierarchyComponent;

// enums
typedef int      plShaderType;
typedef int      plComponentType;
typedef uint64_t plEntity;

//-----------------------------------------------------------------------------
// [SECTION] public api
//-----------------------------------------------------------------------------

plEcsI*    pl_load_ecs_api   (void);
plCameraI* pl_load_camera_api(void);

//-----------------------------------------------------------------------------
// [SECTION] public api struct
//-----------------------------------------------------------------------------

typedef struct _plEcsI
{
    plEntity (*create_entity)    (plComponentLibrary* ptLibrary);
    size_t   (*get_index)        (plComponentManager* ptManager, plEntity tEntity);
    void*    (*get_component)    (plComponentManager* ptManager, plEntity tEntity);
    void*    (*create_component) (plComponentManager* ptManager, plEntity tEntity);
    bool     (*has_entity)       (plComponentManager* ptManager, plEntity tEntity);

    // components
    plEntity (*create_mesh)     (plComponentLibrary* ptLibrary, const char* pcName);
    plEntity (*create_material) (plComponentLibrary* ptLibrary, const char* pcName);
    plEntity (*create_object)   (plComponentLibrary* ptLibrary, const char* pcName);
    plEntity (*create_transform)(plComponentLibrary* ptLibrary, const char* pcName);
    plEntity (*create_camera)   (plComponentLibrary* ptLibrary, const char* pcName, plVec3 tPos, float fYFov, float fAspect, float fNearZ, float fFarZ);

    // hierarchy
    void (*attach_component)   (plComponentLibrary* ptLibrary, plEntity tEntity, plEntity tParent);
    void (*deattach_component) (plComponentLibrary* ptLibrary, plEntity tEntity);

    // material
    void (*material_outline)(plComponentLibrary* ptLibrary, plEntity tEntity);

} plEcsI;

typedef struct _plCameraI
{
    void (*set_fov)        (plCameraComponent* ptCamera, float fYFov);
    void (*set_clip_planes)(plCameraComponent* ptCamera, float fNearZ, float fFarZ);
    void (*set_aspect)     (plCameraComponent* ptCamera, float fAspect);
    void (*set_pos)        (plCameraComponent* ptCamera, float fX, float fY, float fZ);
    void (*set_pitch_yaw)  (plCameraComponent* ptCamera, float fPitch, float fYaw);
    void (*translate)      (plCameraComponent* ptCamera, float fDx, float fDy, float fDz);
    void (*rotate)         (plCameraComponent* ptCamera, float fDPitch, float fDYaw);
    void (*update)         (plCameraComponent* ptCamera);
} plCameraI;

//-----------------------------------------------------------------------------
// [SECTION] structs
//-----------------------------------------------------------------------------

typedef struct _plGlobalInfo
{
    plVec4 tAmbientColor;
    plVec4 tCameraPos;
    plMat4 tCameraView;
    plMat4 tCameraViewProj;
    float  fTime;
    int    _unused[3];
} plGlobalInfo;

typedef struct _plMaterialInfo
{
    plVec4 tAlbedo;
} plMaterialInfo;

typedef struct _plObjectInfo
{
    plMat4   tModel;
    uint32_t uVertexOffset;
    int      _unused[3];
} plObjectInfo;

typedef struct _plObjectComponent
{
    plEntity tMesh;
    plEntity tTransform;
} plObjectComponent;

typedef struct _plHierarchyComponent
{
    plEntity tParent;
} plHierarchyComponent;

typedef struct _plTagComponent
{
    char acName[PL_MAX_NAME_LENGTH];
} plTagComponent;

typedef struct _plTransformComponent
{
    plVec3 tScale;
    plVec4 tRotation;
    plVec3 tTranslation;
    plMat4 tFinalTransform;
    plMat4 tWorld;

    uint64_t     uBindGroup2;
    uint32_t     uBufferOffset;
    plObjectInfo tInfo;
} plTransformComponent;

typedef struct _plSubMesh
{
    plMesh    tMesh;
    plEntity  tMaterial;
    uint32_t  uStorageOffset;

    plVec3*   sbtVertexPositions;
    plVec3*   sbtVertexNormals;
    plVec4*   sbtVertexTangents;
    plVec4*   sbtVertexColors0;
    plVec4*   sbtVertexColors1;
    plVec4*   sbtVertexWeights0;
    plVec4*   sbtVertexWeights1;
    plVec4*   sbtVertexJoints0;
    plVec4*   sbtVertexJoints1;
    plVec2*   sbtVertexTextureCoordinates0;
    plVec2*   sbtVertexTextureCoordinates1;
    uint32_t* sbuIndices;
} plSubMesh;

typedef struct _plMeshComponent
{
    plSubMesh* sbtSubmeshes;

} plMeshComponent;

typedef struct _plMaterialComponent
{
    plShaderType tShaderType;
    
    // properties
    plVec4 tAlbedo;
    float  fAlphaCutoff;
    bool   bDoubleSided;
    bool   bOutline;

    // maps
    uint32_t uAlbedoMap;
    uint32_t uNormalMap;
    uint32_t uEmissiveMap;

    // misc
    uint32_t        uShader;
    uint32_t        uShaderVariant;
    plGraphicsState tGraphicsState;
    uint64_t        uBindGroup1;
    uint64_t        ulShaderTextureFlags;

    // internal
    uint32_t    uBufferOffset;
} plMaterialComponent;

typedef struct _plCameraComponent
{
    plVec3       tPos;
    float        fNearZ;
    float        fFarZ;
    float        fFieldOfView;
    float        fAspectRatio;  // width/height
    plMat4       tViewMat;      // cached
    plMat4       tProjMat;      // cached
    plMat4       tTransformMat; // cached

    // rotations
    float        fPitch; // rotation about right vector
    float        fYaw;   // rotation about up vector
    float        fRoll;  // rotation about forward vector

    // direction vectors
    plVec3       _tUpVec;
    plVec3       _tForwardVec;
    plVec3       _tRightVec;
} plCameraComponent;

typedef struct _plComponentManager
{
    plComponentType tComponentType;
    plEntity*       sbtEntities;
    void*           pData;
    size_t          szStride;
} plComponentManager;

typedef struct _plComponentLibrary
{
    size_t             tNextEntity;
    plComponentManager tTagComponentManager;
    plComponentManager tTransformComponentManager;
    plComponentManager tMeshComponentManager;
    plComponentManager tMaterialComponentManager;
    plComponentManager tOutlineMaterialComponentManager;
    plComponentManager tObjectComponentManager;
    plComponentManager tCameraComponentManager;
    plComponentManager tHierarchyComponentManager;
} plComponentLibrary;

//-----------------------------------------------------------------------------
// [SECTION] enums
//-----------------------------------------------------------------------------

enum _plComponentType
{
    PL_COMPONENT_TYPE_NONE,
    PL_COMPONENT_TYPE_TAG,
    PL_COMPONENT_TYPE_TRANSFORM,
    PL_COMPONENT_TYPE_MESH,
    PL_COMPONENT_TYPE_MATERIAL,
    PL_COMPONENT_TYPE_CAMERA,
    PL_COMPONENT_TYPE_OBJECT,
    PL_COMPONENT_TYPE_HIERARCHY
};

enum _plShaderType
{
    PL_SHADER_TYPE_PBR,
    PL_SHADER_TYPE_UNLIT,
    PL_SHADER_TYPE_CUSTOM,
    
    PL_SHADER_TYPE_COUNT
};

#endif // PL_ECS_EXT_H