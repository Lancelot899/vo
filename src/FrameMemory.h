#pragma once

#include <unordered_map>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <deque>
#include <list>
#include <boost/thread/shared_mutex.hpp>
#include <Eigen/Core> //For EIGEN MACRO

/** Singleton class for re-using buffers in the Frame class. */
class Frame;
class FrameMemory
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	/** Returns the global instance. Creates it when the method is first called. */
	static FrameMemory& getInstance();

	/** Allocates or fetches a buffer with length: size * sizeof(float).
	  * Corresponds to "buffer = new float[size]". */
	float* getFloatBuffer(unsigned int size);

	/** Allocates or fetches a buffer with length: size * sizeof(float).
	  * Corresponds to "buffer = new float[size]". */
	void* getBuffer(unsigned int sizeInByte);
	
	/** Returns an allocated buffer back to the global storage for re-use.
	  * Corresponds to "delete[] buffer". */
	void returnBuffer(void* buffer);
	
	void releaseBuffes();
private:
	FrameMemory();
	void* allocateBuffer(unsigned int sizeInByte);
	
	boost::mutex accessMutex;
	std::unordered_map< void*, unsigned int > bufferSizes;
	std::unordered_map< unsigned int, std::vector< void* > > availableBuffers;

};

