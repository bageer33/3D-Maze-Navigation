#pragma once
#include "cstring"
#include "sstream"
#include "string"
#include "random"
#include "exception"

namespace cs251
{
	enum class Direction
	{
		Right = 1 << 0,
		Left = 1 << 1,
		Top = 1 << 2,
		Bottom = 1 << 3,
		Back = 1 << 4,
		Front = 1 << 5,
		Mark = 1 << 6
	};
	class maze
	{
	public:
		/**
		 * \brief Initialize the maze given dimensions and the seed.
		 * \param dimX The dimension of X axis (right, left)
		 * \param dimY The dimension of Y axis (top, bottom)
		 * \param dimZ The dimension of Z axis (front, back)
		 * \param seed The seed for random direction generator.
		 */
		void initialize(size_t dimX, size_t dimY, size_t dimZ, unsigned seed);
		/**
		 * \brief Function to solve the maze. You should not use recursion to solve the maze.
		 * You should mark the spaces for the shortest path for the map when you successfully find one.
		 * Note that this function may be called multiple times with different start and end coordinates.
		 * You should clear the previous marks before putting new marks on the map.
		 * \param startX start x coordinate
		 * \param startY start y coordinate
		 * \param startZ start z coordinate
		 * \param endX end x coordinate
		 * \param endY end y coordinate
		 * \param endZ end z coordinate
		 * \return Whether the maze is solvable with provided start and end coordinates.
		 */
		bool solve(size_t startX, size_t startY, size_t startZ, size_t endX, size_t endY, size_t endZ);
		/**
		 * \brief Print walls of the 3D maze on a specific layer.
		 * \param z The z value for the target layer.
		 * \return A string represents the walls of the layer.
		 */
		std::string print_walls(size_t z = 0) const;
		/**
		 * \brief Print marks of the 3D maze on a specific layer.
		 * \param z The z value for the target layer.
		 * \return A string represents the marks of the layer.
		 */
		std::string print_marks(size_t z = 0) const;
		/**
		 * \brief Returns the current dimension of the map
		 * \param dimX The dimension of X axis (right, left)
		 * \param dimY The dimension of Y axis (top, bottom)
		 * \param dimZ The dimension of Z axis (front, back)
		 */
		void get_dim(size_t& dimX, size_t& dimY, size_t& dimZ) const;
		
		/**
		 * \brief Get the array index from the coordinate
		 * \param x x coordinate.
		 * \param y y coordinate.
		 * \param z z coordinate.
		 * \return The index of the slot in the map.
		 */
		size_t get_index(size_t x, size_t y, size_t z) const;
	private:
		/**
         * \brief The dimension of X axis (right, left)
         */
        size_t m_dimX = 0;
        /**
         * \brief The dimension of Y axis (top, bottom)
         */
        size_t m_dimY = 0;
        /**
         * \brief The dimension of Z axis (front, back)
         */
        size_t m_dimZ = 0;
        /**
         * \brief The pointer to the array that stores the map of the maze including walls and mark.
         */
        unsigned* m_map = nullptr;
	public:
		/**
		 * \brief The helper function that generate a new random direction based on the input seed.
		 * \param seed The input seed. This function will overwrite seed.
		 * \return The randomly chosen direction.
		 */
		static Direction get_next_direction(unsigned& seed);
        /**
         * \brief The helper function to get the next direction in order
         * \param the current direction
         * \returns the next direction in order
         */
        static Direction get_nonrandom_direction(Direction dir);
        /**
         * \brief The helper function that gets the opposite direction given a direction.
         * \param The direction.
         * \return The opposite direction.
         */
        static Direction get_opposite_direction(Direction dir);
	};
}
