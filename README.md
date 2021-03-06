# Embedded_GameController
This project is done as a term project of embedded lecture (PNU 2021 CP34001: EMBEDDED SYSTEM DESIGN & LAB). \
We implemented retro games (obstacle avoiding game, snake game) on linux and it's controller using stm32 board.

# Overview
- Bluetooth communication is used to connect pc(linux) and controller(stm32).
- To implement pc console game, we used Cpp language and ncurses library.  
- In stm32 board, sensor inputs like button and joystick movement are handled by interrupt handler. 
- We added a sensitivity level setup page for players. Because required tilt sensitivity is different by each game. 
<img src="readme_img/화면흐름도.drawio.png"  width="400">

## Envirnoment
* Board: STM32F107VCT6
* PC OS : tested on ubuntu 20.04 
* Module
   - 3.2" TFT LCD
   - Gyro Sensor [STS90H1-2A] (https://www.devicemart.co.kr/goods/view?no=1330838) 
   - Arcade Game Machine Switch [SZH-ZR003] (https://www.devicemart.co.kr/goods/view?no=1376727)
   - Joystick [SZH-JA038] (https://www.devicemart.co.kr/goods/view?no=13152304 )
   - Bluetooth module [FB755AC]

# demo
### snake game demo
<img src="readme_img/demo1.gif">

### avoiding obstacle game demo
<img src="readme_img/demo2.gif">

# Directory Structure 
* `GameController/` : 
   code for stm32 board, don't upload IAR settings/* Debug/* *.ewp ... etc which can cause IAR version conflict problems. 
   just download codes from this repo and create your local projects and do your works on it. 
   (tested on stm32f107) 
* `GamePC/` : 
  code that works on your pc (linux, tested on ubuntu20.04). 
* `GamePC/ObstaclesGame/` : code for avoiding obstacles game 
* `GamePC/SnakeGame/` : code for snake game 
