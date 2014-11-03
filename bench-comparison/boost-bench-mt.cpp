#include <thread>
#include <vector>
#include <atomic>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

void init()
{
    logging::add_file_log
    (
        keywords::file_name = "logs/boost-sample_%N.log",                              /*< file name pattern >*/
        keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
        keywords::auto_flush = false,
        keywords::format = "[%TimeStamp%]: %Message%"                                 
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}



using namespace std;

int main(int, char*[])
{
	int thread_count = 10;
	int howmany = 1000000;
	
    init();
    logging::add_common_attributes();
    

    using namespace logging::trivial;
    
    src::severity_logger_mt< severity_level > lg;
    
    std::atomic<int > msg_counter {0};
    vector<thread> threads;
    
    for (int t = 0; t < thread_count; ++t)
    {
        threads.push_back(std::thread([&]()
        {
            while (true)
            {
                int counter = ++msg_counter;
                if (counter > howmany) break;
              BOOST_LOG_SEV(lg, info) << "Boost logger message #" << counter;
            }
        }));
    }


    for(auto &t:threads)
    {
        t.join();
    };
    
    
    return 0;
}
