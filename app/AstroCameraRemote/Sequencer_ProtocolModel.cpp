#include "Sequencer_ProtocolModel.h"

#include <QPointer>

#include "AstroBase.h"

namespace Sequencer {

ProtocolModel::ProtocolModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

int ProtocolModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

QVariant ProtocolModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::DisplayRole == role && Qt::Horizontal == orientation)
    {
        switch(section)
        {
        case 0 : return tr("Subject");
        case 1 : return tr("Start time");
        case 2 : return tr("# shots/total");
        case 3 : return tr("Shutter speed");
        case 4 : return tr("ISO");
        case 5 : return tr("Start delay");
        case 6 : return tr("Pause");
        default : break;
        }
    }

    return QVariant();
}

QVariant ProtocolModel::data(const QModelIndex &index, int role) const
{
    Protocol* item = getProtocol(index);
    Q_ASSERT(item);
    if(!item)
        return QVariant();

    switch(role)
    {
    case Qt::DisplayRole :
    {

        switch(index.column())
        {
        case 0 : return item->getSubject();
        case 1 : return item->getStartTime().toString("dd. MM. yyyy hh:mm:ss");
        case 2 : return QString("%0/%1").arg(item->getNumShotsFinished()).arg(item->getProperties().numShots);
        case 3 : return item->getProperties().shutterSpeed;
        case 4 : return item->getProperties().iso;
        case 5 : return item->getProperties().startDelay;
        case 6 : return item->getProperties().pause;
        }
        break;
    }
    case Qt::TextAlignmentRole :
        if(1 == index.column())
            return Qt::AlignHCenter;
        break;
    }


    return QVariant();
}

QModelIndex ProtocolModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    if(row < protocols.count())
        return createIndex(row, column, protocols[row].data());
    else
        return QModelIndex();
}

QModelIndex ProtocolModel::parent(const QModelIndex &idx) const
{
    Q_UNUSED(idx);
    return QModelIndex();
}

int ProtocolModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return protocols.count();
}

Protocol *ProtocolModel::getProtocol(const QModelIndex &index) const
{
    if(!index.isValid())
        return nullptr;

    Protocol* p = static_cast<Protocol*>(index.internalPointer());
    Q_ASSERT(p);

    return p;
}

void ProtocolModel::addProtocol(Protocol *protocol)
{
    beginInsertRows(QModelIndex(), protocols.count(), protocols.count()+1);
    protocols << protocol;
    endInsertRows();
}

bool ProtocolModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    Q_ASSERT(0 <= row && row < protocols.count());
    if(0 > row || row >= protocols.count())
        return false;
    Q_ASSERT(row+count <= protocols.count());
    if(row+count > protocols.count())
        return false;

    while(0 < count)
    {
        Protocol* toRemove = protocols[row];
        Q_ASSERT(toRemove);
        if(!toRemove)
            return false;
        if(!toRemove->deleteFile())
        {
            AB_WRN("Failed to delete protocol file:" << toRemove->getFilePath());
            return false;
        }
        toRemove->deleteLater();
        protocols.removeAt(row);
        count--;
    }

    return true;
}


} // namespace Sequencer
