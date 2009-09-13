#!/usr/bin/env python
#
# Maniplate hifire's log 
#
# Author: U{Lei Xu<mailto:lxu@cse.unl.edu>}
# 
# Copyright: 2009 (c) University of Nebraska - Lincoln
# License: New BSD License
#
# $Id: hifilog.py 23 2009-08-31 21:14:38Z eddyxu $
#

"""This script provides tools for analyze hifire's log
"""

import sys
import optparse 
import math
import numpy as np

def log_file(fobj, comment="#"):
    """Iterate log file
    """
    for line in fobj:
        line = line.strip()
        if not line or line[0] == comment:
            continue

        yield line

    fobj.close()

def extract_columes():
    """Extract several columes, act as "awk"
    """
    parser = optparse.OptionParser()
    parser.add_option("-c", "--columes", dest="cols", 
        help="define the columes that need to be extract.")
    parser.add_option("-t", "--threshold", dest="thresh", 
        help="define the threshold of error that is reasonable.")
    parser.add_option("-j", "--join", dest="join", 
        help="join string", default=" ")

    (options, args) = parser.parse_args()
    cols = options.cols
    if not cols:
        parser.print_help()
        sys.exit(0)

    if len(args) == 0:
        filepath = "result.log"
    else:
        filepath = args[0]

    cols = cols.split(",")
    cols = map(int, cols)
    if options.thresh:
        thresh = float(options.thresh)
    else:
        thresh = 0 
    for line in log_file(open(filepath)):
        result = []
        datas = map(float, line.split(","))
        for colume in cols:
            value = datas[colume]
            if len(cols) == 1 and thresh > 0 and value >= thresh:
                break
            result.append("%f" % value)
        if result:
            print options.join.join(result)


def get_stat_info(fobj, bins, err_col=3):
    """Read statistic information from one log file
    """
    data = np.loadtxt(fobj, delimiter=',', 
        converters = {err_col: lambda s: float(s) * 10 ** 6}, usecols=(3,))
    histo, bins = np.histogram(data, bins)
    return histo

def stat_source():
    """Analyze input trace characteristic 
    """
    parser = optparse.OptionParser(usage="%prog source|src [options] FILE")
    parser.add_option('-b', '--bins', dest='bins', help='set statistic bins')
    parser.add_option("--latex", action="store_true", default="false", 
        dest="latex", help="generate latex output")
    options, args = parser.parse_args()
    if not args:
        fobj = sys.stdin
    else:
        fobj = open(args[0])

    size_bins = [ 2 ** x * 1024 for x in xrange(1, 10) ]
    size_bins.insert(0,0)
    size_bins.append(1024 * 1024 * 4)
    datas = np.loadtxt(fobj, delimiter=',', usecols=(2,))
    histo, bins = np.histogram(datas, size_bins)
    total_count = sum(histo)

    if options.latex:
        print("\\begin{tabular}[ht]{|c|r|r|}")
        for bin, value in zip(size_bins[:-1], histo):
            percentage = float(value) / total_count * 100
            print("    \hline")
            print("    %s & %s & %%%s \\\\" % (bin, value, percentage))

        print("\hline")
        print("\\end{tabular}")
    else:
        pass

        
    
def stat_result():
    """Stat the error time for result
    """
    parser = optparse.OptionParser("%prog st|stat [options] FILE...")
    parser.add_option("-b", "--bins", dest="bins", help="set bins")
    parser.add_option("--latex", action="store_true", default="false", 
        dest="latex", help="print out output in latex format")
    options, args = parser.parse_args()
    if len(args) == 0:
        fobj = sys.stdin
    else:
        fobj = open(args[0])

    count = 0
    if options.bins:
        thresholds = options.bins
    else:
        thresholds = [1, 10, 20, 50, 100, 200, 500]
    counters = [ 0 for i in range(len(thresholds) + 1) ]

    total_error = 0
    for line in log_file(fobj):
        datas = line.strip().split(",")
           
        var_time = math.fabs(float(datas[3]))
        total_error += var_time
        i = 0
        for tvalue in thresholds:
            if var_time <= tvalue * (0.1 ** 6):  # micro second
                counters[i] += 1
                break
            i += 1
                
        if i == len(thresholds):
            counters[i] += 1
        count += 1

    print("Total I/O: %d, Avarage start time error: %f" % 
        (count, total_error / count))
    for tvalue in thresholds:
        tc = counters[thresholds.index(tvalue)]
        print "< %d microsec: %d(%%%f)" % (tvalue, tc, float(tc)/float(count) * 100)

    print(" > %d micorsec: %d(%%%f)" % 
        (thresholds[-1], counters[-1], float(counters[-1])/count * 100))

    


def best_result():
    """Get best result from several result files
    """
    parser = optparse.OptionParser(usage="%prog best [options] FILES...")
    options, args = parser.parse_args()
    if( len(args) < 2 ):
        parser.error("Missing arguments: required at least two files")
        
    best_file = None
    best_ratio = 0
    for logfile in args:
        hist = get_stat_info(logfile, [0, 10, 20, 50, 100, 200, 500, 1000000])
        total = sum(hist)
        ratio = float(hist[0] + hist[1]) / total
        if ratio > best_ratio:
            best_ratio = ratio
            best_file = logfile 

    print("Best result( %f %% ): %s" % ( best_ratio, best_file ))


def iops():
    """
    """
    parser = optparse.OptionParser(usage="%prog iops [options] FILE")
    parser.add_option("-c", "--column", dest="column", type="int", default=0,
        help="set timestamp colume in trace file")
    parser.add_option("-o", "--output", dest="output", metavar="FILE", 
        help="set output file")
    parser.add_option("--delimiter", dest="delimiter", help="set delimiter",
        default=",")
    options, args = parser.parse_args()
    if not args:
        parser.error("Missing arguments")
    fname = args[0]
    datas = np.loadtxt(fname, delimiter=",", usecols=(options.column,))
    datas = map(int, datas)
    results = {}
    for ts in datas:
        try:
            results[ts] += 1
        except KeyError:
            results[ts] = 1

    seconds = results.keys()
    seconds.sort()
    for sec in seconds:
        print("%d%s%d" % (sec, options.delimiter, results[sec]))


def latex_output(datas, ):
    """Generate latex output
    """
    pass


def print_help():
    """Print out help information
    """
    print("""%s [command] [options] FILE
Supported commands:
  ex|extract
  st|stat
  best\t\tfigure out best result.
  iops\t\tcalculate I/O per second from the file.
  help\t\tdisplay this help
  version\t\tdisplay version information
  """ % sys.argv[0])


def version():
    """Print out version information
    """
    print("""%s -- 0.0.1 \n
Bug report to Lei Xu <lxu@cse.unl.edu>""" % sys.argv[0])

def main():
    if len(sys.argv) < 2:
        stat_result()
        sys.exit(0)

    method_map = { "help" : print_help,
        "stat|st" : stat_result,
        "extract|ex" : extract_columes,
        "best" : best_result,
        "iops|io" : iops,
        "source|src" : stat_source,
    }
    command = sys.argv[1]
    sys.argv.remove(command)
    if command in ['-h', '--help', 'help']:
        print_help() 
        sys.exit(0)
    if command in ['-v', '--version', 'version']:
        version()
        sys.exit(0)

    method = None
    for method_name in method_map.keys():
        names = method_name.split("|")
        if command in names:
            method = method_map[method_name]
            break

    if not method:
        print("Unknown command: %s" % command)
        print_help()
        sys.exit(0)

    method()

if __name__ == "__main__":
    main()

