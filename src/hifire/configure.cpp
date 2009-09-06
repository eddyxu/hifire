/**
 * \brief		Implementation of runtime configuration
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: configure.cpp 12 2009-08-12 03:14:53Z eddyxu $
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <exception>

#include <boost/program_options.hpp>

#include "hifire.h"
#include "configure.h"
#include "sysinfo.h"

namespace po = boost::program_options;
using namespace std;

const string DEFAULT_RESULT = "result.log";

Config * 
Config::instance_ = NULL;

Config * 
Config::instance() 
{
	if(!Config::instance_) {
		Config::instance_ = new Config();
	}
	return Config::instance_;
}


Config::Config() : result_file_(DEFAULT_RESULT), dev_type_(DEV_HARDDRIVE), max_time_(0), max_ops_(0), 
	time_scale_(1), range_scale_(1), pre_spin_(0.00001), cpu_num_(2), thread_num_(4), policy_(POLICY_THREAD_POOL),
	output_level_(OUTPUT_BRIEF)
{

}


int
Config::parse_args(int argc, char * argv[]) 
{
	string dev_str;
	po::options_description options("Allowed options");
	options.add_options() 
		("help", "produce help message")
		("version,v", "display version information")
		("input,f", po::value<string>(&trace_file_), "set input trace file")
		("output,o", po::value<string>(&result_file_)->default_value("result.log"), "set output file")
		("output-level", po::value<string>()->default_value("brief"), "set output level. ( brief, full )")
		("dev-type,t", po::value<string>(&dev_str)->default_value("raid"), "set device type")
		("max-time", po::value<size_t>(&max_time_)->default_value(0), "set max time to replaying trace")
		("max-operations", po::value<size_t>(&max_ops_)->default_value(0), "set how many trace will be replayed")
		("capacity,c", po::value<size_t>(&dev_capacity_), "set maximum capacity(KB)")
		("time-scale,i", po::value<double>(&time_scale_)->default_value(1), "set time scale (default:1, faster 2+)")
		("range-scale,r", po::value<double>(&range_scale_)->default_value(1), "set range scale (default:1, largger 2+)")
		("device,d", po::value<string>(&dev_path_), "set device path");
		;

	po::options_description perf_options("Performance options");
	perf_options.add_options()
		("thread-num,z", po::value<size_t>(&thread_num_), "set thread pool size")
		("pre-spin,s", po::value<double>(&pre_spin_), "set pre-spin value to eliminate context switching time")
		("policy,p", po::value<string>()->default_value("pool"), "set scheduler policy. ( afap, pool, spin )")
		;
	options.add(perf_options);

	po::positional_options_description p;
	p.add("device", -1);

	try 
	{
		po::variables_map vm;
		po::parsed_options parsed = po::command_line_parser(argc, argv).options(options).positional(p).allow_unregistered().run();
		po::store(parsed, vm);
		po::notify(vm);

		if(vm.count("help") || argc < 2) {
			throw po::error(""); 
		}
		if(vm.count("version")) {
			version();
			return -1;
		}
		if(vm.count("dev-type")) {
			string devtype = vm["dev-type"].as<string>();
			if(devtype == "raid") {
				dev_type_ = DEV_HARDDRIVE;
			} else {
				dev_type_ = DEV_UNKOWN;
				string errmsg("Unknown device type: ");
				errmsg += devtype;
				throw po::error(errmsg);
			}
		}
		if(vm.count("output-level")) {
			string level = vm["output-level"].as<string>();
			if(level == "brief") {
				output_level_ = OUTPUT_BRIEF;
			} else if (level == "full") {
				output_level_ = OUTPUT_FULL;
			} else {
				string errmsg("Unknown output level: ");
				errmsg += level;
				throw po::error(errmsg);
			}
		}

		if(vm.count("policy")) {
			string pl = vm["policy"].as<string>();
			if(pl == "afap") {
				policy_ = POLICY_AFAP;
			} else if(pl == "pool") {
				policy_ = POLICY_THREAD_POOL;
			} else if(pl == "spin") {
				policy_ = POLICY_SPIN;
			} else {
				string errmsg("Unknown policy: ");
				errmsg += pl;
				throw po::error(errmsg);
			}
		}

		vector<string> args = collect_unrecognized(parsed.options, po::include_positional);
		if(args.empty()) {
			throw po::error("Need device");
		}
		dev_path_ = args[0];

	} 
	catch ( exception & e ) 
	{
		string err = e.what();
		if(!err.empty()) {
			cerr << "Error:" << err << endl;
		}
		cerr << "Usage: hifire [options] device" << endl;
		cerr << options << endl;
		cerr << "Bug report to Lei Xu <lxu@cse.unl.edu>" << endl;
		return -1;
	}
	
	return 0;
}

void
Config::version() 
{
	cerr << "hifire 0.0.1" << endl;
}

