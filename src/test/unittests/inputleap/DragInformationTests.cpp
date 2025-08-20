/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) 2013-2016 Symless Ltd.
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "inputleap/DragInformation.h"

#include <gtest/gtest.h>

namespace inputleap {

TEST(DragInformationTests, parseDragInfo_multipleFiles_parsedOnce)
{
    DragFileList files;
    std::string data = "dir1/file1.txt,10,dir2/file2.txt,20,";

    DragInformation::parseDragInfo(files, 2, data);

    ASSERT_EQ(2u, files.size());
    EXPECT_EQ("file1.txt", files.at(0).getFilename());
    EXPECT_EQ(10u, files.at(0).getFilesize());
    EXPECT_EQ("file2.txt", files.at(1).getFilename());
    EXPECT_EQ(20u, files.at(1).getFilesize());
}

} // namespace inputleap

