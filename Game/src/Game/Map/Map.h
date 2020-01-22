#pragma once

#include "Tile.h"

#include <fstream>

namespace Game
{
	enum ChunkFlags
	{
		CHUNK_ACTIVE = 0x01,
		CHUNK_LOADED = 0x02,
	};

	template <uint32_t chunkWidth, uint32_t chunkHeight>
	class Chunk;
	class MapLoaderAdapter;

	template <uint32_t chunkWidth, uint32_t chunkHeight>
	class ChunkNode
	{
	public:
		ChunkNode(uint32_t width, uint32_t height, uint32_t x, uint32_t y)
			: m_Size(width, height), m_Position(x, y), m_Childs{ nullptr }, m_Flags(0)
		{
			KINEMATICS_TRACE("CHUNK NODE LOADED (x: {}, y: {}, w: {}, z: {})", m_Position.x, m_Position.y, m_Size.x, m_Size.y);
		}

		ChunkNode(ChunkNode* chunk)
			: m_Size(chunk->GetWidth() / 2, chunk->GetHeight() / 2), m_Childs{ nullptr }
		{
		}

		~ChunkNode()
		{
			delete[] m_Childs;
		}

		Chunk<chunkWidth, chunkHeight>* Load(uint32_t x, uint32_t y)
		{
			if (x < m_Position.x || y < m_Position.y || x > m_Position.x + m_Size.x || y > m_Position.y + m_Size.y) return nullptr;


			int index = 0;
			if (x > m_Position.x + m_Size.x / 2) index = 1;
			if (y > m_Position.y + m_Size.y / 2) index = index + 2;

			if (m_Size.x / 2 <= chunkWidth || m_Size.x / 2 <= chunkHeight)
			{
				if (m_Childs[index]) return (Chunk<chunkWidth, chunkHeight>*)m_Childs[index];

				auto chunkX = m_Position.x + (index % 2) * m_Size.x / 2;
				auto chunkY = m_Position.y + (index / 2) * m_Size.y / 2;
				m_Childs[index] = new Chunk<chunkWidth, chunkHeight>(m_Size.x / 2, m_Size.y / 2, chunkX, chunkY, chunkX/chunkWidth + chunkY/chunkHeight * 512/chunkWidth);
				SetFlags(CHUNK_ACTIVE | CHUNK_LOADED);

				return (Chunk<chunkWidth, chunkHeight>*)m_Childs[index];
			}
			else
			{
				if (m_Childs[index])
					return m_Childs[index]->Load(x, y);
				else
				{

					m_Childs[index] = new ChunkNode(m_Size.x / 2, m_Size.y / 2, m_Position.x + (index % 2) * m_Size.x / 2, m_Position.y + (index / 2) * m_Size.y / 2);
					SetFlags(CHUNK_ACTIVE | CHUNK_LOADED);

					return m_Childs[index]->Load(x, y);
				}
			}
		}

		glm::uvec2 GetSize() const { return m_Size; }

		bool IsFlag(ChunkFlags flag)
		{
			return m_Flags & flag;
		}

		unsigned char GetFlags() const { return m_Flags; }
		void SetFlags(unsigned char flags) { m_Flags = flags; }

	protected:
		ChunkNode* m_Childs[4];

		glm::uvec2 m_Position;
		glm::uvec2 m_Size;

		unsigned char m_Flags;
	};

	template <uint32_t chunkWidth, uint32_t chunkHeight>
	class Chunk : public ChunkNode<chunkWidth, chunkHeight>
	{
	public:
		Chunk(uint32_t width, uint32_t height, uint32_t x, uint32_t y, uint32_t chunkId)
			: ChunkNode(width, height, x, y), m_ChunkID(chunkId)
		{
			m_Tiles = new Tile * [width];

			for (int i = 0; i < width; i++)
			{
				m_Tiles[i] = new Tile[height];
			}
		}

		Chunk(Tile** tiles, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
			: ChunkNode(width, height, x, y)
		{
			m_Tiles = new Tile * [width];

			for (int i = 0; i < width; i++)
			{
				m_Tiles[i] = new Tile[height];
				for (int j = 0; j < height; j++)
				{
					m_Tiles[i][j] = tiles[i][j];
				}
			}

			SetFlags(CHUNK_ACTIVE | CHUNK_LOADED);
		}

		void SetTiles(Tile* tiles)
		{
			for (int i = 0; i < m_Size.x; i++)
			{
				for (int j = 0; j < m_Size.y; j++)
				{
					m_Tiles[i][j] = tiles[i * m_Size.x + j];
				}
			}
		}

		uint32_t GetID() const { return m_ChunkID; }

	private:
		Tile** m_Tiles;
		uint32_t m_ChunkID;
	};

	template <uint32_t chunkWidth, uint32_t chunkHeight>
	class ChunkTree
	{
	public:
		ChunkTree(const uint32_t width, const uint32_t height)
		{
			m_Root = new ChunkNode<chunkWidth, chunkHeight>(width, height, 0, 0);
		}

		Chunk<chunkWidth, chunkHeight>* Load(uint32_t x, uint32_t y) { return m_Root->Load(x, y); }

	private:
		ChunkNode<chunkWidth, chunkHeight>* m_Root;
	};

	class FileMapLoaderAdapter;

	class Map
	{
		friend class MapLoaderAdapter;

	public:
		Map(uint32_t width, uint32_t height);
		~Map()
		{
			delete m_Loader;
		}

		void Load(uint32_t x, uint32_t y);
		void Load(uint32_t x, uint32_t y, std::function<void(Chunk<8,8>*)> cb);


	private:
		ChunkTree<8, 8> m_MapTree;
		MapLoaderAdapter* m_Loader;
	};

	class MapLoaderAdapter
	{
	public:
		MapLoaderAdapter(Map* map)
			: m_Map(map)
		{

		}

		virtual void Load(uint32_t x, uint32_t y, std::function<void(Chunk<8, 8>*)> cb) = 0;

	protected:
		Chunk<8, 8>* InternalLoad(uint32_t x, uint32_t y)
		{
			return m_Map->m_MapTree.Load(x, y);
		}

	protected:
		Map* m_Map;
	};

	class FileMapLoaderAdapter : public MapLoaderAdapter
	{
	public:
		FileMapLoaderAdapter(Map* map, std::string mapFilePath)
			: MapLoaderAdapter(map), m_FilePath(mapFilePath)
		{
		}

		virtual void Load(uint32_t x, uint32_t y, std::function<void(Chunk<8, 8>*)> cb) override
		{
			auto chunk = MapLoaderAdapter::InternalLoad(x, y);
			if (chunk->IsFlag(CHUNK_LOADED)) 
			{
				cb(chunk);
				return;
			}

			auto width = chunk->GetSize().x;
			auto height = chunk->GetSize().y;
			auto size = width * height;
			auto filePos = sizeof(TileData) * size * chunk->GetID();

			m_File.open(m_FilePath, std::ios::in | std::ios::binary);
			if (!m_File.fail())
			{
				TileData* tileData = new TileData[size];
				Tile* tiles = new Tile[size];
				m_File.seekg(filePos, std::ios_base::beg);
				m_File.getline((char *)tileData, sizeof(TileData) * size);

				for (int i = 0; i < 64; i++)
				{
					tiles[i].SetPosition(tileData[i]);
				}
				chunk->SetTiles(tiles);
				
				m_File.close();
				delete tileData;

				cb(chunk);
			}
		}

	private:
		std::string m_FilePath;
		std::fstream m_File;
	};
}