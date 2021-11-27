//sudo apt install libgtest-dev
#include "../Application.h"
#include <gtest/gtest.h>

TEST(Feladat4, funcTest) {
  std::string testFileName= "testdata.csv";
  SingletonTable* initTable;
  initTable=new SingletonTable(testFileName);
  Application* myApp =new Application(initTable, true);

  ASSERT_EQ(myApp->GetTable(0).GetCellValue(0,0), "#NAME?");
  EXPECT_NEAR(std::stof(myApp->GetTable(0).GetCellValue(2,1)), 15.0f, 1e-9 );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    

    return RUN_ALL_TESTS();
}
