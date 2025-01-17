# GFCEditor

## Introduction

GFC Editor是一款浏览和编辑GFC文件的PC端工具。

GFC：GFC（global foundation classes）是一种建筑信息数据交换标准，旨在实现不同软件平台之间的数据互通和共享。目前已成为中关村数字建筑绿色发展联盟团标。参考文档《[GFC标准](https://doc.weixin.qq.com/doc/w3_AMUAwwYRACQGbLZjcW1RMWS4yBE7h?scode=ALgASQcQABI4BAENkiAMUAwwYRACQ)》

![image-20241015172628491](https://cdn.jsdelivr.net/gh/Dionysen/BlogCDN@main/imgimage-20241015172628491.png)

![image-20241017173556275](https://cdn.jsdelivr.net/gh/Dionysen/BlogCDN@main/imgimage-20241017173556275.png)

## Dependences

1. [Qt 6.7.2](https://www.qt.io/)
2. [Qt-Advanced-Docking-System](https://github.com/mfreiholz/Qt-Advanced-Docking-System)
3. [QtCustomTitlebarWindow](https://github.com/Dionysen/QtCustomTitlebarWindow)
4. [googletest](https://github.com/google/googletest)

## Requires

1. Qt >= v6.7.2
2. XMake >= v2.9.3

## Build

使用[xmake](https://xmake.io/#/zh-cn/)构建项目。

**确保已经安装Qt6以及对应的C++编译器。**

### 安装xmake

Linux

```bash
curl -fsSL https://xmake.io/shget.text | bash
```

Windows

```powershell
Invoke-Expression (Invoke-Webrequest 'https://xmake.io/psget.text' -UseBasicParsing).Content
```

### 构建

在根目录打开命令行：

```bash
xmake b
```

### 运行

```bash
xmake r
```

### 使用Visual Studio构建

```bash
xmake project -k vsxmake2022
```

会在根目录生成vs解决方案。

### 生成CMake

```bash
xmake project -k cmake
```

会在根目录生成vs解决方案。

---

> XMake更多使用方法见[使用xmake构建c++项目](https://dionysen.github.io/2023/06/23/note/Programming/tools/xmake/)、[xmake](https://xmake.io/#/zh-cn/)。

## 功能表

✅ 为已实现

❔ 为可能已实现，未经测试

❌ 为未实现

| 序号 | 分类 | 功能名           | 功能描述                                                     | 状态 |
| ---- | ---- | ---------------- | ------------------------------------------------------------ | ---- |
| 1    | 文件 | 新建             | 新建GFC文件，默认加入头段和空白数据段，光标停在空白数据段中。 | ✅    |
| 2    | 文件 | 打开             | 打开GFC文件。                                                | ✅    |
| 3    | 文件 | 保存             | 保存到文件中。                                               | ✅    |
| 4    | 文件 | 另存为           | 另存为一个文件。                                             | ✅    |
| 5    | 文件 | 最近打开文件列表 | 记录最近打开的5个文件，点击打开对应文件。                    | ❔    |
| 6    | 文件 | 退出             | 退出当前程序。                                               | ✅    |
| 7    | 编辑 | 撤销             | 撤销上次编辑内容，可多次执行，直到恢复文件打开时状态。       | ✅    |
| 8    | 编辑 | 重复             | 恢复上次撤销内容。                                           | ✅    |
| 9    | 编辑 | 剪切             | 将当前选中文本复制到系统剪切板中，并从文本区删除。           | ✅    |
| 10   | 编辑 | 复制             | 将当前选中文本复制到系统剪切板中。                           | ✅    |
| 11   | 编辑 | 粘贴             | 从系统剪切板中的文本复制到当前应用中。                       | ✅    |
| 12   | 编辑 | 查找             | 根据文本查找，查找选项支持【区分大小写】、【全字匹配】，可将查找结果输出到列表，用于定位。 | ✅    |
| 13   | 编辑 | 查找下一个       | 根据【查找】输入条件，查找下一个符合条件的文本。             | ✅    |
| 14   | 编辑 | 替换             | 用新文本替换搜索到的旧文本。                                 | ❌    |
| 15   | 导航 | 前进             | 用于跳转到用户接下来要定位的GFC实例，‌如果还没有使用后退按钮，‌前进按钮将处于非激活状态。‌ | ❔    |
| 16   | 导航 | 后退             | 允许用户跳转到之前定位的GFC实例，‌通常是最近定位的。‌          | ❌    |
| 17   | 导航 | 定位             | 光标在引用实例的序号上时，可跳转定位到该实例。               | ❌    |
| 18   | 工具 | 界面可见性控制   | 控制视图区、属性区、辅助区、工具栏、状态栏的可见性           | ✅    |
| 19   | 工具 | 设置文本颜色     | 可对头段、数据段分别设置标号、实例名、属性值（区分布尔型、整型、浮点型、字符串型、枚举型和空值）的文本颜色。 | ❌    |
| 20   | 工具 | 被引用实例       | 输出当前实例被引用的所有实例，可用于定位。                   | ❌    |
| 21   | 工具 | 合法性校验       | 输出错误列表，可用于定位。                                   | ❌    |
| 22   | 帮助 | 帮助             | 弹出帮助文档。                                               | ✅    |
| 23   | 帮助 | 关于             | 弹出【关于】对话框。                                         | ✅    |
| 24   | 界面 | 界面舶靠         | 工具栏、视图区、属性区、辅助区都能自由浮动或舶靠。           | ✅    |
| 25   | 界面 | Schema视图       | 点击实例或属性节点时，需同步在“文本区”定位该实例，并在“属性区”显示其属性。 | ❌    |
| 26   | 界面 | 项目视图         | 点击节点（除“构件类型”和“构件”节点）时，需同步在“文本区”定位该实例，并在“属性区”显示其属性。 | ❌    |
| 27   | 界面 | 文本区           | ● 光标在不同的实例上时，需同步切换属性区内容。2分                ● 按下Ctrl，同时点击引用实例的序号，可跳转定位到该实例。4分                ● 编辑GFC文件时，应同步更新视图区和属性区。2分 | ❌    |
| 28   | 界面 | 属性区           | 选中某条属性时，需同步在“文本区”中将当前属性高亮显示。       | ❌    |
| 29   | 界面 | 辅助区           | ● 显示文本搜索匹配的全部结果，每个结果单独一行显示，双击时文本区定位到匹配结果的位置。1分                ● 合法性校验检测出的所有错误，每个错误单独一行显示，双击时文本区定位到错误的位置。1分                ● 显示当前实例被引用的实例列表，以树形式展现，同“Schema视图”实例节点。双击时需同步在“文本区”定位该实例，并在“属性区”显示其属性。2分 | ❌    |
| 30   | 界面 | 状态栏           | 显示文本区当前光标所在的行、列位置；显示文件大小，单位KB；   | ✅    |

## 注意

### 构建模式

debug模式下由于保留许多debug信息且没有进行高度优化，读取大文件时性能较差，性能测试应当在release模式下进行。

### 窗口布局

目前有三种布局方式：

1. 使用Splitter

```cpp
void MainWindow::setupDockingLayout()
{
    QSplitter* vSplitter = new QSplitter(Qt::Vertical, this);
    vSplitter->addWidget(p_editorWidget);
    vSplitter->addWidget(p_auxiliaryArea);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(p_schemaWiddget);
    splitter->addWidget(vSplitter);
    splitter->addWidget(p_attributeArea);

    vSplitter->setHandleWidth(0);
    splitter->setHandleWidth(0);

    this->setCentralWidget(splitter);

    // 设置dock的初始大小
    splitter->setSizes(QList<int>({ 150, 300, 150, 150 }));
}
```

2. 使用QDockWidget

```cpp
void MainWindow::setupDockingLayout()
{
    setDockNestingEnabled(true);

    QDockWidget* p_schemaDockWidget = new QDockWidget(this);
    p_schemaDockWidget->setWidget(p_schemaWiddget);
    p_schemaDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, p_schemaDockWidget);

    QDockWidget* p_editorDockWidget = new QDockWidget(this);
    p_editorDockWidget->setWidget(p_editorWidget);
    p_editorDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, p_editorDockWidget);

    QDockWidget* p_attributeDockWidget = new QDockWidget(this);
    p_attributeDockWidget->setWidget(p_attributeArea);
    p_attributeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, p_attributeDockWidget);

    QDockWidget* p_auxiliaryDockWidget = new QDockWidget(this);
    p_auxiliaryDockWidget->setWidget(p_auxiliaryArea);
    p_auxiliaryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, p_auxiliaryDockWidget);

    // 设置布局
    addDockWidget(Qt::LeftDockWidgetArea, p_schemaDockWidget);
    splitDockWidget(p_schemaDockWidget, p_editorDockWidget, Qt::Horizontal);
    splitDockWidget(p_editorDockWidget, p_attributeDockWidget, Qt::Horizontal);
    splitDockWidget(p_editorDockWidget, p_auxiliaryDockWidget, Qt::Vertical);

    // 设置dock的初始大小
    resizeDocks({ p_schemaDockWidget }, { 150 }, Qt::Horizontal);
    resizeDocks({ p_attributeDockWidget }, { 150 }, Qt::Horizontal);
    resizeDocks({ p_auxiliaryDockWidget }, { 150 }, Qt::Vertical);
    // 删除centralwidget，所有部件均为dockwidget
    QWidget* p = takeCentralWidget();
    if (p)
        delete p;
}
```

3. 使用[Qt-Advanced-Docking-System](https://github.com/mfreiholz/Qt-Advanced-Docking-System)


```cpp
void MainWindow::setupDockingLayout()
{
	p_container = new ADS_NS::ContainerWidget();
    setCentralWidget(p_container);

    ADS_NS::SectionContent::RefPtr p_EditorContent =
        ADS_NS::SectionContent::newSectionContent(QString("Editor"), p_container, new QLabel("Editor"), p_editorWidget);
    p_container->addSectionContent(p_EditorContent, NULL, ADS_NS::CenterDropArea);


    ADS_NS::SectionContent::RefPtr p_AuxiliaryContent =
        ADS_NS::SectionContent::newSectionContent(QString("Auxiliary"), p_container, new QLabel("Auxiliary"), p_auxiliaryArea);

    p_container->addSectionContent(p_AuxiliaryContent, NULL, ADS_NS::BottomDropArea);

    ADS_NS::SectionContent::RefPtr p_AttributeContent =
        ADS_NS::SectionContent::newSectionContent(QString("Attribute"), p_container, new QLabel("Attribute"), p_attributeArea);
    p_container->addSectionContent(p_AttributeContent, NULL, ADS_NS::RightDropArea);

    ADS_NS::SectionContent::RefPtr p_SchemaContent =
        ADS_NS::SectionContent::newSectionContent(QString("Schema"), p_container, new QLabel("Schema"), p_schemaWiddget);
    p_container->addSectionContent(p_SchemaContent, NULL, ADS_NS::LeftDropArea);
}
```

其中1、3性能比较好，3功能更多，但还未完成（魔改）。

### 单元测试

单元测试目前能通过，但测试内容可能并不完全正确。

测试代码应该完善。
