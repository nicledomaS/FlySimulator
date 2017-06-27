#ifndef MATRIXMODEL_H
#define MATRIXMODEL_H
#include <QAbstractTableModel>
#include <QAbstractItemDelegate>
#include "flysimulator.h"

class MatrixModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    MatrixModel(const Area &area);
    virtual ~MatrixModel();

    void reload();
    virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role) const;

private:
    void updateAll();
    const Area &rArea;
    int column;
    int row;
};

class Renderer : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    Renderer(QObject *parent = 0);
    virtual ~Renderer();

    virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // MATRIXMODEL_H
