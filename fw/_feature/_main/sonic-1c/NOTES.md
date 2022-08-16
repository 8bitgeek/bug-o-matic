# MODBUS SETTINGS

## Firmware Update

- If 'LOADER.S19' does not exist on the MicroSD card, copy it there.
- Copy the firmware update 'FIRMWARE.S19' file to the MicroSD card. 
- With MicroSD card removed, press and hold the "INPUT" button.
- With "INPUT" button pressed, insert the MicroSD card.
- Releasing the "INPUT" button within 4 seconds initiates the firmware update.

## LOADER.S19 Firmware Loader.

Contains a bootstrap loader that will be loaded into the DTCM SRAM area, and executed.
The loader will take control of the system and read the 'FIRMWARE.S19' file contents
write it to FLASH memory. 

## FIRMWARE.S19

Contains the firmware update image.s

## BAUD.TXT

Contains the MODBUS baud rate (ex. 19200)

To change baud rate, remove the MicroSD card and edit the text file BAUD.TXT to contain the desired baud rate.

Example:

- 9600
- 19200
- 38400
- 57600
- 115200

## ID.TXT

Contains the MODBUS slave ID (ex. 1)

To change the MODBSU slave ID, remove the MicroSD card and edit the test file ID.TXT to contain the desired ID.

Valid Range 1-255

# REGISTER SETTINGS

## To Start With Factory Default Register Settings
1. Press "Input" Button and "Reset" button at the same time.
2. Release "Reset" button while continuing to hold "Input" button.
3. When "X" and "Y" LEDs begin flashing, release "Input" button.

## To Store Current Register Settings
- Manually
1. Press and hold "Input" button.
2. Hold for about 4 seconds, when "Status" LED begins to blink.
3. Release "Input" Button.
- Automatically
1. Write a value to any of the writable holding registers.
2. Observe the "Status" led flash, at this point the value is stored.

## MODBUS Register Map

| NAME                  | OFFSET   |  TYPE  | DESCRIPTION                   |
|:----------------------|:---------|:-------|:------------------------------|
| HOLD_REG_FIRMWARE_VERSION |   1 |     uint16_t | Firmware Version X.Y |
| HOLD_REG_TOF_X |              2 |     uint16_t | X time-of-flight in microseconds |
| HOLD_REG_TOF_Y |			    3 |     uint16_t | Y time-of-flight in microseconds |
| HOLD_REG_VELOCITY |           4 |     uint16_t | Velocity in mm/sec |
| HOLD_REG_LOW_PASS_FILTER |    5 |     uint16_t | Low pass (averaging) filter (0-100) |
| HOLD_REG_REVERSE |            6 |     uint16_t | Reverse Flow Direction (bool) |
| HOLD_REG_RX_GAIN |            7 |     uint16_t | Receiver gain 0-14 |
| HOLD_REG_TX_GAIN |		    8 |     uint16_t | Transmitter gain 0-100 |
| HOLD_REG_DURATION |		    9 |     uint16_t | Transmitter cycles 1-32 |
| HOLD_REG_DETECTOR |           10 |    uint16_t | Detector Simple, Envelope, XZ (0-2) |
| HOLD_REG_DETECT_AMPLITUDE |   11 |    uint16_t | Detection Amplitude (0-100) |
| HOLD_REG_DETECT_ZERO_OFFSET | 12 |     int16_t | Detection Offset microseconds |
| HOLD_REG_COUNTER |            13 |    uint16_t | Seconds Counter / Heartbeat |
| HOLD_REG_STATUS |             14 |    uint16_t | bit#0=null reading/obstruction |
| HOLD_REG_FAULT_COUNT |        15 |    uint16_t | fault counter  |
| HOLD_REG_DAYS |               16 |    uint16_t | Days Uptime |
| HOLD_REG_HOURS |              17 |    uint16_t | Hours Uptime |
| HOLD_REG_MINUTES |            18 |    uint16_t | Minutes Uptime |
| HOLD_REG_SECONDS |            19 |    uint16_t | Seconds Uptime |
| HOLD_REG_TOF_COUNT |          20 |    uint16_t | Number of TOF Measurements |
| HOLD_REG_WAVEFORMS |          21 |    uint16_t | Number Waveform Captures |
| * | * | * | *|
| HOLD_REG_BASELINESET |        50 |    uint16_t | Baseline in mm (WRITE) |
| HOLD_REG_BASELINEGET |        51 |    uint16_t | Baseline in mm (READ) |
| HOLD_REG_FACE2FACESET |       52 |    uint16_t | Face2Face in mm (WRITE) |
| HOLD_REG_FACE2FACEGET |       53 |    uint16_t | Face2Face in mm (READ) |
| * | * | * | *|
| HOLD_REG_V_M_SEC_FLOAT32 |    100 |   float32  | Velocity in meters/sec |
| HOLD_REG_V_FPM_FLOAT32 |      102 |   float32  | Volume in Feet per Minute |
| HOLD_REG_V_CFM_FLOAT32 |      104 |   float32  | Volume in Cubic Feet per Minute |
| HOLD_REG_V_KCFM_FLOAT32 |     106 |   float32  | Volume in K Cubic Feet per Minute |
| HOLD_REG_V_CMS_FLOAT32 |      108 |   float32  | Volume in Cubic Meters per Second |
| HOLD_REG_V_DELTAT_FLOAT32 |   110 |   float32  | Delta Time in seconds |
| * | * | * | *|
| HOLD_REG_SQUARE_METERS |      120 |   float32  | Area in square meters |
| * | * | * | *|
| HOLD_REG_CORRECTION_FACTOR |  150 |   float32  | Correction Factor meters/sec |
| HOLD_REG_ZERO_CUTOFF |        152 |   float32  | Zero cut-off meters/sec |
| * | * | * | *|
| HOLD_REG_BAUD_RATE_100 |      200 |   uint16_t | Baud Rate/100 |
| HOLD_REG_SLAVE_ID |           201 |   uint16_t | MODBUS Slave ID |
| * | * | * | *|
| HOLD_REG_AQ_SERIAL_NO_0 |     300 |   uint16_t |  Serial_Number[0] |
| HOLD_REG_AQ_SERIAL_NO_1 |     301 |   uint16_t |  Serial_Number[1] |
| HOLD_REG_AQ_SERIAL_NO_2 |     302 |   uint16_t |  Serial_Number[2] |
| HOLD_REG_AQ_SERIAL_NO_3 |     303 |   uint16_t |  Serial_Number[3] |
| HOLD_REG_AQ_SERIAL_NO_4 |     304 |   uint16_t |  Serial_Number[4] |
| HOLD_REG_AQ_SERIAL_NO_5 |     305 |   uint16_t |  Serial_Number[5] |
| * | * | * | *|
| HOLD_REG_AFX_X_SERIAL_NO_0 |  306 |   uint16_t |  XDUC X Serial_Number[0] |
| HOLD_REG_AFX_X_SERIAL_NO_1 |  307 |   uint16_t |  XDUC X Serial_Number[1] |
| HOLD_REG_AFX_X_SERIAL_NO_2 |  308 |   uint16_t |  XDUC X Serial_Number[2] |
| HOLD_REG_AFX_X_SERIAL_NO_3 |  309 |   uint16_t |  XDUC X Serial_Number[3] |
| HOLD_REG_AFX_X_SERIAL_NO_4 |  310 |   uint16_t |  XDUC X Serial_Number[4] |
| HOLD_REG_AFX_X_SERIAL_NO_5 |  311 |   uint16_t |  XDUC X Serial_Number[5] |
| * | * | * | *|
| HOLD_REG_AFX_Y_SERIAL_NO_0 |  312 |   uint16_t |  XDUC Y Serial_Number[0] |
| HOLD_REG_AFX_Y_SERIAL_NO_1 |  313 |   uint16_t |  XDUC Y Serial_Number[1] |
| HOLD_REG_AFX_Y_SERIAL_NO_2 |  314 |   uint16_t |  XDUC Y Serial_Number[2] |
| HOLD_REG_AFX_Y_SERIAL_NO_3 |  315 |   uint16_t |  XDUC Y Serial_Number[3] |
| HOLD_REG_AFX_Y_SERIAL_NO_4 |  316 |   uint16_t |  XDUC Y Serial_Number[4] |
| HOLD_REG_AFX_Y_SERIAL_NO_5 |  317 |   uint16_t |  XDUC Y Serial_Number[5] |
| * | * | * | *|
| HOLD_REG_XFR_BYTES_CTRL     | 500 |   uint16_t | XFER Control bits |
| HOLD_REG_XFR_BYTES_STATUS   | 501 |   uint16_t | XFER Status bits  |
| HOLD_REG_XFR_BYTES_PSIZE    | 502 |   uint16_t | Number of bytes per full page |
| HOLD_REG_XFR_BYTE_READY     | 503 |   uint16_t | Number of XFER bytes ready |
| HOLD_REG_XFR_BYTES          | 504 |   uint16_t | Payload bytes (LSB is first byte) |

# Bulk Transfer 

## HOLD_REG_XFR_BYTES_CTRL 

| NAME                    | MASK   | DESCRIPTION                   |
|:------------------------|:-------|:------------------------------|
| XFER_CTRL_DL_WAV        | 0x0001 | xfer request download wave |
| XFER_CTRL_DIR_MSK       | 0x0002 |  0 = x->y, 1 = y->x |

## HOLD_REG_XFR_BYTES_STATUS

| NAME                    | MASK   | DESCRIPTION                   |
|:------------------------|:-------|:------------------------------|
| XFER_STATUS_BUSY        | 0x0001 | xfer request download wave |
| XFER_STATUS_EOF         | 0x0002 | xfer is end-of-file (EOF) |
| XFER_STATUS_FLT_MSK     | 0xF000 | xfer fault code mask |
| XFER_STATUS_FLT_IO      | 0x1000 | I/O fault |
| XFER_STATUS_FLT_TO      | 0x2000 | time-out fault |

## HOLD_REG_XFR_BYTES_PSIZE 

| NAME                    | MASK   | DESCRIPTION                   |
|:------------------------|:-------|:------------------------------|
| XFR_BYTES_PSIZE         | 0x00FF | A full page size in bytes |

## HOLD_REG_XFR_BYTES_READY

| NAME                    | MASK   | DESCRIPTION                   |
|:------------------------|:-------|:------------------------------|
| XFR_BYTES_READY         | 0x00FF | Number of bytes ready to xfer |

## Bulk Transfer Use Case

Master writes (XFER_CTRL_DL_WAV | XFER_CTRL_DIR_MSK) bits to HOLD_REG_XFR_BYTES_CTRL
register to start trasnfer.

Slave sets XFER_STATUS_BUSY, clears XFER_STATUS_EOF bits in HOLD_REG_XFR_BYTES_STATUS
register, and clears the HOLD_REG_XFR_BYTE_READY register.

When the page is ready to be read out, the slave will populate the HOLD_REG_XFR_BYTE_READY
register with the number of bytes in the page, and clear the XFER_STATUS_BUSY bit in the 
HOLD_REG_XFR_BYTES_STATUS register.

When it is the last page, the slave will set the XFER_STATUS_EOF bit of the 
HOLD_REG_XFR_BYTES_STATUS status register.

## HOLD_REG_FACE2FACESET vs HOLD_REG_FACE2FACEGET

When the contents of the HOLD_REG_FACE2FACESET register is zero, then the face 
to face distance will be calucated continuously, and presented on that 
HOLD_REG_FACE2FACEGET register.

Writing a non-zero value to HOLD_REG_FACE2FACESET will cause the written
value to be used at the face to face distance, and will also be reflected
in the HOLD_REG_FACE2FACEGET register.

## HOLD_REG_BASELINESET vs HOLD_REG_BASELINEGET

When the contents of the HOLD_REG_BASELINESET register is zero, then the contents
of the HOLD_REG_FACE2FACEGET is transfered to the HOLD_REG_BASELINEGET register.

When the contents of the HOLD_REG_BASELINESET register is non-zero, then the
contents are used in the baseline calculation, and the value is transfered to
the HOLD_REG_BASELINEGET register.

## HOLD_REG_BAUD_RATE_100 and HOLD_REG_SLAVE_ID

Writing to the HOLD_REG_BAUD_RATE_100 or HOLD_REG_SLAVE_ID will take effect
upon the next restart.

