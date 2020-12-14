#include "ut_qtermwidget_test.h"

#include "qtermwidget.h"
#include "Emulation.h"
#include "SearchBar.h"
#include "TerminalDisplay.h"

//Qt单元测试相关头文件
#include <QTest>
#include <QtGui>
#include <QDebug>

using namespace Konsole;

UT_QTermWidget_Test::UT_QTermWidget_Test()
{
}

void UT_QTermWidget_Test::SetUp()
{
    m_termWidget = new QTermWidget;
    m_termWidget->startShellProgram();
    m_termWidget->show();
#ifdef ENABLE_UI_TEST
    QTest::qWait(UT_WAIT_TIME);
#endif
}

void UT_QTermWidget_Test::TearDown()
{
    delete m_termWidget;
}

#ifdef UT_QTERMWIDGET_TEST

TEST_F(UT_QTermWidget_Test, setWindowTitle)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->setWindowTitle("Terminal");
}

TEST_F(UT_QTermWidget_Test, setWindowIcon)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->setWindowIcon(QIcon());
}

TEST_F(UT_QTermWidget_Test, setScrollBarPosition)
{
    EXPECT_EQ(m_termWidget != nullptr, true);

    m_termWidget->setScrollBarPosition(QTermWidget::NoScrollBar);

    m_termWidget->setScrollBarPosition(QTermWidget::ScrollBarRight);
}

TEST_F(UT_QTermWidget_Test, setKeyboardCursorShape)
{
    EXPECT_EQ(m_termWidget != nullptr, true);

    m_termWidget->setKeyboardCursorShape(Emulation::KeyboardCursorShape::BlockCursor);
}


TEST_F(UT_QTermWidget_Test, availableColorSchemes)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    EXPECT_GE(m_termWidget->availableColorSchemes().size(), 0);
}

TEST_F(UT_QTermWidget_Test, setColorScheme)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->setColorScheme("Light");

    m_termWidget->setColorScheme("Dark");
}

TEST_F(UT_QTermWidget_Test, setShellProgram)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    //设置shell类型
    m_termWidget->setShellProgram("/bin/bash");
}

TEST_F(UT_QTermWidget_Test, setKeyBindings)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->setKeyBindings("linux");
}

TEST_F(UT_QTermWidget_Test, getsetTerminalFont)
{
    EXPECT_EQ(m_termWidget != nullptr, true);

    int fontSize = 15;
    QFont font("Noto Mono");
    font.setPixelSize(fontSize);
    m_termWidget->setTerminalFont(font);
//    EXPECT_EQ(m_termWidget->getTerminalFont().pixelSize(), fontSize);
}

TEST_F(UT_QTermWidget_Test, setTerminalOpacity)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->setTerminalOpacity(1.0);
}

TEST_F(UT_QTermWidget_Test, setWorkingDirectory)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    QString path = "/usr/share";
    m_termWidget->setWorkingDirectory(path);
}

TEST_F(UT_QTermWidget_Test, setEnvironment)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->setEnvironment(QStringList());
}

TEST_F(UT_QTermWidget_Test, startShellProgram)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->setShellProgram("/bin/sh");
    m_termWidget->startShellProgram();
}

TEST_F(UT_QTermWidget_Test, setFlowControlEnabled)
{
    EXPECT_EQ(m_termWidget != nullptr, true);

    m_termWidget->setFlowControlEnabled(false);
    EXPECT_EQ(m_termWidget->flowControlEnabled(), false);

    m_termWidget->setFlowControlEnabled(true);
    EXPECT_EQ(m_termWidget->flowControlEnabled(), true);
}

TEST_F(UT_QTermWidget_Test, clear)
{
    EXPECT_EQ(m_termWidget != nullptr, true);
    m_termWidget->clear();
}

/*******************************************************************************
 1. @函数:    hasRunningProcess
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    是否有正在运行的程序
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, hasRunningProcess)
{
    // 此时应该没有
    bool hasRunning = m_termWidget->hasRunningProcess();
    EXPECT_EQ(hasRunning, false);

    // 运行ping
    m_termWidget->sendText("ping 127.0.0.1\n");
#ifdef ENABLE_UI_TEST
    QTest::qWait(UT_WAIT_TIME);
#endif
    hasRunning = m_termWidget->hasRunningProcess();
    qDebug() << "has process running:" << hasRunning;
//    EXPECT_EQ(hasRunning, true);
}

/*******************************************************************************
 1. @函数:    workingDirectory
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    获取当前工作目录
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, workingDirectory)
{
    // 当前终端获取当前工作目录
    QString currentDir = QDir::currentPath();
    QString result = m_termWidget->workingDirectory();
    qDebug() << "current path : " << currentDir << " workingDirectory : " << result;
    EXPECT_EQ(currentDir, result);

    // 指定工作目录
    QString workDir = "/";
    QTermWidget *term = new QTermWidget(0);
    term->setWorkingDirectory(workDir);
    term->startShellProgram();
    EXPECT_EQ(term->workingDirectory(), workDir);
    delete term;
}

/*******************************************************************************
 1. @函数:    find
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    测试原装查找函数
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, find)
{
    // 初始化searchBar
    m_termWidget->m_searchBar = new SearchBar(m_termWidget);
    // 调用查找函数
    m_termWidget->find();
    m_termWidget->findNext();
    m_termWidget->findPrevious();
    // 无异常
    EXPECT_NE(m_termWidget, nullptr);

    // 调用search函数
    m_termWidget->search(false, true);
    m_termWidget->search(true, false);
    EXPECT_NE(m_termWidget, nullptr);
}

/*******************************************************************************
 1. @函数:    setIsAllowScroll
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    设置是否允许输出时滚动
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, setIsAllowScroll)
{
    // 不允许
    m_termWidget->setIsAllowScroll(false);
    EXPECT_EQ(m_termWidget->getIsAllowScroll(), false);
    // 允许
    m_termWidget->setIsAllowScroll(true);
    EXPECT_EQ(m_termWidget->getIsAllowScroll(), true);
}

/*******************************************************************************
 1. @函数:    setZoom
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    设置字体缩放
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, setZoom)
{
    // 设置当前term的字体大小
    QFont initfont;
    initfont.setPointSize(20);
    m_termWidget->setTerminalFont(initfont);
    // 获取当前字体大小
    QFont font = m_termWidget->getTerminalFont();
    int fontSize = font.pointSize();
    qDebug() << "term font size: " << fontSize;

    // 放大
    m_termWidget->setZoom(10);
    font = m_termWidget->getTerminalFont();
    EXPECT_EQ(font.pointSize(), 30);
    // 缩小
    m_termWidget->setZoom(-20);
    font = m_termWidget->getTerminalFont();
    EXPECT_EQ(font.pointSize(), 10);
}

/*******************************************************************************
 1. @函数:    zoomIn zoomOut
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    单步放大和缩小
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, zoom)
{
    // 设置当前term的字体大小
    QFont initfont;
    initfont.setPointSize(20);
    m_termWidget->setTerminalFont(initfont);
    // 获取当前字体大小
    QFont font = m_termWidget->getTerminalFont();
    int fontSize = font.pointSize();
    qDebug() << "term font size: " << fontSize;

    // 放大
    m_termWidget->zoomIn();
    font = m_termWidget->getTerminalFont();
    EXPECT_EQ(font.pointSize(), 21);
    // 缩小
    m_termWidget->zoomOut();
    font = m_termWidget->getTerminalFont();
    EXPECT_EQ(font.pointSize(), 20);
}

/*******************************************************************************
 1. @函数:    getForegroundProcessId
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    获取当前前端进程的processID
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, getForegroundProcessId)
{
    // 当前shell的pid
    int pid = m_termWidget->getForegroundProcessId();
    EXPECT_NE(pid, -1);

    // ping的pid
    m_termWidget->sendText("ping 127.0.0.1\n");
#ifdef ENABLE_UI_TEST
    QTest::qWait(UT_WAIT_TIME);
#endif
    int pid2 = m_termWidget->getForegroundProcessId();
    qDebug() << "pid :" << pid << "pid2 :" << pid2;
//    EXPECT_NE(pid2, pid);
}

/*******************************************************************************
 1. @函数:    getForegroundProcessName
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-12
 4. @说明:    获取前端程序名称
*******************************************************************************/
TEST_F(UT_QTermWidget_Test, getForegroundProcessName)
{
    // ping
    m_termWidget->sendText("ping 127.0.0.1\n");
#ifdef ENABLE_UI_TEST
    QTest::qWait(UT_WAIT_TIME);
#endif
    QString processName = m_termWidget->getForegroundProcessName();
    qDebug() << "foreground process name:" << processName;
//    EXPECT_EQ(processName, QString("ping"));
}


#endif