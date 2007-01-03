#ifndef _VFS_MIXARCHIVE_H
#define _VFS_MIXARCHIVE_H
//
// Implements the Archive interface for mix archives
//

#include <map>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/smart_ptr.hpp>

#include "archive.h"

namespace VFS
{
    struct MixArchiveFile;

    class MixArchive : public Archive
    {
    public:
        MixArchive(const boost::filesystem::path& mixfile);
        ~MixArchive();
        
        // Archive info
        std::string archive_path();

        // File management
        int open(const std::string& filename, bool writable);
        void close(int filenum);

        // Reading and Writing
        int read(int filenum, std::vector<char>& buf, int count);
        int read(int filenum, std::vector<char>& buf, char delim);
        int write(int filenum, std::vector<char>& buf);
        void flush(int filenum);

        // File position
        bool eof(int filenum) const;
        void seek_cur(int filenum, int pos);
        void seek_end(int filenum, int pos);
        void seek_start(int filenum, int pos);
        int tell(int filenum) const;

        // File info
        std::string name(int filenum) const;
        int size(int filenum) const;

    private:
        boost::filesystem::path mixfile;
        int curfilenum;
        std::map<int, boost::shared_ptr<MixArchiveFile> > files;
    };
}

#endif