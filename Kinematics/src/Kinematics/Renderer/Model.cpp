#include "mtepch.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <glm/glm.hpp>

namespace Kinematics {

	struct ObjVertex
	{
		size_t position;
		size_t textureCoord = -1;
		std::map<size_t, size_t> normals;
	};

	struct FaceVertexData
	{
		size_t position;
		size_t textureCoord;
		size_t normal;
	};

	struct ObjFace
	{
		size_t vertices[3];
	};

	Ref<Model> Model::Load(const std::string& path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		std::vector<float> vertices;
		std::vector<float> normal;
		std::vector<float> textureCoords;
		std::vector<uint32_t> index;
		std::vector<ObjVertex> objVertices;
		std::vector<FaceVertexData> objVerticesData;
		std::vector<ObjFace> objFaces;


		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

		objVertices.resize(attrib.vertices.size() / 3);
		objVerticesData.resize(attrib.vertices.size() / 3);

		for (int i = 0; i < attrib.vertices.size() / 3; ++i)
		{			
			objVertices[i].position = i;
			objVerticesData[i].position = i;
		}

		if (!warn.empty())
		{
			KINEMATICS_CORE_WARN("{}", warn);
		}

		if (!err.empty())
		{
			KINEMATICS_CORE_ERROR("{}", err);
		}

		auto model = CreateRef<Model>();
		for (size_t s = 0; s < shapes.size(); s++)
		{
			// Loop over faces(polygon)
			size_t index_offset = 0;
			auto mesh = Mesh::Create();
			objFaces.clear();
			objFaces.reserve(shapes[s].mesh.num_face_vertices.size());

			normal.reserve(attrib.vertices.size());
			textureCoords.reserve(attrib.vertices.size() / 3 * 2);
			index.reserve(shapes[s].mesh.indices.size() * 3);

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				int fv = shapes[s].mesh.num_face_vertices[f];
				ObjFace face;

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++)
				{
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					if (objVertices[idx.vertex_index].normals.size() == 0)
					{
						objVerticesData[idx.vertex_index].position = idx.vertex_index;
						objVerticesData[idx.vertex_index].normal = idx.normal_index;
						objVerticesData[idx.vertex_index].textureCoord = idx.texcoord_index;

						face.vertices[v] = idx.vertex_index;
						objVertices[idx.vertex_index].normals[idx.normal_index] = face.vertices[v];
					}
					else if (objVertices[idx.vertex_index].normals.find(idx.normal_index) == objVertices[idx.vertex_index].normals.end())
					{
						FaceVertexData data;
						data.position = idx.vertex_index;
						data.normal = idx.normal_index;
						data.textureCoord = idx.texcoord_index;
						objVerticesData.push_back(data);

						face.vertices[v] = objVerticesData.size() - 1;
						objVertices[idx.vertex_index].normals[idx.normal_index] = face.vertices[v];
					}
					else
					{
						face.vertices[v] = objVertices[idx.vertex_index].normals[idx.normal_index];
					}


				}
				objFaces.push_back(face);
				index_offset += fv;

				// per-face material
				shapes[s].mesh.material_ids[f];
			}

			for (size_t i = 0; i < objVerticesData.size(); ++i)
			{
				auto p_Index = objVerticesData[i].position * 3;
				vertices.push_back(attrib.vertices[p_Index]);
				vertices.push_back(attrib.vertices[p_Index + 1]);
				vertices.push_back(attrib.vertices[p_Index + 2]);

				auto p_Normal = objVerticesData[i].normal * 3;
				normal.push_back(attrib.normals[p_Normal]);
				normal.push_back(attrib.normals[p_Normal + 1]);
				normal.push_back(attrib.normals[p_Normal + 2]);

				auto p_TC = objVerticesData[i].textureCoord * 2;
				textureCoords.push_back(attrib.texcoords[p_TC]);
				textureCoords.push_back(attrib.texcoords[p_TC + 1]);
			}

			for (size_t i = 0; i < objFaces.size(); i++)
			{
				index.push_back(objFaces[i].vertices[0]);
				index.push_back(objFaces[i].vertices[1]);
				index.push_back(objFaces[i].vertices[2]);
			}


			mesh->SetPositionVertexBuffer(&vertices[0], sizeof(float) * vertices.size());
			mesh->SetNormalVertexBuffer(&normal[0], sizeof(float) * normal.size());
			mesh->SetTextureVertexBuffer(&textureCoords[0], sizeof(float) * textureCoords.size());
			mesh->SetIndexBuffer(&index[0], index.size());
			model->PushMesh(mesh);
		}

		return model;
	}
}