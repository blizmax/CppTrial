#include "IO/FileStream.h"
#include <filesystem>

// IO::FileStream::FileStream(const String& path, AccessMode accessMode, FileMode fileMode) :
//     pathStr(path), accessMode(accessMode), fileMode(fileMode)
// {
//     std::ios::openmode mode = std::ios::binary;
//     if(IsReadable())
//     {
//         mode |= std::ios::in;
//     }
//     if(IsWriteable())
//     {
//         mode |= std::ios::out;
//     }
//     if(fileMode == FileMode::Append)
//     {
//         mode |= std::ios::app;
//     }
//     else
//     {
//         mode |= std::ios::trunc;
//     }

//     fstream.open(std::filesystem::path(*path), mode);

//     if(!fstream.fail())
//     {
//         fstream.seekg(0, std::ios::end);
//         size = fstream.tellg();
//         fstream.seekg(0, std::ios::beg);
//     }
// }

// IO::FileStream::~FileStream()
// {
//     Close();
// }

// bool IO::FileStream::IsOpen() const
// {
//     return fstream.is_open();
// }

// bool IO::FileStream::IsEnd() const
// {
//     return fstream.eof();
// }

// SizeType IO::FileStream::Size() const
// {
//     return size;
// }