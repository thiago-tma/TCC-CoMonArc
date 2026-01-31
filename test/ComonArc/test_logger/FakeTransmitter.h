#ifndef D_COMONARCFAKETRANSMITTER_H
#define D_COMONARCFAKETRANSMITTER_H

#include <Transmitter/include/Transmitter.h>

void FakeTransmitter_Reset (void);

bool FakeTransmitter_GetInitialized (void);
void FakeTransmitter_ResetTransmitBuffer (void);
void FakeTransmitter_GetTransmitBuffer  (uint8_t ** buffer, size_t * bufferSize);

#endif /*D_COMONARCFAKETRANSMITTER_H*/