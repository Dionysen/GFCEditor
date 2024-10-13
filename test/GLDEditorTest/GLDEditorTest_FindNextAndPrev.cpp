#include <gtest/gtest.h>
#include "GLDEditorWidget.h"
#include "qtextcursor.h"

// Test the editor widget's findAllText function

class GLDEditorTestFindNextAndPrev : public ::testing::Test
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

TEST_F(GLDEditorTestFindNextAndPrev, test_findNextText)
{
    editorWidget->moveCursor(QTextCursor::Start);
    editorWidget->findAllText("GFCFLOOR", true, true);
    QPair<int, int> matchIndex = editorWidget->findNextText();
    EXPECT_EQ(matchIndex.first, 11);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findNextText();
    EXPECT_EQ(matchIndex.first, 12);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findNextText();
    EXPECT_EQ(matchIndex.first, 13);
    EXPECT_EQ(matchIndex.second, 4);
}


TEST_F(GLDEditorTestFindNextAndPrev, test_findPreviousText)
{
    editorWidget->moveCursor(QTextCursor::Start);
    editorWidget->findAllText("GFCFLOOR", true, true);
    QPair<int, int> matchIndex = editorWidget->findPreviousText();
    EXPECT_EQ(matchIndex.first, 16);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findPreviousText();
    EXPECT_EQ(matchIndex.first, 15);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findPreviousText();
    EXPECT_EQ(matchIndex.first, 14);
    EXPECT_EQ(matchIndex.second, 4);
}


TEST_F(GLDEditorTestFindNextAndPrev, test_findPreviousTextAndNextText)
{
    editorWidget->moveCursor(QTextCursor::Start);
    editorWidget->findAllText("GFCFLOOR", true, true);
    QPair<int, int> matchIndex = editorWidget->findPreviousText();
    EXPECT_EQ(matchIndex.first, 16);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findPreviousText();
    EXPECT_EQ(matchIndex.first, 15);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findPreviousText();
    EXPECT_EQ(matchIndex.first, 14);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findNextText();
    EXPECT_EQ(matchIndex.first, 15);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findNextText();
    EXPECT_EQ(matchIndex.first, 16);
    EXPECT_EQ(matchIndex.second, 4);

    matchIndex = editorWidget->findNextText();
    EXPECT_EQ(matchIndex.first, 10);
    EXPECT_EQ(matchIndex.second, 4);
}
