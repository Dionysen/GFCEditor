#ifndef FLOATINGWIDGET_H
#define FLOATINGWIDGET_H

#include <QWidget>
class QHBoxLayout;

#include "ads/API.h"
#include "ads/SectionContent.h"

ADS_NAMESPACE_BEGIN
class ContainerWidget;
class SectionTitleWidget;
class SectionContentWidget;
class InternalContentData;

// FloatingWidget holds and displays SectionContent as a floating window.
// It can be resized, moved and dropped back into a SectionWidget.
class FloatingWidget : public QWidget
{
    Q_OBJECT

    friend class ContainerWidget;

    enum class ResizeRegion
    {
        None = 0,
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };


  public:
    FloatingWidget(ContainerWidget* container, SectionContent::RefPtr sc, SectionTitleWidget* titleWidget, SectionContentWidget* contentWidget,
                   QWidget* parent = NULL);
    virtual ~FloatingWidget();

    SectionContent::RefPtr content() const
    {
        return _content;
    }

  public:  // private:
    bool takeContent(InternalContentData& data);


  private:
    ResizeRegion getResizeRegion(const QPoint& pos);
    void         resizeWindow(const QPoint& globalPos);
    void         setAllChildrenMouseTracking(QWidget* parent);

  protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

  private slots:
    void onCloseButtonClicked();

  private:
    ContainerWidget*       _container;
    SectionContent::RefPtr _content;
    SectionTitleWidget*    _titleWidget;
    SectionContentWidget*  _contentWidget;

    // Resize
    bool         m_isResizing;
    int          m_borderWidth;
    ResizeRegion m_resizeRegion;
    QPoint       m_lastMousePos;

    QHBoxLayout* _titleLayout;
};

ADS_NAMESPACE_END
#endif
