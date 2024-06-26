.include "_util.inc"

DEFINE_OS_FUNC Serial_Open, 0x8008533e
DEFINE_OS_FUNC Serial_Close, 0x800853e6
DEFINE_OS_FUNC Serial_ReadSingle, 0x8008547a
DEFINE_OS_FUNC Serial_Read, 0x800854b8
DEFINE_OS_FUNC Serial_Peek, 0x80084f22
DEFINE_OS_FUNC Serial_WriteSingle, 0x800855d0
DEFINE_OS_FUNC Serial_WriteUnbuffered, 0x80085616
DEFINE_OS_FUNC Serial_Write, 0x80085652
DEFINE_OS_FUNC Serial_PollRX, 0x800856b0
DEFINE_OS_FUNC Serial_PollTX, 0x800856c4
DEFINE_OS_FUNC Serial_ClearRX, 0x800856d8
DEFINE_OS_FUNC Serial_ClearTX, 0x800856fe
DEFINE_OS_FUNC Serial_IsOpen, 0x8008587c
