#include "common\autoref.h"
#include "common\file.h"

#include "gtest\gtest.h"

#include "config.h"

namespace FileTests {

    using namespace Common;

    TEST(File, ReadFile)
    {
        FileRef file = File::Create();
        ASSERT_TRUE(file.IsAttached() != 0);

        ASSERT_TRUE(file->Read(TESTS_SOURCE_DIR "file.txt") != 0) << "Cannot read the file";
        ASSERT_TRUE(file->GetSize() == 12);

        const void * fileData = file->GetData();
        ASSERT_TRUE(fileData != NULL);

        ASSERT_TRUE(memcmp(fileData, "Hello World!", static_cast<size_t>(file->GetSize())) == 0);
    }

} // namespace FileTests
