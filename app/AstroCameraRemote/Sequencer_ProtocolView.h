#ifndef SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
#define SEQUENCER_SEQUENCER_PROTOCOLVIEW_H

#include <QTableView>

namespace Sequencer {

class Protocol;
class ProtocolModel;

class ProtocolView : public QTableView
{
    Q_OBJECT

public:
    explicit ProtocolView(QWidget *parent = nullptr);
    ~ProtocolView();

    Protocol* getSelectedProtocol() const;

private Q_SLOTS :
    void contextMenu(const QPoint& pos);

protected:
    void keyPressEvent(QKeyEvent* ke);

    ProtocolModel *getProtocolModel() const;

};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
