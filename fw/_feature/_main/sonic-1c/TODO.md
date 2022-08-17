# TODO

## Flow Calculation

- [X] Correction Factor.
- [X] Velocity low-pass filter.
- [X] Trigger Threashold Level.
- [X] Zero Cut-off.
- [X] Volumetric inputs (width/height).
- [X] Volumetric Outputs (Cubic/Meters).
- [X] Verification.

## Diagnostoc / Performance

- [ ] Write Waveform to file with meta.
- [ ] Receiver energy detection (quantify).
- [ ] Power Envelope detector.
- [ ] Power envelope quantification.
- [ ] Power quantity to modbus registers.

## Zero-Crossing Filter

- [ ] Detection box.
- [ ] MODBUS Register Settings.

## Upstream Modbus

- [X] English measure registers.
- [X] Floating point registers.
- [ ] Temperature Readings.
- [ ] Accelerometer readings.
- [X] Fault code registers.
- [ ] Hours/Minutes uptime counters.

## Controller

- [X] Make serial number available.
- [X] Firmware Update
- [X] Settings, BAUD, ID, etc..
- [ ] Enable USB UART Emulation.
- [ ] Textual settings menu.
- [ ] Enable bus current monitoring.
- [ ] Enable bus power switching.
- [ ] POST / Fault detect + emit.
- [ ] Enable write-protect on read-only registers.

## Firmware Update / Bootstrap Loader

- [X] Bootstrap finds "BOOT.S19" and "LOADER.S19".
- [X] Caribou-less version of chan's filesystem.
- [X] Linker script pointing ro RAM for loader exec.
- [X] Stop systick.
- [X] Load "LOADER.S19" into RAM.
- [X] Branch to "LOADER" entry point.


## Post / Fault Detection

- [ ] Transducers okay.
- [ ] Transducer bus current okay.
- [ ] Obstruction detection by echo range finding.
