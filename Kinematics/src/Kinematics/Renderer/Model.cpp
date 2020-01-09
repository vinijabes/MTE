#include "mtepch.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace Kinematics
{
	Ref<Model> Model::Load(const std::string& path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

        std::vector<float> normal;
        std::vector<float> textureCoords;
        std::vector<uint32_t> index;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

		if (!warn.empty()) {
			KINEMATICS_CORE_WARN("{}", warn);
		}

		if (!err.empty()) {
			KINEMATICS_CORE_ERROR("{}", err);
		}

        auto model = CreateRef<Model>();
        for (size_t s = 0; s < shapes.size(); s++) {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            auto mesh = Mesh::Create();
            mesh->SetPositionVertexBuffer(&attrib.vertices[0], sizeof(float) * attrib.vertices.size());

            //normal.reserve(shapes[s].mesh.indices.size() * 3);
            //textureCoords.reserve(shapes[s].mesh.indices.size() * 2);
            index.reserve(shapes[s].mesh.indices.size() * 3);
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                int fv = shapes[s].mesh.num_face_vertices[f];

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    index.push_back(idx.vertex_index);
                    /*normal.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    normal.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    normal.push_back(attrib.normals[3 * idx.normal_index + 2]);

                    textureCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    textureCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);*/
                }
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }

            //mesh->SetNormalVertexBuffer(&normal[0], sizeof(float) * normal.size());
            //mesh->SetTextureVertexBuffer(&textureCoords[0], sizeof(float) * textureCoords.size());
            mesh->SetIndexBuffer(&index[0], index.size());
            model->PushMesh(mesh);
        }        

		return model;
	}
}