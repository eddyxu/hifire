/**
 * \brief		Get configuration from command line parameters.
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: configure.h 12 2009-08-12 03:14:53Z eddyxu $
 */


#ifndef _HIFIRE_CONFIG_H_
#define _HIFIRE_CONFIG_H_

#include <string>

using std::string;

/**
 * Configuration class
 *
 * This class is implement as a singleton object, which stores the command line 
 * parameters
 */
class Config {
	static Config *instance_;
public:
	/// get configuration singleton
	static Config * instance();

	enum { DEV_UNKOWN=0, DEV_HARDDRIVE, DEV_SSDSIM };
	enum { POLICY_THREAD_POOL, POLICY_AFAP, POLICY_SPIN };
	enum { OUTPUT_BRIEF, OUTPUT_FULL }; // output level 

	Config();

	/**
	 * Parse arguments
	 */
	int parse_args(int argc, char * argv[]);

	/// get path of input trace file
	string trace_file() const { return trace_file_; }

	/// get path of output file
	string result_file() const { return result_file_; }

	/// get name of device
	string dev_path() const { return dev_path_; }

	/// get capacity of device
	size_t dev_capacity() const { return dev_capacity_; }

	/// get device type
	int dev_type() const { return dev_type_; }

	int policy() const { return policy_; }

	/// get total number of CPUs on this machine
	size_t cpu_num() const { return cpu_num_; }

	size_t thread_num() const { return thread_num_; }

	double pre_spin() const { return pre_spin_; }

	double time_scale() const { return time_scale_; }

	double range_scale() const { return range_scale_; }

	int output_level() const { return output_level_; }
private:
	/**
	 * Print out usage information to stderr
	 */
	void usage();

	void version();

	string trace_file_;
	string result_file_;
	string dev_path_;
	size_t dev_capacity_;
	int dev_type_;
	size_t max_time_;
	size_t max_ops_;
	double time_scale_;
	double range_scale_;
	double pre_spin_;
	size_t cpu_num_;
	size_t thread_num_;
	int policy_;
	int output_level_;
};

#endif /* _HIFIRE_CONFIG_H_ */
