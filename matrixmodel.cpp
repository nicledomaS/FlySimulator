#include <QPainter>
#include "matrixmodel.h"

MatrixModel::MatrixModel(const Area &area)
    :   rArea(area),
        maxColumn(0),
        maxRow(0)
{
    connect(&rArea, Area::updated, this, MatrixModel::updateAll, Qt::QueuedConnection);
}

MatrixModel::~MatrixModel()
{

}

void MatrixModel::reload()
{
    emit layoutAboutToBeChanged();
    maxColumn = rArea.size();
    maxRow = rArea.size();
    emit layoutChanged();
    updateAll();
}

void MatrixModel::updateAll()
{
    dataChanged(index(0,0),index(maxRow-1,maxColumn-1));
}

QModelIndex MatrixModel::index(int row, int col, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(row < maxRow && col < maxColumn)
        return createIndex(row,col);
   return QModelIndex();
}

int MatrixModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return maxRow;
}

int MatrixModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return maxColumn;
}

QVariant MatrixModel::data(const QModelIndex & index, int role) const
{
    Q_UNUSED(role);
    QVariant res;
    if(!index.isValid())
        return res;

    const unsigned short row = index.row();
    const unsigned short col = index.column();
    if (row >= maxRow || col >= maxColumn)
        return res;

    Cell cell = rArea.getData(row, col);
    int live = cell.flies - cell.diedFlies;
    int dead = cell.diedFlies;
    QString value = (live || dead)?QString("%1:%2").arg(live).arg(dead):"";
    return QVariant(value);
}

Renderer::Renderer(QObject *parent)
    :QAbstractItemDelegate(parent)
{
}

Renderer::~Renderer()
{

}

QSize Renderer::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return  QSize(30,30);
}

void Renderer::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(!index.isValid())
        return;

    QRect rect = option.rect;
    QRect tr = rect;
    tr -= QMargins(5,0,0,5);

    QString rawData = index.data().toString();
    QStringList flies = rawData.split(':');
    if(flies.size() != 2)
        return;

    if (flies[0].size() && flies[0] != "0" ) {
        painter->setPen(Qt::red);
        painter->drawText(tr, Qt::AlignLeft|Qt::AlignVCenter, flies[0]);
    }

    if (flies[1].size() && flies[1] != "0" ) {
        painter->setPen(Qt::darkGray);
        painter->drawText(tr, Qt::AlignRight|Qt::AlignVCenter, flies[1]);
    }
}
