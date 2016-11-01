#include "FrameMemory.h"
#include "Frame.h"


FrameMemory::FrameMemory()
{
}

FrameMemory& FrameMemory::getInstance()
{
	static FrameMemory theOneAndOnly;
	return theOneAndOnly;
}

void FrameMemory::releaseBuffes()
{
	boost::unique_lock<boost::mutex> lock(accessMutex);
	int total = 0;


	for(auto p : availableBuffers)
	{
		total += p.second.size() * p.first;

		for(unsigned int i=0;i<p.second.size();i++)
		{
			Eigen::internal::aligned_free(p.second[i]);
			bufferSizes.erase(p.second[i]);
		}

		p.second.clear();
	}
	availableBuffers.clear();

    printf("released %.1f MB!\n", total / (1000000.0f));
}


void* FrameMemory::getBuffer(unsigned int sizeInByte)
{
	boost::unique_lock<boost::mutex> lock(accessMutex);
	
	if (availableBuffers.count(sizeInByte) > 0)
	{
		std::vector< void* >& availableOfSize = availableBuffers.at(sizeInByte);
		if (availableOfSize.empty())
		{
			void* buffer = allocateBuffer(sizeInByte);
//			assert(buffer != 0);
			return buffer;
		}
		else
		{
			void* buffer = availableOfSize.back();
			availableOfSize.pop_back();

//			assert(buffer != 0);
			return buffer;
		}
	}
	else
	{
		void* buffer = allocateBuffer(sizeInByte);
//		assert(buffer != 0);
		return buffer;
	}
}

float* FrameMemory::getFloatBuffer(unsigned int size)
{
	return (float*)getBuffer(sizeof(float) * size);
}

void FrameMemory::returnBuffer(void* buffer)
{
	if(buffer==0) return;

	boost::unique_lock<boost::mutex> lock(accessMutex);
	
	unsigned int size = bufferSizes.at(buffer);
	//printf("returnFloatBuffer(%d)\n", size);
	if (availableBuffers.count(size) > 0)
		availableBuffers.at(size).push_back(buffer);
	else
	{
		std::vector< void* > availableOfSize;
		availableOfSize.push_back(buffer);
		availableBuffers.insert(std::make_pair(size, availableOfSize));
	}
}

void* FrameMemory::allocateBuffer(unsigned int size)
{
	//printf("allocateFloatBuffer(%d)\n", size);
	
	void* buffer = Eigen::internal::aligned_malloc(size);
	bufferSizes.insert(std::make_pair(buffer, size));
	return buffer;
}

