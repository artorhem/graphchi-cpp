/**
 * @file
 * @author  Aapo Kyrola <akyrola@cs.cmu.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright [2012] [Aapo Kyrola, Guy Blelloch, Carlos Guestrin / Carnegie Mellon University]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 *
 * @section DESCRIPTION
 *
 * Non-interactive version of sharder_basic. All parameters must be
 * provided via command-line arguments.
 *
 * Usage:
 *   sharder_basic_cmdline --file=<path> --edgedatatype=<type> --nshards=<n|auto> --filetype=<fmt>
 *
 * Supported edgedatatypes:
 *   int, uint, short, float, char, double, boolean, long,
 *   float-float, int-int, none
 *
 * Supported filetypes:
 *   edgelist, adjlist, binedgelist, metis, cassovary
 */

#define GRAPHCHI_DISABLE_COMPRESSION

#include <iostream>
#include <stdlib.h>
#include <string>
#include <assert.h>

#include "logger/logger.hpp"
#include "preprocessing/conversions.hpp"
#include "preprocessing/sharder.hpp"
#include "util/cmdopts.hpp"
#include "util/memcounter.hpp"

using namespace graphchi;

static void print_usage(const char *prog) {
    std::cerr << "Usage: " << prog << " --file=<path> --edgedatatype=<type> --nshards=<n|auto> --filetype=<fmt>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "  --file=<path>   Path to the input graph" << std::endl;
    std::cerr << "  --edgedatatype=<type>" << std::endl;
    std::cerr << "                  One of: int, uint, short, float, char, double," << std::endl;
    std::cerr << "                  boolean, long, float-float, int-int, none" << std::endl;
    std::cerr << "  --nshards=<n>   Number of shards to create, or 'auto'" << std::endl;
    std::cerr << "  --filetype=<fmt>" << std::endl;
    std::cerr << "                  One of: edgelist, adjlist, binedgelist, metis, cassovary" << std::endl;
}

int main(int argc, const char ** argv) {
    _flexograph_profile::MemoryCounter memcounter;
    graphchi_init(argc, argv);

    global_logger().set_log_level(LOG_DEBUG);

    std::string basefile = get_option_string("file", "");
    std::string edge_data_type = get_option_string("edgedatatype", "");
    std::string nshards_str = get_option_string("nshards", "");

    if (basefile.empty() || edge_data_type.empty() || nshards_str.empty()) {
        if (basefile.empty())
            std::cerr << "Error: --file is required." << std::endl;
        if (edge_data_type.empty())
            std::cerr << "Error: --edgedatatype is required." << std::endl;
        if (nshards_str.empty())
            std::cerr << "Error: --nshards is required." << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    if (edge_data_type == "float") {
        convert<float, float>(basefile, nshards_str);
    } else if (edge_data_type == "float-float") {
        convert<PairContainer<float>, PairContainer<float> >(basefile, nshards_str);
    } else if (edge_data_type == "int") {
        convert<int, int>(basefile, nshards_str);
    } else if (edge_data_type == "uint") {
        convert<unsigned int, unsigned int>(basefile, nshards_str);
    } else if (edge_data_type == "int-int") {
        convert<PairContainer<int>, PairContainer<int> >(basefile, nshards_str);
    } else if (edge_data_type == "short") {
        convert<short, short>(basefile, nshards_str);
    } else if (edge_data_type == "double") {
        convert<double, double>(basefile, nshards_str);
    } else if (edge_data_type == "char") {
        convert<char, char>(basefile, nshards_str);
    } else if (edge_data_type == "boolean") {
        convert<bool, bool>(basefile, nshards_str);
    } else if (edge_data_type == "long") {
        convert<long, long>(basefile, nshards_str);
    } else if (edge_data_type == "none") {
        convert_none(basefile, nshards_str);
    } else {
        std::cerr << "Unknown edgedatatype: " << edge_data_type << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
