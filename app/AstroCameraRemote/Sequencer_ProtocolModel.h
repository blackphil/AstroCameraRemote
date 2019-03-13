#ifndef SEQUENCER_PROTOCOLMODEL_H
#define SEQUENCER_PROTOCOLMODEL_H

#include <QAbstractItemModel>

#include "Sequencer_Protocol.h"

namespace Sequencer {

class ProtocolModel : public QAbstractItemModel
{
    Q_OBJECT

    QList<QPointer<Protocol> > protocols;

public:
    ProtocolModel(QObject* parent = nullptr);

    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & idx) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    Protocol* getProtocol(const QModelIndex& index) const;

    void addProtocol(Protocol* protocol);
    void removeProtocol(const QModelIndex& index);
};

} // namespace Sequencer

#endif // SEQUENCER_PROTOCOLMODEL_H
