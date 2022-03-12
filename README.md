# Embedded_GameController
implement old games and it's controller using stm32 board. 

### playing Snake game 
<img src="https://github.com/zetwhite/Embedded_GameController/blob/main/demo1.gif">

### playing Avoid Obstacle game
<img src="https://github.com/zetwhite/Embedded_GameController/blob/main/demo2.gif">

# Directory Structure 
* `GameController/` : 
   code for stm32 board, don't upload IAR settings/* Debug/* *.ewp ... etc which can cause IAR version conflict problems. 
   just download codes from this repo and create your local projects and do your works on it. 
* `GamePC/` : 
  code that works on your pc (linux, tested on ubuntu20.04). 
* `GamePC/ObstaclesGame/` : code for avoiding obstacles game 
* `GamePC/SnakeGame/` : code for snake game 
