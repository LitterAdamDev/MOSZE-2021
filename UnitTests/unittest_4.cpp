//sudo apt install libgtest-dev
#include "../SingletonTable.h"
#include <gtest/gtest.h>

TEST(Feladat4, funcTest) {
  char testFile[13]= "testdata.csv";
  char* paraFileName[2];
  paraFileName[1]= testFile;
  SingletonTable* singleton = SingletonTable::GetInstance(2,paraFileName);

  ASSERT_EQ(singleton->GetCellValue(0,0), "#NAME?");
  EXPECT_NEAR(std::stof(singleton->GetCellValue(2,1)), 15.0f, 1e-9 );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    

    return RUN_ALL_TESTS();
}
