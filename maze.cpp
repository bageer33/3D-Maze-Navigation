#include "maze.hpp"
#include "cyclic_double_queue.hpp"

using namespace cs251;

void maze::initialize(const size_t dimX, const size_t dimY, const size_t dimZ, unsigned seed) {
    // Set maze dimensions
    m_dimX = dimX;
    m_dimY = dimY;
    m_dimZ = dimZ;
    m_map = new unsigned[m_dimX * m_dimY * m_dimZ];

    // Initialize the maze map with all walls
    for (size_t z = 0; z < m_dimZ; ++z) {
        for (size_t y = 0; y < m_dimY; ++y) {
            for (size_t x = 0; x < m_dimX; ++x) {
                m_map[get_index(x, y, z)] = 63; // 63 represents all walls
            }
        }
    }

    // Set strides for indexing
    size_t x_stride = 1;
    size_t y_stride = m_dimX;
    size_t z_stride = m_dimX * m_dimY;

    // Set starting coordinates
    size_t x = 0;
    size_t y = 0;
    size_t z = 0;

    // Create a stack for backtracking
    cyclic_double_queue<size_t> stack;
    stack.enqueue_front(get_index(x, y, z));

    // Loop until stack is empty
    while (!stack.empty()) {
        size_t coord = stack.front();
        x = (coord % z_stride) % y_stride;
        z = (coord - x) / z_stride;
        y = (coord - x - (z * z_stride)) / y_stride;

        // Mark the current coordinate as visited
        m_map[coord] |= static_cast<unsigned>(Direction::Mark);

        // Get random direction
        Direction testDirection = get_next_direction(seed);

        // Loop until you find a valid neighbor or all directions are tested
        bool foundValidNeighbor = false;
        for (int i = 0; i < 6; ++i) {
            // Find the target neighbor coordinate in the test direction
            size_t targetX = x;
            size_t targetY = y;
            size_t targetZ = z;
            switch (testDirection) {
                case Direction::Right:
                    targetX += 1;
                    break;
                case Direction::Left:
                    targetX -= 1;
                    break;
                case Direction::Top:
                    targetY += 1;
                    break;
                case Direction::Bottom:
                    targetY -= 1;
                    break;
                case Direction::Back:
                    targetZ += 1;
                    break;
                case Direction::Front:
                    targetZ -= 1;
                    break;
            }
            // Check if the target is within bounds
            if ((targetX < m_dimX) && (targetY < m_dimY) && (targetZ < m_dimZ) && (targetX >= 0) && (targetY >= 0) && (targetZ >= 0) &&                    (m_map[get_index(targetX, targetY, targetZ)] & static_cast<unsigned>(Direction::Mark)) != static_cast<unsigned>                          (Direction::Mark)) {
                        // Remove the wall between the target neighbor cell and the current cell
                        m_map[get_index(x, y, z)] &= ~static_cast<unsigned>(testDirection);
                        m_map[get_index(targetX, targetY, targetZ)] &= ~static_cast<unsigned>(get_opposite_direction(testDirection));
                        stack.enqueue_front(get_index(targetX, targetY, targetZ));
                        foundValidNeighbor = true;
                        break;
            } else {
                // Check the next direction in the order
                testDirection = get_nonrandom_direction(testDirection);
            }
        }
        // If no valid neighbor was found, backtrack
        if (!foundValidNeighbor) {
            stack.pop_front();
        }
    }
    return;
}

bool maze::solve(const size_t startX, const size_t startY, const size_t startZ, const  size_t endX, const size_t endY, const size_t endZ){
    for (size_t z = 0; z < m_dimZ; ++z) {
        for (size_t y = 0; y < m_dimY; ++y) {
            for (size_t x = 0; x < m_dimX; ++x) {
                m_map[get_index(x, y, z)] &= ~static_cast<unsigned>(Direction::Mark);
            }
        }
    }
	cyclic_double_queue<size_t> stack;
    size_t startCoord = get_index(startX, startY, startZ);
    stack.enqueue_front(startCoord);
    unsigned* prevCoord = new unsigned[m_dimX * m_dimY * m_dimZ];
    size_t x_stride = 1;
    size_t y_stride = m_dimX;    
    size_t z_stride = m_dimX * m_dimY;
    size_t endCoord = get_index(endX, endY, endZ);
    bool foundEnd = false;
    
    while(!stack.empty()) {
        size_t coord = stack.dequeue_front();
        m_map[coord] |= static_cast<unsigned>(Direction::Mark);
        if (coord == endCoord) {
            foundEnd = true;
            break;   
        }
        size_t x = (coord % z_stride) % y_stride;
        size_t z = (coord - x) / z_stride;
        size_t y = (coord - x - (z * z_stride)) / y_stride;
        Direction dir = Direction::Right;
        for (int i = 0; i < 6; ++i) {
            size_t targetX = x;
            size_t targetY = y;
            size_t targetZ = z;
            switch (dir) {
                case Direction::Right:
                    targetX += 1;
                    break;
                case Direction::Left:
                    targetX -= 1;
                    break;
                case Direction::Top:
                    targetY += 1;
                    break;
                case Direction::Bottom:
                    targetY -= 1;
                    break;
                case Direction::Back:
                    targetZ += 1;
                    break;
                case Direction::Front:
                    targetZ -= 1;
                    break;
            }
            if ((targetX < m_dimX) && (targetY < m_dimY) && (targetZ < m_dimZ) && (targetX >= 0) && (targetY >= 0) && (targetZ >= 0)) {                   if (!(m_map[get_index(targetX, targetY, targetZ)] & static_cast<unsigned>(Direction::Mark))) {
                    if (!(m_map[coord] & static_cast<unsigned>(dir))) {
                        stack.enqueue_front(get_index(targetX, targetY, targetZ));
                        prevCoord[get_index(targetX, targetY, targetZ)] = coord;
                    }
                }                                                                                                
            }
            dir = get_nonrandom_direction(dir);
        }
    }
    if (foundEnd) {
        for (size_t z = 0; z < m_dimZ; ++z) {
            for (size_t y = 0; y < m_dimY; ++y) {
                for (size_t x = 0; x < m_dimX; ++x) {
                    m_map[get_index(x, y, z)] &= ~static_cast<unsigned>(Direction::Mark);
                }    
            }
        }
        size_t coord = endCoord;
        while (coord != startCoord) {
              m_map[coord] |= static_cast<unsigned>(Direction::Mark);  
              coord = prevCoord[coord];
        }
        m_map[startCoord] |= static_cast<unsigned>(Direction::Mark);
    }
    return foundEnd;
}

std::string maze::print_walls(const size_t z) const {
    std::stringstream ss;
    // Print the top (bottom) line
    for (size_t x = 0; x < m_dimX; ++x) {
        ss << "+";
        unsigned cellValue = m_map[get_index(x, 0, z)];
        if ((cellValue & static_cast<unsigned>(Direction::Bottom)) == static_cast<unsigned>(Direction::Bottom)) {
            ss << "---";
        } else {
            ss << "   ";
        }
    }
    ss << "+\n";

    for (size_t y = 0; y < m_dimY; ++y) {
        unsigned cellValue = m_map[get_index(0, y, z)];
        // Print the left wall for the current row
        if ((cellValue & static_cast<unsigned>(Direction::Left)) == static_cast<unsigned>(Direction::Left)) {
                ss << "|";
            } else {
                ss << " ";
            }
        for (size_t x = 0; x < m_dimX; ++x) {
            unsigned cellValue = m_map[get_index(x, y, z)];
            // Check for walls to the front and back
            if ((cellValue & static_cast<unsigned>(Direction::Front)) == static_cast<unsigned>(Direction::Front)) {
                if ((cellValue & static_cast<unsigned>(Direction::Back)) == static_cast<unsigned>(Direction::Back)) {
                    ss << " X ";
                } else {
                    ss << " F ";
                }
            } else if ((cellValue & static_cast<unsigned>(Direction::Back)) == static_cast<unsigned>(Direction::Back)) {
                ss << " B ";
            } else {
                ss << "   ";
            }

            // Print the right wall for the current cell
            if ((cellValue & static_cast<unsigned>(Direction::Right)) == static_cast<unsigned>(Direction::Right)) {
                ss << "|";
            } else {
                ss << " ";
            }
        }
        ss << "\n";
        // Print the bottom (top) line for the current row
        for (size_t x = 0; x < m_dimX; ++x) {
            ss << "+";
            unsigned cellValue = m_map[get_index(x, y, z)];
            if ((cellValue & static_cast<unsigned>(Direction::Top)) == static_cast<unsigned>(Direction::Top)) {
                ss << "---";
            } else {
                ss << "   ";
            }
        }
        ss << "+\n";
    }
    return ss.str();
}

std::string maze::print_marks(const size_t z) const {
    std::stringstream ss;
    // Print the top (bottom) line
    for (size_t x = 0; x < m_dimX; ++x) {
        ss << "+";
        unsigned cellValue = m_map[get_index(x, 0, z)];
        if ((cellValue & static_cast<unsigned>(Direction::Bottom)) == static_cast<unsigned>(Direction::Bottom)) {
            ss << "---";
        } else {
            ss << "   ";
        }
    }
    ss << "+\n";

    for (size_t y = 0; y < m_dimY; ++y) {
        unsigned cellValue = m_map[get_index(0, y, z)];
        // Print the left wall for the current row
        if ((cellValue & static_cast<unsigned>(Direction::Left)) == static_cast<unsigned>(Direction::Left)) {
                ss << "|";
            } else {
                ss << " ";
            }
        for (size_t x = 0; x < m_dimX; ++x) {
            unsigned cellValue = m_map[get_index(x, y, z)];
            // Print marks
            if ((cellValue & static_cast<unsigned>(Direction::Mark)) == static_cast<unsigned>(Direction::Mark)) {
                ss << " # ";
            } else {
                ss << "   ";
            }

            // Print the right wall for the current cell
            if ((cellValue & static_cast<unsigned>(Direction::Right)) == static_cast<unsigned>(Direction::Right)) {
                ss << "|";
            } else {
                ss << " ";
            }
        }
        ss << "\n";
        // Print the bottom (top) line for the current row
        for (size_t x = 0; x < m_dimX; ++x) {
            ss << "+";
            unsigned cellValue = m_map[get_index(x, y, z)];
            if ((cellValue & static_cast<unsigned>(Direction::Top)) == static_cast<unsigned>(Direction::Top)) {
                ss << "---";
            } else {
                ss << "   ";
            }
        }
        ss << "+\n";
    }
    return ss.str();
}

void maze::get_dim(size_t& dimX, size_t& dimY, size_t& dimZ) const {
    dimX = m_dimX;
    dimY = m_dimY;
    dimZ = m_dimZ;
    return;
}

Direction maze::get_next_direction(unsigned& seed) {
    // Do not modify this function
    // It provides a deterministic random direction
    std::mt19937 gen(seed);
    seed = gen();
    return static_cast<Direction>(1 << seed % 6);
}

size_t maze::get_index(const size_t x, const size_t y, const size_t z) const {
    size_t x_stride = 1;
    size_t y_stride = m_dimX;
    size_t z_stride = m_dimX * m_dimY;
    return((x * x_stride) + (y * y_stride) + (z * z_stride));
}

Direction maze::get_nonrandom_direction(Direction dir) {
    switch (dir) {
        case Direction::Right: 
            return Direction::Left;
        case Direction::Left: 
            return Direction::Top;
        case Direction::Top: 
            return Direction::Bottom;
        case Direction::Bottom: 
            return Direction::Back;
        case Direction::Back: 
            return Direction::Front;
        case Direction::Front: 
            return Direction::Right;
    }
    return dir;
}

Direction maze::get_opposite_direction(Direction dir) {
    switch (dir) {
        case Direction::Right:
            return Direction::Left;
        case Direction::Left:
            return Direction::Right;
        case Direction::Top:
            return Direction::Bottom;
        case Direction::Bottom:
            return Direction::Top;
        case Direction::Back:
            return Direction::Front;
        case Direction::Front:
            return Direction::Back;
    }
    return dir;
}
