#ifndef _QUAKE3BSP_H
#define _QUAKE3BSP_H

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "../aux/glad.h"

// GLM Mathemtics
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

typedef unsigned char byte;
#define FACE_POLYGON 1
#define MAX_TEXTURES 1000


// This is our BSP header structure
struct tBSPHeader {
    char strID[4]; // This should always be 'IBSP'
    int  version;  // This should be 0x2e for Quake 3 files
};

// This is our BSP lump structure
struct tBSPLump {
    int offset; // The offset into the file for the start of this lump
    int length; // The length in bytes for this lump
};

// This is our BSP vertex structure
struct tBSPVertex {
    glm::vec3 vPosition;      // (x, y, z) position.
    glm::vec2 vTextureCoord;  // (u, v) texture coordinate
    glm::vec2 vLightmapCoord; // (u, v) lightmap coordinate
    glm::vec3 vNormal;        // (x, y, z) normal vector
    byte      color[4];       // RGBA color for the vertex
};

// This is our BSP face structure
struct tBSPFace {
    int       textureID;      // The index into the texture array
    int       effect;         // The index for the effects (or -1 = n/a)
    int       type;           // 1=polygon, 2=patch, 3=mesh, 4=billboard
    int       startVertIndex; // The starting index into this face's first vertex
    int       numOfVerts;     // The number of vertices for this face
    int       startIndex;     // The starting index into the indices array for this face
    int       numOfIndices;   // The number of indices for this face
    int       lightmapID;     // The texture index for the lightmap
    int       lMapCorner[2];  // The face's lightmap corner in the image
    int       lMapSize[2];    // The size of the lightmap section
    glm::vec3 lMapPos;        // The 3D origin of lightmap.
    glm::vec3 lMapVecs[2];    // The 3D space for s and t unit vectors.
    glm::vec3 vNormal;        // The face normal.
    int       size[2];        // The bezier patch dimensions.
};

// This is our BSP texture structure
struct tBSPTexture {
    char strName[64]; // The name of the texture w/o the extension
    int  flags;       // The surface flags (unknown)
    int  contents;    // The content flags (unknown)
};

struct tBSPLightmap {
    byte imageBits[128][128][3]; // The RGB data in a 128x128 image
};

struct FaceBuffers {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

struct FaceBuffArray {
    std::map<int, FaceBuffers> FB_Idx;
};

struct RenderBuffers // m_renderBuffers.m_faceVBOs[idx].m_vertexBuffer
{
    std::map<int, std::vector<GLfloat>> v_faceVBOs;
    std::map<int, std::vector<GLuint>>  v_faceIDXs;
    std::map<int, std::string>          texvec;
    std::vector<tBSPLightmap>           G_lightMaps;

    std::map<GLuint, GLuint> tx_ID; // optimized texture IDs
    std::map<GLuint, GLuint> lm_ID; // optimized lightmap IDs
};

// This is our lumps enumeration
enum eLumps {
    kEntities = 0, // Stores player/object positions, etc...
    kTextures,     // Stores texture information
    kPlanes,       // Stores the splitting planes
    kNodes,        // Stores the BSP nodes
    kLeafs,        // Stores the leafs of the nodes
    kLeafFaces,    // Stores the leaf's indices into the faces
    kLeafBrushes,  // Stores the leaf's indices into the brushes
    kModels,       // Stores the info of world models
    kBrushes,      // Stores the brushes info (for collision)
    kBrushSides,   // Stores the brush surfaces info
    kVertices,     // Stores the level vertices
    kIndices,      // Stores the level indices
    kShaders,      // Stores the shader files (blending, anims..)
    kFaces,        // Stores the faces for the level
    kLightmaps,    // Stores the lightmaps for the level
    kLightVolumes, // Stores extra world lighting information
    kVisData,      // Stores PVS and cluster info (visibility)
    kMaxLumps      // A constant to store the number of lumps
};

// This is our Quake3 BSP class
class CQuake3BSP {

  public:
    // Our constructor
    CQuake3BSP();

    // This loads a .bsp file by it's file name (Returns true if successful)
    bool LoadBSP(const char* filename);
    int m_numOfVerts; // The number of verts in the model

    int count;
    int indcount;
    int tcoordcount;

    char   tname[MAX_TEXTURES][64];
    int    textureID; // The index into the texture array
    GLuint texture[MAX_TEXTURES];

    // private:

    // r3c:: new functions
    void GenerateTexture();
    void GenerateLightmap();
    void RenderSingleFace(int index);
    void renderFaces();
    void VBOFiller(int index);
    void BuildVBO();
    void CreateVBO(int m_numOfFaces);
    void BSPDebug(int index);
    void CreateRenderBuffers(int index);
    void CreateIndices(int index);

    int m_numOfFaces;    // The number of faces in the model
    int m_numOfIndices;  // The number of indices for the model
    int m_numOfTextures; // The number of texture maps
    int m_numOfLightmaps;
    int m_textures[MAX_TEXTURES];  // The texture and lightmap array for the level
    int m_lightmaps[MAX_TEXTURES]; // The lightmap texture array
    int numVisibleFaces;
    int skipindices;

    GLuint* m_lightmap_gen_IDs;
    GLuint* m_Textures;
    GLuint  missing_LM_id;
    GLuint  missing_id;

    int*        m_pIndices; // The object's indices for rendering
    tBSPVertex* m_pVerts;   // The object's vertices
    tBSPFace*   m_pFaces;   // The faces information of the object

    FaceBuffArray FB_array;
    RenderBuffers Rbuffers;
    tBSPTexture*  pTextures;
    tBSPLightmap* pLightmaps;
};
#endif
