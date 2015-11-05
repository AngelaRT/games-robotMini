# games-robotMini
My TFM (Master in Robotics) code:

There exist two different games in this repository: cardsGame (kind of memory game, the aim is to find all the cards pairs) and differencesGame (the typical "find the N differences"!). 

Both games have been built with a similar architecture:

- There is an user interface which lays over HTML, js, CSS code. This means that we can "play" the games accessing the URL of the game from a web browser. In fact, it's Mini who opens this browser when we ask her to play one of these two games.

- And, there is a backend. As this takes part of a robotics master, there is no other way of building the backend but using ROS. I have taken advantage of the previous Mini's code. She was already able to talk (at the same time that she moves her neck, arms  and she blinks), to understand some phrases (recorded as grammars) and, in short, to use this skills to play some basic games.
