Authors:
    Adam Pekný (xpekny00)
    Samuel Slávik (xslavi37)

Make targets:
    make - build project
    make run - build and run project
    make doxygen - generate documentation
    make pack - zip project

Controls:
    You can view controls when you click on "Controls" button in the main menu of our application.

How to play:
    1, Use make run.
    2, Click on "Play" or "Load a map" button.
        2a, In case you chose "Load a map", choose a map located on your machine.
        2b, In case you chose "Play", feel free to choose any of 3 maps differentiated by difficulty.
    3, Press any button to start game when the map loads.
    4, Enjoy.

Saving replay:
    At any point of your gameplay press ESC key to display overlay menu and click on save replay button.
    Or simply when you win or lose, the same overlay menu is displayed, and you can click on save replay button.
    Replay is then saved into resources/replays/ directory.

Watching replay:
    1, Use make run.
    2, Click on "Watch replay" button.
    3, Press any button to start replay.

Implemented requirements:
    - Continuous movement during gameplay (direction change on button click).
    - Loading any maps accessible in machine.
    - Saving and loading replays.
    - Interactive elements showing moves done from the start of the game and keys collected.
    - Continuous replay with time flow control via left and right arrow keys.
    - Possibility of starting replay from the end or start (by resetting with R key and being in respective time flow
        or pressing S for reset to start and B for reset to end)
    - Possibility to control game time (stopping, restarting)

