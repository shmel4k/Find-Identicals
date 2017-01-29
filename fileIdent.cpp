#include <unistd.h>

#include <iostream>
#include <algorithm>

#include <string>
#include <vector>
#include <unordered_set>

#include <boost/filesystem.hpp>     
#include <boost/iostreams/device/mapped_file.hpp>
 
    
bool isEqual(std::string lFile, std::string rFile)
{
    boost::iostreams::mapped_file_source f1(lFile);
    boost::iostreams::mapped_file_source f2(rFile);

    return f1.size() == f2.size() && std::equal(f1.data(), f1.data() + f1.size(), f2.data());
}

int main(int argc, char **argv)
{
    // Check command line arguments.
    if (argc != 5)
    {
      std::cerr << "Usage: identify -f <first directory> -d <second directory>\n";
      return 1;
    }
    
    std::string dir1, dir2;
    
    int rez = 0;
    while ( -1 != (rez = getopt(argc,argv,"f:d:")) )
    {
        switch (rez)
        {
	    case 'f': dir1 = optarg;
		      break;
	    case 'd': dir2 = optarg;
		      break;
//	    case '?':   system handler for this error
        }
    }
    
    std::vector < std::string > Dirs( {dir1, dir2} );
    
    std::unordered_set< std::string > StartList; // start list of files (in both dir),
                                                 // easy to remove elements from
    try
    {
        boost::filesystem::path p;
        
        for (auto &dir : Dirs)
        {
            p = dir;
            if (boost::filesystem::exists(p))
            {
                if (boost::filesystem::is_directory(p))
                {
                    for (auto &x : boost::filesystem::directory_iterator(p))
                    {
                        if (boost::filesystem::is_regular_file(x.path()) &&
                            boost::filesystem::file_size(x.path()) > 0 )
                        {
                            // TODO:
                            // for optimization reasons it may be useful to remember
                            // file size as well not to retrieve it again later;
                            // to do that, algorithm has to be modified a bit
                            
                            StartList.insert(x.path().string());
                        }                      
                    }
                }
            }
        }
              
        std::unordered_set< std::string > Dupes; // list of identical files
        std::vector< std::unordered_set<std::string> > FinalGroups;

std::cout << FinalGroups.size() << std::endl;
        for (auto x = StartList.cbegin(); x != StartList.cend(); )
        {
            Dupes.clear();            
            for (auto y = std::next(x); y != StartList.cend(); )
            {
                // TODO:
                // for optimization reason it could be more effective 
                // to anchor content of *x while in inner cycle not
                // to map it in isEqual() each compare pass
                
                if (boost::filesystem::file_size(*x) == boost::filesystem::file_size(*y)
                    && isEqual(*x, *y)) // identical files found
                {
                    Dupes.insert(*y);
                    y = StartList.erase(y);
                }
                else
                {
                    y++;
                }
            }
            
            if (!Dupes.empty())
            {
                Dupes.insert(*x); // base file
                x = StartList.erase(x);
                FinalGroups.push_back(Dupes);
            }
            else
            {
                x++;
            }
        }
        
 std::cout << FinalGroups.size() << std::endl;
 for(auto x: FinalGroups)
    std::cout << "chain size: " << x.size() << std::endl;
    
    }
    
    catch (const boost::filesystem::filesystem_error& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    
    return 0;
}