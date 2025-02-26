/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   https://lammps.sandia.gov/, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include "../testing/utils.h"
#include "compressed_dump_test.h"
#include "fmt/format.h"
#include "utils.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>


using ::testing::Eq;

class DumpAtomCompressTest : public CompressedDumpTest {
public:
    DumpAtomCompressTest() : CompressedDumpTest("atom") {
    }
};

//-------------------------------------------------------------------------------------------------
// compressed files
//-------------------------------------------------------------------------------------------------

TEST_F(DumpAtomCompressTest, compressed_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto text_file       = text_dump_filename("run0.melt");
    auto compressed_file = compressed_dump_filename("run0.melt");

    if(compression_style == "atom/zstd") {
        generate_text_and_compressed_dump(text_file, compressed_file, "", "", "", "checksum yes", 0);
    } else {
        generate_text_and_compressed_dump(text_file, compressed_file, "", "", 0);
    }

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_THAT(converted_file, Eq(converted_dump_filename("run0.melt")));
    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}

TEST_F(DumpAtomCompressTest, compressed_multi_file_run1)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name         = "multi_file_run1_*.melt";
    auto base_name_0       = "multi_file_run1_0.melt";
    auto base_name_1       = "multi_file_run1_1.melt";
    auto text_file         = text_dump_filename(base_name);
    auto text_file_0       = text_dump_filename(base_name_0);
    auto text_file_1       = text_dump_filename(base_name_1);
    auto compressed_file   = compressed_dump_filename(base_name);
    auto compressed_file_0 = compressed_dump_filename(base_name_0);
    auto compressed_file_1 = compressed_dump_filename(base_name_1);

    if(compression_style == "atom/zstd") {
        generate_text_and_compressed_dump(text_file, compressed_file, "", "", "", "checksum no", 1);
    } else {
        generate_text_and_compressed_dump(text_file, compressed_file, "", "", 1);
    }

    TearDown();

    auto converted_file_0 = convert_compressed_to_text(compressed_file_0);
    auto converted_file_1 = convert_compressed_to_text(compressed_file_1);

    ASSERT_THAT(converted_file_0, Eq(converted_dump_filename(base_name_0)));
    ASSERT_THAT(converted_file_1, Eq(converted_dump_filename(base_name_1)));
    ASSERT_FILE_EXISTS(converted_file_0);
    ASSERT_FILE_EXISTS(converted_file_1);
    ASSERT_FILE_EQUAL(text_file_0, converted_file_0);
    ASSERT_FILE_EQUAL(text_file_1, converted_file_1);

    delete_file(text_file_0);
    delete_file(text_file_1);
    delete_file(compressed_file_0);
    delete_file(compressed_file_1);
    delete_file(converted_file_0);
    delete_file(converted_file_1);
}

TEST_F(DumpAtomCompressTest, compressed_multi_file_with_pad_run1)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name         = "multi_file_pad_run1_*.melt";
    auto base_name_0       = "multi_file_pad_run1_000.melt";
    auto base_name_1       = "multi_file_pad_run1_001.melt";
    auto text_file         = text_dump_filename(base_name);
    auto text_file_0       = text_dump_filename(base_name_0);
    auto text_file_1       = text_dump_filename(base_name_1);
    auto compressed_file   = compressed_dump_filename(base_name);
    auto compressed_file_0 = compressed_dump_filename(base_name_0);
    auto compressed_file_1 = compressed_dump_filename(base_name_1);

    generate_text_and_compressed_dump(text_file, compressed_file, "", "pad 3", 1);

    TearDown();

    ASSERT_FILE_EXISTS(text_file_0);
    ASSERT_FILE_EXISTS(text_file_1);
    ASSERT_FILE_EXISTS(compressed_file_0);
    ASSERT_FILE_EXISTS(compressed_file_1);

    auto converted_file_0 = convert_compressed_to_text(compressed_file_0);
    auto converted_file_1 = convert_compressed_to_text(compressed_file_1);

    ASSERT_THAT(converted_file_0, Eq(converted_dump_filename(base_name_0)));
    ASSERT_THAT(converted_file_1, Eq(converted_dump_filename(base_name_1)));
    ASSERT_FILE_EXISTS(converted_file_0);
    ASSERT_FILE_EXISTS(converted_file_1);
    ASSERT_FILE_EQUAL(text_file_0, converted_file_0);
    ASSERT_FILE_EQUAL(text_file_1, converted_file_1);

    delete_file(text_file_0);
    delete_file(text_file_1);
    delete_file(compressed_file_0);
    delete_file(compressed_file_1);
    delete_file(converted_file_0);
    delete_file(converted_file_1);
}

TEST_F(DumpAtomCompressTest, compressed_multi_file_with_maxfiles_run1)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name         = "multi_file_maxfiles_run1_*.melt";
    auto base_name_0       = "multi_file_maxfiles_run1_0.melt";
    auto base_name_1       = "multi_file_maxfiles_run1_1.melt";
    auto base_name_2       = "multi_file_maxfiles_run1_2.melt";
    auto text_file         = text_dump_filename(base_name);
    auto text_file_0       = text_dump_filename(base_name_0);
    auto text_file_1       = text_dump_filename(base_name_1);
    auto text_file_2       = text_dump_filename(base_name_2);
    auto compressed_file   = compressed_dump_filename(base_name);
    auto compressed_file_0 = compressed_dump_filename(base_name_0);
    auto compressed_file_1 = compressed_dump_filename(base_name_1);
    auto compressed_file_2 = compressed_dump_filename(base_name_2);

    generate_text_and_compressed_dump(text_file, compressed_file, "", "maxfiles 2", 2);

    TearDown();

    ASSERT_FILE_NOT_EXISTS(text_file_0);
    ASSERT_FILE_EXISTS(text_file_1);
    ASSERT_FILE_EXISTS(text_file_2);
    ASSERT_FILE_NOT_EXISTS(compressed_file_0);
    ASSERT_FILE_EXISTS(compressed_file_1);
    ASSERT_FILE_EXISTS(compressed_file_2);

    auto converted_file_1 = convert_compressed_to_text(compressed_file_1);
    auto converted_file_2 = convert_compressed_to_text(compressed_file_2);

    ASSERT_THAT(converted_file_1, Eq(converted_dump_filename(base_name_1)));
    ASSERT_THAT(converted_file_2, Eq(converted_dump_filename(base_name_2)));
    ASSERT_FILE_EXISTS(converted_file_1);
    ASSERT_FILE_EXISTS(converted_file_2);
    ASSERT_FILE_EQUAL(text_file_1, converted_file_1);
    ASSERT_FILE_EQUAL(text_file_2, converted_file_2);

    delete_file(text_file_1);
    delete_file(text_file_2);
    delete_file(compressed_file_1);
    delete_file(compressed_file_2);
    delete_file(converted_file_1);
    delete_file(converted_file_2);
}

TEST_F(DumpAtomCompressTest, compressed_with_units_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name       = "with_units_run0.melt";
    auto text_file       = text_dump_filename(base_name);
    auto compressed_file = compressed_dump_filename(base_name);

    generate_text_and_compressed_dump(text_file, compressed_file, "", "scale no units yes", 0);

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}

TEST_F(DumpAtomCompressTest, compressed_with_time_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name       = "with_time_run0.melt";
    auto text_file       = text_dump_filename(base_name);
    auto compressed_file = compressed_dump_filename(base_name);

    generate_text_and_compressed_dump(text_file, compressed_file, "", "scale no time yes", 0);

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}

TEST_F(DumpAtomCompressTest, compressed_triclinic_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name       = "tri_run0.melt";
    auto text_file       = text_dump_filename(base_name);
    auto compressed_file = compressed_dump_filename(base_name);

    enable_triclinic();
    generate_text_and_compressed_dump(text_file, compressed_file, "", "", 0);

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}

TEST_F(DumpAtomCompressTest, compressed_triclinic_with_units_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name       = "tri_with_units_run0.melt";
    auto text_file       = text_dump_filename(base_name);
    auto compressed_file = compressed_dump_filename(base_name);

    enable_triclinic();
    generate_text_and_compressed_dump(text_file, compressed_file, "", "scale no units yes", 0);

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}

TEST_F(DumpAtomCompressTest, compressed_triclinic_with_time_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name       = "tri_with_time_run0.melt";
    auto text_file       = text_dump_filename(base_name);
    auto compressed_file = compressed_dump_filename(base_name);

    enable_triclinic();
    generate_text_and_compressed_dump(text_file, compressed_file, "", "scale no time yes", 0);

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}

TEST_F(DumpAtomCompressTest, compressed_triclinic_with_image_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name       = "tri_with_image_run0.melt";
    auto text_file       = text_dump_filename(base_name);
    auto compressed_file = compressed_dump_filename(base_name);

    enable_triclinic();
    generate_text_and_compressed_dump(text_file, compressed_file, "", "image yes", 0);

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}

TEST_F(DumpAtomCompressTest, compressed_modify_bad_param)
{
    if (compression_style != "atom/gz") GTEST_SKIP();

    if (!verbose) ::testing::internal::CaptureStdout();
    command(fmt::format("dump id1 all {} 1 {}", compression_style, compressed_dump_filename("modify_bad_param_run0_*.melt")));
    if (!verbose) ::testing::internal::GetCapturedStdout();

    TEST_FAILURE(".*ERROR: Illegal dump_modify command: compression level must in the range of.*",
        command("dump_modify id1 compression_level 12");
    );
}

TEST_F(DumpAtomCompressTest, compressed_modify_multi_bad_param)
{
    if (compression_style != "atom/gz") GTEST_SKIP();

    if (!verbose) ::testing::internal::CaptureStdout();
    command(fmt::format("dump id1 all {} 1 {}", compression_style, compressed_dump_filename("modify_multi_bad_param_run0_*.melt")));
    if (!verbose) ::testing::internal::GetCapturedStdout();

    TEST_FAILURE(".*ERROR: Illegal dump_modify command: compression level must in the range of.*",
        command("dump_modify id1 pad 3 compression_level 12");
    );
}

TEST_F(DumpAtomCompressTest, compressed_modify_clevel_run0)
{
    if (!COMPRESS_BINARY) GTEST_SKIP();

    auto base_name       = "modify_clevel_run0.melt";
    auto text_file       = text_dump_filename(base_name);
    auto compressed_file = compressed_dump_filename(base_name);

    generate_text_and_compressed_dump(text_file, compressed_file, "", "", "", "compression_level 3", 0);

    TearDown();

    ASSERT_FILE_EXISTS(text_file);
    ASSERT_FILE_EXISTS(compressed_file);

    auto converted_file = convert_compressed_to_text(compressed_file);

    ASSERT_THAT(converted_file, Eq(converted_dump_filename(base_name)));
    ASSERT_FILE_EXISTS(converted_file);
    ASSERT_FILE_EQUAL(text_file, converted_file);
    delete_file(text_file);
    delete_file(compressed_file);
    delete_file(converted_file);
}
