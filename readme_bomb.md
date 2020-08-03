bomb mod
by Somerunce
ported to 0.6 by heinrich5991
ported to 0.7 by Zwelf
-----------------------------

Bomb is a survival Teeworlds modification, compatible with Teeworlds 0.7.
It's an catch-like, hammer-only game mode in which all players start as a bomb.

Whenever you are a bomb, you can become human again by hammering another tee, bomb and human alike. The hammered tee either stays bomb, or becomes bomb, if the player was a human beforehand. Thus, the amount of bombs in a round can decrease each hit, however never below 1.
If you are human on the other hand, you can hammer other tees to stun them for a short period (0.25 secs). Either hit bombs to escape to safety or humans to sabotage them.

A timer is counting down from 20 seconds. As soon as 0 is reached, all bombs explode and are eliminated for the current round.
Every human survivor then becomes a bomb again, once more with a timer of 20 seconds. This cycles continues until only one tee remains. At that point, the next round starts and everyone becomes a bomb again.
Whenever you survive as a human, you gain 1 point.

Changes from 0.6
----------------

Instead of randomly selecting 1 tee to be the bomb in each round, everyone starts as a bomb which they have to get rid of.
This is intended to make the game mode symmetrical and force more confrontation with other tees.

Config
------

* `sv_bomb_time`: time in seconds until bombs explode (default 20)
* `sv_bomb_explode_message`: displayed message when bombs explode (default "BOOM!")
* `sv_bomb_score_survive`: score gain for surviving as a human (default 1)
* `sv_bomb_score_pass_bomb`: score gain for passing bomb onto other tee (default 0)
* `sv_bomb_score_stun_tee`: score gain for stunning a tee (default 0)
* `sv_bomb_score_last_survivor`: score gain for being the last survivor (default 0)
* `sv_bomb_allow_kill`: allow kill via console kill commando (default 1)
* `sv_bomb_probability`: Probability of a player to become a bomb when choosing a new bomb.
  At least one player is chosen, therefore playing with `sv_bomb_probability 0` always
  one player is chosen and with `sv_bomb_probability 100` every player starts as a bomb.
  Default is set to 50.

Example `autoexec.cfg`
----------------------

```
# Probability for a tee to become a bomb at the start of the round
# when set to 0 always one player starts as bomb
# when set to 100 all players start as bomb
sv_bomb_probability 50

# Scoring points gain for surviving as a human
sv_bomb_score_survive 1       # for surviving as a human
sv_bomb_score_last_survivor 0 # for being the last survivor
sv_bomb_score_pass_bomb 0     # for passing the own bomb onto other tee
sv_bomb_score_stun_tee 0      # for stunning other tee

# allow kill command
sv_bomb_allow_kill 1

add_vote "One player is selected as a bomb" "sv_bomb_probability 0"
add_vote "Randomize the number of bombs" "sv_bomb_probability 50"
add_vote "All player start as a bombs" "sv_bomb_probability 100"

add_vote "Map: bomb1 (at least 2 players recommended)" "sv_map bomb1"
add_vote "Map: bomb2 (at least 3 players recommended)" "sv_map bomb2"
add_vote "Map: bomb3 (at least 4 players recommended)" "sv_map bomb3"
```
