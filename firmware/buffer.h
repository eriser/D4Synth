#include <stdbool.h>

/** The modulo operator. % is the remainder which makes a difference 
 * with negative numbers.
 * @return a mod b
 **/
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

/** A versatile Ringbuffer class. It does the management to access data
 * from a ringbuffer, but does not store the data itself. Instead, it
 * must be supplied with a pointer to a block of memory to be used for 
 * the buffer.
 **/
template <typename T>
class RingBuffer
{
public:
	/** Creates a ringbuffer of the size 0. Use setInternalBuffer()
	 * to assign memory to it. 
	 **/
	RingBuffer() 
	{
		setInternalBuffer(NULL, 0);
	}
	
	/** Creates a ringbuffer of the using the given block of memory
	 **/
	RingBuffer(T* data, int size)
	{
		setInternalBuffer(data, size);
	}
	
	/** Destructor. The block of memory used before is now free. 
	 **/
	~RingBuffer()
	{
	}

	/** Assignes the given block of memory to the ringbuffer. Previously
	 * assigned memory will become unused. Initialized the buffer to 
	 * be empty.
	 **/
	inline void setInternalBuffer(T* data, int size)
	{
		m_data = data;
		m_size = size;
		m_head = m_tail = 0;
	}
	
	/** Returns the total size of the buffer
	 **/
	inline int getSize() const
	{
		return m_size;
	}
	
	/** Returns true, if the buffer is empty 
	 **/
	inline bool isEmpty() const
	{
		return (m_head == m_tail);
	}
	 
	/** Returns true, if the buffer can hold at least one more element.
	 * Faster alternative to (getNumberFree() >= 1)
	 **/
	inline bool canTakeNewData() const
	{
		if (m_tail == 0)
			return m_head != m_size-1;
		else
			return m_head != m_tail-1;
	}
	
	/** Returns the remaining capacity of the buffer.
	 **/
	inline int getNumberFree() const
	{
		return mod(((m_tail - m_head) - 1), m_size);
	}
	
	/** Returns the number of data elements stored in the buffer.
	 **/
	inline int getNumberStored() const
	{
		return (m_head - m_tail) % m_size;
	}
	 
	/** Returns the element at the buffers tail (=oldest element)
	 * 
	 * returns NULL, if no elements available.
	 **/
	inline const T* getTail() const 
	{
		if (!isEmpty())
			return &m_data[m_tail];
		else
			return NULL;
	}
	 
	/** Reads and removes the element at the buffers tail (=oldest 
	 * element). This is not interrupt/thread safe. The element is 
	 * marked as free, before it is returned. An interruption in
	 * between might overwrite the element. Use
	 * getTail() first to read it safely, then use removeTail();
	 * 
	 * returns NULL, if no elements available.
	 **/
	inline const T* removeTail() 
	{
		if (!isEmpty())
		{
			int old_tail = m_tail;
			m_tail = (m_tail+1)%m_size;
			return &m_data[old_tail];
		}
		else
			return NULL;
	}
	
	/** places a new element into the buffer. returns true, if 
	 * successfull
	 **/
	inline bool add(const T& element)
	{
		if (canTakeNewData())
		{
			m_data[m_head] = element;
			m_head = (m_head+1)%m_size;
			return true;
		}
		else
			return false;
	}
	 
private:
	// do not allow copy and assignment
	RingBuffer(const RingBuffer& other) {}
	RingBuffer& operator=(const RingBuffer& other) {}

	T* m_data;
	int m_size;
	int m_head;
	int m_tail;
};
