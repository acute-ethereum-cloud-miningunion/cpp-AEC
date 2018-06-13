/*
    This file is part of cpp-aecmio.
 
    cpp-aecmio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    cpp-aecmio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with cpp-aecmio.  If not, see <http://www.gnu.org/licenses/>.
 */
/** @file Common.cpp
 * @author Marek Kotewicz <marek@aecdev.com>
 * @date 2015
 */

#include <random>
#include <libdevcore/CommonData.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/FileSystem.h>
#include <test/tools/libtestaec/Options.h>
#include "Common.h"
#include <boost/filesystem.hpp>

using namespace std;
using namespace dev;
using namespace dev::test;
namespace fs = boost::filesystem;

boost::filesystem::path dev::test::getTestPath()
{
    if (!Options::get().testpath.empty())
        return Options::get().testpath;

    string testPath;
    const char* ptestPath = getenv("aecmio_TEST_PATH");

    if (ptestPath == nullptr)
    {
        clog(VerbosityWarning, "test")
            << " could not find environment variable aecmio_TEST_PATH \n";
        testPath = "../../test/jsontests";
    }
    else
        testPath = ptestPath;

    return boost::filesystem::path(testPath);
}

int dev::test::randomNumber()
{
    static std::mt19937 randomGenerator(utcTime());
    randomGenerator.seed(std::random_device()());
    return std::uniform_int_distribution<int>(1)(randomGenerator);
}

Json::Value dev::test::loadJsonFromFile(fs::path const& _path)
{
    Json::Reader reader;
    Json::Value result;
    string s = dev::contentsString(_path);
    if (!s.length())
        clog(VerbosityWarning, "test")
            << "Contents of " << _path.string()
            << " is empty. Have you cloned the 'tests' repo branch develop and "
               "set aecmio_TEST_PATH to its path?";
    else
        clog(VerbosityWarning, "test") << "FIXTURE: loaded test from file: " << _path.string();

    reader.parse(s, result);
    return result;
}

fs::path dev::test::toTestFilePath(std::string const& _filename)
{
    return getTestPath() / fs::path(_filename + ".json");
}

fs::path dev::test::getRandomPath()
{
    std::stringstream stream;
    stream << randomNumber();
    return getDataDir("aecmioTests") / fs::path(stream.str());
}

