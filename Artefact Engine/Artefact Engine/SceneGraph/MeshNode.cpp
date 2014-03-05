#include "MeshNode.h"
#include "../Asset/AssetLoader.h"

MeshNode::MeshNode(std::string path) : SceneNode(NodeType::MESH), path(path)
{
	AssetLoader ass = AssetLoader();
	ass.loadAssets(path, this);
}


MeshNode::~MeshNode()
{
}
