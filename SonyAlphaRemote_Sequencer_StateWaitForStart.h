#ifndef SONYALPHAREMOTE_SEQUENCER_STATEWAITFORSTART_H
#define SONYALPHAREMOTE_SEQUENCER_STATEWAITFORSTART_H

#include "SonyAlphaRemote_Sequencer_StateBase.h"


namespace SonyAlphaRemote {
namespace Sequencer {

class StateWaitForStart : public StateBase
{
    Q_OBJECT

    QPointer<QTimer> t;
public :
    StateWaitForStart(QTimer* t);

};

} // namespace Sequencer
} // namespace SonyAlphaRemote

#endif // SONYALPHAREMOTE_SEQUENCER_STATEWAITFORSTART_H
