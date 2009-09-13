/*
 * sequence structure
 *
 * Author: Lei Xu <lxu@cse.unl.edu>
 */

#ifndef _HIFIRE_SEQUENCE_H_
#define _HIFIRE_SEQUENCE_H_

class atom_queue_iterator;

class atom_queue {
	friend class atom_queue_iterator;
public:
	atom_queue();
	atom_queue(const atom_queue & rhs);
	atom_queue & operator=(const atom_queue & rhs);
	virtual ~atom_queue();

	int next();
};


class atom_queue_iterator {
public:

};


#endif /* _HIFIRE_SEQUENCE_H_ */


