#include "mtepch.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <glm/glm.hpp>
#include <filesystem>
#include "Kinematics/Framework/Managers/ResourceManager.h"

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
		uint16_t material;
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
		std::vector<Ref<Material>> objMaterials;


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

		for (size_t i = 0; i < materials.size(); i++)
		{
			Ref<Material> m = CreateRef<Material>();
			auto material = materials[i];

			m->SetKa(glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]));
			m->SetKd(glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]));
			m->SetKs(glm::vec3(material.specular[0], material.specular[1], material.specular[2]));
			m->SetKe(glm::vec3(material.emission[0], material.emission[1], material.emission[2]));
		
			m->SetNs(material.shininess);
			m->SetIllum(material.illum);
		
			if (material.diffuse_texname != "")
			{
				auto tex = Texture2D::Create(material.diffuse_texname, WrappingOption::KINEMATICS_REPEAT, WrappingOption::KINEMATICS_REPEAT);
				m->PushTexture(MaterialTypes::DIFFUSE_TEXTURE, tex);
				Kinematics::Resources::Add(std::filesystem::path(material.diffuse_texname).stem().string(), tex);
			}

			objMaterials.push_back(m);
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
						objVerticesData[idx.vertex_index].material = shapes[s].mesh.material_ids[f];

						face.vertices[v] = idx.vertex_index;
						objVertices[idx.vertex_index].normals[idx.normal_index] = face.vertices[v];
					}
					else if (objVertices[idx.vertex_index].normals.find(idx.normal_index) == objVertices[idx.vertex_index].normals.end())
					{
						FaceVertexData data;
						data.position = idx.vertex_index;
						data.normal = idx.normal_index;
						data.textureCoord = idx.texcoord_index;
						data.material = shapes[s].mesh.material_ids[f];
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
				textureCoords.push_back(objVerticesData[i].material);
			}

			for (size_t i = 0; i < objFaces.size(); i++)
			{
				index.push_back(objFaces[i].vertices[0]);
				index.push_back(objFaces[i].vertices[1]);
				index.push_back(objFaces[i].vertices[2]);
			}


			mesh->SetVertices(vertices);
			mesh->SetNormals(normal);
			mesh->SetTextureCoords(textureCoords, true);
			mesh->SetIndices(index);

			model->PushMesh(mesh);
		}

		for (auto m : objMaterials)
		{
			model->PushMaterial(m);
		}

		return model;
	}
}