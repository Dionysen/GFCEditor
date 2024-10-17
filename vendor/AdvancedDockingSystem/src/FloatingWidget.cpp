#include "ads/FloatingWidget.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QStyle>
#include <QPainter>

#include <Dwmapi.h>

#include "ads/ContainerWidget.h"
#include "ads/SectionTitleWidget.h"
#include "ads/SectionContentWidget.h"
#include "ads/Internal.h"

ADS_NAMESPACE_BEGIN

FloatingWidget::FloatingWidget(ContainerWidget* container, SectionContent::RefPtr sc, SectionTitleWidget* titleWidget,
                               SectionContentWidget* contentWidget, QWidget* parent)
    : QWidget(parent, Qt::CustomizeWindowHint | Qt::Tool | Qt::FramelessWindowHint)
    , _container(container)
    , _content(sc)
    , _titleWidget(titleWidget)
    , _contentWidget(contentWidget)
    , m_isResizing(false)
    , m_borderWidth(2)
    , m_resizeRegion(ResizeRegion::None)
    , m_lastMousePos(QPoint())
{
    QVBoxLayout* l = new QVBoxLayout();
    l->setContentsMargins(1, 1, 1, 1);
    l->setSpacing(0);
    setLayout(l);

    // Title + Controls
    _titleLayout = new QHBoxLayout();
    _titleLayout->addWidget(titleWidget, 1);
    l->addLayout(_titleLayout, 0);
    titleWidget->setActiveTab(false);

    if (sc->flags().testFlag(SectionContent::Closeable))
    {
        QPushButton* closeButton = new QPushButton();
        closeButton->setObjectName("closeButton");
        closeButton->setFlat(true);
        // closeButton->setIcon(GLDStyleManager::StyleManager()->getIcon("close").pixmap(10, 10));
        closeButton->setToolTip(tr("Close"));
        closeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _titleLayout->addWidget(closeButton);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QObject::connect(closeButton, &QPushButton::clicked, this, &FloatingWidget::onCloseButtonClicked);
#else
        QObject::connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked()));
#endif
    }

    // Content
    l->addWidget(contentWidget, 1);
    contentWidget->show();
    //	_container->_floatingWidgets.append(this);
    this->setMouseTracking(true);
    this->_titleWidget->setMouseTracking(true);
    this->_contentWidget->setMouseTracking(true);
    setAllChildrenMouseTracking(this);
}

void FloatingWidget::setAllChildrenMouseTracking(QWidget* parent)
{
    // 获取所有子对象
    const auto& children = parent->children();

    // 遍历每个子对象
    for (QObject* child : children)
    {
        // 只处理QWidget类型的子对象
        QWidget* widget = qobject_cast<QWidget*>(child);
        if (widget)
        {
            widget->setMouseTracking(true);
            // 递归遍历其子部件
            setAllChildrenMouseTracking(widget);
        }
    }
}


void FloatingWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isResizing)
    {
        resizeWindow(event->globalPosition().toPoint());
    }
    else
    {
        // 根据鼠标位置设置不同的光标形状
        ResizeRegion region = getResizeRegion(event->pos());
        switch (region)
        {
        case ResizeRegion::Top:
        case ResizeRegion::Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
        case ResizeRegion::Left:
        case ResizeRegion::Right:
            setCursor(Qt::SizeHorCursor);
            break;
        case ResizeRegion::TopLeft:
        case ResizeRegion::BottomRight:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case ResizeRegion::TopRight:
        case ResizeRegion::BottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }
    }
    event->accept();
}

void FloatingWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_resizeRegion = getResizeRegion(event->pos());
        if (m_resizeRegion != ResizeRegion::None)
        {
            m_isResizing   = true;
            m_lastMousePos = event->globalPosition().toPoint();
            event->accept();
        }
    }
}

void FloatingWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // // Resize
    m_isResizing = false;
    setCursor(Qt::ArrowCursor);
    event->accept();
}

FloatingWidget::ResizeRegion FloatingWidget::getResizeRegion(const QPoint& pos)
{
    bool onLeft   = pos.x() < m_borderWidth;
    bool onRight  = pos.x() > width() - m_borderWidth;
    bool onTop    = pos.y() < m_borderWidth;
    bool onBottom = pos.y() > height() - m_borderWidth;

    if (onTop && onLeft)
        return ResizeRegion::TopLeft;
    if (onTop && onRight)
        return ResizeRegion::TopRight;
    if (onBottom && onLeft)
        return ResizeRegion::BottomLeft;
    if (onBottom && onRight)
        return ResizeRegion::BottomRight;
    if (onTop)
        return ResizeRegion::Top;
    if (onBottom)
        return ResizeRegion::Bottom;
    if (onLeft)
        return ResizeRegion::Left;
    if (onRight)
        return ResizeRegion::Right;

    return ResizeRegion::None;
}

void FloatingWidget::resizeWindow(const QPoint& globalPos)
{
    QRect geom  = geometry();
    int   diffX = globalPos.x() - m_lastMousePos.x();
    int   diffY = globalPos.y() - m_lastMousePos.y();

    switch (m_resizeRegion)
    {
    case ResizeRegion::Top:
        geom.setTop(geom.top() + diffY);
        break;
    case ResizeRegion::Bottom:
        geom.setBottom(geom.bottom() + diffY);
        break;
    case ResizeRegion::Left:
        geom.setLeft(geom.left() + diffX);
        break;
    case ResizeRegion::Right:
        geom.setRight(geom.right() + diffX);
        break;
    case ResizeRegion::TopLeft:
        geom.setTop(geom.top() + diffY);
        geom.setLeft(geom.left() + diffX);
        break;
    case ResizeRegion::TopRight:
        geom.setTop(geom.top() + diffY);
        geom.setRight(geom.right() + diffX);
        break;
    case ResizeRegion::BottomLeft:
        geom.setBottom(geom.bottom() + diffY);
        geom.setLeft(geom.left() + diffX);
        break;
    case ResizeRegion::BottomRight:
        geom.setBottom(geom.bottom() + diffY);
        geom.setRight(geom.right() + diffX);
        break;
    default:
        break;
    }
    setGeometry(geom);
    m_lastMousePos = globalPos;
}

FloatingWidget::~FloatingWidget()
{
    _container->_floatings.removeAll(this);  // Note: I don't like this here, but we have to remove it from list...
}

bool FloatingWidget::takeContent(InternalContentData& data)
{
    data.content       = _content;
    data.titleWidget   = _titleWidget;
    data.contentWidget = _contentWidget;

    _titleLayout->removeWidget(_titleWidget);
    _titleWidget->setParent(_container);
    _titleWidget = NULL;

    layout()->removeWidget(_contentWidget);
    _contentWidget->setParent(_container);
    _contentWidget = NULL;

    return true;
}

void FloatingWidget::onCloseButtonClicked()
{
    _container->hideSectionContent(_content);
}

ADS_NAMESPACE_END
