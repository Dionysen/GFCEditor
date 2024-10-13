#ifndef GLDEDITORTEST_FINDALLTEXT_INCLUDED
#define GLDEDITORTEST_FINDALLTEXT_INCLUDED

#include <gtest/gtest.h>
#include "GLDEditorWidget.h"

// Test the editor widget's findAllText function

class GLDEditorTestFindAllText : public ::testing::Test
{
  public:
    GLDEditorWidget* editorWidget;

    void SetUp() override
    {
        editorWidget = new GLDEditorWidget();
        QFile file("test/test_data/test.gfc");
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        editorWidget->setPlainText(QString::fromUtf8(data));
        file.close();
    }

    void TearDown() override
    {
        delete editorWidget;
    }
};


TEST_F(GLDEditorTestFindAllText, test_findAllText_sensitive_wholeWord)
{
    // case sensitive and whole word match
    editorWidget->findAllText("GFCFLOOR", true, true);
    QMap<QPair<int, int>, QString> matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 7);

    EXPECT_EQ(matchedList.value(QPair<int, int>(10, 4)), "#3=GFCFLOOR('2305870771882300468','建筑-1F',3000,1,-3000,1,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(11, 4)), "#4=GFCFLOOR('2305870771882298642','建筑1F',1403.5822253814,1,0,2,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(12, 4)),
              "#5=GFCFLOOR('2305870771882300332','建筑室外地坪 1',1596.4177746186,1,1403.5822253814,3,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(13, 4)), "#6=GFCFLOOR('2305870771882298626','建筑2F',3000,1,3000,4,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(14, 4)), "#7=GFCFLOOR('2305870771882300451','建筑3F',3000,1,6000,5,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(15, 4)), "#8=GFCFLOOR('2305870771882300434','建筑4F',5000,1,9000,6,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(16, 4)), "#9=GFCFLOOR('2305870771882300417','建筑5F',6000,1,14000,7,$);");

    editorWidget->findAllText("GfcFloor", true, true);
    matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 1);
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 1)), "GfcFloor is a floor class");

    editorWidget->findAllText("floo", true, true);
    matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 0);
}

TEST_F(GLDEditorTestFindAllText, test_findAllText_insensitive_wholeWord)
{
    // case insensitive and whole word match
    editorWidget->findAllText("GfcFloor", false, true);
    QMap<QPair<int, int>, QString> matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 8);
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 1)), "GfcFloor is a floor class");

    editorWidget->findAllText("floor", false, true);
    matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 1);
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 15)), "GfcFloor is a floor class");

    editorWidget->findAllText("floo", false, true);
    matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 0);
}

TEST_F(GLDEditorTestFindAllText, test_findAllText_sensitive_notWholeWord)
{
    // case sensitive and not whole word match
    editorWidget->findAllText("GfcF", true, false);
    QMap<QPair<int, int>, QString> matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 1);
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 1)), "GfcFloor is a floor class");

    editorWidget->findAllText("GFCF", true, false);
    matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 7);
    EXPECT_EQ(matchedList.value(QPair<int, int>(10, 4)), "#3=GFCFLOOR('2305870771882300468','建筑-1F',3000,1,-3000,1,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(11, 4)), "#4=GFCFLOOR('2305870771882298642','建筑1F',1403.5822253814,1,0,2,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(12, 4)),
              "#5=GFCFLOOR('2305870771882300332','建筑室外地坪 1',1596.4177746186,1,1403.5822253814,3,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(13, 4)), "#6=GFCFLOOR('2305870771882298626','建筑2F',3000,1,3000,4,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(14, 4)), "#7=GFCFLOOR('2305870771882300451','建筑3F',3000,1,6000,5,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(15, 4)), "#8=GFCFLOOR('2305870771882300434','建筑4F',5000,1,9000,6,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(16, 4)), "#9=GFCFLOOR('2305870771882300417','建筑5F',6000,1,14000,7,$);");

    editorWidget->findAllText("floo", true, false);
    matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 1);
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 15)), "GfcFloor is a floor class");
}


TEST_F(GLDEditorTestFindAllText, test_findAllText_insensitive_notWholeWord)
{
    // case insensitive and not whole word match
    editorWidget->findAllText("GfcFloor", false, false);
    QMap<QPair<int, int>, QString> matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 8);

    EXPECT_EQ(matchedList.value(QPair<int, int>(10, 4)), "#3=GFCFLOOR('2305870771882300468','建筑-1F',3000,1,-3000,1,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(11, 4)), "#4=GFCFLOOR('2305870771882298642','建筑1F',1403.5822253814,1,0,2,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(12, 4)),
              "#5=GFCFLOOR('2305870771882300332','建筑室外地坪 1',1596.4177746186,1,1403.5822253814,3,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(13, 4)), "#6=GFCFLOOR('2305870771882298626','建筑2F',3000,1,3000,4,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(14, 4)), "#7=GFCFLOOR('2305870771882300451','建筑3F',3000,1,6000,5,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(15, 4)), "#8=GFCFLOOR('2305870771882300434','建筑4F',5000,1,9000,6,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(16, 4)), "#9=GFCFLOOR('2305870771882300417','建筑5F',6000,1,14000,7,$);");
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 1)), "GfcFloor is a floor class");

    editorWidget->findAllText("floo", false, false);
    matchedList = editorWidget->getMatchedList();
    EXPECT_EQ(matchedList.size(), 9);
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 15)), "GfcFloor is a floor class");
    EXPECT_EQ(matchedList.value(QPair<int, int>(17, 4)), "GfcFloor is a floor class");
}

#endif  // GLDEDITORTEST_FINDALLTEXT_INCLUDED