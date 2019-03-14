#ifndef SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
#define SEQUENCER_SEQUENCER_PROTOCOLVIEW_H

#include <QTableView>

namespace Sequencer {


class ProtocolView : public QTableView
{
    Q_OBJECT

public:
    explicit ProtocolView(QWidget *parent = nullptr);
    ~ProtocolView();

protected:
    void keyPressEvent(QKeyEvent* ke);

};


} // namespace Sequencer
#endif // SEQUENCER_SEQUENCER_PROTOCOLVIEW_H
