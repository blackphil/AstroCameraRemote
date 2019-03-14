#ifndef SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
#define SEQUENCER_SEQUENCER_PROTOCOLVIEW_H

#include <QTableView>

namespace Sequencer {

class Protocol;


class ProtocolView : public QTableView
{
    Q_OBJECT

public:
    explicit ProtocolView(QWidget *parent = nullptr);
    ~ProtocolView();

    Protocol* getSelectedProtocol() const;

protected:
    void keyPressEvent(QKeyEvent* ke);

};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
