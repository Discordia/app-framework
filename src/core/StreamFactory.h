#pragma once

#include <core/Stream.h>

#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

class StreamFactory
{
public:
    StreamFactory() {}
    virtual ~StreamFactory() {}

    virtual shared_ptr<Stream> open(const string& fileName) = 0;
};

