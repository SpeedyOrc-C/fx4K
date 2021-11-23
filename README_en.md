# fx4K

[简体中文](README.md) - **English**

## Introduction

This is a 4K (four keys) rhythm game on Casio calculators fx-9750, fx-9860 and its series. 

## How to Play?

Press the corresponding key (`7`, `8`, `DEL` and `AC`) when a note fall to the bottom of its column. 

For the short notes (CLICK), you need to tap the keys. For the long notes (HOLD), you need to hold the keys as it touch the bottom, and release it after it goes off. **You can hold the keys as long as you wish in HOLD notes.** 

## Feasibility of this Game

### Refresh Rate

Theoretically, the FPS of these calculators can be very high, but the LCD display has a great latency when a pixel blinks, which results in a bad experience. Also, it needs accurate timing (Retrieve time from OS function by magic address is hard to understand) so the FPS in this game is 40 (25 millisecond each frame). 

### Get the Chart

I do not know how to create a chart by myself and have never involved in any development of rhythm game, so there is a Python script for you to turn the Malody chart into the format that fx4K can read. 

## fx4K Chart Binary Format

### Header

| Byte\(s\) | Length | Meaning            |
| :-------- | :----- | :----------------- |
| `00~1F`   | 16     | song title string  |
| `20~3F`   | 16     | song artist string |
| `40,41`   | 2      | BPM                |
| `42,43`   | 2      | note count         |
| `44,45`   | 2      | song length        |
| `46~4F`   | 12     | LEAVE BLANK        |

### Note (Starts at the end of header)

| Byte\(s\) | Length | Meaning                    |
| :-------- | :----- | :------------------------- |
| `00,01`   | 2      | note start time            |
| `02,03`   | 2      | note end time \(optional\) |
| `04`      | 1      | note type (`C` or `H`)     |
| `05`      | 1      | column                     |
| `06,07`   | 2      | LEAVE BLANK                |

