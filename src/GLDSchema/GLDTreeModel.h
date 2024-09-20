#include "qcontainerfwd.h"
#include <QException>
#include <QAbstractItemModel>

// 节点可能是类、实例、属性
enum class NodeType
{
    CLASS = 0x00,
    ENTITY,
    ATTRIBUTE
};

class GLDTreeItem
{
  public:
    GLDTreeItem(const QString& data, GLDTreeItem* parent = nullptr)
        : m_Content(data)
        , m_Parent(parent)
        , m_childrenLoaded(false)
        , m_Type(NodeType::CLASS)
    {
        m_Children.clear();
    }

    ~GLDTreeItem()
    {
    }

    void appendChild(GLDTreeItem* child)
    {
        m_Children.append(child);
    }

    GLDTreeItem* child(int row)
    {
        if (row >= 0 && row < m_Children.length())
            return m_Children[row];
        else
            throw std::runtime_error("无效的行索引");
    }

    int childCount() const
    {
        // 添加安全检查
        if (!m_childrenLoaded)
        {
            return 0;  // 如果子项还未加载，返回0
        }
        return m_Children.length();
    }

    int columnCount() const
    {
        return 1;
    }

    QString data(int column = 0) const
    {
        return m_Content;
    }

    int row() const
    {
        if (m_Parent)
        {
            return m_Parent->m_Children.indexOf(const_cast<GLDTreeItem*>(this));
        }
        return 0;
    }

    GLDTreeItem* parentItem()
    {
        return m_Parent;
    }

    bool isChildrenLoaded() const
    {
        return m_childrenLoaded;
    }

    void setChildrenLoaded(bool loaded)
    {
        m_childrenLoaded = loaded;
    }


  public:
    GLDTreeItem*          m_Parent;
    QVector<GLDTreeItem*> m_Children;

    NodeType m_Type;
    QString  m_Content;
    bool     m_childrenLoaded;
};


#include <QAbstractItemModel>
class GLDTreeModel : public QAbstractItemModel
{
    Q_OBJECT

  public:
    explicit GLDTreeModel(QObject* parent = nullptr);
    ~GLDTreeModel();

    // 以下是重写的方法
    QVariant    data(const QModelIndex& index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int         rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int         columnCount(const QModelIndex& parent = QModelIndex()) const override;

    // 以下是新增的函数
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

  private:
    void setupModelData(GLDTreeItem* parent);

    GLDTreeItem* getItem(const QModelIndex& index) const;

    GLDTreeItem* rootItem;
};