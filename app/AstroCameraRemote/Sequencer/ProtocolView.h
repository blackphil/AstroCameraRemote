#ifndef SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
#define SEQUENCER_SEQUENCER_PROTOCOLVIEW_H

#include <QTableView>

#include "../MessagePoster.h"

namespace Sequencer {

class Protocol;
class ProtocolModel;

class ProtocolView : public QTableView
{
    Q_OBJECT

    MessagePoster* msgPoster;

public:
    explicit ProtocolView(QWidget *parent = nullptr);
    ~ProtocolView();

    Protocol* getSelectedProtocol() const;

private Q_SLOTS :
    void contextMenu(const QPoint& pos);

Q_SIGNALS :
    void activateSelectedProtocol();

protected:
    void keyPressEvent(QKeyEvent* ke);

    ProtocolModel *getProtocolModel() const;

};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
