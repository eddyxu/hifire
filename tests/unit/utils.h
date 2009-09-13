/**
 * \brief		Utilities for test cases
 *
 * \author		Xu Lei <lxu@cse.unl.edu>
 *
 * $Id: utils.h 7 2009-08-11 05:00:09Z eddyxu $
 */

#ifndef _HIFIRE_TEST_UTILS_H_
#define _HIFIRE_TEST_UTILS_H_

#include <string>

using std::string;

#if defined(__linux__) || defined(linux)
static const string TEMP_FILE_TEMPLATE = "/tmp/hifire-test.XXXXXX";
#endif /* __linux__ */

class TestUtils {
public:
};

#endif /* _HIFIRE_TEST_UTILS_H_ */
