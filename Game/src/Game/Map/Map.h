#pragma once

#include "Tile.h"

namespace Game
{
	enum ChunkFlags
	{
		CHUNK_ACTIVE = 0x01,
		CHUNK_LOADED = 0x02,
	};

	template <uint32_t chunkWidth, uint32_t chunkHeight>
	class Chunk;

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
				m_Childs[index] = new Chunk<chunkWidth, chunkHeight>(m_Size.x / 2, m_Size.y / 2, m_Position.x + (index % 2) * m_Size.x / 2, m_Position.y + (index / 2) * m_Size.y / 2);
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

	private:
		ChunkNode* m_Childs[4];

		glm::uvec2 m_Position;
		glm::uvec2 m_Size;

		unsigned char m_Flags;
	};

	template <uint32_t chunkWidth, uint32_t chunkHeight>
	class Chunk : public ChunkNode<chunkWidth, chunkHeight>
	{
	public:
		Chunk(uint32_t width, uint32_t height, uint32_t x, uint32_t y)
			: ChunkNode(width, height, x, y)
		{}

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

	private:
		Tile** m_Tiles;
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

	class Map
	{
	public:
		Map(uint32_t width, uint32_t height)
			: m_MapTree(width, height)
		{
		}

		Chunk<8, 8>* Load(uint32_t x, uint32_t y)
		{
			return m_MapTree.Load(x, y);
		}
	private:
		ChunkTree<8, 8> m_MapTree;
	};
}