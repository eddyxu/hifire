/**
 * \brief		A block chain container template
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 */

#ifndef _QLIB_BLOCK_CHAIN_H_
#define _QLIB_BLOCK_CHAIN_H_

#include <vector>
#include <list>
#include <stdexcept>
#include "iterator.h"

using std::list;
using std::vector;

/**
 * A chain with fixed size blocks
 */
template<class T, size_t blkSize = 1024 * 1024>
class block_chain {
	class BCIterator;
public:
	static const size_t BLOCKSIZE = blkSize; // 1M 
	typedef size_t size_type;
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef BCIterator iterator;
public:
	block_chain();
	virtual ~block_chain();

	void push_back(const T & x);

	/**
	 * Get an I/O event by index
	 */
	pointer get(size_type index) const;

	const_reference at(size_type index) const { return at(index); }
	reference at(size_type index);

	const_reference operator [] ( size_type index) const { return at(index); }
	reference operator[] ( size_type index ) { return at(index); }

	/// get the total number of io events
	size_type size() const;

	size_type capacity() const;

	bool empty() const { return size() == 0; }
private:
	friend class BCIterator;
	class BCIterator : public Iterator< block_chain > {
	public:
		BCIterator( block_chain & chain ) : Iterator<block_chain>(chain) { 
			//Iterator<block_chain>::Iterator(chain);
			blk_iter = chain.blocks_.begin();
			if(hasNext()) {
				item_iter = (*blk_iter)->begin();
			} else {
				item_iter = (*blk_iter)->end(); //TODO: fix it
			}
		}

		inline bool hasNext() { return blk_iter != this->container_.blocks_.end(); }

		inline reference next() {
			reference item = *item_iter;
			++item_iter;
			if(item_iter == (*blk_iter)->end()) {
				++blk_iter;
				if(hasNext()) {
					item_iter = (*blk_iter)->begin();
				}
			}
			return item;
		}
	private:
		typedef typename block_chain::block_container block_container;
		typedef typename block_chain::block_t block_t;
		typename block_container::iterator blk_iter;
		typename block_t::iterator item_iter;
	};
public:
	iterator begin() { return iterator(*this); }
	iterator end() {}
private:
	typedef std::vector<T> block_t;
	typedef std::list<block_t *> block_container;
	block_container blocks_;
};

template<typename T, size_t blkSize>
void block_chain<T, blkSize>::push_back(const T & x) {
	if(blocks_.empty() || blocks_.back()->size() >= BLOCKSIZE) {
		block_t * new_blk = new block_t;
		new_blk->reserve(BLOCKSIZE);
		blocks_.push_back(new_blk);
	}
	block_t * blk = blocks_.back();
	blk->push_back(x);
	return;
}


template<typename T, size_t blkSize>
typename block_chain<T, blkSize>::reference 
block_chain<T,blkSize>::at(typename block_chain<T, blkSize>::size_type index)
{
	if(index >= size())
		throw std::out_of_range("block_chain::at: out of range");
	
	typename block_container::size_type block_idx;
	typename block_t::size_type item_idx;
	block_idx = index / BLOCKSIZE;
	item_idx = index % BLOCKSIZE;

	//TODO: optimize this silly loop
	typename block_container::iterator it = blocks_.begin();
	while(block_idx) {
		++it;
		block_idx--;
	}
	return (*it)->at(item_idx);
}


template<typename T, size_t blkSize>
block_chain<T, blkSize>::block_chain()
{
	block_t * blk = new block_t;
	blk->reserve(BLOCKSIZE);
	blocks_.push_back(blk);
}


template<typename T, size_t blkSize>
block_chain<T, blkSize>::~block_chain() 
{
	for(typename block_container::iterator it = blocks_.begin(); it != blocks_.end(); ++it) {
		delete *it;
	}
};


template<typename T, size_t blkSize>
typename block_chain<T,blkSize>::size_type 
block_chain<T, blkSize>::size() const 
{
	if(blocks_.empty())
		return 0;

	return (blocks_.size() - 1) * BLOCKSIZE + blocks_.back()->size();
};

template<typename T, size_t blkSize>
typename block_chain<T,blkSize>::size_type 
block_chain<T, blkSize>::capacity() const 
{
	return blocks_.size() * BLOCKSIZE;
};

#endif /* _QLIB_BLOCK_CHAIN_H_ */


