#include "AStaticActor.h"

//AStaticActor::AStaticActor(char* path)
//{
//	loadModel(path);
//}
//
//void AStaticActor::Draw(Shader& shader)
//{
//	for (AMesh curMesh : meshes)
//	{
//		curMesh.Draw(shader);
//	}
//}
//
//void AStaticActor::loadModel(std::string path)
//{
//	Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
//
//	if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		std::cout << "ERROR: ASSIMP::" << importer.GetErrorString() << std::endl;
//		return;
//	}
//
//	directory = path.substr(0, path.find_last_of('/'));
//
//	processNode(scene->mRootNode, scene);
//}
//
//void AStaticActor::processNode(aiNode* node, const aiScene* scene)
//{
//
//	//process all nodes meshes 
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(processMesh(mesh, scene));
//	}
//
//	//Do the same for its children
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene);
//	}
//}

