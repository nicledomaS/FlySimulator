#include <QPainter>
#include "matrixmodel.h"

MatrixModel::MatrixModel(const Area &area)
    :   rArea(area),
        column(0),
        row(0)
{
    connect(&rArea, Area::updated, this, MatrixModel::updateAll, Qt::QueuedConnection);
}

MatrixModel::~MatrixModel()
{

}

void MatrixModel::reload()
{
    emit layoutAboutToBeChanged();
    column = rArea.size();
    row = rArea.size();
    emit layoutChanged();
    updateAll();
}

void MatrixModel::updateAll()
{
    dataChanged(index(0,0),index(row-1,column-1));
}

QModelIndex MatrixModel::index(int row, int col, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(row < row && col < column)
        return createIndex(row,col);
   return QModelIndex();
}

int MatrixModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return row;
}

int MatrixModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return column;
}

QVariant MatrixModel::data(const QModelIndex & index, int role) const
{
    Q_UNUSED(role);
    QVariant res;
    if(!index.isValid())
        return res;

    const unsigned short irow = index.row();
    const unsigned short icol = index.column();
    if (irow >= row || icol >= column)
        return res;

    Cell cell = rArea.population(irow, icol);
    int live = cell.all - cell.died;
    int dead = cell.died;
    QString value = (live || dead)?QString("%1:%2:%3").arg(cell.all).arg(live).arg(dead):"";
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
    rect -= QMargins(3,0,3,0);

    QString rawData = index.data().toString();
    QStringList flies = rawData.split(':');
    if(flies.size() != 3)
        return;

    if (flies[1] != "0" )
    {
        painter->setPen(Qt::red);
        painter->drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, flies[1]);
    }

    if (flies[2] != "0" )
    {
        painter->setPen(Qt::darkGray);
        painter->drawText(rect, Qt::AlignRight|Qt::AlignVCenter, flies[2]);
    }
}
