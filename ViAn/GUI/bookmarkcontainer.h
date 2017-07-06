#ifndef BOOKMARKCONTAINER_H
#define BOOKMARKCONTAINER_H
#include <QWidget>
#include <QListWidget>
#include <QLayout>
#include <QDropEvent>

/**
 * @brief BookmarkContainer::BookmarkContainer
 * @param parent
 * This container should contain bookmarks
 * and images when fully implemeted.
 * It should support drag and drop from and to the container.
 */
class BookmarkContainer : public QListWidget
{
    Q_OBJECT
    QVBoxLayout* layout;
public:
    explicit BookmarkContainer(QWidget* parent = nullptr);
    ~BookmarkContainer();
protected:
    void dropEvent(QDropEvent* event);
};

#endif // BOOKMARKCONTAINER_H
